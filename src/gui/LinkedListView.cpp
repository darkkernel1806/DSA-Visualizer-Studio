#include "gui/LinkedListView.hpp"
#include <cstdio>
#include <cstdlib>

namespace gui {

LinkedListView::LinkedListView()
    : valInput({0, 0, 70, 32}, "Val", true, 5),
      idxInput({0, 0, 60, 32}, "Idx", true, 3),
      insertHeadBtn({0, 0, 78, 32}, "+ Head", COLOR_ACCENT_PRIMARY),
      insertTailBtn({0, 0, 78, 32}, "+ Tail", COLOR_ACCENT_PRIMARY),
      insertAtBtn({0, 0, 84, 32}, "+ At Idx", COLOR_ACCENT_INFO),
      deleteHeadBtn({0, 0, 78, 32}, "- Head", COLOR_ACCENT_DANGER),
      deleteTailBtn({0, 0, 78, 32}, "- Tail", COLOR_ACCENT_DANGER),
      deleteValBtn({0, 0, 84, 32}, "- Value", COLOR_ACCENT_DANGER),
      searchBtn({0, 0, 78, 32}, "Search", COLOR_ACCENT_WARNING),
      randomBtn({0, 0, 84, 32}, "+ Rand", COLOR_ACCENT_SUCCESS),
      clearBtn({0, 0, 70, 32}, "Clear", COLOR_PANEL_LIGHT),
      searchTarget(0), searchHighlightIdx(-1), isSearching(false),
      searchTimer(0.0f), searchStepDelay(0.35f), searchFound(false) {
    complexity.set("O(1) Head / O(n) Tail", "O(n)", "O(n)", "O(n)",
                   "Head ops are O(1); arbitrary index ops and value searches require O(n) traversal.");

    // Pre-populate with initial linked list
    list.insertTail(12);
    list.insertTail(35);
    list.insertTail(58);
    list.insertTail(74);
    console.log("Linked List initialized with 4 nodes: 12 -> 35 -> 58 -> 74 -> nullptr.", StatusType::INFO);
}

void LinkedListView::reset() {
    list.clear();
    isSearching = false;
    searchHighlightIdx = -1;
    valInput.clear();
    idxInput.clear();
    console.log("Linked List reset.", StatusType::INFO);
}

void LinkedListView::update(float dt) {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 82.0f;

    // Arrange controls in 2 clean compact rows
    // Row 1: Inputs and Insertions
    valInput.setBounds({ startX, startY, 70, 32 });
    idxInput.setBounds({ startX + 78.0f, startY, 56, 32 });
    insertHeadBtn.setBounds({ startX + 142.0f, startY, 78, 32 });
    insertTailBtn.setBounds({ startX + 228.0f, startY, 78, 32 });
    insertAtBtn.setBounds({ startX + 314.0f, startY, 84, 32 });
    searchBtn.setBounds({ startX + 406.0f, startY, 78, 32 });
    randomBtn.setBounds({ startX + 492.0f, startY, 84, 32 });

    // Row 2: Deletions and Clear
    float row2Y = startY + 38.0f;
    deleteHeadBtn.setBounds({ startX + 142.0f, row2Y, 78, 32 });
    deleteTailBtn.setBounds({ startX + 228.0f, row2Y, 78, 32 });
    deleteValBtn.setBounds({ startX + 314.0f, row2Y, 84, 32 });
    clearBtn.setBounds({ startX + 406.0f, row2Y, 78, 32 });

    valInput.update();
    idxInput.update();
    insertHeadBtn.update();
    insertTailBtn.update();
    insertAtBtn.update();
    deleteHeadBtn.update();
    deleteTailBtn.update();
    deleteValBtn.update();
    searchBtn.update();
    randomBtn.update();
    clearBtn.update();

    // Animated Search Step Logic
    if (isSearching) {
        searchTimer += dt;
        if (searchTimer >= searchStepDelay) {
            searchTimer = 0.0f;
            searchHighlightIdx++;

            int currVal = 0;
            if (list.getAt(static_cast<size_t>(searchHighlightIdx), currVal)) {
                if (currVal == searchTarget) {
                    isSearching = false;
                    searchFound = true;
                    char msg[96];
                    std::snprintf(msg, sizeof(msg), "SEARCH SUCCESS: Found target %d at index %d!", searchTarget, searchHighlightIdx);
                    console.log(msg, StatusType::SUCCESS);
                }
            } else {
                // Reached end of list without match
                isSearching = false;
                searchFound = false;
                searchHighlightIdx = -1;
                char msg[96];
                std::snprintf(msg, sizeof(msg), "SEARCH COMPLETE: Value %d does not exist in the list.", searchTarget);
                console.log(msg, StatusType::WARNING);
            }
        }
    }

    // Insert Head
    if (insertHeadBtn.isClicked()) {
        int v = 0;
        if (valInput.getIntValue(v)) {
            if (list.getSize() >= 8) {
                console.log("Display limit reached (max 8 nodes for visualizer layout).", StatusType::WARNING);
            } else {
                list.insertHead(v);
                char msg[96];
                std::snprintf(msg, sizeof(msg), "Inserted %d at HEAD. List size: %zu.", v, list.getSize());
                console.log(msg, StatusType::SUCCESS);
                valInput.clear();
            }
        } else {
            console.log("Enter a valid integer in 'Val' box.", StatusType::DANGER);
        }
    }

    // Insert Tail
    if (insertTailBtn.isClicked()) {
        int v = 0;
        if (valInput.getIntValue(v)) {
            if (list.getSize() >= 8) {
                console.log("Display limit reached (max 8 nodes).", StatusType::WARNING);
            } else {
                list.insertTail(v);
                char msg[96];
                std::snprintf(msg, sizeof(msg), "Inserted %d at TAIL. List size: %zu.", v, list.getSize());
                console.log(msg, StatusType::SUCCESS);
                valInput.clear();
            }
        } else {
            console.log("Enter a valid integer in 'Val' box.", StatusType::DANGER);
        }
    }

    // Insert At Index
    if (insertAtBtn.isClicked()) {
        int v = 0, idx = 0;
        if (valInput.getIntValue(v) && idxInput.getIntValue(idx)) {
            if (idx < 0 || static_cast<size_t>(idx) > list.getSize()) {
                console.log("Index out of bounds! Must be between 0 and size.", StatusType::DANGER);
            } else if (list.getSize() >= 8) {
                console.log("Display limit reached (max 8 nodes).", StatusType::WARNING);
            } else {
                list.insertAt(static_cast<size_t>(idx), v);
                char msg[96];
                std::snprintf(msg, sizeof(msg), "Inserted %d at index %d. List size: %zu.", v, idx, list.getSize());
                console.log(msg, StatusType::SUCCESS);
                valInput.clear();
                idxInput.clear();
            }
        } else {
            console.log("Enter both 'Val' and 'Idx' to insert at index.", StatusType::DANGER);
        }
    }

    // Delete Head
    if (deleteHeadBtn.isClicked()) {
        int val = 0;
        if (list.deleteHead(val)) {
            char msg[96];
            std::snprintf(msg, sizeof(msg), "Deleted HEAD node (%d). New size: %zu.", val, list.getSize());
            console.log(msg, StatusType::SUCCESS);
        } else {
            console.log("List is empty! Cannot delete head.", StatusType::DANGER);
        }
    }

    // Delete Tail
    if (deleteTailBtn.isClicked()) {
        int val = 0;
        if (list.deleteTail(val)) {
            char msg[96];
            std::snprintf(msg, sizeof(msg), "Deleted TAIL node (%d). New size: %zu.", val, list.getSize());
            console.log(msg, StatusType::SUCCESS);
        } else {
            console.log("List is empty! Cannot delete tail.", StatusType::DANGER);
        }
    }

    // Delete Value
    if (deleteValBtn.isClicked()) {
        int v = 0;
        if (valInput.getIntValue(v)) {
            size_t delIdx = 0;
            if (list.deleteValue(v, &delIdx)) {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "Deleted node with value %d from index %zu.", v, delIdx);
                console.log(msg, StatusType::SUCCESS);
                valInput.clear();
            } else {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "Value %d not found in the list.", v);
                console.log(msg, StatusType::WARNING);
            }
        } else {
            console.log("Enter target value to delete in 'Val' box.", StatusType::DANGER);
        }
    }

    // Search Action
    if (searchBtn.isClicked()) {
        int v = 0;
        if (valInput.getIntValue(v)) {
            if (list.isEmpty()) {
                console.log("List is empty! Nothing to search.", StatusType::WARNING);
            } else {
                searchTarget = v;
                isSearching = true;
                searchTimer = 0.0f;
                searchHighlightIdx = 0;
                searchFound = false;

                char msg[96];
                std::snprintf(msg, sizeof(msg), "Initiating linear search for %d starting at HEAD...", searchTarget);
                console.log(msg, StatusType::INFO);

                // Check index 0 immediately
                int currVal = 0;
                if (list.getAt(0, currVal) && currVal == searchTarget) {
                    isSearching = false;
                    searchFound = true;
                    std::snprintf(msg, sizeof(msg), "SEARCH SUCCESS: Target %d found immediately at HEAD (index 0)!", searchTarget);
                    console.log(msg, StatusType::SUCCESS);
                }
            }
        } else {
            console.log("Enter target value in 'Val' box to search.", StatusType::DANGER);
        }
    }

    // Random Action
    if (randomBtn.isClicked()) {
        if (list.getSize() >= 8) {
            console.log("Display limit reached (max 8 nodes).", StatusType::WARNING);
        } else {
            int r = GetRandomValue(10, 99);
            list.insertTail(r);
            char msg[96];
            std::snprintf(msg, sizeof(msg), "Appended random value %d at tail.", r);
            console.log(msg, StatusType::SUCCESS);
        }
    }

    // Clear Action
    if (clearBtn.isClicked()) {
        list.clear();
        isSearching = false;
        searchHighlightIdx = -1;
        console.log("List cleared. All nodes freed.", StatusType::INFO);
    }
}

void LinkedListView::draw() {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 82.0f;
    float availW = WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f;

    // Controls container
    Rectangle controlsCard = { startX, startY - 6.0f, availW, 82.0f };
    DrawCard(controlsCard, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    valInput.draw();
    idxInput.draw();
    insertHeadBtn.draw();
    insertTailBtn.draw();
    insertAtBtn.draw();
    deleteHeadBtn.draw();
    deleteTailBtn.draw();
    deleteValBtn.draw();
    searchBtn.draw();
    randomBtn.draw();
    clearBtn.draw();

    // Stats Pill
    char sizeBuf[48];
    std::snprintf(sizeBuf, sizeof(sizeBuf), "Nodes: %zu", list.getSize());
    DrawBadge("List Size", sizeBuf, { controlsCard.x + controlsCard.width - 140.0f, controlsCard.y + 12.0f }, COLOR_ACCENT_SUCCESS);

    // Canvas Card
    float canvasY = startY + 86.0f;
    float canvasH = 398.0f;
    Rectangle canvas = { startX, canvasY, availW, canvasH };
    DrawCard(canvas, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    DrawText("SINGLY LINKED NODE TRAVERSAL CANVAS", static_cast<int>(canvas.x + 20.0f), static_cast<int>(canvas.y + 16.0f), 12, COLOR_TEXT_DIM);

    size_t count = 0;
    int* elements = list.toArray(count);

    if (count == 0) {
        DrawText("List is currently EMPTY",
                 static_cast<int>(canvas.x + canvas.width * 0.5f - 80.0f), static_cast<int>(canvas.y + 140.0f), 16, COLOR_TEXT_DIM);
        DrawText("Use the controls above to insert nodes at HEAD, TAIL, or specific indices.",
                 static_cast<int>(canvas.x + canvas.width * 0.5f - 220.0f), static_cast<int>(canvas.y + 168.0f), 12, COLOR_TEXT_DIM);
    } else {
        float nodeTotalW = 90.0f;
        float dataW = 56.0f;
        float ptrW = 34.0f;
        float nodeH = 48.0f;
        float arrowLen = 32.0f;
        float stepX = nodeTotalW + arrowLen;

        float startNodeX = canvas.x + 36.0f;
        float nodeY = canvas.y + 140.0f;

        for (size_t i = 0; i < count; ++i) {
            float nx = startNodeX + i * stepX;
            Rectangle dataRect = { nx, nodeY, dataW, nodeH };
            Rectangle ptrRect  = { nx + dataW, nodeY, ptrW, nodeH };
            Rectangle fullRect = { nx, nodeY, nodeTotalW, nodeH };

            Color nodeBg = COLOR_PANEL_LIGHT;
            Color borderC = COLOR_ACCENT_PRIMARY;

            // Highlight if part of search animation
            if (static_cast<int>(i) == searchHighlightIdx) {
                if (searchFound) {
                    nodeBg = ColorAlpha(COLOR_ACCENT_SUCCESS, 0.5f);
                    borderC = COLOR_ACCENT_SUCCESS;
                } else {
                    nodeBg = ColorAlpha(COLOR_ACCENT_WARNING, 0.4f);
                    borderC = COLOR_ACCENT_WARNING;
                }
            }

            // Draw data compartment
            DrawRectangleRounded(dataRect, 0.15f, 4, nodeBg);
            DrawRectangleRoundedLines(dataRect, 0.15f, 4, 1.5f, borderC);

            // Draw next* pointer compartment
            DrawRectangleRounded(ptrRect, 0.15f, 4, ColorAlpha(COLOR_PANEL_DARK, 0.8f));
            DrawRectangleRoundedLines(ptrRect, 0.15f, 4, 1.5f, borderC);

            // Pointer bullet dot inside ptrRect
            DrawCircle(static_cast<int>(ptrRect.x + ptrW * 0.5f), static_cast<int>(ptrRect.y + nodeH * 0.5f), 4.0f, COLOR_ACCENT_PRIMARY);

            // Node index badge above
            char idxStr[16];
            std::snprintf(idxStr, sizeof(idxStr), "idx %zu", i);
            DrawText(idxStr, static_cast<int>(fullRect.x + 4.0f), static_cast<int>(fullRect.y - 18.0f), 10, COLOR_TEXT_DIM);

            // Value text
            char valStr[32];
            std::snprintf(valStr, sizeof(valStr), "%d", elements[i]);
            int vw = MeasureText(valStr, 15);
            DrawText(valStr, static_cast<int>(dataRect.x + (dataW - vw) * 0.5f), static_cast<int>(dataRect.y + 16.0f), 15, COLOR_TEXT_PRIMARY);

            // Compartment label "data" and "next"
            DrawText("data", static_cast<int>(dataRect.x + 4.0f), static_cast<int>(dataRect.y + 4.0f), 9, COLOR_TEXT_MUTED);
            DrawText("next*", static_cast<int>(ptrRect.x + 3.0f), static_cast<int>(ptrRect.y + 4.0f), 9, COLOR_ACCENT_INFO);

            // Connecting Arrow to next node
            Vector2 arrowStart = { ptrRect.x + ptrW * 0.5f, ptrRect.y + nodeH * 0.5f };
            Vector2 arrowEnd   = { ptrRect.x + ptrW + arrowLen - 2.0f, ptrRect.y + nodeH * 0.5f };

            if (i < count - 1) {
                DrawArrow(arrowStart, arrowEnd, COLOR_ACCENT_INFO, 2.5f, 7.0f);
            } else {
                // Last node points to nullptr
                DrawArrow(arrowStart, arrowEnd, COLOR_TEXT_DIM, 2.5f, 7.0f);

                // NULL / nullptr badge
                Rectangle nullRect = { arrowEnd.x + 4.0f, nodeY + 10.0f, 62.0f, 28.0f };
                DrawRectangleRounded(nullRect, 0.25f, 4, COLOR_PANEL_LIGHT);
                DrawRectangleRoundedLines(nullRect, 0.25f, 4, 1.2f, COLOR_ACCENT_DANGER);
                DrawText("nullptr", static_cast<int>(nullRect.x + 10.0f), static_cast<int>(nullRect.y + 7.0f), 12, COLOR_ACCENT_DANGER);
            }

            // HEAD indicator above node 0
            if (i == 0) {
                Vector2 startP = { fullRect.x + nodeTotalW * 0.5f, fullRect.y - 48.0f };
                Vector2 endP   = { fullRect.x + nodeTotalW * 0.5f, fullRect.y - 20.0f };
                DrawArrow(startP, endP, COLOR_ACCENT_SUCCESS, 2.5f, 7.0f);
                DrawText("HEAD", static_cast<int>(startP.x - 16.0f), static_cast<int>(startP.y - 14.0f), 11, COLOR_ACCENT_SUCCESS);
            }

            // TAIL indicator below node count - 1
            if (i == count - 1) {
                Vector2 startP = { fullRect.x + nodeTotalW * 0.5f, fullRect.y + nodeH + 42.0f };
                Vector2 endP   = { fullRect.x + nodeTotalW * 0.5f, fullRect.y + nodeH + 6.0f };
                DrawArrow(startP, endP, COLOR_ACCENT_PURPLE, 2.5f, 7.0f);
                DrawText("TAIL", static_cast<int>(startP.x - 14.0f), static_cast<int>(startP.y + 6.0f), 11, COLOR_ACCENT_PURPLE);
            }
        }
    }

    delete[] elements;

    // CS Theory Notes Card inside canvas
    Rectangle notesBox = { canvas.x + 36.0f, canvas.y + 266.0f, canvas.width - 72.0f, 112.0f };
    DrawCard(notesBox, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.08f);
    DrawText("Memory Architecture & Pointer Manipulation (Viva Notes):",
             static_cast<int>(notesBox.x + 16.0f), static_cast<int>(notesBox.y + 12.0f), 13, COLOR_TEXT_PRIMARY);
    DrawText("* Each node is dynamically allocated on the heap (sizeof(Node) = sizeof(int) + sizeof(Node*) = 16 bytes on 64-bit).\n* Head insertion is O(1) as it directly updates the head pointer without shifting subsequent elements.\n* Deletion by value requires linear traversal O(n) keeping track of previous node to bridge pointers (prev->next = curr->next).\n* Destructor traverses from head to end, calling delete on each node to ensure ZERO memory leaks.",
             static_cast<int>(notesBox.x + 16.0f), static_cast<int>(notesBox.y + 34.0f), 11, COLOR_TEXT_MUTED);

    // Complexity and Status Console side-by-side at bottom
    float bottomY = canvasY + canvasH + 12.0f;
    float halfW = (availW - 16.0f) * 0.5f;

    complexity.draw({ startX, bottomY, halfW, 82.0f });
    console.draw({ startX + halfW + 16.0f, bottomY, halfW, 82.0f });
}

} // namespace gui

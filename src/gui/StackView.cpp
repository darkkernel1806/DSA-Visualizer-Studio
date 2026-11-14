#include "gui/StackView.hpp"
#include <cstdio>
#include <cstdlib>

namespace gui {

StackView::StackView()
    : valInput({0, 0, 90, 34}, "Value", true, 5),
      pushBtn({0, 0, 84, 34}, "Push", COLOR_ACCENT_PRIMARY),
      popBtn({0, 0, 84, 34}, "Pop", COLOR_ACCENT_DANGER),
      peekBtn({0, 0, 84, 34}, "Peek", COLOR_ACCENT_INFO),
      clearBtn({0, 0, 84, 34}, "Clear", COLOR_PANEL_LIGHT),
      randomBtn({0, 0, 110, 34}, "+ Random", COLOR_ACCENT_SUCCESS),
      peekHighlightIdx(-1), highlightTimer(0.0f) {
    lastPoppedValue[0] = '\0';
    complexity.set("O(1)", "O(1)", "O(1)", "O(n)", "All top operations (Push/Pop/Peek) run in constant O(1) time.");

    // Pre-populate with sample stack data for immediate visual impact
    stack.push(15);
    stack.push(42);
    stack.push(88);
    console.log("Stack initialized with sample elements [15, 42, 88]. Top is 88.", StatusType::INFO);
}

void StackView::reset() {
    stack.clear();
    peekHighlightIdx = -1;
    highlightTimer = 0.0f;
    lastPoppedValue[0] = '\0';
    valInput.clear();
    console.log("Stack reset. Ready.", StatusType::INFO);
}

void StackView::update(float dt) {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 86.0f;

    // Layout controls horizontally
    valInput.setBounds({ startX, startY, 90, 34 });
    pushBtn.setBounds({ startX + 100.0f, startY, 80, 34 });
    popBtn.setBounds({ startX + 188.0f, startY, 80, 34 });
    peekBtn.setBounds({ startX + 276.0f, startY, 80, 34 });
    randomBtn.setBounds({ startX + 364.0f, startY, 96, 34 });
    clearBtn.setBounds({ startX + 468.0f, startY, 80, 34 });

    valInput.update();
    pushBtn.update();
    popBtn.update();
    peekBtn.update();
    randomBtn.update();
    clearBtn.update();

    if (highlightTimer > 0.0f) {
        highlightTimer -= dt;
        if (highlightTimer <= 0.0f) {
            peekHighlightIdx = -1;
        }
    }

    // Push Action
    if (pushBtn.isClicked()) {
        int val = 0;
        if (valInput.getIntValue(val)) {
            if (stack.getSize() >= 12) {
                console.log("Stack display limit reached (max 12 for visual layout).", StatusType::WARNING);
            } else {
                stack.push(val);
                char msg[96];
                std::snprintf(msg, sizeof(msg), "PUSH operation: %d placed on top. Size is now %zu.", val, stack.getSize());
                console.log(msg, StatusType::SUCCESS);
                valInput.clear();
            }
        } else {
            console.log("Invalid input! Please enter an integer value to push.", StatusType::DANGER);
        }
    }

    // Pop Action
    if (popBtn.isClicked()) {
        int popped = 0;
        if (stack.pop(popped)) {
            std::snprintf(lastPoppedValue, sizeof(lastPoppedValue), "%d", popped);
            char msg[96];
            std::snprintf(msg, sizeof(msg), "POP operation: %d removed from top. Size is now %zu.", popped, stack.getSize());
            console.log(msg, StatusType::SUCCESS);
        } else {
            console.log("STACK UNDERFLOW! Cannot pop from an empty stack.", StatusType::DANGER);
        }
    }

    // Peek Action
    if (peekBtn.isClicked()) {
        int topVal = 0;
        if (stack.peek(topVal)) {
            peekHighlightIdx = 0;
            highlightTimer = 1.8f;
            char msg[96];
            std::snprintf(msg, sizeof(msg), "PEEK operation: Top element is %d (stack unchanged).", topVal);
            console.log(msg, StatusType::INFO);
        } else {
            console.log("Stack is empty! Nothing to peek.", StatusType::WARNING);
        }
    }

    // Random Action
    if (randomBtn.isClicked()) {
        if (stack.getSize() >= 12) {
            console.log("Stack visual display limit reached.", StatusType::WARNING);
        } else {
            int r = (GetRandomValue(10, 99));
            stack.push(r);
            char msg[96];
            std::snprintf(msg, sizeof(msg), "Pushed random value %d onto stack.", r);
            console.log(msg, StatusType::SUCCESS);
        }
    }

    // Clear Action
    if (clearBtn.isClicked()) {
        stack.clear();
        peekHighlightIdx = -1;
        console.log("Stack cleared. All nodes dynamically freed.", StatusType::INFO);
    }
}

void StackView::draw() {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 86.0f;
    float availW = WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f;

    // Controls bar container card
    Rectangle controlsCard = { startX, startY - 8.0f, availW, 50.0f };
    DrawCard(controlsCard, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    valInput.draw();
    pushBtn.draw();
    popBtn.draw();
    peekBtn.draw();
    randomBtn.draw();
    clearBtn.draw();

    // Stats Pill on the right of controls
    char sizeBuf[48];
    std::snprintf(sizeBuf, sizeof(sizeBuf), "Elements: %zu", stack.getSize());
    DrawBadge("Stack Size", sizeBuf, { controlsCard.x + controlsCard.width - 160.0f, controlsCard.y + 13.0f }, COLOR_ACCENT_PRIMARY);

    // Canvas Card
    float canvasY = startY + 54.0f;
    float canvasH = 430.0f;
    Rectangle canvas = { startX, canvasY, availW, canvasH };
    DrawCard(canvas, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    // Canvas Sub-labels
    DrawText("LIFO STACK CONTAINER", static_cast<int>(canvas.x + 20.0f), static_cast<int>(canvas.y + 16.0f), 12, COLOR_TEXT_DIM);

    // Draw Stack Beaker
    float beakerW = 220.0f;
    float beakerH = 340.0f;
    float beakerX = canvas.x + (canvas.width - beakerW) * 0.5f;
    float beakerY = canvas.y + 60.0f;

    // Beaker walls (left, right, bottom)
    float wallThick = 4.0f;
    Color beakerColor = COLOR_PANEL_LIGHT;
    DrawLineEx({ beakerX, beakerY }, { beakerX, beakerY + beakerH }, wallThick, beakerColor);
    DrawLineEx({ beakerX + beakerW, beakerY }, { beakerX + beakerW, beakerY + beakerH }, wallThick, beakerColor);
    DrawLineEx({ beakerX - 2.0f, beakerY + beakerH }, { beakerX + beakerW + 2.0f, beakerY + beakerH }, wallThick + 2.0f, beakerColor);

    // Bottom Base platform
    Rectangle basePlat = { beakerX - 16.0f, beakerY + beakerH, beakerW + 32.0f, 8.0f };
    DrawRectangleRounded(basePlat, 0.5f, 4, beakerColor);

    // Draw Stack Elements
    size_t count = 0;
    int* elements = stack.toArray(count);

    float elemH = 26.0f;
    float elemGap = 4.0f;
    float elemW = beakerW - 16.0f;
    float elemX = beakerX + 8.0f;

    if (count == 0) {
        DrawText("Stack is currently EMPTY",
                 static_cast<int>(beakerX + 22.0f), static_cast<int>(beakerY + beakerH * 0.5f - 10.0f),
                 14, COLOR_TEXT_DIM);
        DrawText("(LIFO: Push elements to populate)",
                 static_cast<int>(beakerX + 14.0f), static_cast<int>(beakerY + beakerH * 0.5f + 12.0f),
                 11, COLOR_TEXT_DIM);
    } else {
        // elements[0] is TOP, elements[count - 1] is BOTTOM
        for (size_t i = 0; i < count; ++i) {
            // Render from bottom to top
            // i=0 is top, drawn at the highest level
            // i=count-1 is bottom, drawn at the lowest level
            size_t fromBottom = count - 1 - i;
            float elemY = beakerY + beakerH - (fromBottom + 1) * (elemH + elemGap);

            Rectangle elemRect = { elemX, elemY, elemW, elemH };

            Color elemBg = COLOR_PANEL_LIGHT;
            Color borderC = COLOR_ACCENT_PRIMARY;

            // Highlight top node or peek node
            if (i == 0) {
                elemBg = ColorAlpha(COLOR_ACCENT_PRIMARY, 0.35f);
                borderC = COLOR_ACCENT_HOVER;
            }
            if (i == 0 && peekHighlightIdx == 0) {
                elemBg = ColorAlpha(COLOR_ACCENT_SUCCESS, 0.45f);
                borderC = COLOR_ACCENT_SUCCESS;
            }

            DrawRectangleRounded(elemRect, 0.25f, 4, elemBg);
            DrawRectangleRoundedLines(elemRect, 0.25f, 4, 1.5f, borderC);

            // Node index badge on left
            char idxText[16];
            std::snprintf(idxText, sizeof(idxText), "[%zu]", fromBottom);
            DrawText(idxText, static_cast<int>(elemRect.x + 8.0f), static_cast<int>(elemRect.y + 6.0f), 11, COLOR_TEXT_DIM);

            // Node Value centered
            char valText[32];
            std::snprintf(valText, sizeof(valText), "%d", elements[i]);
            int valW = MeasureText(valText, 14);
            DrawText(valText, static_cast<int>(elemRect.x + (elemRect.width - valW) * 0.5f),
                     static_cast<int>(elemRect.y + 6.0f), 14, COLOR_TEXT_PRIMARY);

            // Draw Pointer for Top Element
            if (i == 0) {
                Vector2 arrowStart = { elemRect.x + elemRect.width + 50.0f, elemRect.y + elemRect.height * 0.5f };
                Vector2 arrowEnd   = { elemRect.x + elemRect.width + 4.0f, elemRect.y + elemRect.height * 0.5f };
                DrawArrow(arrowStart, arrowEnd, COLOR_ACCENT_WARNING, 3.0f, 9.0f);

                DrawText("TOP", static_cast<int>(arrowStart.x + 8.0f), static_cast<int>(arrowStart.y - 8.0f), 13, COLOR_ACCENT_WARNING);
            }
        }
    }

    delete[] elements;

    // Operation explanation card on left side of beaker
    Rectangle infoBox = { canvas.x + 24.0f, canvas.y + 70.0f, 210.0f, 240.0f };
    DrawCard(infoBox, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.08f);
    DrawText("LIFO Operations", static_cast<int>(infoBox.x + 14.0f), static_cast<int>(infoBox.y + 12.0f), 14, COLOR_TEXT_PRIMARY);
    DrawText("* PUSH(val): Allocates a new\n  node and points its next to\n  current top. Sets top = node.\n  Time: O(1)\n\n* POP(): Unlinks current top\n  node, updates top = top->next,\n  frees memory.\n  Time: O(1)\n\n* PEEK(): Reads top->data.\n  Time: O(1)",
             static_cast<int>(infoBox.x + 14.0f), static_cast<int>(infoBox.y + 36.0f), 11, COLOR_TEXT_MUTED);

    // Complexity and Status Console side-by-side at bottom
    float bottomY = canvasY + canvasH + 12.0f;
    float halfW = (availW - 16.0f) * 0.5f;

    complexity.draw({ startX, bottomY, halfW, 82.0f });
    console.draw({ startX + halfW + 16.0f, bottomY, halfW, 82.0f });
}

} // namespace gui

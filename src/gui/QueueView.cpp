#include "gui/QueueView.hpp"
#include <cstdio>
#include <cstdlib>

namespace gui {

QueueView::QueueView()
    : valInput({0, 0, 90, 34}, "Value", true, 5),
      enqueueBtn({0, 0, 90, 34}, "Enqueue", COLOR_ACCENT_PRIMARY),
      dequeueBtn({0, 0, 90, 34}, "Dequeue", COLOR_ACCENT_DANGER),
      frontBtn({0, 0, 80, 34}, "Front", COLOR_ACCENT_INFO),
      clearBtn({0, 0, 80, 34}, "Clear", COLOR_PANEL_LIGHT),
      randomBtn({0, 0, 110, 34}, "+ Random", COLOR_ACCENT_SUCCESS),
      frontHighlightTimer(0.0f) {
    lastDequeuedValue[0] = '\0';
    complexity.set("O(1)", "O(1)", "O(1)", "O(n)", "All FIFO queue operations run in guaranteed O(1) constant time.");

    // Pre-populate with sample elements
    queue.enqueue(10);
    queue.enqueue(25);
    queue.enqueue(60);
    queue.enqueue(95);
    console.log("Queue initialized with sample elements [10, 25, 60, 95]. Front is 10, Rear is 95.", StatusType::INFO);
}

void QueueView::reset() {
    queue.clear();
    frontHighlightTimer = 0.0f;
    lastDequeuedValue[0] = '\0';
    valInput.clear();
    console.log("Queue reset. Ready.", StatusType::INFO);
}

void QueueView::update(float dt) {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 86.0f;

    valInput.setBounds({ startX, startY, 90, 34 });
    enqueueBtn.setBounds({ startX + 100.0f, startY, 90, 34 });
    dequeueBtn.setBounds({ startX + 198.0f, startY, 90, 34 });
    frontBtn.setBounds({ startX + 296.0f, startY, 80, 34 });
    randomBtn.setBounds({ startX + 384.0f, startY, 96, 34 });
    clearBtn.setBounds({ startX + 488.0f, startY, 80, 34 });

    valInput.update();
    enqueueBtn.update();
    dequeueBtn.update();
    frontBtn.update();
    randomBtn.update();
    clearBtn.update();

    if (frontHighlightTimer > 0.0f) {
        frontHighlightTimer -= dt;
    }

    // Enqueue Action
    if (enqueueBtn.isClicked()) {
        int val = 0;
        if (valInput.getIntValue(val)) {
            if (queue.getSize() >= 10) {
                console.log("Queue display capacity reached (max 10 elements in visual pipeline).", StatusType::WARNING);
            } else {
                queue.enqueue(val);
                char msg[96];
                std::snprintf(msg, sizeof(msg), "ENQUEUE operation: %d entered at REAR. Size: %zu.", val, queue.getSize());
                console.log(msg, StatusType::SUCCESS);
                valInput.clear();
            }
        } else {
            console.log("Invalid input! Please enter an integer value to enqueue.", StatusType::DANGER);
        }
    }

    // Dequeue Action
    if (dequeueBtn.isClicked()) {
        int dequeued = 0;
        if (queue.dequeue(dequeued)) {
            std::snprintf(lastDequeuedValue, sizeof(lastDequeuedValue), "%d", dequeued);
            char msg[96];
            std::snprintf(msg, sizeof(msg), "DEQUEUE operation: %d removed from FRONT. Size: %zu.", dequeued, queue.getSize());
            console.log(msg, StatusType::SUCCESS);
        } else {
            console.log("QUEUE UNDERFLOW! Cannot dequeue from an empty queue.", StatusType::DANGER);
        }
    }

    // Front Action
    if (frontBtn.isClicked()) {
        int fVal = 0;
        if (queue.front(fVal)) {
            frontHighlightTimer = 1.8f;
            char msg[96];
            std::snprintf(msg, sizeof(msg), "FRONT peek: %d is currently at the front of the queue.", fVal);
            console.log(msg, StatusType::INFO);
        } else {
            console.log("Queue is empty! No front element.", StatusType::WARNING);
        }
    }

    // Random Action
    if (randomBtn.isClicked()) {
        if (queue.getSize() >= 10) {
            console.log("Queue visual display limit reached.", StatusType::WARNING);
        } else {
            int r = GetRandomValue(10, 99);
            queue.enqueue(r);
            char msg[96];
            std::snprintf(msg, sizeof(msg), "Enqueued random value %d.", r);
            console.log(msg, StatusType::SUCCESS);
        }
    }

    // Clear Action
    if (clearBtn.isClicked()) {
        queue.clear();
        frontHighlightTimer = 0.0f;
        console.log("Queue cleared. All dynamically allocated nodes freed.", StatusType::INFO);
    }
}

void QueueView::draw() {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 86.0f;
    float availW = WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f;

    // Controls bar container card
    Rectangle controlsCard = { startX, startY - 8.0f, availW, 50.0f };
    DrawCard(controlsCard, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    valInput.draw();
    enqueueBtn.draw();
    dequeueBtn.draw();
    frontBtn.draw();
    randomBtn.draw();
    clearBtn.draw();

    // Stats Pill
    char sizeBuf[48];
    std::snprintf(sizeBuf, sizeof(sizeBuf), "Elements: %zu", queue.getSize());
    DrawBadge("Queue Size", sizeBuf, { controlsCard.x + controlsCard.width - 160.0f, controlsCard.y + 13.0f }, COLOR_ACCENT_INFO);

    // Canvas Card
    float canvasY = startY + 54.0f;
    float canvasH = 430.0f;
    Rectangle canvas = { startX, canvasY, availW, canvasH };
    DrawCard(canvas, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    DrawText("FIFO QUEUE SEQUENTIAL PIPELINE", static_cast<int>(canvas.x + 20.0f), static_cast<int>(canvas.y + 16.0f), 12, COLOR_TEXT_DIM);

    // Pipeline Channel Bounds
    float pipeY = canvas.y + 160.0f;
    float pipeH = 80.0f;
    float pipeX = canvas.x + 40.0f;
    float pipeW = canvas.width - 80.0f;

    // Boundary guides for pipeline (top & bottom rails)
    DrawLineEx({ pipeX, pipeY }, { pipeX + pipeW, pipeY }, 2.0f, COLOR_PANEL_LIGHT);
    DrawLineEx({ pipeX, pipeY + pipeH }, { pipeX + pipeW, pipeY + pipeH }, 2.0f, COLOR_PANEL_LIGHT);

    // Flow indicators
    DrawText("<< EXIT (DEQUEUE / FRONT)", static_cast<int>(pipeX), static_cast<int>(pipeY - 24.0f), 12, COLOR_ACCENT_DANGER);
    DrawText("ENTRY (ENQUEUE / REAR) <<", static_cast<int>(pipeX + pipeW - 190.0f), static_cast<int>(pipeY - 24.0f), 12, COLOR_ACCENT_PRIMARY);

    // Elements
    size_t count = 0;
    int* elements = queue.toArray(count);

    if (count == 0) {
        DrawText("Queue is currently EMPTY",
                 static_cast<int>(canvas.x + canvas.width * 0.5f - 90.0f), static_cast<int>(pipeY + 30.0f), 16, COLOR_TEXT_DIM);
        DrawText("(FIFO: Elements enter from REAR on right, leave from FRONT on left)",
                 static_cast<int>(canvas.x + canvas.width * 0.5f - 210.0f), static_cast<int>(pipeY + 54.0f), 12, COLOR_TEXT_DIM);
    } else {
        float nodeW = 68.0f;
        float nodeH = 50.0f;
        float nodeGap = 16.0f;
        float startNodeX = pipeX + 30.0f;

        for (size_t i = 0; i < count; ++i) {
            float nx = startNodeX + i * (nodeW + nodeGap);
            float ny = pipeY + (pipeH - nodeH) * 0.5f;

            Rectangle nodeRect = { nx, ny, nodeW, nodeH };

            Color bg = COLOR_PANEL_LIGHT;
            Color border = COLOR_ACCENT_INFO;

            if (i == 0 && frontHighlightTimer > 0.0f) {
                bg = ColorAlpha(COLOR_ACCENT_SUCCESS, 0.45f);
                border = COLOR_ACCENT_SUCCESS;
            } else if (i == 0) {
                border = COLOR_ACCENT_DANGER; // Front accent
            } else if (i == count - 1) {
                border = COLOR_ACCENT_PRIMARY; // Rear accent
            }

            DrawRectangleRounded(nodeRect, 0.25f, 4, bg);
            DrawRectangleRoundedLines(nodeRect, 0.25f, 4, 1.8f, border);

            // Node index badge
            char idxText[16];
            std::snprintf(idxText, sizeof(idxText), "[%zu]", i);
            DrawText(idxText, static_cast<int>(nodeRect.x + 6.0f), static_cast<int>(nodeRect.y + 4.0f), 10, COLOR_TEXT_DIM);

            // Node Value
            char valText[32];
            std::snprintf(valText, sizeof(valText), "%d", elements[i]);
            int valW = MeasureText(valText, 16);
            DrawText(valText, static_cast<int>(nodeRect.x + (nodeRect.width - valW) * 0.5f),
                     static_cast<int>(nodeRect.y + 18.0f), 16, COLOR_TEXT_PRIMARY);

            // Inter-node directional arrow
            if (i < count - 1) {
                Vector2 arrowStart = { nodeRect.x + nodeRect.width + 2.0f, nodeRect.y + nodeH * 0.5f };
                Vector2 arrowEnd   = { nodeRect.x + nodeRect.width + nodeGap - 2.0f, nodeRect.y + nodeH * 0.5f };
                DrawArrow(arrowEnd, arrowStart, COLOR_TEXT_DIM, 2.0f, 6.0f); // arrows point left towards exit
            }

            // FRONT indicator above element 0
            if (i == 0) {
                Vector2 startP = { nodeRect.x + nodeW * 0.5f, nodeRect.y - 36.0f };
                Vector2 endP   = { nodeRect.x + nodeW * 0.5f, nodeRect.y - 4.0f };
                DrawArrow(startP, endP, COLOR_ACCENT_DANGER, 2.5f, 7.0f);
                DrawText("FRONT", static_cast<int>(startP.x - 20.0f), static_cast<int>(startP.y - 14.0f), 11, COLOR_ACCENT_DANGER);
            }

            // REAR indicator above element (count - 1)
            if (i == count - 1) {
                Vector2 startP = { nodeRect.x + nodeW * 0.5f, nodeRect.y - 36.0f };
                Vector2 endP   = { nodeRect.x + nodeW * 0.5f, nodeRect.y - 4.0f };
                DrawArrow(startP, endP, COLOR_ACCENT_PRIMARY, 2.5f, 7.0f);
                DrawText("REAR", static_cast<int>(startP.x - 16.0f), static_cast<int>(startP.y - 14.0f), 11, COLOR_ACCENT_PRIMARY);
            }
        }
    }

    delete[] elements;

    // Operation explanation card
    Rectangle infoBox = { canvas.x + 40.0f, canvas.y + 280.0f, canvas.width - 80.0f, 110.0f };
    DrawCard(infoBox, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.08f);
    DrawText("FIFO Architecture Insights (For Practical Viva):",
             static_cast<int>(infoBox.x + 16.0f), static_cast<int>(infoBox.y + 12.0f), 13, COLOR_TEXT_PRIMARY);
    DrawText("* ENQUEUE(val): Inserts a new node at the REAR pointer. Updates rear->next = node, rear = node. Time: O(1)\n* DEQUEUE(): Extracts element from FRONT. Updates front = front->next. If front becomes null, sets rear = null. Time: O(1)\n* FRONT(): Inspects front->data without altering pointers. Time: O(1)\n* Singly-linked list implementation guarantees dynamic scaling without array reallocation overhead.",
             static_cast<int>(infoBox.x + 16.0f), static_cast<int>(infoBox.y + 34.0f), 11, COLOR_TEXT_MUTED);

    // Complexity and Status Console side-by-side at bottom
    float bottomY = canvasY + canvasH + 12.0f;
    float halfW = (availW - 16.0f) * 0.5f;

    complexity.draw({ startX, bottomY, halfW, 82.0f });
    console.draw({ startX + halfW + 16.0f, bottomY, halfW, 82.0f });
}

} // namespace gui

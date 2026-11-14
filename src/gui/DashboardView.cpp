#include "gui/DashboardView.hpp"
#include "gui/Application.hpp"

namespace gui {

DashboardView::DashboardView(Application* application)
    : app(application),
      stackBtn({0, 0, 110, 32}, "Open Stack", COLOR_ACCENT_PRIMARY),
      queueBtn({0, 0, 110, 32}, "Open Queue", COLOR_ACCENT_INFO),
      listBtn({0, 0, 110, 32}, "Open List", COLOR_ACCENT_SUCCESS),
      bstBtn({0, 0, 110, 32}, "Open BST", COLOR_ACCENT_PURPLE),
      sortBtn({0, 0, 110, 32}, "Open Sorter", COLOR_ACCENT_WARNING) {}

void DashboardView::update(float /*dt*/) {
    stackBtn.update();
    queueBtn.update();
    listBtn.update();
    bstBtn.update();
    sortBtn.update();

    if (stackBtn.isClicked()) app->setView(ViewType::STACK);
    if (queueBtn.isClicked()) app->setView(ViewType::QUEUE);
    if (listBtn.isClicked()) app->setView(ViewType::LINKED_LIST);
    if (bstBtn.isClicked()) app->setView(ViewType::BST);
    if (sortBtn.isClicked()) app->setView(ViewType::SORTING);
}

void DashboardView::draw() {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 86.0f;
    float availW = WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f;

    // Welcome Banner Card
    Rectangle bannerRect = { startX, startY, availW, 110.0f };
    DrawCard(bannerRect, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    // Accent line
    DrawRectangleRounded({ bannerRect.x + 12.0f, bannerRect.y + 16.0f, 4.0f, 78.0f }, 0.5f, 4, COLOR_ACCENT_PRIMARY);

    DrawText("DSA Visualizer Studio - Interactive Simulation Suite",
             static_cast<int>(bannerRect.x + 28.0f), static_cast<int>(bannerRect.y + 18.0f), 20, COLOR_TEXT_PRIMARY);
    DrawText("A production-grade educational desktop application built with modern C++17 and raylib.",
             static_cast<int>(bannerRect.x + 28.0f), static_cast<int>(bannerRect.y + 44.0f), 13, COLOR_TEXT_MUTED);
    DrawText("Features modular from-scratch data structures (zero STL in core), pointer manipulation, and interactive algorithms.",
             static_cast<int>(bannerRect.x + 28.0f), static_cast<int>(bannerRect.y + 64.0f), 12, COLOR_TEXT_DIM);

    // Technology Pills
    DrawBadge("Language", "C++17", { bannerRect.x + 28.0f, bannerRect.y + 82.0f }, COLOR_ACCENT_PRIMARY);
    DrawBadge("GUI", "raylib 5.0", { bannerRect.x + 160.0f, bannerRect.y + 82.0f }, COLOR_ACCENT_INFO);
    DrawBadge("Compiler", "GCC UCRT64", { bannerRect.x + 288.0f, bannerRect.y + 82.0f }, COLOR_ACCENT_SUCCESS);
    DrawBadge("Core DSA", "100% Custom", { bannerRect.x + 442.0f, bannerRect.y + 82.0f }, COLOR_ACCENT_WARNING);

    // Grid of 5 Module Cards: 3 in top row, 2 in bottom row
    float cardGap = 16.0f;
    float cardW3 = (availW - cardGap * 2.0f) / 3.0f;
    float cardH = 180.0f;
    float row1Y = startY + 126.0f;

    // Helper lambda to draw module card
    auto drawModuleCard = [&](Rectangle rect, const char* title, const char* principle,
                              const char* desc, const char* ops, Button& btn, Color accent) {
        DrawCard(rect, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.08f);

        // Header accent dot
        DrawCircle(static_cast<int>(rect.x + 18.0f), static_cast<int>(rect.y + 20.0f), 5.0f, accent);
        DrawText(title, static_cast<int>(rect.x + 30.0f), static_cast<int>(rect.y + 12.0f), 16, COLOR_TEXT_PRIMARY);
        DrawText(principle, static_cast<int>(rect.x + 16.0f), static_cast<int>(rect.y + 36.0f), 11, accent);

        DrawText(desc, static_cast<int>(rect.x + 16.0f), static_cast<int>(rect.y + 56.0f), 11, COLOR_TEXT_MUTED);

        // Operations list
        DrawText("Operations:", static_cast<int>(rect.x + 16.0f), static_cast<int>(rect.y + 98.0f), 11, COLOR_TEXT_DIM);
        DrawText(ops, static_cast<int>(rect.x + 16.0f), static_cast<int>(rect.y + 114.0f), 11, COLOR_TEXT_PRIMARY);

        // Position & Draw button
        btn.setBounds({ rect.x + 16.0f, rect.y + rect.height - 44.0f, rect.width - 32.0f, 32.0f });
        btn.draw();
    };

    // Card 1: Stack
    Rectangle rStack = { startX, row1Y, cardW3, cardH };
    drawModuleCard(rStack, "Stack Visualizer", "LIFO Principle",
                   "Dynamic singly linked node stack.\nInstant O(1) top access with\nunderflow protection.",
                   "Push, Pop, Peek, Clear", stackBtn, COLOR_ACCENT_PRIMARY);

    // Card 2: Queue
    Rectangle rQueue = { startX + cardW3 + cardGap, row1Y, cardW3, cardH };
    drawModuleCard(rQueue, "Queue Visualizer", "FIFO Principle",
                   "Sequential pipeline with dynamic\nlinked nodes, Front and Rear\npointers for O(1) ops.",
                   "Enqueue, Dequeue, Front, Clear", queueBtn, COLOR_ACCENT_INFO);

    // Card 3: Linked List
    Rectangle rList = { startX + (cardW3 + cardGap) * 2.0f, row1Y, cardW3, cardH };
    drawModuleCard(rList, "Singly Linked List", "Linear Pointer Chain",
                   "Linked node structures with data\nand next pointers. Demonstrates\ninsertion, deletion & search.",
                   "Insert H/T/Idx, Del, Search", listBtn, COLOR_ACCENT_SUCCESS);

    // Row 2: BST and Sorting
    float row2Y = row1Y + cardH + cardGap;
    float cardW2 = (availW - cardGap) * 0.5f;

    // Card 4: BST
    Rectangle rBST = { startX, row2Y, cardW2, cardH };
    drawModuleCard(rBST, "Binary Search Tree (BST)", "Hierarchical Tree (L < Root < R)",
                   "Dynamic recursive 2D tree layout with logarithmic search.\nIncludes interactive Inorder, Preorder, and Postorder traversals\nwith animated step-by-step visited node path.",
                   "Insert, Delete (3 cases), Search, In/Pre/Postorder", bstBtn, COLOR_ACCENT_PURPLE);

    // Card 5: Sorting Visualizer
    Rectangle rSort = { startX + cardW2 + cardGap, row2Y, cardW2, cardH };
    drawModuleCard(rSort, "Sorting Algorithms Visualizer", "Comparative Analysis (O(n) to O(n^2))",
                   "Real-time animated bar chart visualizer for Bubble Sort,\nSelection Sort, and Insertion Sort. Features play/pause, step,\ncustom array sizes, speed slider, and comparison counters.",
                   "Bubble Sort, Selection Sort, Insertion Sort", sortBtn, COLOR_ACCENT_WARNING);

    // Quick Viva Tips Footer Card
    float footerY = row2Y + cardH + cardGap;
    float footerH = 64.0f;
    Rectangle footerRect = { startX, footerY, availW, footerH };
    DrawCard(footerRect, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    DrawCircle(static_cast<int>(footerRect.x + 18.0f), static_cast<int>(footerRect.y + 20.0f), 4.0f, COLOR_ACCENT_SUCCESS);
    DrawText("College Viva & Practical Exam Ready",
             static_cast<int>(footerRect.x + 30.0f), static_cast<int>(footerRect.y + 12.0f), 13, COLOR_TEXT_PRIMARY);
    DrawText("All data structures adhere strictly to raw pointer dynamic allocation with zero memory leaks. Full viva notes included in VIVA_PREPARATION.md.",
             static_cast<int>(footerRect.x + 30.0f), static_cast<int>(footerRect.y + 34.0f), 12, COLOR_TEXT_MUTED);
}

} // namespace gui

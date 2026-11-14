#include "gui/Application.hpp"
#include "gui/DashboardView.hpp"
#include "gui/StackView.hpp"
#include "gui/QueueView.hpp"
#include "gui/LinkedListView.hpp"
#include "gui/BSTView.hpp"
#include "gui/SortingView.hpp"

#include <cstdio>

namespace gui {

Application::Application()
    : running(true),
      currentViewType(ViewType::DASHBOARD),
      hoveredNavIdx(-1) {

    // Setup navigation menu items
    navItems[0] = { ViewType::DASHBOARD,   "Dashboard",      "#"  };
    navItems[1] = { ViewType::STACK,       "Stack (LIFO)",   "="  };
    navItems[2] = { ViewType::QUEUE,       "Queue (FIFO)",   ">"  };
    navItems[3] = { ViewType::LINKED_LIST, "Linked List",    "->" };
    navItems[4] = { ViewType::BST,         "Binary Tree",    "^"  };
    navItems[5] = { ViewType::SORTING,     "Sort Visualizer","|"  };

    // Instantiate views
    dashboardView  = std::make_unique<DashboardView>(this);
    stackView      = std::make_unique<StackView>();
    queueView      = std::make_unique<QueueView>();
    linkedListView = std::make_unique<LinkedListView>();
    bstView        = std::make_unique<BSTView>();
    sortingView    = std::make_unique<SortingView>();
}

Application::~Application() {
    CloseWindow();
}

void Application::init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DSA Visualizer Studio - C++17 Desktop Suite");
    SetTargetFPS(TARGET_FPS);
}

void Application::setView(ViewType type) {
    currentViewType = type;
}

View* Application::getCurrentView() {
    switch (currentViewType) {
        case ViewType::DASHBOARD:   return dashboardView.get();
        case ViewType::STACK:       return stackView.get();
        case ViewType::QUEUE:       return queueView.get();
        case ViewType::LINKED_LIST: return linkedListView.get();
        case ViewType::BST:         return bstView.get();
        case ViewType::SORTING:     return sortingView.get();
    }
    return dashboardView.get();
}

void Application::drawSidebar() {
    Rectangle sidebarRect = { 0, 0, SIDEBAR_WIDTH, static_cast<float>(WINDOW_HEIGHT) };
    DrawRectangleRec(sidebarRect, COLOR_PANEL_DARK);
    DrawLineEx({ SIDEBAR_WIDTH, 0 }, { SIDEBAR_WIDTH, static_cast<float>(WINDOW_HEIGHT) }, 1.5f, COLOR_PANEL_LIGHT);

    // App Brand Logo & Title Box
    Rectangle logoBox = { 18.0f, 18.0f, 36.0f, 36.0f };
    DrawRectangleRounded(logoBox, 0.28f, 4, COLOR_ACCENT_PRIMARY);
    DrawText("DS", static_cast<int>(logoBox.x + 8.0f), static_cast<int>(logoBox.y + 10.0f), 17, COLOR_TEXT_PRIMARY);

    DrawText("DSA Studio", static_cast<int>(logoBox.x + 46.0f), static_cast<int>(logoBox.y + 3.0f), 16, COLOR_TEXT_PRIMARY);
    DrawText("VISUALIZER v1.0", static_cast<int>(logoBox.x + 46.0f), static_cast<int>(logoBox.y + 22.0f), 10, COLOR_ACCENT_INFO);

    // Navigation Section Header
    float navStartY = 82.0f;
    DrawText("MODULE NAVIGATION", 22, static_cast<int>(navStartY), 10, COLOR_TEXT_DIM);

    Vector2 mouse = GetMousePosition();
    hoveredNavIdx = -1;

    float itemY = navStartY + 18.0f;
    float itemH = 42.0f;
    float itemW = SIDEBAR_WIDTH - 28.0f;

    for (size_t i = 0; i < NAV_COUNT; ++i) {
        Rectangle itemRect = { 14.0f, itemY, itemW, itemH };
        bool isCurrent = (navItems[i].type == currentViewType);
        bool isHovered = CheckCollisionPointRec(mouse, itemRect);

        if (isHovered) {
            hoveredNavIdx = static_cast<int>(i);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                setView(navItems[i].type);
            }
        }

        // Draw item container
        if (isCurrent) {
            DrawRectangleRounded(itemRect, 0.2f, 4, ColorAlpha(COLOR_ACCENT_PRIMARY, 0.22f));
            DrawRectangleRoundedLines(itemRect, 0.2f, 4, 1.2f, ColorAlpha(COLOR_ACCENT_PRIMARY, 0.8f));

            // Left vertical active pill
            Rectangle pill = { itemRect.x + 3.0f, itemRect.y + 8.0f, 3.5f, itemRect.height - 16.0f };
            DrawRectangleRounded(pill, 0.5f, 4, COLOR_ACCENT_PRIMARY);
        } else if (isHovered) {
            DrawRectangleRounded(itemRect, 0.2f, 4, ColorAlpha(COLOR_PANEL_LIGHT, 0.45f));
        }

        // Icon badge
        Rectangle iconRect = { itemRect.x + 12.0f, itemRect.y + 11.0f, 20.0f, 20.0f };
        DrawRectangleRounded(iconRect, 0.25f, 4, isCurrent ? COLOR_ACCENT_PRIMARY : COLOR_PANEL_LIGHT);
        DrawText(navItems[i].icon, static_cast<int>(iconRect.x + 5.0f), static_cast<int>(iconRect.y + 3.0f), 12, COLOR_TEXT_PRIMARY);

        // Label text
        Color textC = isCurrent ? COLOR_TEXT_PRIMARY : (isHovered ? COLOR_TEXT_PRIMARY : COLOR_TEXT_MUTED);
        DrawText(navItems[i].label, static_cast<int>(itemRect.x + 40.0f), static_cast<int>(itemRect.y + 13.0f), 14, textC);

        itemY += itemH + 6.0f;
    }

    // Sidebar Footer (System & Build Info)
    float footerY = static_cast<float>(WINDOW_HEIGHT) - 96.0f;
    Rectangle footerRect = { 14.0f, footerY, itemW, 82.0f };
    DrawCard(footerRect, COLOR_BG_DARK, COLOR_PANEL_LIGHT, 0.1f);

    DrawCircle(static_cast<int>(footerRect.x + 14.0f), static_cast<int>(footerRect.y + 16.0f), 3.5f, COLOR_ACCENT_SUCCESS);
    DrawText("System Ready", static_cast<int>(footerRect.x + 24.0f), static_cast<int>(footerRect.y + 10.0f), 11, COLOR_TEXT_PRIMARY);

    DrawText("C++17 | raylib 5.0", static_cast<int>(footerRect.x + 14.0f), static_cast<int>(footerRect.y + 30.0f), 10, COLOR_TEXT_MUTED);
    DrawText("MSYS2 GCC (UCRT64)", static_cast<int>(footerRect.x + 14.0f), static_cast<int>(footerRect.y + 46.0f), 10, COLOR_TEXT_MUTED);
    DrawText("Zero STL in Core DSA", static_cast<int>(footerRect.x + 14.0f), static_cast<int>(footerRect.y + 62.0f), 10, COLOR_ACCENT_INFO);
}

void Application::run() {
    init();

    while (!WindowShouldClose() && running) {
        float dt = GetFrameTime();

        View* view = getCurrentView();
        if (view) {
            view->update(dt);
        }

        BeginDrawing();
        ClearBackground(COLOR_BG_DARK);

        // Draw Left Navigation Sidebar
        drawSidebar();

        // Draw Top Header Bar
        if (view) {
            Rectangle headerBounds = { SIDEBAR_WIDTH + 24.0f, 16.0f, WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f, 60.0f };
            DrawHeaderBar(view->getTitle(), view->getSubtitle(), headerBounds);

            // Draw current active view content
            view->draw();
        }

        EndDrawing();
    }
}

} // namespace gui

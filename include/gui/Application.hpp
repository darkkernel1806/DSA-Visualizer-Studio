#ifndef GUI_APPLICATION_HPP
#define GUI_APPLICATION_HPP

#include "raylib.h"
#include "gui/UICommon.hpp"
#include "gui/View.hpp"
#include <memory>

namespace gui {

class DashboardView;
class StackView;
class QueueView;
class LinkedListView;
class BSTView;
class SortingView;

struct NavItem {
    ViewType type;
    const char* label;
    const char* icon;
};

class Application {
private:
    bool running;
    ViewType currentViewType;

    // View pointers
    std::unique_ptr<DashboardView> dashboardView;
    std::unique_ptr<StackView> stackView;
    std::unique_ptr<QueueView> queueView;
    std::unique_ptr<LinkedListView> linkedListView;
    std::unique_ptr<BSTView> bstView;
    std::unique_ptr<SortingView> sortingView;

    // Navigation state
    static constexpr size_t NAV_COUNT = 6;
    NavItem navItems[NAV_COUNT];
    int hoveredNavIdx;

    void drawSidebar();
    View* getCurrentView();

public:
    Application();
    ~Application();

    void init();
    void run();
    void setView(ViewType type);
    ViewType getCurrentViewType() const { return currentViewType; }
};

} // namespace gui

#endif // GUI_APPLICATION_HPP

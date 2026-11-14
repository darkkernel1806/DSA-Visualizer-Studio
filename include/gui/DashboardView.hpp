#ifndef GUI_DASHBOARD_VIEW_HPP
#define GUI_DASHBOARD_VIEW_HPP

#include "gui/View.hpp"
#include "gui/UICommon.hpp"
#include "gui/UIComponents.hpp"

namespace gui {

class Application; // Forward declaration

class DashboardView : public View {
private:
    Application* app;
    Button stackBtn;
    Button queueBtn;
    Button listBtn;
    Button bstBtn;
    Button sortBtn;

public:
    explicit DashboardView(Application* application);

    void update(float dt) override;
    void draw() override;
    const char* getTitle() const override { return "Studio Overview Dashboard"; }
    const char* getSubtitle() const override { return "Interactive C++17 Algorithm & Data Structure Visualizer Suite"; }
    void reset() override {}
};

} // namespace gui

#endif // GUI_DASHBOARD_VIEW_HPP

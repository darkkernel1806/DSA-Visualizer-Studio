#ifndef GUI_STACK_VIEW_HPP
#define GUI_STACK_VIEW_HPP

#include "gui/View.hpp"
#include "gui/UICommon.hpp"
#include "gui/UIComponents.hpp"
#include "dsa/Stack.hpp"

namespace gui {

class StackView : public View {
private:
    dsa::Stack stack;

    TextBox valInput;
    Button pushBtn;
    Button popBtn;
    Button peekBtn;
    Button clearBtn;
    Button randomBtn;

    StatusConsole console;
    ComplexityCard complexity;

    // Visual feedback animations
    int peekHighlightIdx;
    float highlightTimer;
    char lastPoppedValue[32];

public:
    StackView();

    void update(float dt) override;
    void draw() override;
    const char* getTitle() const override { return "Stack Visualizer (LIFO)"; }
    const char* getSubtitle() const override { return "Last-In, First-Out dynamic linked node architecture with O(1) top operations"; }
    void reset() override;
};

} // namespace gui

#endif // GUI_STACK_VIEW_HPP

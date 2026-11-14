#ifndef GUI_QUEUE_VIEW_HPP
#define GUI_QUEUE_VIEW_HPP

#include "gui/View.hpp"
#include "gui/UICommon.hpp"
#include "gui/UIComponents.hpp"
#include "dsa/Queue.hpp"

namespace gui {

class QueueView : public View {
private:
    dsa::Queue queue;

    TextBox valInput;
    Button enqueueBtn;
    Button dequeueBtn;
    Button frontBtn;
    Button clearBtn;
    Button randomBtn;

    StatusConsole console;
    ComplexityCard complexity;

    float frontHighlightTimer;
    char lastDequeuedValue[32];

public:
    QueueView();

    void update(float dt) override;
    void draw() override;
    const char* getTitle() const override { return "Queue Visualizer (FIFO)"; }
    const char* getSubtitle() const override { return "First-In, First-Out sequential pipeline with dedicated Front and Rear pointers"; }
    void reset() override;
};

} // namespace gui

#endif // GUI_QUEUE_VIEW_HPP

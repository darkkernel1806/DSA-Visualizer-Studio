#ifndef GUI_VIEW_HPP
#define GUI_VIEW_HPP

#include "raylib.h"

namespace gui {

enum class ViewType {
    DASHBOARD = 0,
    STACK,
    QUEUE,
    LINKED_LIST,
    BST,
    SORTING
};

class View {
public:
    virtual ~View() = default;

    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual const char* getTitle() const = 0;
    virtual const char* getSubtitle() const = 0;
    virtual void reset() = 0;
};

} // namespace gui

#endif // GUI_VIEW_HPP

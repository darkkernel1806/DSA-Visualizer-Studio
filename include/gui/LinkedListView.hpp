#ifndef GUI_LINKED_LIST_VIEW_HPP
#define GUI_LINKED_LIST_VIEW_HPP

#include "gui/View.hpp"
#include "gui/UICommon.hpp"
#include "gui/UIComponents.hpp"
#include "dsa/SinglyLinkedList.hpp"

namespace gui {

class LinkedListView : public View {
private:
    dsa::SinglyLinkedList list;

    TextBox valInput;
    TextBox idxInput;

    Button insertHeadBtn;
    Button insertTailBtn;
    Button insertAtBtn;
    Button deleteHeadBtn;
    Button deleteTailBtn;
    Button deleteValBtn;
    Button searchBtn;
    Button clearBtn;
    Button randomBtn;

    StatusConsole console;
    ComplexityCard complexity;

    // Search and action animations
    int searchTarget;
    int searchHighlightIdx;
    bool isSearching;
    float searchTimer;
    float searchStepDelay;
    bool searchFound;

public:
    LinkedListView();

    void update(float dt) override;
    void draw() override;
    const char* getTitle() const override { return "Singly Linked List Visualizer"; }
    const char* getSubtitle() const override { return "Dynamic chaining with [Data | Next*] node compartments and pointer navigation"; }
    void reset() override;
};

} // namespace gui

#endif // GUI_LINKED_LIST_VIEW_HPP

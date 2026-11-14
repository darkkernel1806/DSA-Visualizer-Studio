#ifndef GUI_BST_VIEW_HPP
#define GUI_BST_VIEW_HPP

#include "gui/View.hpp"
#include "gui/UICommon.hpp"
#include "gui/UIComponents.hpp"
#include "dsa/BinarySearchTree.hpp"

namespace gui {

class BSTView : public View {
private:
    dsa::BinarySearchTree bst;

    TextBox valInput;
    Button insertBtn;
    Button deleteBtn;
    Button searchBtn;
    Button inorderBtn;
    Button preorderBtn;
    Button postorderBtn;
    Button clearBtn;
    Button sampleBtn;

    StatusConsole console;
    ComplexityCard complexity;

    // Search and Traversal animated playback
    int searchHighlightValue;
    float searchHighlightTimer;

    int* activeTraversal;
    size_t traversalCount;
    int traversalStepIdx;
    float traversalTimer;
    float traversalStepDelay;
    bool isTraversing;
    const char* traversalName;

    void drawTreeRecursive(const dsa::BSTNode* node, float x, float y, float xOffset, int depth);

public:
    BSTView();
    ~BSTView() override;

    void update(float dt) override;
    void draw() override;
    const char* getTitle() const override { return "Binary Search Tree (BST)"; }
    const char* getSubtitle() const override { return "Self-organizing binary tree with left < root < right property and full traversals"; }
    void reset() override;
};

} // namespace gui

#endif // GUI_BST_VIEW_HPP

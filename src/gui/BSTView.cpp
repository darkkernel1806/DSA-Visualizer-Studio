#include "gui/BSTView.hpp"
#include <cstdio>
#include <cstdlib>

namespace gui {

BSTView::BSTView()
    : valInput({0, 0, 70, 32}, "Value", true, 5),
      insertBtn({0, 0, 78, 32}, "Insert", COLOR_ACCENT_PRIMARY),
      deleteBtn({0, 0, 78, 32}, "Delete", COLOR_ACCENT_DANGER),
      searchBtn({0, 0, 78, 32}, "Search", COLOR_ACCENT_WARNING),
      inorderBtn({0, 0, 86, 32}, "Inorder", COLOR_ACCENT_INFO),
      preorderBtn({0, 0, 86, 32}, "Preorder", COLOR_ACCENT_INFO),
      postorderBtn({0, 0, 86, 32}, "Postorder", COLOR_ACCENT_INFO),
      sampleBtn({0, 0, 96, 32}, "Sample Tree", COLOR_ACCENT_SUCCESS),
      clearBtn({0, 0, 68, 32}, "Clear", COLOR_PANEL_LIGHT),
      searchHighlightValue(-99999), searchHighlightTimer(0.0f),
      activeTraversal(nullptr), traversalCount(0), traversalStepIdx(-1),
      traversalTimer(0.0f), traversalStepDelay(0.6f), isTraversing(false),
      traversalName("") {
    complexity.set("O(log n) Avg / O(n) Worst", "O(log n)", "O(n) Skewed", "O(n)",
                   "Search/Insert/Delete are O(h) where h is height. In balanced BST, h = log2(n).");

    // Load initial sample tree
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);
    console.log("BST initialized with 7 nodes (balanced tree: Root 50).", StatusType::INFO);
}

BSTView::~BSTView() {
    delete[] activeTraversal;
}

void BSTView::reset() {
    bst.clear();
    delete[] activeTraversal;
    activeTraversal = nullptr;
    traversalCount = 0;
    traversalStepIdx = -1;
    isTraversing = false;
    searchHighlightValue = -99999;
    valInput.clear();
    console.log("BST cleared.", StatusType::INFO);
}

void BSTView::update(float dt) {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 82.0f;

    // Controls Layout
    valInput.setBounds({ startX, startY, 70, 32 });
    insertBtn.setBounds({ startX + 78.0f, startY, 76, 32 });
    deleteBtn.setBounds({ startX + 160.0f, startY, 76, 32 });
    searchBtn.setBounds({ startX + 242.0f, startY, 76, 32 });
    inorderBtn.setBounds({ startX + 324.0f, startY, 84, 32 });
    preorderBtn.setBounds({ startX + 414.0f, startY, 84, 32 });
    postorderBtn.setBounds({ startX + 504.0f, startY, 86, 32 });
    sampleBtn.setBounds({ startX + 596.0f, startY, 96, 32 });
    clearBtn.setBounds({ startX + 698.0f, startY, 68, 32 });

    valInput.update();
    insertBtn.update();
    deleteBtn.update();
    searchBtn.update();
    inorderBtn.update();
    preorderBtn.update();
    postorderBtn.update();
    sampleBtn.update();
    clearBtn.update();

    if (searchHighlightTimer > 0.0f) {
        searchHighlightTimer -= dt;
        if (searchHighlightTimer <= 0.0f) {
            searchHighlightValue = -99999;
        }
    }

    // Traversal Animation Step
    if (isTraversing && activeTraversal != nullptr) {
        traversalTimer += dt;
        if (traversalTimer >= traversalStepDelay) {
            traversalTimer = 0.0f;
            traversalStepIdx++;
            if (static_cast<size_t>(traversalStepIdx) >= traversalCount) {
                isTraversing = false;
                char msg[96];
                std::snprintf(msg, sizeof(msg), "%s traversal animation complete! Visited %zu nodes.",
                              traversalName, traversalCount);
                console.log(msg, StatusType::SUCCESS);
            } else {
                searchHighlightValue = activeTraversal[traversalStepIdx];
            }
        }
    }

    // Insert Node
    if (insertBtn.isClicked()) {
        int val = 0;
        if (valInput.getIntValue(val)) {
            if (bst.insert(val)) {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "INSERT: Value %d successfully added. Tree size: %zu, Height: %zu.",
                              val, bst.getSize(), bst.getHeight());
                console.log(msg, StatusType::SUCCESS);
                searchHighlightValue = val;
                searchHighlightTimer = 1.8f;
                valInput.clear();
            } else {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "DUPLICATE: Value %d already exists in the BST! Insertion skipped.", val);
                console.log(msg, StatusType::WARNING);
            }
        } else {
            console.log("Enter a valid integer in 'Value' box to insert.", StatusType::DANGER);
        }
    }

    // Delete Node
    if (deleteBtn.isClicked()) {
        int val = 0;
        if (valInput.getIntValue(val)) {
            if (bst.remove(val)) {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "DELETE: Removed node %d from BST. New size: %zu, Height: %zu.",
                              val, bst.getSize(), bst.getHeight());
                console.log(msg, StatusType::SUCCESS);
                searchHighlightValue = -99999;
                valInput.clear();
            } else {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "NOT FOUND: Value %d does not exist in the BST.", val);
                console.log(msg, StatusType::WARNING);
            }
        } else {
            console.log("Enter target value to delete in 'Value' box.", StatusType::DANGER);
        }
    }

    // Search Node
    if (searchBtn.isClicked()) {
        int val = 0;
        if (valInput.getIntValue(val)) {
            size_t depth = 0;
            if (bst.search(val, depth)) {
                searchHighlightValue = val;
                searchHighlightTimer = 2.5f;
                char msg[96];
                std::snprintf(msg, sizeof(msg), "SEARCH SUCCESS: Found %d in BST at depth %zu (Root = depth 0).", val, depth);
                console.log(msg, StatusType::SUCCESS);
            } else {
                char msg[96];
                std::snprintf(msg, sizeof(msg), "SEARCH NOT FOUND: Value %d is not in the BST.", val);
                console.log(msg, StatusType::WARNING);
            }
        } else {
            console.log("Enter target value to search in 'Value' box.", StatusType::DANGER);
        }
    }

    // Inorder Traversal (Left, Node, Right) -> Sorted order
    if (inorderBtn.isClicked()) {
        if (bst.isEmpty()) {
            console.log("Tree is empty! Cannot perform Inorder traversal.", StatusType::WARNING);
        } else {
            delete[] activeTraversal;
            activeTraversal = bst.getInorder(traversalCount);
            traversalStepIdx = 0;
            traversalTimer = 0.0f;
            isTraversing = true;
            traversalName = "Inorder (LNR - Ascending)";
            searchHighlightValue = activeTraversal[0];
            console.log("Started Inorder Traversal (Left -> Root -> Right). Produces sorted sequence!", StatusType::INFO);
        }
    }

    // Preorder Traversal (Node, Left, Right)
    if (preorderBtn.isClicked()) {
        if (bst.isEmpty()) {
            console.log("Tree is empty! Cannot perform Preorder traversal.", StatusType::WARNING);
        } else {
            delete[] activeTraversal;
            activeTraversal = bst.getPreorder(traversalCount);
            traversalStepIdx = 0;
            traversalTimer = 0.0f;
            isTraversing = true;
            traversalName = "Preorder (NLR - Prefix)";
            searchHighlightValue = activeTraversal[0];
            console.log("Started Preorder Traversal (Root -> Left -> Right). Useful for tree cloning!", StatusType::INFO);
        }
    }

    // Postorder Traversal (Left, Right, Node)
    if (postorderBtn.isClicked()) {
        if (bst.isEmpty()) {
            console.log("Tree is empty! Cannot perform Postorder traversal.", StatusType::WARNING);
        } else {
            delete[] activeTraversal;
            activeTraversal = bst.getPostorder(traversalCount);
            traversalStepIdx = 0;
            traversalTimer = 0.0f;
            isTraversing = true;
            traversalName = "Postorder (LRN - Postfix)";
            searchHighlightValue = activeTraversal[0];
            console.log("Started Postorder Traversal (Left -> Right -> Root). Useful for bottom-up cleanup!", StatusType::INFO);
        }
    }

    // Load Sample Tree
    if (sampleBtn.isClicked()) {
        bst.clear();
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);
        bst.insert(20);
        bst.insert(40);
        bst.insert(60);
        bst.insert(80);
        searchHighlightValue = -99999;
        delete[] activeTraversal;
        activeTraversal = nullptr;
        traversalCount = 0;
        isTraversing = false;
        console.log("Loaded standard balanced 3-level BST: Root 50, Children [30, 70], Leaves [20, 40, 60, 80].", StatusType::INFO);
    }

    // Clear Tree
    if (clearBtn.isClicked()) {
        bst.clear();
        searchHighlightValue = -99999;
        delete[] activeTraversal;
        activeTraversal = nullptr;
        traversalCount = 0;
        isTraversing = false;
        console.log("BST cleared. All dynamic tree nodes freed recursively.", StatusType::INFO);
    }
}

void BSTView::drawTreeRecursive(const dsa::BSTNode* node, float x, float y, float xOffset, int depth) {
    if (node == nullptr) return;

    float nodeRadius = 18.0f;
    float nextY = y + 54.0f;
    float nextOffset = xOffset * 0.52f;
    if (nextOffset < 22.0f) nextOffset = 22.0f;

    // Draw branch line to Left child
    if (node->left != nullptr) {
        float childX = x - xOffset;
        DrawLineEx({ x, y + nodeRadius }, { childX, nextY - nodeRadius }, 2.0f, COLOR_PANEL_LIGHT);
        drawTreeRecursive(node->left, childX, nextY, nextOffset, depth + 1);
    }

    // Draw branch line to Right child
    if (node->right != nullptr) {
        float childX = x + xOffset;
        DrawLineEx({ x, y + nodeRadius }, { childX, nextY - nodeRadius }, 2.0f, COLOR_PANEL_LIGHT);
        drawTreeRecursive(node->right, childX, nextY, nextOffset, depth + 1);
    }

    // Draw Current Node Circle
    Color circleBg = COLOR_PANEL_DARK;
    Color borderC  = COLOR_ACCENT_PRIMARY;

    if (node->data == searchHighlightValue) {
        circleBg = ColorAlpha(COLOR_ACCENT_WARNING, 0.4f);
        borderC  = COLOR_ACCENT_WARNING;
        DrawCircle(static_cast<int>(x), static_cast<int>(y), nodeRadius + 4.0f, ColorAlpha(COLOR_ACCENT_WARNING, 0.25f));
    }

    DrawCircle(static_cast<int>(x), static_cast<int>(y), nodeRadius, circleBg);
    DrawCircleLines(static_cast<int>(x), static_cast<int>(y), nodeRadius, borderC);

    // Node Value Text
    char valStr[16];
    std::snprintf(valStr, sizeof(valStr), "%d", node->data);
    int vw = MeasureText(valStr, 13);
    DrawText(valStr, static_cast<int>(x - vw * 0.5f), static_cast<int>(y - 6.0f), 13, COLOR_TEXT_PRIMARY);

    // Root badge above depth 0
    if (depth == 0) {
        DrawText("ROOT", static_cast<int>(x - 14.0f), static_cast<int>(y - nodeRadius - 14.0f), 10, COLOR_ACCENT_SUCCESS);
    }
}

void BSTView::draw() {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 82.0f;
    float availW = WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f;

    // Controls container
    Rectangle controlsCard = { startX, startY - 6.0f, availW, 46.0f };
    DrawCard(controlsCard, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    valInput.draw();
    insertBtn.draw();
    deleteBtn.draw();
    searchBtn.draw();
    inorderBtn.draw();
    preorderBtn.draw();
    postorderBtn.draw();
    sampleBtn.draw();
    clearBtn.draw();

    // Stats Badges on top right
    char statBuf[48];
    std::snprintf(statBuf, sizeof(statBuf), "N: %zu | H: %zu", bst.getSize(), bst.getHeight());
    DrawBadge("Tree Metrics", statBuf, { controlsCard.x + controlsCard.width - 164.0f, controlsCard.y + 11.0f }, COLOR_ACCENT_PURPLE);

    // Canvas Card
    float canvasY = startY + 48.0f;
    float canvasH = 436.0f;
    Rectangle canvas = { startX, canvasY, availW, canvasH };
    DrawCard(canvas, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    DrawText("HIERARCHICAL 2D BINARY SEARCH TREE CANVAS", static_cast<int>(canvas.x + 20.0f), static_cast<int>(canvas.y + 16.0f), 12, COLOR_TEXT_DIM);

    if (bst.isEmpty()) {
        DrawText("Binary Search Tree is EMPTY",
                 static_cast<int>(canvas.x + canvas.width * 0.5f - 95.0f), static_cast<int>(canvas.y + 150.0f), 16, COLOR_TEXT_DIM);
        DrawText("Click 'Sample Tree' or enter values and click 'Insert'.",
                 static_cast<int>(canvas.x + canvas.width * 0.5f - 140.0f), static_cast<int>(canvas.y + 178.0f), 12, COLOR_TEXT_DIM);
    } else {
        float rootX = canvas.x + canvas.width * 0.5f;
        float rootY = canvas.y + 54.0f;
        float initOffset = 180.0f;
        drawTreeRecursive(bst.getRoot(), rootX, rootY, initOffset, 0);
    }

    // Traversal Sequence Ribbon (at bottom of canvas)
    if (activeTraversal != nullptr && traversalCount > 0) {
        Rectangle ribCard = { canvas.x + 20.0f, canvas.y + canvas.height - 72.0f, canvas.width - 40.0f, 54.0f };
        DrawCard(ribCard, ColorAlpha(COLOR_PANEL_LIGHT, 0.85f), COLOR_ACCENT_INFO, 0.15f);

        DrawText(traversalName, static_cast<int>(ribCard.x + 14.0f), static_cast<int>(ribCard.y + 8.0f), 11, COLOR_ACCENT_INFO);

        // Sequence items
        float itemStartX = ribCard.x + 14.0f;
        for (size_t i = 0; i < traversalCount && i < 16; ++i) {
            char itemStr[16];
            std::snprintf(itemStr, sizeof(itemStr), "%d", activeTraversal[i]);
            int iw = MeasureText(itemStr, 12);
            Rectangle itemRect = { itemStartX, ribCard.y + 24.0f, static_cast<float>(iw + 14), 22.0f };

            Color ibg = (static_cast<int>(i) == traversalStepIdx) ? COLOR_ACCENT_WARNING : COLOR_PANEL_DARK;
            Color itext = (static_cast<int>(i) == traversalStepIdx) ? COLOR_BG_DARK : COLOR_TEXT_PRIMARY;

            DrawRectangleRounded(itemRect, 0.3f, 4, ibg);
            DrawText(itemStr, static_cast<int>(itemRect.x + 7.0f), static_cast<int>(itemRect.y + 5.0f), 12, itext);

            itemStartX += itemRect.width + 6.0f;
            if (i < traversalCount - 1) {
                DrawText("->", static_cast<int>(itemStartX), static_cast<int>(itemRect.y + 5.0f), 11, COLOR_TEXT_DIM);
                itemStartX += 18.0f;
            }
        }
    }

    // Complexity and Status Console side-by-side at bottom
    float bottomY = canvasY + canvasH + 12.0f;
    float halfW = (availW - 16.0f) * 0.5f;

    complexity.draw({ startX, bottomY, halfW, 82.0f });
    console.draw({ startX + halfW + 16.0f, bottomY, halfW, 82.0f });
}

} // namespace gui

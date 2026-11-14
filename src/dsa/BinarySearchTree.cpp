#include "dsa/BinarySearchTree.hpp"

namespace dsa {

BinarySearchTree::BinarySearchTree() : root(nullptr), count(0) {}

BinarySearchTree::~BinarySearchTree() {
    clear();
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept
    : root(other.root), count(other.count) {
    other.root = nullptr;
    other.count = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        count = other.count;
        other.root = nullptr;
        other.count = 0;
    }
    return *this;
}

BSTNode* BinarySearchTree::insertRec(BSTNode* node, int value, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new BSTNode(value);
    }

    if (value < node->data) {
        node->left = insertRec(node->left, value, inserted);
    } else if (value > node->data) {
        node->right = insertRec(node->right, value, inserted);
    } else {
        // Duplicate value - do not insert
        inserted = false;
    }
    return node;
}

bool BinarySearchTree::insert(int value) {
    bool inserted = false;
    root = insertRec(root, value, inserted);
    if (inserted) {
        count++;
    }
    return inserted;
}

BSTNode* BinarySearchTree::findMin(BSTNode* node) const {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

BSTNode* BinarySearchTree::deleteRec(BSTNode* node, int value, bool& deleted) {
    if (node == nullptr) {
        deleted = false;
        return nullptr;
    }

    if (value < node->data) {
        node->left = deleteRec(node->left, value, deleted);
    } else if (value > node->data) {
        node->right = deleteRec(node->right, value, deleted);
    } else {
        // Node found!
        deleted = true;

        // Case 1: Leaf node (no children)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2: One child
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Two children
        // Find inorder successor (smallest in the right subtree)
        BSTNode* successor = findMin(node->right);
        node->data = successor->data;
        // Delete the inorder successor from right subtree
        bool dummy = false;
        node->right = deleteRec(node->right, successor->data, dummy);
    }
    return node;
}

bool BinarySearchTree::remove(int value) {
    bool deleted = false;
    root = deleteRec(root, value, deleted);
    if (deleted) {
        count--;
    }
    return deleted;
}

bool BinarySearchTree::searchRec(BSTNode* node, int value, size_t depth, size_t& foundDepth) const {
    if (node == nullptr) {
        return false;
    }
    if (node->data == value) {
        foundDepth = depth;
        return true;
    }
    if (value < node->data) {
        return searchRec(node->left, value, depth + 1, foundDepth);
    } else {
        return searchRec(node->right, value, depth + 1, foundDepth);
    }
}

bool BinarySearchTree::search(int value, size_t& outDepth) const {
    return searchRec(root, value, 0, outDepth);
}

void BinarySearchTree::clearRec(BSTNode* node) {
    if (node != nullptr) {
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }
}

void BinarySearchTree::clear() {
    clearRec(root);
    root = nullptr;
    count = 0;
}

bool BinarySearchTree::isEmpty() const {
    return root == nullptr;
}

size_t BinarySearchTree::getSize() const {
    return count;
}

size_t BinarySearchTree::heightRec(BSTNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    size_t leftH = heightRec(node->left);
    size_t rightH = heightRec(node->right);
    return 1 + (leftH > rightH ? leftH : rightH);
}

size_t BinarySearchTree::getHeight() const {
    return heightRec(root);
}

const BSTNode* BinarySearchTree::getRoot() const {
    return root;
}

void BinarySearchTree::inorderRec(BSTNode* node, int* arr, size_t& idx) const {
    if (node != nullptr) {
        inorderRec(node->left, arr, idx);
        arr[idx++] = node->data;
        inorderRec(node->right, arr, idx);
    }
}

int* BinarySearchTree::getInorder(size_t& outCount) const {
    outCount = count;
    if (count == 0) return nullptr;
    int* arr = new int[count];
    size_t idx = 0;
    inorderRec(root, arr, idx);
    return arr;
}

void BinarySearchTree::preorderRec(BSTNode* node, int* arr, size_t& idx) const {
    if (node != nullptr) {
        arr[idx++] = node->data;
        preorderRec(node->left, arr, idx);
        preorderRec(node->right, arr, idx);
    }
}

int* BinarySearchTree::getPreorder(size_t& outCount) const {
    outCount = count;
    if (count == 0) return nullptr;
    int* arr = new int[count];
    size_t idx = 0;
    preorderRec(root, arr, idx);
    return arr;
}

void BinarySearchTree::postorderRec(BSTNode* node, int* arr, size_t& idx) const {
    if (node != nullptr) {
        postorderRec(node->left, arr, idx);
        postorderRec(node->right, arr, idx);
        arr[idx++] = node->data;
    }
}

int* BinarySearchTree::getPostorder(size_t& outCount) const {
    outCount = count;
    if (count == 0) return nullptr;
    int* arr = new int[count];
    size_t idx = 0;
    postorderRec(root, arr, idx);
    return arr;
}

} // namespace dsa

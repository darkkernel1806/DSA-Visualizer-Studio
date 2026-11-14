#ifndef DSA_BINARY_SEARCH_TREE_HPP
#define DSA_BINARY_SEARCH_TREE_HPP

#include <cstddef>

namespace dsa {

/**
 * @brief Node for Binary Search Tree.
 */
struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;

    explicit BSTNode(int value)
        : data(value), left(nullptr), right(nullptr) {}
};

/**
 * @brief Custom Binary Search Tree implementation using raw pointers and dynamic memory.
 * No STL containers used.
 */
class BinarySearchTree {
private:
    BSTNode* root;
    size_t count;

    // Recursive helper functions
    BSTNode* insertRec(BSTNode* node, int value, bool& inserted);
    BSTNode* deleteRec(BSTNode* node, int value, bool& deleted);
    BSTNode* findMin(BSTNode* node) const;
    bool searchRec(BSTNode* node, int value, size_t depth, size_t& foundDepth) const;
    void clearRec(BSTNode* node);
    size_t heightRec(BSTNode* node) const;

    void inorderRec(BSTNode* node, int* arr, size_t& idx) const;
    void preorderRec(BSTNode* node, int* arr, size_t& idx) const;
    void postorderRec(BSTNode* node, int* arr, size_t& idx) const;

public:
    BinarySearchTree();
    ~BinarySearchTree();

    // Prevent inadvertent shallow copying
    BinarySearchTree(const BinarySearchTree& other) = delete;
    BinarySearchTree& operator=(const BinarySearchTree& other) = delete;

    // Move semantics
    BinarySearchTree(BinarySearchTree&& other) noexcept;
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

    /**
     * @brief Inserts a value into the BST.
     * @param value Integer to insert.
     * @return true if inserted, false if value already exists (no duplicates).
     */
    bool insert(int value);

    /**
     * @brief Deletes a value from the BST.
     * @param value Value to remove.
     * @return true if removed, false if not found.
     */
    bool remove(int value);

    /**
     * @brief Searches for a value in the BST.
     * @param value Value to find.
     * @param outDepth Output depth where found (root is depth 0).
     * @return true if found, false otherwise.
     */
    bool search(int value, size_t& outDepth) const;

    /**
     * @brief Clears all nodes from the BST and frees memory.
     */
    void clear();

    /**
     * @brief Checks if tree is empty.
     */
    bool isEmpty() const;

    /**
     * @brief Returns total number of nodes in the tree.
     */
    size_t getSize() const;

    /**
     * @brief Returns height of the tree (0 for empty tree, 1 for single node).
     */
    size_t getHeight() const;

    /**
     * @brief Returns root node pointer (read-only for rendering).
     */
    const BSTNode* getRoot() const;

    /**
     * @brief Returns Inorder traversal (Left, Root, Right) as dynamic array.
     * Caller must delete[] the returned array.
     */
    int* getInorder(size_t& outCount) const;

    /**
     * @brief Returns Preorder traversal (Root, Left, Right) as dynamic array.
     * Caller must delete[] the returned array.
     */
    int* getPreorder(size_t& outCount) const;

    /**
     * @brief Returns Postorder traversal (Left, Right, Root) as dynamic array.
     * Caller must delete[] the returned array.
     */
    int* getPostorder(size_t& outCount) const;
};

} // namespace dsa

#endif // DSA_BINARY_SEARCH_TREE_HPP

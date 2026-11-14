#ifndef DSA_SINGLY_LINKED_LIST_HPP
#define DSA_SINGLY_LINKED_LIST_HPP

#include <cstddef>

namespace dsa {

/**
 * @brief Node for a Singly Linked List.
 */
struct ListNode {
    int data;
    ListNode* next;

    explicit ListNode(int value, ListNode* nextNode = nullptr)
        : data(value), next(nextNode) {}
};

/**
 * @brief Custom Singly Linked List implementation using raw pointers and dynamic memory.
 * No STL containers used.
 */
class SinglyLinkedList {
private:
    ListNode* head;
    size_t count;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    // Prevent inadvertent shallow copying
    SinglyLinkedList(const SinglyLinkedList& other) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList& other) = delete;

    // Move semantics
    SinglyLinkedList(SinglyLinkedList&& other) noexcept;
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept;

    /**
     * @brief Inserts a new node with given value at the head (beginning).
     */
    void insertHead(int value);

    /**
     * @brief Inserts a new node with given value at the tail (end).
     */
    void insertTail(int value);

    /**
     * @brief Inserts a new node at a specified 0-based index.
     * If index == 0, equivalent to insertHead.
     * If index >= count, inserts at tail.
     */
    bool insertAt(size_t index, int value);

    /**
     * @brief Deletes the head node.
     * @param outValue Value of the deleted node.
     * @return true if deleted; false if list was empty.
     */
    bool deleteHead(int& outValue);

    /**
     * @brief Deletes the tail node.
     * @param outValue Value of the deleted node.
     * @return true if deleted; false if list was empty.
     */
    bool deleteTail(int& outValue);

    /**
     * @brief Searches and deletes the first occurrence of a specific value.
     * @param value Target value to delete.
     * @param deletedIndex Optional out param for 0-based index where value was found.
     * @return true if found and deleted; false otherwise.
     */
    bool deleteValue(int value, size_t* deletedIndex = nullptr);

    /**
     * @brief Deletes the node at a specified 0-based index.
     * @param index Target index.
     * @param outValue Value of deleted node.
     * @return true if deleted; false if index is out of bounds.
     */
    bool deleteAt(size_t index, int& outValue);

    /**
     * @brief Searches for a value in the list.
     * @param value Target value to find.
     * @param foundIndex Output index where value was found.
     * @return true if found, false otherwise.
     */
    bool search(int value, size_t& foundIndex) const;

    /**
     * @brief Retrieves value at specified 0-based index.
     */
    bool getAt(size_t index, int& outValue) const;

    /**
     * @brief Clears all nodes from the list and frees allocated memory.
     */
    void clear();

    /**
     * @brief Checks if list is empty.
     */
    bool isEmpty() const;

    /**
     * @brief Returns number of nodes in list.
     */
    size_t getSize() const;

    /**
     * @brief Returns the head pointer (read-only) for traversal inspection.
     */
    const ListNode* getHead() const;

    /**
     * @brief Exports node values in order into a dynamically allocated array.
     * Caller must delete[] the returned array.
     */
    int* toArray(size_t& outCount) const;
};

} // namespace dsa

#endif // DSA_SINGLY_LINKED_LIST_HPP

#ifndef DSA_QUEUE_HPP
#define DSA_QUEUE_HPP

#include <cstddef>

namespace dsa {

/**
 * @brief Node structure representing an element in the Queue.
 */
struct QueueNode {
    int data;
    QueueNode* next;

    explicit QueueNode(int value, QueueNode* nextNode = nullptr)
        : data(value), next(nextNode) {}
};

/**
 * @brief Custom Queue implementation from scratch using raw pointers.
 * Follows FIFO (First-In-First-Out) principle.
 */
class Queue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;
    size_t count;

public:
    Queue();
    ~Queue();

    // Prevent inadvertent shallow copying
    Queue(const Queue& other) = delete;
    Queue& operator=(const Queue& other) = delete;

    // Move semantics
    Queue(Queue&& other) noexcept;
    Queue& operator=(Queue&& other) noexcept;

    /**
     * @brief Inserts an element at the rear of the queue.
     * @param value The integer value to enqueue.
     */
    void enqueue(int value);

    /**
     * @brief Removes the element at the front of the queue.
     * @param outValue Reference to receive the dequeued value.
     * @return true if dequeued successfully; false if queue was empty (underflow).
     */
    bool dequeue(int& outValue);

    /**
     * @brief Retrieves the front element without removing it.
     * @param outValue Reference to receive the front value.
     * @return true if succeeded; false if queue was empty.
     */
    bool front(int& outValue) const;

    /**
     * @brief Removes all elements and frees memory.
     */
    void clear();

    /**
     * @brief Returns true if queue has no elements.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the number of elements in the queue.
     */
    size_t getSize() const;

    /**
     * @brief Exports queue elements from front to rear into a dynamic array.
     * Caller must delete[] the returned array.
     */
    int* toArray(size_t& outCount) const;
};

} // namespace dsa

#endif // DSA_QUEUE_HPP

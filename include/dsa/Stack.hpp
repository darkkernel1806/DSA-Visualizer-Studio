#ifndef DSA_STACK_HPP
#define DSA_STACK_HPP

#include <cstddef>

namespace dsa {

/**
 * @brief Node structure representing an element in the Stack.
 * Implemented using linked list node for dynamic growth without memory waste.
 */
struct StackNode {
    int data;
    StackNode* next;

    explicit StackNode(int value, StackNode* nextNode = nullptr)
        : data(value), next(nextNode) {}
};

/**
 * @brief Custom Stack implementation from scratch using raw pointers.
 * Follows LIFO (Last-In-First-Out) principle.
 * Strict no-STL policy; handles dynamic allocation and cleanup explicitly.
 */
class Stack {
private:
    StackNode* topNode;
    size_t count;

public:
    Stack();
    ~Stack();

    // Prevent inadvertent shallow copying
    Stack(const Stack& other) = delete;
    Stack& operator=(const Stack& other) = delete;

    // Move semantics
    Stack(Stack&& other) noexcept;
    Stack& operator=(Stack&& other) noexcept;

    /**
     * @brief Pushes an element onto the top of the stack.
     * @param value The integer value to push.
     */
    void push(int value);

    /**
     * @brief Pops the top element from the stack.
     * @param outValue Reference to store the popped value.
     * @return true if popped successfully; false if stack was empty (underflow).
     */
    bool pop(int& outValue);

    /**
     * @brief Peeks at the top element without removing it.
     * @param outValue Reference to store the peeked value.
     * @return true if peek succeeded; false if stack was empty.
     */
    bool peek(int& outValue) const;

    /**
     * @brief Removes all elements from the stack and frees memory.
     */
    void clear();

    /**
     * @brief Checks whether the stack has no elements.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the current number of elements in the stack.
     */
    size_t getSize() const;

    /**
     * @brief Exports stack elements from top to bottom into a dynamically allocated array.
     * Useful for visualizer rendering. Caller must delete[] the returned array.
     * @param outCount Reference to receive element count.
     * @return Pointer to int array, or nullptr if empty.
     */
    int* toArray(size_t& outCount) const;
};

} // namespace dsa

#endif // DSA_STACK_HPP

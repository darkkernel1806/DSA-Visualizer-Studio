#include "dsa/Stack.hpp"

namespace dsa {

Stack::Stack() : topNode(nullptr), count(0) {}

Stack::~Stack() {
    clear();
}

Stack::Stack(Stack&& other) noexcept : topNode(other.topNode), count(other.count) {
    other.topNode = nullptr;
    other.count = 0;
}

Stack& Stack::operator=(Stack&& other) noexcept {
    if (this != &other) {
        clear();
        topNode = other.topNode;
        count = other.count;
        other.topNode = nullptr;
        other.count = 0;
    }
    return *this;
}

void Stack::push(int value) {
    // Allocate new node with next pointing to the current top
    StackNode* newNode = new StackNode(value, topNode);
    topNode = newNode;
    count++;
}

bool Stack::pop(int& outValue) {
    if (isEmpty()) {
        return false; // Underflow
    }
    StackNode* temp = topNode;
    outValue = temp->data;
    topNode = topNode->next;
    delete temp;
    count--;
    return true;
}

bool Stack::peek(int& outValue) const {
    if (isEmpty()) {
        return false;
    }
    outValue = topNode->data;
    return true;
}

void Stack::clear() {
    while (topNode != nullptr) {
        StackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
    count = 0;
}

bool Stack::isEmpty() const {
    return topNode == nullptr;
}

size_t Stack::getSize() const {
    return count;
}

int* Stack::toArray(size_t& outCount) const {
    outCount = count;
    if (count == 0) {
        return nullptr;
    }

    int* arr = new int[count];
    StackNode* curr = topNode;
    size_t idx = 0;
    while (curr != nullptr && idx < count) {
        arr[idx++] = curr->data;
        curr = curr->next;
    }
    return arr;
}

} // namespace dsa

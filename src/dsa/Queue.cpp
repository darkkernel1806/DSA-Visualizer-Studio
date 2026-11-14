#include "dsa/Queue.hpp"

namespace dsa {

Queue::Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

Queue::~Queue() {
    clear();
}

Queue::Queue(Queue&& other) noexcept
    : frontNode(other.frontNode), rearNode(other.rearNode), count(other.count) {
    other.frontNode = nullptr;
    other.rearNode = nullptr;
    other.count = 0;
}

Queue& Queue::operator=(Queue&& other) noexcept {
    if (this != &other) {
        clear();
        frontNode = other.frontNode;
        rearNode = other.rearNode;
        count = other.count;
        other.frontNode = nullptr;
        other.rearNode = nullptr;
        other.count = 0;
    }
    return *this;
}

void Queue::enqueue(int value) {
    QueueNode* newNode = new QueueNode(value, nullptr);
    if (isEmpty()) {
        frontNode = newNode;
        rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    count++;
}

bool Queue::dequeue(int& outValue) {
    if (isEmpty()) {
        return false; // Underflow
    }

    QueueNode* temp = frontNode;
    outValue = temp->data;
    frontNode = frontNode->next;

    if (frontNode == nullptr) {
        rearNode = nullptr; // Queue became empty
    }

    delete temp;
    count--;
    return true;
}

bool Queue::front(int& outValue) const {
    if (isEmpty()) {
        return false;
    }
    outValue = frontNode->data;
    return true;
}

void Queue::clear() {
    while (frontNode != nullptr) {
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
    }
    rearNode = nullptr;
    count = 0;
}

bool Queue::isEmpty() const {
    return frontNode == nullptr;
}

size_t Queue::getSize() const {
    return count;
}

int* Queue::toArray(size_t& outCount) const {
    outCount = count;
    if (count == 0) {
        return nullptr;
    }

    int* arr = new int[count];
    QueueNode* curr = frontNode;
    size_t idx = 0;
    while (curr != nullptr && idx < count) {
        arr[idx++] = curr->data;
        curr = curr->next;
    }
    return arr;
}

} // namespace dsa

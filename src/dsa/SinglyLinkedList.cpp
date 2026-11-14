#include "dsa/SinglyLinkedList.hpp"

namespace dsa {

SinglyLinkedList::SinglyLinkedList() : head(nullptr), count(0) {}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

SinglyLinkedList::SinglyLinkedList(SinglyLinkedList&& other) noexcept
    : head(other.head), count(other.count) {
    other.head = nullptr;
    other.count = 0;
}

SinglyLinkedList& SinglyLinkedList::operator=(SinglyLinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        count = other.count;
        other.head = nullptr;
        other.count = 0;
    }
    return *this;
}

void SinglyLinkedList::insertHead(int value) {
    ListNode* newNode = new ListNode(value, head);
    head = newNode;
    count++;
}

void SinglyLinkedList::insertTail(int value) {
    ListNode* newNode = new ListNode(value, nullptr);
    if (head == nullptr) {
        head = newNode;
    } else {
        ListNode* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
    count++;
}

bool SinglyLinkedList::insertAt(size_t index, int value) {
    if (index > count) {
        return false;
    }
    if (index == 0) {
        insertHead(value);
        return true;
    }
    if (index == count) {
        insertTail(value);
        return true;
    }

    ListNode* curr = head;
    for (size_t i = 0; i < index - 1; ++i) {
        curr = curr->next;
    }

    ListNode* newNode = new ListNode(value, curr->next);
    curr->next = newNode;
    count++;
    return true;
}

bool SinglyLinkedList::deleteHead(int& outValue) {
    if (head == nullptr) {
        return false;
    }
    ListNode* temp = head;
    outValue = temp->data;
    head = head->next;
    delete temp;
    count--;
    return true;
}

bool SinglyLinkedList::deleteTail(int& outValue) {
    if (head == nullptr) {
        return false;
    }
    if (head->next == nullptr) {
        return deleteHead(outValue);
    }

    ListNode* curr = head;
    while (curr->next->next != nullptr) {
        curr = curr->next;
    }

    ListNode* temp = curr->next;
    outValue = temp->data;
    curr->next = nullptr;
    delete temp;
    count--;
    return true;
}

bool SinglyLinkedList::deleteValue(int value, size_t* deletedIndex) {
    if (head == nullptr) {
        return false;
    }

    if (head->data == value) {
        int dummy;
        if (deletedIndex) *deletedIndex = 0;
        return deleteHead(dummy);
    }

    ListNode* curr = head;
    size_t idx = 1;
    while (curr->next != nullptr && curr->next->data != value) {
        curr = curr->next;
        idx++;
    }

    if (curr->next == nullptr) {
        return false; // Not found
    }

    ListNode* temp = curr->next;
    curr->next = temp->next;
    delete temp;
    count--;
    if (deletedIndex) *deletedIndex = idx;
    return true;
}

bool SinglyLinkedList::deleteAt(size_t index, int& outValue) {
    if (index >= count || head == nullptr) {
        return false;
    }
    if (index == 0) {
        return deleteHead(outValue);
    }

    ListNode* curr = head;
    for (size_t i = 0; i < index - 1; ++i) {
        curr = curr->next;
    }

    ListNode* temp = curr->next;
    outValue = temp->data;
    curr->next = temp->next;
    delete temp;
    count--;
    return true;
}

bool SinglyLinkedList::search(int value, size_t& foundIndex) const {
    ListNode* curr = head;
    size_t idx = 0;
    while (curr != nullptr) {
        if (curr->data == value) {
            foundIndex = idx;
            return true;
        }
        curr = curr->next;
        idx++;
    }
    return false;
}

bool SinglyLinkedList::getAt(size_t index, int& outValue) const {
    if (index >= count || head == nullptr) {
        return false;
    }
    ListNode* curr = head;
    for (size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    outValue = curr->data;
    return true;
}

void SinglyLinkedList::clear() {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
    count = 0;
}

bool SinglyLinkedList::isEmpty() const {
    return head == nullptr;
}

size_t SinglyLinkedList::getSize() const {
    return count;
}

const ListNode* SinglyLinkedList::getHead() const {
    return head;
}

int* SinglyLinkedList::toArray(size_t& outCount) const {
    outCount = count;
    if (count == 0) {
        return nullptr;
    }

    int* arr = new int[count];
    ListNode* curr = head;
    size_t idx = 0;
    while (curr != nullptr && idx < count) {
        arr[idx++] = curr->data;
        curr = curr->next;
    }
    return arr;
}

} // namespace dsa

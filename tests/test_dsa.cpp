#include "dsa/Stack.hpp"
#include "dsa/Queue.hpp"
#include "dsa/SinglyLinkedList.hpp"
#include "dsa/BinarySearchTree.hpp"
#include "algorithms/SortingAlgorithms.hpp"

#include <iostream>
#include <cstdlib>

#define CHECK(expr) do { \
    if (!(expr)) { \
        std::cerr << "FAILED CHECK: " #expr " at line " << __LINE__ << std::endl; \
        std::abort(); \
    } \
} while(0)

void testStack() {
    std::cout << "[TEST] Running Stack tests..." << std::endl;
    dsa::Stack s;
    CHECK(s.isEmpty());
    CHECK(s.getSize() == 0);

    int val = 0;
    CHECK(!s.pop(val)); // Underflow test
    CHECK(!s.peek(val));

    s.push(10);
    s.push(20);
    s.push(30);
    CHECK(!s.isEmpty());
    CHECK(s.getSize() == 3);

    CHECK(s.peek(val) && val == 30);

    size_t count = 0;
    int* arr = s.toArray(count);
    CHECK(count == 3);
    CHECK(arr[0] == 30 && arr[1] == 20 && arr[2] == 10);
    delete[] arr;

    CHECK(s.pop(val) && val == 30);
    CHECK(s.pop(val) && val == 20);
    CHECK(s.getSize() == 1);
    CHECK(s.peek(val) && val == 10);

    s.clear();
    CHECK(s.isEmpty());
    CHECK(s.getSize() == 0);
    CHECK(!s.pop(val));

    std::cout << "  -> Stack tests passed successfully!" << std::endl;
}

void testQueue() {
    std::cout << "[TEST] Running Queue tests..." << std::endl;
    dsa::Queue q;
    CHECK(q.isEmpty());
    CHECK(q.getSize() == 0);

    int val = 0;
    CHECK(!q.dequeue(val)); // Underflow test
    CHECK(!q.front(val));

    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);
    CHECK(!q.isEmpty());
    CHECK(q.getSize() == 3);

    CHECK(q.front(val) && val == 100);

    size_t count = 0;
    int* arr = q.toArray(count);
    CHECK(count == 3);
    CHECK(arr[0] == 100 && arr[1] == 200 && arr[2] == 300);
    delete[] arr;

    CHECK(q.dequeue(val) && val == 100);
    CHECK(q.dequeue(val) && val == 200);
    CHECK(q.getSize() == 1);
    CHECK(q.front(val) && val == 300);

    CHECK(q.dequeue(val) && val == 300);
    CHECK(q.isEmpty());
    CHECK(!q.dequeue(val));

    std::cout << "  -> Queue tests passed successfully!" << std::endl;
}

void testSinglyLinkedList() {
    std::cout << "[TEST] Running SinglyLinkedList tests..." << std::endl;
    dsa::SinglyLinkedList list;
    CHECK(list.isEmpty());
    CHECK(list.getSize() == 0);

    int val = 0;
    CHECK(!list.deleteHead(val));
    CHECK(!list.deleteTail(val));

    list.insertHead(20);
    list.insertHead(10);
    list.insertTail(30);
    // List: 10 -> 20 -> 30
    CHECK(list.getSize() == 3);

    size_t idx = 0;
    CHECK(list.search(20, idx) && idx == 1);
    CHECK(list.search(30, idx) && idx == 2);
    CHECK(!list.search(99, idx));

    // Insert at index 1: 10 -> 15 -> 20 -> 30
    CHECK(list.insertAt(1, 15));
    CHECK(list.getSize() == 4);
    CHECK(list.getAt(1, val) && val == 15);

    // Delete value 20
    CHECK(list.deleteValue(20));
    CHECK(list.getSize() == 3);
    CHECK(!list.search(20, idx));

    // Delete head
    CHECK(list.deleteHead(val) && val == 10);
    CHECK(list.getSize() == 2);

    // Delete tail
    CHECK(list.deleteTail(val) && val == 30);
    CHECK(list.getSize() == 1);
    CHECK(list.getAt(0, val) && val == 15);

    list.clear();
    CHECK(list.isEmpty());
    std::cout << "  -> SinglyLinkedList tests passed successfully!" << std::endl;
}

void testBinarySearchTree() {
    std::cout << "[TEST] Running BinarySearchTree tests..." << std::endl;
    dsa::BinarySearchTree bst;
    CHECK(bst.isEmpty());
    CHECK(bst.getSize() == 0);

    /*
            50
          /    \
        30      70
       /  \    /  \
      20  40  60  80
    */
    CHECK(bst.insert(50));
    CHECK(bst.insert(30));
    CHECK(bst.insert(70));
    CHECK(bst.insert(20));
    CHECK(bst.insert(40));
    CHECK(bst.insert(60));
    CHECK(bst.insert(80));

    // Duplicates should be rejected
    CHECK(!bst.insert(50));
    CHECK(!bst.insert(30));
    CHECK(bst.getSize() == 7);

    // Search tests
    size_t depth = 0;
    CHECK(bst.search(50, depth) && depth == 0);
    CHECK(bst.search(30, depth) && depth == 1);
    CHECK(bst.search(80, depth) && depth == 2);
    CHECK(!bst.search(99, depth));

    // Inorder traversal must be strictly ascending: 20, 30, 40, 50, 60, 70, 80
    size_t count = 0;
    int* inArr = bst.getInorder(count);
    CHECK(count == 7);
    int expectedInorder[] = {20, 30, 40, 50, 60, 70, 80};
    for (size_t i = 0; i < count; ++i) {
        CHECK(inArr[i] == expectedInorder[i]);
    }
    delete[] inArr;

    // Preorder: 50, 30, 20, 40, 70, 60, 80
    int* preArr = bst.getPreorder(count);
    CHECK(count == 7);
    int expectedPreorder[] = {50, 30, 20, 40, 70, 60, 80};
    for (size_t i = 0; i < count; ++i) {
        CHECK(preArr[i] == expectedPreorder[i]);
    }
    delete[] preArr;

    // Postorder: 20, 40, 30, 60, 80, 70, 50
    int* postArr = bst.getPostorder(count);
    CHECK(count == 7);
    int expectedPostorder[] = {20, 40, 30, 60, 80, 70, 50};
    for (size_t i = 0; i < count; ++i) {
        CHECK(postArr[i] == expectedPostorder[i]);
    }
    delete[] postArr;

    // Deletion Case 1: Leaf node (20)
    CHECK(bst.remove(20));
    CHECK(bst.getSize() == 6);
    CHECK(!bst.search(20, depth));

    // Deletion Case 2: Node with 1 child
    // Add child 85 to 80
    CHECK(bst.insert(85));
    // Now delete 80 (has one child 85)
    CHECK(bst.remove(80));
    CHECK(bst.search(85, depth) && depth >= 1);

    // Deletion Case 3: Node with 2 children (50 - root)
    CHECK(bst.remove(50));
    CHECK(bst.getSize() == 5);
    CHECK(!bst.search(50, depth));
    // Verify tree remains a valid BST after root removal
    inArr = bst.getInorder(count);
    for (size_t i = 0; i < count - 1; ++i) {
        CHECK(inArr[i] < inArr[i + 1]);
    }
    delete[] inArr;

    bst.clear();
    CHECK(bst.isEmpty());
    CHECK(bst.getSize() == 0);
    std::cout << "  -> BinarySearchTree tests passed successfully!" << std::endl;
}

void testSorting() {
    std::cout << "[TEST] Running Sorting tests..." << std::endl;
    // Bubble Sort
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    size_t comp = 0, swaps = 0;
    algorithms::SortingAlgorithms::bubbleSort(arr1, 7, comp, swaps);
    for (int i = 0; i < 6; ++i) {
        CHECK(arr1[i] <= arr1[i + 1]);
    }

    // Selection Sort
    int arr2[] = {29, 10, 14, 37, 13};
    algorithms::SortingAlgorithms::selectionSort(arr2, 5, comp, swaps);
    for (int i = 0; i < 4; ++i) {
        CHECK(arr2[i] <= arr2[i + 1]);
    }

    // Insertion Sort
    int arr3[] = {12, 11, 13, 5, 6};
    algorithms::SortingAlgorithms::insertionSort(arr3, 5, comp, swaps);
    for (int i = 0; i < 4; ++i) {
        CHECK(arr3[i] <= arr3[i + 1]);
    }

    // Test Step Recorder
    int arr4[] = {5, 2, 8, 1, 9};
    algorithms::StepRecorder recorder;
    algorithms::SortStats stats;
    algorithms::SortingAlgorithms::recordBubbleSort(arr4, 5, recorder, stats);
    CHECK(recorder.getCount() > 0);
    // Check that final step contains a sorted array
    const algorithms::SortStep& lastStep = recorder.getStep(recorder.getCount() - 1);
    CHECK(lastStep.type == algorithms::StepType::FINISHED);
    for (size_t i = 0; i < lastStep.arraySize - 1; ++i) {
        CHECK(lastStep.arraySnapshot[i] <= lastStep.arraySnapshot[i + 1]);
    }

    std::cout << "  -> Sorting tests passed successfully!" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << " DSA Visualizer Studio - Core Test Suite " << std::endl;
    std::cout << "========================================" << std::endl;

    testStack();
    testQueue();
    testSinglyLinkedList();
    testBinarySearchTree();
    testSorting();

    std::cout << "========================================" << std::endl;
    std::cout << " ALL DATA STRUCTURES & ALGORITHMS PASSED!" << std::endl;
    std::cout << "========================================" << std::endl;
    return 0;
}


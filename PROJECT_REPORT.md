# ACADEMIC PROJECT REPORT

---

## **DSA VISUALIZER STUDIO**
### An Interactive C++17 Desktop Application for Data Structure Simulation and Algorithmic Analysis

```
Project Title        : DSA Visualizer Studio
Domain               : Data Structures, Algorithms & System Software Design
Language / Standard  : C++17
GUI Framework        : raylib 5.0
Build System         : CMake 3.16+ with Ninja
Compiler             : GCC 15.2.0 (MSYS2 UCRT64)
Target Platform      : Windows 11 (x64)
Architecture         : Modular Layered Architecture (Zero STL Containers in Core Library)
```

---

## TABLE OF CONTENTS

1. [Abstract](#1-abstract)
2. [Introduction & Motivation](#2-introduction--motivation)
3. [Project Objectives & Scope](#3-project-objectives--scope)
4. [System Requirements & Toolchain](#4-system-requirements--toolchain)
5. [Software Architecture & Design Principles](#5-software-architecture--design-principles)
6. [Detailed Data Structure Implementation (From Scratch)](#6-detailed-data-structure-implementation-from-scratch)
   - 6.1 [Custom Stack (LIFO)](#61-custom-stack-lifo)
   - 6.2 [Custom Queue (FIFO)](#62-custom-queue-fifo)
   - 6.3 [Custom Singly Linked List](#63-custom-singly-linked-list)
   - 6.4 [Custom Binary Search Tree (BST)](#64-custom-binary-search-tree-bst)
7. [Sorting Algorithms & Step-Recording Engine](#7-sorting-algorithms--step-recording-engine)
   - 7.1 [Bubble Sort](#71-bubble-sort)
   - 7.2 [Selection Sort](#72-selection-sort)
   - 7.3 [Insertion Sort](#73-insertion-sort)
   - 7.4 [The Step Recorder State Machine](#74-the-step-recorder-state-machine)
8. [GUI / UX System Architecture](#8-gui--ux-system-architecture)
   - 8.1 [Theme and Color Palette](#81-theme-and-color-palette)
   - 8.2 [Custom UI Widgets](#82-custom-ui-widgets)
   - 8.3 [Visualization Canvases](#83-visualization-canvases)
9. [Memory Management, RAII & Error Handling](#9-memory-management-raii--error-handling)
10. [Verification, Testing & Results](#10-verification-testing--results)
11. [Conclusion & Future Enhancements](#11-conclusion--future-enhancements)
12. [References](#12-references)

---

## 1. ABSTRACT

Understanding abstract data structures and pointer-driven memory management is a fundamental hurdle in computer science education. Traditional pedagogical methods rely on static blackboard diagrams or terminal-based text dumps, which fail to convey the dynamic pointer re-assignments and dynamic heap allocation occurring at runtime.

**DSA Visualizer Studio** is an interactive, native desktop suite designed to bridge the gap between theoretical algorithmic analysis and real-time visual execution. Engineered in modern **C++17** and rendered with hardware-accelerated **raylib 5.0**, the application simulates the internal operations of fundamental data structures (**Stack, Queue, Singly Linked List, Binary Search Tree**) and comparative sorting algorithms (**Bubble Sort, Selection Sort, Insertion Sort**). 

Crucially, the core data structures library is developed **strictly without Standard Template Library (STL) containers** (`std::vector`, `std::stack`, `std::queue`, `std::list`). All nodes are dynamically allocated on the heap using raw pointers, managed via explicit constructors and destructors (RAII), guarded against underflow and invalid inputs, and verified to be 100% memory-leak free.

---

## 2. INTRODUCTION & MOTIVATION

Data structures are specialized formats for organizing, processing, retrieving, and storing data. While foundational, topics such as node chaining, pointer redirection, recursion trees, and in-place array reordering are notoriously difficult to conceptualize without visual aids.

Many existing visualizers are browser-based web applications written in JavaScript. While visually pleasant, they obscure low-level computer systems concepts:
* Memory is managed by an automatic garbage collector rather than explicit programmer control.
* Pointers and addresses are abstracted away into high-level object references.
* True native execution speed, OS-level windowing, and compile-time type safety are absent.

This project was conceived to deliver a **native, compiled C++ desktop application** that provides real-time visualization while preserving true systems-level mechanics: manual heap allocation (`new`), memory deallocation (`delete`), pointer navigation (`curr = curr->next`), and theoretical algorithmic complexity monitoring ($O(1)$, $O(n)$, $O(\log n)$).

---

## 3. PROJECT OBJECTIVES & SCOPE

### 3.1 Primary Objectives
1. **Zero-STL Core Library:** Implement Stack, Queue, Singly Linked List, and Binary Search Tree entirely from scratch with raw pointers and dynamic heap allocation.
2. **Modular Architecture:** Completely decouple the core algorithms and data structures library (`dsa_core`) from the graphical user interface (`gui`).
3. **Comprehensive Operation Coverage:**
   * **Stack:** Push, Pop, Peek, Clear, Underflow handling.
   * **Queue:** Enqueue, Dequeue, Front, Clear, Underflow handling.
   * **Singly Linked List:** Insert (Head, Tail, Arbitrary Index), Delete (Head, Tail, Value, Index), Linear Search, Traversal.
   * **Binary Search Tree:** Recursive Insert, Search with depth tracking, Node Deletion across all 3 structural cases (Leaf, One Child, Two Children with Inorder Successor), and Traversals (Inorder, Preorder, Postorder).
   * **Sorting Visualizer:** Bubble Sort, Selection Sort, Insertion Sort with Play/Pause, Step Forward, Reset, Shuffle, and Speed regulation.
4. **Professional UI/UX:** Dark-themed presentation, interactive controls (custom text inputs, buttons, sliders), real-time status logging, and complexity cards.
5. **Memory Safety & Reliability:** Strict adhere to the Rule of Five, zero dangling pointers, and complete prevention of memory leaks.

---

## 4. SYSTEM REQUIREMENTS & TOOLCHAIN

### 4.1 Target Environment
* **Operating System:** Windows 11 (64-bit)
* **Architecture:** x86_64

### 4.2 Build Toolchain
* **Language Standard:** C++17 (`-std=c++17`)
* **C/C++ Compiler:** GCC 15.2.0 (MSYS2 UCRT64 toolchain)
* **Build System:** CMake (Version 3.16 to 3.30)
* **Build Generator:** Ninja 1.13+
* **GUI Library:** raylib 5.0 (statically compiled from source with GLFW and OpenGL 3.3 backend)
* **OS Libraries:** `opengl32`, `gdi32`, `winmm`

---

## 5. SOFTWARE ARCHITECTURE & DESIGN PRINCIPLES

The system follows a **Three-Tier Modular Architecture**:

```
+-------------------------------------------------------------+
|                     PRESENTATION TIER                       |
|   (Application Shell, Sidebar Navigation, Header Bar)       |
+-------------------------------------------------------------+
                              |
+-------------------------------------------------------------+
|                        VIEW TIER                            |
|   (DashboardView, StackView, QueueView, LinkedListView,     |
|              BSTView, SortingView, UIComponents)            |
+-------------------------------------------------------------+
                              |
+-------------------------------------------------------------+
|                      CORE DSA TIER                          |
|   (Stack, Queue, SinglyLinkedList, BST, SortingAlgorithms)  |
|            * 100% Custom, Zero STL Containers *             |
+-------------------------------------------------------------+
```

### Key Design Principles:
1. **Single Responsibility Principle (SRP):** Core data structures manage data integrity and memory allocation only. They have no dependency on raylib or GUI headers.
2. **Open-Closed Principle (OCP):** Views inherit from a base `View` abstract polymorphic class (`update()`, `draw()`, `reset()`), enabling new visualizers to be integrated without modifying the application harness.
3. **Resource Acquisition Is Initialization (RAII):** Container classes own their allocated heap memory and release all nodes cleanly in their respective destructors.
4. **Copy Prevention:** Copy constructors and assignment operators are explicitly marked `= delete` on node-based containers to prevent inadvertent shallow copies that would cause double-free errors.

---

## 6. DETAILED DATA STRUCTURE IMPLEMENTATION (FROM SCRATCH)

### 6.1 Custom Stack (LIFO)
* **Header:** `include/dsa/Stack.hpp` | **Source:** `src/dsa/Stack.cpp`
* **Node Structure:**
  ```cpp
  struct StackNode {
      int data;
      StackNode* next;
      explicit StackNode(int value, StackNode* nextNode = nullptr)
          : data(value), next(nextNode) {}
  };
  ```
* **Internal Mechanics:**
  * `topNode`: Pointer to the node at the top of the stack.
  * `push(val)`: Allocates `new StackNode(val, topNode)` and sets `topNode = newNode`. Runs in $O(1)$ time.
  * `pop(outVal)`: Validates underflow (`topNode == nullptr`). If valid, extracts `data`, advances `topNode = topNode->next`, and invokes `delete` on the old top node. Runs in $O(1)$ time.
  * `peek(outVal)`: Reads `topNode->data` without altering pointers ($O(1)$).
  * `clear()`: Iteratively traverses from `topNode`, freeing every node ($O(n)$).

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| Push | $O(1)$ | $O(1)$ |
| Pop | $O(1)$ | $O(1)$ |
| Peek | $O(1)$ | $O(1)$ |
| Clear | $O(n)$ | $O(1)$ |

---

### 6.2 Custom Queue (FIFO)
* **Header:** `include/dsa/Queue.hpp` | **Source:** `src/dsa/Queue.cpp`
* **Internal Mechanics:**
  * Uses two raw pointers: `frontNode` and `rearNode`.
  * `enqueue(val)`: Creates `new QueueNode(val, nullptr)`. If empty, sets `frontNode = rearNode = newNode`. Otherwise, attaches `rearNode->next = newNode` and updates `rearNode = newNode`. Time: $O(1)$.
  * `dequeue(outVal)`: Checks for underflow. Extracts `frontNode->data`. Advances `frontNode = frontNode->next`. If `frontNode` becomes `nullptr`, sets `rearNode = nullptr`. Deletes the extracted node. Time: $O(1)$.
  * `front(outVal)`: Direct read of `frontNode->data` in $O(1)$.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| Enqueue | $O(1)$ | $O(1)$ |
| Dequeue | $O(1)$ | $O(1)$ |
| Front | $O(1)$ | $O(1)$ |
| Clear | $O(n)$ | $O(1)$ |

---

### 6.3 Custom Singly Linked List
* **Header:** `include/dsa/SinglyLinkedList.hpp` | **Source:** `src/dsa/SinglyLinkedList.cpp`
* **Node Anatomy:** Each `ListNode` stores `int data` (4 bytes) and `ListNode* next` (8 bytes on 64-bit), padded to 16 bytes.
* **Core Operations:**
  * `insertHead(val)`: $O(1)$ operation setting `newNode->next = head; head = newNode;`.
  * `insertTail(val)`: Traverses to `curr->next == nullptr`, then attaches `curr->next = newNode`. Time: $O(n)$.
  * `insertAt(index, val)`: Validates bounds, walks $index - 1$ steps, and inserts between `curr` and `curr->next`. Time: $O(n)$.
  * `deleteHead(outVal)`: Unlinks `head`, advances to `head->next`, and deletes old head in $O(1)$.
  * `deleteValue(val)`: Employs a trailing pointer pattern (`curr->next->data == val`) to bridge pointers (`curr->next = temp->next`) before freeing `temp`. Time: $O(n)$.
  * `search(val, foundIdx)`: Sequentially compares each node from index 0 upward. Time: $O(n)$.

| Operation | Best Time | Average Time | Worst Time | Space |
| :--- | :--- | :--- | :--- | :--- |
| Insert Head | $O(1)$ | $O(1)$ | $O(1)$ | $O(1)$ |
| Insert Tail | $O(1)^*$ / $O(n)$ | $O(n)$ | $O(n)$ | $O(1)$ |
| Insert At Index | $O(1)$ (idx 0) | $O(n)$ | $O(n)$ | $O(1)$ |
| Delete Head | $O(1)$ | $O(1)$ | $O(1)$ | $O(1)$ |
| Delete Value | $O(1)$ (at head) | $O(n)$ | $O(n)$ | $O(1)$ |
| Search | $O(1)$ (at head) | $O(n)$ | $O(n)$ | $O(1)$ |

---

### 6.4 Custom Binary Search Tree (BST)
* **Header:** `include/dsa/BinarySearchTree.hpp` | **Source:** `src/dsa/BinarySearchTree.cpp`
* **BST Invariant:**
  $$\forall x \in \text{LeftSubtree}(R): x.\text{data} < R.\text{data} \quad \land \quad \forall y \in \text{RightSubtree}(R): y.\text{data} > R.\text{data}$$
* **Insertion:** Recursively navigates left or right. If duplicate key encountered, rejects insertion to preserve strict uniqueness.
* **Search:** Recursive binary search with depth tracking ($O(\log n)$ average).
* **The 3 Deletion Cases:**
  1. **Leaf Node (0 children):** Node is deleted directly; parent pointer is updated to `nullptr`.
  2. **Single Child (1 child):** Node is bypassed by linking parent directly to the non-null child; node is freed.
  3. **Two Children (2 children):** Node cannot be unlinked directly. The algorithm locates the **inorder successor** (the minimum node in the right subtree: `findMin(node->right)`). The successor's value is copied into the target node, and `deleteRec` is recursively called on the right subtree to remove the duplicate successor node.
* **Traversals:**
  * **Inorder (Left $\to$ Root $\to$ Right):** Produces elements in strictly ascending sorted order.
  * **Preorder (Root $\to$ Left $\to$ Right):** Parent first; ideal for topological serialization and cloning.
  * **Postorder (Left $\to$ Right $\to$ Root):** Children first; required for bottom-up memory deallocation.

| Operation | Average Case | Worst Case (Skewed Tree) | Space Complexity |
| :--- | :--- | :--- | :--- |
| Insert | $O(\log n)$ | $O(n)$ | $O(\log n)$ call stack |
| Search | $O(\log n)$ | $O(n)$ | $O(\log n)$ call stack |
| Delete | $O(\log n)$ | $O(n)$ | $O(\log n)$ call stack |
| Traversals | $O(n)$ | $O(n)$ | $O(n)$ auxiliary array |

---

## 7. SORTING ALGORITHMS & STEP-RECORDING ENGINE

### 7.1 Bubble Sort
Compares adjacent pairs $(j, j+1)$ and swaps if out of order. Optimized with a `swapped` boolean flag: if an entire pass completes without a single swap, the sort terminates early in $O(n)$ best-case time.
* **Time:** Best $O(n)$ | Average $O(n^2)$ | Worst $O(n^2)$
* **Auxiliary Space:** $O(1)$ | **Stability:** Stable

### 7.2 Selection Sort
Divides the array into a sorted prefix and unsorted suffix. Finds the global minimum in the unsorted suffix and swaps it to index $i$.
* **Time:** Best $O(n^2)$ | Average $O(n^2)$ | Worst $O(n^2)$
* **Auxiliary Space:** $O(1)$ | **Stability:** Unstable | **Max Swaps:** $n - 1$

### 7.3 Insertion Sort
Maintains a sorted subarray on the left. Takes element $i$ (`key`), compares backwards, shifts larger elements right, and inserts `key` at the opened slot.
* **Time:** Best $O(n)$ | Average $O(n^2)$ | Worst $O(n^2)$
* **Auxiliary Space:** $O(1)$ | **Stability:** Stable | **Adaptive:** Excellent on nearly-sorted data.

### 7.4 The Step Recorder State Machine
To avoid freezing the GUI with blocking `sleep()` calls, algorithms run through a **Step Recording Engine** (`SortStep.hpp`). The engine executes the sort instantaneously off-screen, recording atomic `SortStep` events:
* `StepType` (`START`, `COMPARE`, `SWAP`, `SHIFT`, `MARK_MIN`, `MARK_SORTED`, `FINISHED`)
* Active indices $A$ and $B$
* Dynamic array snapshot at that exact millisecond
* Explanatory text description (e.g., *"Compare index 2 (34) and index 3 (12) -> Swap required"*).

The GUI then plays back these pre-recorded steps with variable speed control, single-stepping, pause, and reset capabilities.

---

## 8. GUI / UX SYSTEM ARCHITECTURE

### 8.1 Theme and Color Palette
* **Background Deep Slate:** `#0f172a` (`Color{15, 23, 42, 255}`)
* **Card & Panel Slate:** `#1e293b` (`Color{30, 41, 59, 255}`)
* **Border & Line:** `#334155` (`Color{51, 65, 85, 255}`)
* **Primary Accent (Indigo):** `#6366f1` (`Color{99, 102, 241, 255}`)
* **Success / Sorted (Emerald):** `#10b981` (`Color{16, 185, 129, 255}`)
* **Warning / Comparing (Amber):** `#f59e0b` (`Color{245, 158, 11, 255}`)
* **Danger / Swapping (Rose):** `#f43f5e` (`Color{244, 63, 94, 255}`)

### 8.2 Custom UI Widgets
* **`Button`:** Handles hover tinting, active press down-shifts (tactile feedback), disabled states, and clean rounded rectangle borders.
* **`TextBox`:** Numeric input validation, cursor blink timer, active focus highlight, placeholder rendering, and Enter key submission.
* **`Slider`:** Interactive mouse drag tracking, clamped ratio interpolation, and value formatting.
* **`StatusConsole`:** Circular message history showing timestamped `[INFO]`, `[SUCCESS]`, `[WARNING]`, and `[ERROR]` logs with colored indicator dots.
* **`ComplexityCard`:** Formatted grid displaying theoretical Best, Average, Worst Time, and Auxiliary Space complexities.

### 8.3 Visualization Canvases
1. **Stack Canvas:** Renders a vertical glass beaker with base platform, stacked element blocks, a dynamic `TOP ->` pointer arrow, and a glowing green peek highlight.
2. **Queue Canvas:** Horizontal pipeline container with `ENTRY (REAR)` on the right, `EXIT (FRONT)` on the left, inter-node flow arrows, and pointer badges.
3. **Linked List Canvas:** CS-standard `[ Data | Next* ]` node compartments with pointer arrows connecting to the next node and terminating at a styled `nullptr` badge. Features an animated linear search scan highlighting nodes step-by-step.
4. **BST Canvas:** Dynamic recursive 2D tree layout calculating $(x, y)$ coordinates with level-halved offsets so branches never intersect. Features interactive step-by-step Inorder, Preorder, and Postorder traversal playback with an ordered visited sequence ribbon.
5. **Sorting Canvas:** Vertical bar chart scaled to value, dynamic color-coding based on step state, live comparison and swap counters, and text description ribbon.

---

## 9. MEMORY MANAGEMENT, RAII & ERROR HANDLING

1. **Explicit Destruction:** All containers implement destructors that loop through all allocated nodes and call `delete`, guaranteeing zero memory leaks.
2. **Rule of Five Implementation:**
   * Copy Constructor: Marked `= delete` to eliminate shallow pointer copies.
   * Copy Assignment: Marked `= delete` to eliminate double-free bugs.
   * Move Constructor: Implemented with `noexcept`, transferring ownership of root/head pointers and nullifying the source.
   * Move Assignment: Implemented with `noexcept`, cleaning existing resources before transferring ownership.
3. **Underflow Protection:** All destructive extraction methods (`pop`, `dequeue`, `deleteHead`, `deleteTail`, `deleteValue`) explicitly verify if the structure is empty and return boolean status flags instead of dereferencing null pointers.
4. **Input Validation:** GUI text boxes restrict keystrokes to numeric digits and optional leading minus signs, blocking non-numeric inputs at the interface level.

---

## 10. VERIFICATION, TESTING & RESULTS

### 10.1 Test Methodology
A dedicated test harness (`tests/test_dsa.cpp`) was constructed to validate every function, boundary condition, and edge case:

```cpp
void testStack();              // Push, Pop, Peek, Underflow, Clear
void testQueue();              // Enqueue, Dequeue, Front, Underflow, Clear
void testSinglyLinkedList();   // Head/Tail/Index Insert, Head/Tail/Value Delete, Search
void testBinarySearchTree();   // Insert, Duplicates, 3 Deletion Cases, In/Pre/Postorder
void testSorting();            // In-place sort correctness, Step recorder verification
```

### 10.2 Test Output
Compiled with GCC 15.2.0 (`-std=c++17 -Wall -Wextra -O2`):

```text
========================================
 DSA Visualizer Studio - Core Test Suite 
========================================
[TEST] Running Stack tests...
  -> Stack tests passed successfully!
[TEST] Running Queue tests...
  -> Queue tests passed successfully!
[TEST] Running SinglyLinkedList tests...
  -> SinglyLinkedList tests passed successfully!
[TEST] Running BinarySearchTree tests...
  -> BinarySearchTree tests passed successfully!
[TEST] Running Sorting tests...
  -> Sorting tests passed successfully!
========================================
 ALL DATA STRUCTURES & ALGORITHMS PASSED!
========================================

Test project D:/java coding/build
    Start 1: CoreDSATests
1/1 Test #1: CoreDSATests .....................   Passed    0.04 sec

100% tests passed out of 1
```

### 10.3 Performance Summary
* **Frame Rate:** Locked 60 FPS VSync with minimal GPU/CPU overhead.
* **Executable Size:** ~1.4 MB standalone executable.
* **Build Time:** ~12 seconds with Ninja parallel compilation.

---

## 11. CONCLUSION & FUTURE ENHANCEMENTS

### 11.1 Conclusion
**DSA Visualizer Studio** demonstrates that low-level systems programming in C++17 can be merged with modern GUI design to create an educational software application. By implementing all data structures from scratch without STL containers, the project showcases rigorous dynamic memory management, pointer manipulation, and algorithmic efficiency.

### 11.2 Future Enhancements
* **Self-Balancing Trees:** Adding AVL Tree and Red-Black Tree visualizations with animated tree rotation diagrams.
* **Advanced Sorting:** Extending the comparative suite to Quick Sort (pivot partitioning), Merge Sort (divide-and-conquer), and Heap Sort.
* **Graph Algorithms:** Adding Dijkstra's Shortest Path, Prim's/Kruskal's MST, and animated BFS/DFS graph traversals.

---

## 12. REFERENCES

1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
2. Stroustrup, B. (2018). *The C++ Programming Language* (4th ed.). Addison-Wesley.
3. Meyers, S. (2014). *Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14*. O'Reilly Media.
4. Ray, R. (2023). *raylib: A simple and easy-to-use library to enjoy videogames programming*. https://www.raylib.com
5. ISO/IEC. (2017). *ISO International Standard ISO/IEC 14882:2017(E) - Programming Language C++*.

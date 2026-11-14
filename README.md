# DSA Visualizer Studio (C++17 + raylib)

A desktop application and educational software suite for simulating, visualizing, and mastering core Data Structures and Algorithms. Built from the ground up in modern **C++17** and **raylib 5.0**, powered by **CMake** and **GCC (MSYS2 UCRT64)** on Windows 11.

---

## Key Highlights

- **Zero STL in Core DSA Library:** Core data structures (Stack, Queue, Singly Linked List, Binary Search Tree) are built entirely from scratch with raw pointers, explicit dynamic memory management (`new` / `delete`), and strict underflow/overflow handling.
- **Hardware-Accelerated Dark Theme UI:** Sleek modern interface with anti-aliasing, rounded cards, live status activity consoles, and theoretical complexity tags ($O(1)$, $O(n)$, $O(\log n)$).
- **Interactive Visualizers:**
  - **Stack Visualizer (LIFO):** Vertical beaker container with animated top pointer, push/pop mechanisms, and peek highlight glow.
  - **Queue Visualizer (FIFO):** Horizontal sequential pipeline with front and rear pointer indicators and directional flow.
  - **Singly Linked List Visualizer:** Linked node compartments `[Data | Next*]` with pointer arrows, `nullptr` terminator, head/tail markers, and animated step-by-step linear search scan.
  - **Binary Search Tree Visualizer (BST):** Dynamic recursive 2D tree layout, logarithmic search, 3-case node deletion, and animated step-by-step Inorder, Preorder, and Postorder traversals with a live visited node ribbon.
  - **Sorting Visualizer:** Comparative bar chart visualizer for Bubble Sort, Selection Sort, and Insertion Sort with Play/Pause, Step Forward, Reset, Shuffle, speed slider, and live comparison/swap counters.
- **Robust Error Handling:** Rejects invalid or empty text inputs, catches underflow conditions gracefully, and reports operation results to the activity console.
- **Zero Memory Leaks:** Rigorously tested destructors, move semantics, and automated unit test suite.
- **College Viva Ready:** Includes [VIVA_PREPARATION.md](file:///d:/java%20coding/VIVA_PREPARATION.md) covering theoretical concepts, internal memory diagrams, and 30+ examiner questions with model answers.

---

## Project Structure

```
d:\java coding\
├── CMakeLists.txt              # Top-level CMake configuration
├── build.bat                   # 1-click build script (CMake + Ninja + GCC)
├── run.bat                     # 1-click execution script (launches dsa_studio.exe)
├── run_tests.bat               # 1-click unit test runner
├── README.md                   # Project documentation and guide
├── VIVA_PREPARATION.md         # College viva and practical exam notes
├── third_party\
│   └── raylib\                 # raylib 5.0 source library
├── include\
│   ├── dsa\
│   │   ├── Stack.hpp           # Custom LIFO Stack header
│   │   ├── Queue.hpp           # Custom FIFO Queue header
│   │   ├── SinglyLinkedList.hpp# Custom Singly Linked List header
│   │   └── BinarySearchTree.hpp# Custom BST header
│   ├── algorithms\
│   │   ├── SortingAlgorithms.hpp# Bubble, Selection, Insertion Sort
│   │   └── SortStep.hpp        # Step recorder engine for visualizer
│   └── gui\
│       ├── UICommon.hpp        # Palette, geometry, drawing helpers
│       ├── UIComponents.hpp    # Button, TextBox, Slider, Console, Complexity
│       ├── View.hpp            # Base view polymorphic interface
│       ├── DashboardView.hpp   # Studio overview and navigation cards
│       ├── StackView.hpp       # Stack visualizer view
│       ├── QueueView.hpp       # Queue visualizer view
│       ├── LinkedListView.hpp  # Singly Linked List visualizer view
│       ├── BSTView.hpp         # BST visualizer view
│       ├── SortingView.hpp     # Sorting algorithms visualizer view
│       └── Application.hpp     # Application shell and sidebar controller
├── src\
│   ├── dsa\
│   │   ├── Stack.cpp
│   │   ├── Queue.cpp
│   │   ├── SinglyLinkedList.cpp
│   │   └── BinarySearchTree.cpp
│   ├── algorithms\
│   │   └── SortingAlgorithms.cpp
│   ├── gui\
│   │   ├── UICommon.cpp
│   │   ├── UIComponents.cpp
│   │   ├── DashboardView.cpp
│   │   ├── StackView.cpp
│   │   ├── QueueView.cpp
│   │   ├── LinkedListView.cpp
│   │   ├── BSTView.cpp
│   │   ├── SortingView.cpp
│   │   └── Application.cpp
│   └── main.cpp                # Application entry point
└── tests\
    └── test_dsa.cpp            # Core verification and unit test suite
```

---

## Prerequisites & Development Environment

- **Operating System:** Windows 11 (64-bit)
- **Compiler:** GCC 15.2.0 (MSYS2 UCRT64 located at `C:\msys64\ucrt64\bin`)
- **Build System:** CMake 3.16+ and Ninja
- **Graphics / GUI:** raylib 5.0 (vendored locally in `third_party/raylib`, statically compiled)

---

## Build & Execution Instructions

### Option 1: Quick Batch Scripts (Recommended)

1. **Build the Application:**
   Double-click `build.bat` or run:
   ```cmd
   .\build.bat
   ```
2. **Run the Application:**
   Double-click `run.bat` or run:
   ```cmd
   .\run.bat
   ```
3. **Run the Unit Test Suite:**
   Double-click `run_tests.bat` or run:
   ```cmd
   .\run_tests.bat
   ```

### Option 2: Command Line (PowerShell / CMD)

From the project root directory (`d:\java coding`):

```powershell
# 1. Configure the build directory using CMake and Ninja
cmake -B "build" -S "." -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release

# 2. Compile the desktop application and test suite
cmake --build "build" --config Release

# 3. Launch the desktop GUI visualizer
.\build\dsa_studio.exe

# 4. Run automated test suite
.\build\test_dsa.exe
```

---

## Implemented Data Structures & Algorithms

### 1. Stack
- **Principle:** LIFO (Last-In, First-Out)
- **Operations:** `push(int value)`, `pop(int& out)`, `peek(int& out)`, `clear()`, `isEmpty()`, `getSize()`, `toArray()`.
- **Complexity:** All top operations are $O(1)$ constant time; space is $O(n)$.
- **Visual Features:** Vertical beaker container, dynamic element stack, top pointer indicator arrow, peek highlight glow effect, and underflow warning message.

### 2. Queue
- **Principle:** FIFO (First-In, First-Out)
- **Operations:** `enqueue(int value)`, `dequeue(int& out)`, `front(int& out)`, `clear()`, `isEmpty()`, `getSize()`, `toArray()`.
- **Complexity:** All operations are $O(1)$ constant time; space is $O(n)$.
- **Visual Features:** Horizontal pipeline with exit on left and entry on right, front/rear pointer indicators, inter-node directional arrows, and underflow protection.

### 3. Singly Linked List
- **Principle:** Linear pointer-chained dynamic node structure.
- **Operations:**
  - `insertHead(int value)`
  - `insertTail(int value)`
  - `insertAt(size_t index, int value)`
  - `deleteHead(int& out)`
  - `deleteTail(int& out)`
  - `deleteValue(int value)`
  - `deleteAt(size_t index, int& out)`
  - `search(int value, size_t& foundIndex)`
  - `clear()`, `isEmpty()`, `getSize()`, `toArray()`.
- **Complexity:** Head insert/delete $O(1)$; arbitrary index insertion, deletion, and search are $O(n)$.
- **Visual Features:** Standard `[ Data | Next* ]` node compartments, connecting arrows, terminating `nullptr` badge, animated linear search scan highlighting nodes step-by-step.

### 4. Binary Search Tree (BST)
- **Principle:** Hierarchical binary tree maintaining $\text{Left} < \text{Root} < \text{Right}$.
- **Operations:**
  - `insert(int value)` (rejects duplicates)
  - `remove(int value)` (handles Leaf, Single Child, and Two Children with Inorder Successor)
  - `search(int value, size_t& outDepth)`
  - `getInorder()`, `getPreorder()`, `getPostorder()`
  - `clear()`, `isEmpty()`, `getSize()`, `getHeight()`.
- **Complexity:** Average $O(\log n)$ search/insert/delete; worst-case skewed tree $O(n)$.
- **Visual Features:** Dynamic 2D recursive tree layout with anti-aliased branch lines, animated search node highlight, and interactive step-by-step traversal playback with an ordered sequence ribbon.

### 5. Sorting Algorithms
- **Bubble Sort:** Pairwise adjacent comparisons, pass counter, swap counter, early termination flag when already sorted.
- **Selection Sort:** Unsorted suffix scanning, candidate minimum pointer, single swap per pass, pass counter, swap counter.
- **Insertion Sort:** Key selection, backwards comparison and shift loop, placement at correct position, sorted subarray tracking.
- **Playback Controls:** Play/Pause auto playback, Step Forward, Reset, Shuffle, Array Size selector (10, 15, 20 items), Speed Slider (0.2x to 3.0x).

---

## Test Verification Results

All automated tests passed with 0 failures:

```
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

100% tests passed out of 1
Test #1: CoreDSATests ..................... Passed (0.04 sec)
```

---

## College Viva Preparation Guide

For in-depth explanations, algorithmic trade-offs, and 30+ commonly asked viva questions, refer to [VIVA_PREPARATION.md](file:///d:/java%20coding/VIVA_PREPARATION.md).

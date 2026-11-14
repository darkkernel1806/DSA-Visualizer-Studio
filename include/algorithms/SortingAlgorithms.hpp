#ifndef ALGORITHMS_SORTING_ALGORITHMS_HPP
#define ALGORITHMS_SORTING_ALGORITHMS_HPP

#include "algorithms/SortStep.hpp"
#include <cstddef>

namespace algorithms {

enum class SortAlgorithmType {
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT
};

struct SortStats {
    size_t comparisons;
    size_t swaps;
    const char* name;
    const char* bestCase;
    const char* avgCase;
    const char* worstCase;
    const char* spaceComplexity;
    bool isStable;
};

class SortingAlgorithms {
public:
    // Direct in-place sorts (returns comparison and swap counts)
    static void bubbleSort(int* arr, size_t size, size_t& comparisons, size_t& swaps);
    static void selectionSort(int* arr, size_t size, size_t& comparisons, size_t& swaps);
    static void insertionSort(int* arr, size_t size, size_t& comparisons, size_t& swaps);

    // Step-by-step generator for visualizer animation
    static void recordBubbleSort(int* arr, size_t size, StepRecorder& recorder, SortStats& stats);
    static void recordSelectionSort(int* arr, size_t size, StepRecorder& recorder, SortStats& stats);
    static void recordInsertionSort(int* arr, size_t size, StepRecorder& recorder, SortStats& stats);

    // Metadata helpers
    static SortStats getStatsFor(SortAlgorithmType type);
    static const char* getAlgorithmName(SortAlgorithmType type);
};

} // namespace algorithms

#endif // ALGORITHMS_SORTING_ALGORITHMS_HPP

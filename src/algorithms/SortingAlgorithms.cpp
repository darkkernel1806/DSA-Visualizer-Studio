#include "algorithms/SortingAlgorithms.hpp"
#include <cstdio>

namespace algorithms {

static void swapInt(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void SortingAlgorithms::bubbleSort(int* arr, size_t size, size_t& comparisons, size_t& swaps) {
    comparisons = 0;
    swaps = 0;
    if (!arr || size <= 1) return;

    for (size_t i = 0; i < size - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < size - i - 1; ++j) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                swapInt(arr[j], arr[j + 1]);
                swaps++;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void SortingAlgorithms::selectionSort(int* arr, size_t size, size_t& comparisons, size_t& swaps) {
    comparisons = 0;
    swaps = 0;
    if (!arr || size <= 1) return;

    for (size_t i = 0; i < size - 1; ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < size; ++j) {
            comparisons++;
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swapInt(arr[i], arr[minIdx]);
            swaps++;
        }
    }
}

void SortingAlgorithms::insertionSort(int* arr, size_t size, size_t& comparisons, size_t& swaps) {
    comparisons = 0;
    swaps = 0;
    if (!arr || size <= 1) return;

    for (size_t i = 1; i < size; ++i) {
        int key = arr[i];
        int j = static_cast<int>(i) - 1;

        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                swaps++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

void SortingAlgorithms::recordBubbleSort(int* arr, size_t size, StepRecorder& recorder, SortStats& stats) {
    stats = getStatsFor(SortAlgorithmType::BUBBLE_SORT);
    recorder.clear();

    if (!arr || size == 0) return;

    char desc[160];
    std::snprintf(desc, sizeof(desc), "Initial Array State (Size %zu)", size);
    recorder.addStep(StepType::START, -1, -1, -1, desc, arr, size);

    if (size == 1) {
        recorder.addStep(StepType::FINISHED, 0, -1, -1, "Array of size 1 is already sorted", arr, size);
        return;
    }

    for (size_t i = 0; i < size - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < size - i - 1; ++j) {
            stats.comparisons++;
            std::snprintf(desc, sizeof(desc), "Compare index %zu (%d) and index %zu (%d)",
                          j, arr[j], j + 1, arr[j + 1]);
            recorder.addStep(StepType::COMPARE, static_cast<int>(j), static_cast<int>(j + 1), -1, desc, arr, size);

            if (arr[j] > arr[j + 1]) {
                swapInt(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;

                std::snprintf(desc, sizeof(desc), "Swap: %d > %d at indices %zu and %zu",
                              arr[j + 1], arr[j], j, j + 1);
                recorder.addStep(StepType::SWAP, static_cast<int>(j), static_cast<int>(j + 1), -1, desc, arr, size);
            }
        }

        size_t sortedIdx = size - 1 - i;
        std::snprintf(desc, sizeof(desc), "Pass %zu complete. Element %d at index %zu is in sorted position",
                      i + 1, arr[sortedIdx], sortedIdx);
        recorder.addStep(StepType::MARK_SORTED, static_cast<int>(sortedIdx), -1, -1, desc, arr, size);

        if (!swapped) {
            std::snprintf(desc, sizeof(desc), "No swaps occurred in pass %zu. Array is fully sorted early!", i + 1);
            recorder.addStep(StepType::MARK_SORTED, -1, -1, -1, desc, arr, size);
            break;
        }
    }

    // Mark index 0 as sorted as well
    recorder.addStep(StepType::MARK_SORTED, 0, -1, -1, "Element at index 0 is in sorted position", arr, size);
    std::snprintf(desc, sizeof(desc), "Bubble Sort Complete! Comparisons: %zu, Swaps: %zu",
                  stats.comparisons, stats.swaps);
    recorder.addStep(StepType::FINISHED, -1, -1, -1, desc, arr, size);
}

void SortingAlgorithms::recordSelectionSort(int* arr, size_t size, StepRecorder& recorder, SortStats& stats) {
    stats = getStatsFor(SortAlgorithmType::SELECTION_SORT);
    recorder.clear();

    if (!arr || size == 0) return;

    char desc[160];
    std::snprintf(desc, sizeof(desc), "Initial Array State (Size %zu)", size);
    recorder.addStep(StepType::START, -1, -1, -1, desc, arr, size);

    if (size == 1) {
        recorder.addStep(StepType::FINISHED, 0, -1, -1, "Array of size 1 is already sorted", arr, size);
        return;
    }

    for (size_t i = 0; i < size - 1; ++i) {
        size_t minIdx = i;
        std::snprintf(desc, sizeof(desc), "Pass %zu: Initial minimum assumed at index %zu (%d)",
                      i + 1, i, arr[i]);
        recorder.addStep(StepType::MARK_MIN, static_cast<int>(i), static_cast<int>(minIdx), arr[minIdx], desc, arr, size);

        for (size_t j = i + 1; j < size; ++j) {
            stats.comparisons++;
            std::snprintf(desc, sizeof(desc), "Compare index %zu (%d) with current min at index %zu (%d)",
                          j, arr[j], minIdx, arr[minIdx]);
            recorder.addStep(StepType::COMPARE, static_cast<int>(j), static_cast<int>(minIdx), arr[minIdx], desc, arr, size);

            if (arr[j] < arr[minIdx]) {
                minIdx = j;
                std::snprintf(desc, sizeof(desc), "New minimum found: %d at index %zu", arr[minIdx], minIdx);
                recorder.addStep(StepType::MARK_MIN, static_cast<int>(i), static_cast<int>(minIdx), arr[minIdx], desc, arr, size);
            }
        }

        if (minIdx != i) {
            swapInt(arr[i], arr[minIdx]);
            stats.swaps++;
            std::snprintf(desc, sizeof(desc), "Swap min %d (index %zu) into sorted position at index %zu",
                          arr[i], minIdx, i);
            recorder.addStep(StepType::SWAP, static_cast<int>(i), static_cast<int>(minIdx), -1, desc, arr, size);
        } else {
            std::snprintf(desc, sizeof(desc), "Minimum %d was already at index %zu (no swap needed)", arr[i], i);
            recorder.addStep(StepType::MARK_SORTED, static_cast<int>(i), -1, -1, desc, arr, size);
        }

        std::snprintf(desc, sizeof(desc), "Index %zu is now locked in sorted order", i);
        recorder.addStep(StepType::MARK_SORTED, static_cast<int>(i), -1, -1, desc, arr, size);
    }

    // Last element is also sorted
    recorder.addStep(StepType::MARK_SORTED, static_cast<int>(size - 1), -1, -1, "Last element is sorted", arr, size);
    std::snprintf(desc, sizeof(desc), "Selection Sort Complete! Comparisons: %zu, Swaps: %zu",
                  stats.comparisons, stats.swaps);
    recorder.addStep(StepType::FINISHED, -1, -1, -1, desc, arr, size);
}

void SortingAlgorithms::recordInsertionSort(int* arr, size_t size, StepRecorder& recorder, SortStats& stats) {
    stats = getStatsFor(SortAlgorithmType::INSERTION_SORT);
    recorder.clear();

    if (!arr || size == 0) return;

    char desc[160];
    std::snprintf(desc, sizeof(desc), "Initial Array State (Size %zu)", size);
    recorder.addStep(StepType::START, -1, -1, -1, desc, arr, size);

    if (size == 1) {
        recorder.addStep(StepType::FINISHED, 0, -1, -1, "Array of size 1 is already sorted", arr, size);
        return;
    }

    // First element by itself is trivially sorted
    recorder.addStep(StepType::MARK_SORTED, 0, -1, -1, "Single element at index 0 is trivially sorted", arr, size);

    for (size_t i = 1; i < size; ++i) {
        int key = arr[i];
        int j = static_cast<int>(i) - 1;

        std::snprintf(desc, sizeof(desc), "Pass %zu: Pick key = %d at index %zu to insert into sorted left portion",
                      i, key, i);
        recorder.addStep(StepType::START, static_cast<int>(i), -1, key, desc, arr, size);

        while (j >= 0) {
            stats.comparisons++;
            std::snprintf(desc, sizeof(desc), "Compare key %d with left element %d at index %d",
                          key, arr[j], j);
            recorder.addStep(StepType::COMPARE, j, static_cast<int>(i), key, desc, arr, size);

            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                stats.swaps++; // counted as shift/data movement
                std::snprintf(desc, sizeof(desc), "Shift %d to index %d to open slot for key %d",
                              arr[j + 1], j + 1, key);
                recorder.addStep(StepType::SHIFT, j, j + 1, key, desc, arr, size);
                j--;
            } else {
                std::snprintf(desc, sizeof(desc), "%d <= key %d, found insertion point at index %d",
                              arr[j], key, j + 1);
                recorder.addStep(StepType::COMPARE, j, j + 1, key, desc, arr, size);
                break;
            }
        }

        arr[j + 1] = key;
        std::snprintf(desc, sizeof(desc), "Placed key %d at index %d", key, j + 1);
        recorder.addStep(StepType::SWAP, j + 1, -1, key, desc, arr, size);

        for (size_t k = 0; k <= i; ++k) {
            std::snprintf(desc, sizeof(desc), "Elements 0..%zu are now in sorted order", i);
            recorder.addStep(StepType::MARK_SORTED, static_cast<int>(k), -1, -1, desc, arr, size);
        }
    }

    std::snprintf(desc, sizeof(desc), "Insertion Sort Complete! Comparisons: %zu, Shifts: %zu",
                  stats.comparisons, stats.swaps);
    recorder.addStep(StepType::FINISHED, -1, -1, -1, desc, arr, size);
}

SortStats SortingAlgorithms::getStatsFor(SortAlgorithmType type) {
    SortStats stats{};
    switch (type) {
        case SortAlgorithmType::BUBBLE_SORT:
            stats.name = "Bubble Sort";
            stats.bestCase = "O(n)";
            stats.avgCase = "O(n^2)";
            stats.worstCase = "O(n^2)";
            stats.spaceComplexity = "O(1)";
            stats.isStable = true;
            break;
        case SortAlgorithmType::SELECTION_SORT:
            stats.name = "Selection Sort";
            stats.bestCase = "O(n^2)";
            stats.avgCase = "O(n^2)";
            stats.worstCase = "O(n^2)";
            stats.spaceComplexity = "O(1)";
            stats.isStable = false;
            break;
        case SortAlgorithmType::INSERTION_SORT:
            stats.name = "Insertion Sort";
            stats.bestCase = "O(n)";
            stats.avgCase = "O(n^2)";
            stats.worstCase = "O(n^2)";
            stats.spaceComplexity = "O(1)";
            stats.isStable = true;
            break;
    }
    return stats;
}

const char* SortingAlgorithms::getAlgorithmName(SortAlgorithmType type) {
    return getStatsFor(type).name;
}

} // namespace algorithms

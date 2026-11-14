#ifndef ALGORITHMS_SORT_STEP_HPP
#define ALGORITHMS_SORT_STEP_HPP

#include <cstddef>
#include <cstring>
#include <utility>

namespace algorithms {

enum class StepType {
    NONE,
    START,
    COMPARE,
    SWAP,
    SHIFT,
    MARK_MIN,
    MARK_SORTED,
    FINISHED
};

struct SortStep {
    StepType type;
    int indexA;
    int indexB;
    int auxValue; // e.g. key in insertion sort or min index in selection sort
    char description[160];
    int* arraySnapshot; // Array state at this specific step
    size_t arraySize;

    SortStep()
        : type(StepType::NONE), indexA(-1), indexB(-1), auxValue(-1),
          arraySnapshot(nullptr), arraySize(0) {
        description[0] = '\0';
    }

    ~SortStep() {
        delete[] arraySnapshot;
    }

    // Rule of five: custom copy and move for deep copying snapshot
    SortStep(const SortStep& other)
        : type(other.type), indexA(other.indexA), indexB(other.indexB),
          auxValue(other.auxValue), arraySnapshot(nullptr), arraySize(other.arraySize) {
        std::strncpy(description, other.description, sizeof(description) - 1);
        description[sizeof(description) - 1] = '\0';
        if (other.arraySnapshot && other.arraySize > 0) {
            arraySnapshot = new int[other.arraySize];
            for (size_t i = 0; i < other.arraySize; ++i) {
                arraySnapshot[i] = other.arraySnapshot[i];
            }
        }
    }

    SortStep& operator=(const SortStep& other) {
        if (this != &other) {
            delete[] arraySnapshot;
            arraySnapshot = nullptr;
            type = other.type;
            indexA = other.indexA;
            indexB = other.indexB;
            auxValue = other.auxValue;
            arraySize = other.arraySize;
            std::strncpy(description, other.description, sizeof(description) - 1);
            description[sizeof(description) - 1] = '\0';
            if (other.arraySnapshot && other.arraySize > 0) {
                arraySnapshot = new int[other.arraySize];
                for (size_t i = 0; i < other.arraySize; ++i) {
                    arraySnapshot[i] = other.arraySnapshot[i];
                }
            }
        }
        return *this;
    }

    SortStep(SortStep&& other) noexcept
        : type(other.type), indexA(other.indexA), indexB(other.indexB),
          auxValue(other.auxValue), arraySnapshot(other.arraySnapshot), arraySize(other.arraySize) {
        std::strncpy(description, other.description, sizeof(description) - 1);
        description[sizeof(description) - 1] = '\0';
        other.arraySnapshot = nullptr;
        other.arraySize = 0;
    }

    SortStep& operator=(SortStep&& other) noexcept {
        if (this != &other) {
            delete[] arraySnapshot;
            type = other.type;
            indexA = other.indexA;
            indexB = other.indexB;
            auxValue = other.auxValue;
            arraySize = other.arraySize;
            arraySnapshot = other.arraySnapshot;
            std::strncpy(description, other.description, sizeof(description) - 1);
            description[sizeof(description) - 1] = '\0';
            other.arraySnapshot = nullptr;
            other.arraySize = 0;
        }
        return *this;
    }

    void setSnapshot(const int* arr, size_t size) {
        delete[] arraySnapshot;
        arraySnapshot = nullptr;
        arraySize = size;
        if (arr && size > 0) {
            arraySnapshot = new int[size];
            for (size_t i = 0; i < size; ++i) {
                arraySnapshot[i] = arr[i];
            }
        }
    }
};

/**
 * @brief Custom dynamic array of SortSteps with strict manual memory management.
 * No STL container used.
 */
class StepRecorder {
private:
    SortStep* steps;
    size_t capacity;
    size_t count;

    void ensureCapacity(size_t minCap) {
        if (minCap <= capacity) return;
        size_t newCap = capacity == 0 ? 16 : capacity * 2;
        if (newCap < minCap) newCap = minCap;

        SortStep* newSteps = new SortStep[newCap];
        for (size_t i = 0; i < count; ++i) {
            newSteps[i] = std::move(steps[i]);
        }
        delete[] steps;
        steps = newSteps;
        capacity = newCap;
    }

public:
    StepRecorder() : steps(nullptr), capacity(0), count(0) {}

    ~StepRecorder() {
        delete[] steps;
    }

    StepRecorder(const StepRecorder&) = delete;
    StepRecorder& operator=(const StepRecorder&) = delete;

    StepRecorder(StepRecorder&& other) noexcept
        : steps(other.steps), capacity(other.capacity), count(other.count) {
        other.steps = nullptr;
        other.capacity = 0;
        other.count = 0;
    }

    StepRecorder& operator=(StepRecorder&& other) noexcept {
        if (this != &other) {
            delete[] steps;
            steps = other.steps;
            capacity = other.capacity;
            count = other.count;
            other.steps = nullptr;
            other.capacity = 0;
            other.count = 0;
        }
        return *this;
    }

    void addStep(StepType type, int indexA, int indexB, int auxValue,
                 const char* desc, const int* currentArray, size_t size) {
        ensureCapacity(count + 1);
        SortStep& step = steps[count++];
        step.type = type;
        step.indexA = indexA;
        step.indexB = indexB;
        step.auxValue = auxValue;
        std::strncpy(step.description, desc, sizeof(step.description) - 1);
        step.description[sizeof(step.description) - 1] = '\0';
        step.setSnapshot(currentArray, size);
    }

    size_t getCount() const { return count; }

    const SortStep& getStep(size_t index) const {
        return steps[index];
    }

    void clear() {
        delete[] steps;
        steps = nullptr;
        capacity = 0;
        count = 0;
    }
};

} // namespace algorithms

#endif // ALGORITHMS_SORT_STEP_HPP

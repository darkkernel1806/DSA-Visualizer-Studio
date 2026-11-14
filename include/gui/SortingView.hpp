#ifndef GUI_SORTING_VIEW_HPP
#define GUI_SORTING_VIEW_HPP

#include "gui/View.hpp"
#include "gui/UICommon.hpp"
#include "gui/UIComponents.hpp"
#include "algorithms/SortingAlgorithms.hpp"
#include "algorithms/SortStep.hpp"

namespace gui {

class SortingView : public View {
private:
    algorithms::SortAlgorithmType currentAlgorithm;
    algorithms::StepRecorder recorder;
    algorithms::SortStats stats;

    static constexpr size_t MAX_ELEMENTS = 30;
    int array[MAX_ELEMENTS];
    int originalArray[MAX_ELEMENTS];
    size_t arraySize;

    // Playback state
    size_t currentStepIdx;
    bool isPlaying;
    float stepTimer;
    Slider speedSlider;

    // Controls
    Button bubbleBtn;
    Button selectBtn;
    Button insertBtn;

    Button playPauseBtn;
    Button stepBtn;
    Button resetBtn;
    Button shuffleBtn;
    Button size10Btn;
    Button size15Btn;
    Button size20Btn;

    StatusConsole console;
    ComplexityCard complexity;

    void generateRandomArray(size_t size);
    void recomputeSteps();

public:
    SortingView();

    void update(float dt) override;
    void draw() override;
    const char* getTitle() const override { return "Sorting Algorithms Visualizer"; }
    const char* getSubtitle() const override { return "Step-by-step comparative animation of Bubble, Selection, and Insertion Sort"; }
    void reset() override;
};

} // namespace gui

#endif // GUI_SORTING_VIEW_HPP

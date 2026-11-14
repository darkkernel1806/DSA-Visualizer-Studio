#include "gui/SortingView.hpp"
#include <cstdio>
#include <cstdlib>

namespace gui {

SortingView::SortingView()
    : currentAlgorithm(algorithms::SortAlgorithmType::BUBBLE_SORT),
      arraySize(12),
      currentStepIdx(0),
      isPlaying(false),
      stepTimer(0.0f),
      speedSlider({0, 0, 130, 20}, 0.2f, 3.0f, 1.0f, "Speed"),
      bubbleBtn({0, 0, 96, 32}, "Bubble Sort", COLOR_ACCENT_PRIMARY),
      selectBtn({0, 0, 102, 32}, "Selection Sort", COLOR_PANEL_LIGHT),
      insertBtn({0, 0, 102, 32}, "Insertion Sort", COLOR_PANEL_LIGHT),
      playPauseBtn({0, 0, 80, 32}, "Play", COLOR_ACCENT_SUCCESS),
      stepBtn({0, 0, 78, 32}, "Step >", COLOR_ACCENT_INFO),
      resetBtn({0, 0, 74, 32}, "Reset", COLOR_PANEL_LIGHT),
      shuffleBtn({0, 0, 84, 32}, "Shuffle", COLOR_ACCENT_WARNING),
      size10Btn({0, 0, 60, 32}, "10", COLOR_PANEL_LIGHT),
      size15Btn({0, 0, 60, 32}, "15", COLOR_ACCENT_PRIMARY),
      size20Btn({0, 0, 60, 32}, "20", COLOR_PANEL_LIGHT) {
    stats = algorithms::SortingAlgorithms::getStatsFor(currentAlgorithm);
    complexity.set(stats.bestCase, stats.avgCase, stats.worstCase, stats.spaceComplexity,
                   "In-place comparison sort. Bubble & Insertion are stable; Selection is unstable.");

    generateRandomArray(15);
}

void SortingView::generateRandomArray(size_t size) {
    if (size > MAX_ELEMENTS) size = MAX_ELEMENTS;
    if (size < 5) size = 5;
    arraySize = size;

    for (size_t i = 0; i < arraySize; ++i) {
        array[i] = GetRandomValue(15, 95);
        originalArray[i] = array[i];
    }

    recomputeSteps();
    console.log("Generated new random array.", StatusType::INFO);
}

void SortingView::recomputeSteps() {
    isPlaying = false;
    currentStepIdx = 0;
    stepTimer = 0.0f;
    playPauseBtn.setLabel("Play");
    playPauseBtn.setColor(COLOR_ACCENT_SUCCESS);

    // Restore working copy of array
    for (size_t i = 0; i < arraySize; ++i) {
        array[i] = originalArray[i];
    }

    switch (currentAlgorithm) {
        case algorithms::SortAlgorithmType::BUBBLE_SORT:
            algorithms::SortingAlgorithms::recordBubbleSort(array, arraySize, recorder, stats);
            break;
        case algorithms::SortAlgorithmType::SELECTION_SORT:
            algorithms::SortingAlgorithms::recordSelectionSort(array, arraySize, recorder, stats);
            break;
        case algorithms::SortAlgorithmType::INSERTION_SORT:
            algorithms::SortingAlgorithms::recordInsertionSort(array, arraySize, recorder, stats);
            break;
    }

    complexity.set(stats.bestCase, stats.avgCase, stats.worstCase, stats.spaceComplexity,
                   "Theoretical complexity benchmarks updated for selected algorithm.");
}

void SortingView::reset() {
    isPlaying = false;
    currentStepIdx = 0;
    stepTimer = 0.0f;
    playPauseBtn.setLabel("Play");
    playPauseBtn.setColor(COLOR_ACCENT_SUCCESS);
    for (size_t i = 0; i < arraySize; ++i) {
        array[i] = originalArray[i];
    }
    console.log("Animation reset to initial unsorted state.", StatusType::INFO);
}

void SortingView::update(float dt) {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 82.0f;

    // Controls Layout
    bubbleBtn.setBounds({ startX, startY, 96, 32 });
    selectBtn.setBounds({ startX + 102.0f, startY, 104, 32 });
    insertBtn.setBounds({ startX + 212.0f, startY, 104, 32 });

    playPauseBtn.setBounds({ startX + 326.0f, startY, 80, 32 });
    stepBtn.setBounds({ startX + 412.0f, startY, 78, 32 });
    resetBtn.setBounds({ startX + 496.0f, startY, 74, 32 });
    shuffleBtn.setBounds({ startX + 576.0f, startY, 84, 32 });

    speedSlider.update();
    bubbleBtn.update();
    selectBtn.update();
    insertBtn.update();
    playPauseBtn.update();
    stepBtn.update();
    resetBtn.update();
    shuffleBtn.update();

    // Size buttons
    size10Btn.setBounds({ startX + 668.0f, startY, 44, 32 });
    size15Btn.setBounds({ startX + 716.0f, startY, 44, 32 });
    size20Btn.setBounds({ startX + 764.0f, startY, 44, 32 });

    size10Btn.update();
    size15Btn.update();
    size20Btn.update();

    // Algorithm Tab Switching
    if (bubbleBtn.isClicked() && currentAlgorithm != algorithms::SortAlgorithmType::BUBBLE_SORT) {
        currentAlgorithm = algorithms::SortAlgorithmType::BUBBLE_SORT;
        bubbleBtn.setColor(COLOR_ACCENT_PRIMARY);
        selectBtn.setColor(COLOR_PANEL_LIGHT);
        insertBtn.setColor(COLOR_PANEL_LIGHT);
        recomputeSteps();
        console.log("Switched to Bubble Sort algorithm.", StatusType::INFO);
    }
    if (selectBtn.isClicked() && currentAlgorithm != algorithms::SortAlgorithmType::SELECTION_SORT) {
        currentAlgorithm = algorithms::SortAlgorithmType::SELECTION_SORT;
        bubbleBtn.setColor(COLOR_PANEL_LIGHT);
        selectBtn.setColor(COLOR_ACCENT_PRIMARY);
        insertBtn.setColor(COLOR_PANEL_LIGHT);
        recomputeSteps();
        console.log("Switched to Selection Sort algorithm.", StatusType::INFO);
    }
    if (insertBtn.isClicked() && currentAlgorithm != algorithms::SortAlgorithmType::INSERTION_SORT) {
        currentAlgorithm = algorithms::SortAlgorithmType::INSERTION_SORT;
        bubbleBtn.setColor(COLOR_PANEL_LIGHT);
        selectBtn.setColor(COLOR_PANEL_LIGHT);
        insertBtn.setColor(COLOR_ACCENT_PRIMARY);
        recomputeSteps();
        console.log("Switched to Insertion Sort algorithm.", StatusType::INFO);
    }

    // Size Changing
    if (size10Btn.isClicked() && arraySize != 10) {
        size10Btn.setColor(COLOR_ACCENT_PRIMARY);
        size15Btn.setColor(COLOR_PANEL_LIGHT);
        size20Btn.setColor(COLOR_PANEL_LIGHT);
        generateRandomArray(10);
    }
    if (size15Btn.isClicked() && arraySize != 15) {
        size10Btn.setColor(COLOR_PANEL_LIGHT);
        size15Btn.setColor(COLOR_ACCENT_PRIMARY);
        size20Btn.setColor(COLOR_PANEL_LIGHT);
        generateRandomArray(15);
    }
    if (size20Btn.isClicked() && arraySize != 20) {
        size10Btn.setColor(COLOR_PANEL_LIGHT);
        size15Btn.setColor(COLOR_PANEL_LIGHT);
        size20Btn.setColor(COLOR_ACCENT_PRIMARY);
        generateRandomArray(20);
    }

    // Play/Pause Action
    if (playPauseBtn.isClicked()) {
        if (currentStepIdx >= recorder.getCount() - 1) {
            // Already finished, restart
            currentStepIdx = 0;
        }
        isPlaying = !isPlaying;
        if (isPlaying) {
            playPauseBtn.setLabel("Pause");
            playPauseBtn.setColor(COLOR_ACCENT_DANGER);
            console.log("Animation playing.", StatusType::INFO);
        } else {
            playPauseBtn.setLabel("Play");
            playPauseBtn.setColor(COLOR_ACCENT_SUCCESS);
            console.log("Animation paused.", StatusType::INFO);
        }
    }

    // Single Step Forward Action
    if (stepBtn.isClicked()) {
        isPlaying = false;
        playPauseBtn.setLabel("Play");
        playPauseBtn.setColor(COLOR_ACCENT_SUCCESS);
        if (currentStepIdx < recorder.getCount() - 1) {
            currentStepIdx++;
        }
    }

    // Reset Action
    if (resetBtn.isClicked()) {
        reset();
    }

    // Shuffle Action
    if (shuffleBtn.isClicked()) {
        generateRandomArray(arraySize);
    }

    // Auto Playback Tick
    if (isPlaying) {
        float speed = speedSlider.getValue();
        float baseInterval = 0.28f / speed;
        stepTimer += dt;
        if (stepTimer >= baseInterval) {
            stepTimer = 0.0f;
            if (currentStepIdx < recorder.getCount() - 1) {
                currentStepIdx++;
            } else {
                isPlaying = false;
                playPauseBtn.setLabel("Play");
                playPauseBtn.setColor(COLOR_ACCENT_SUCCESS);
                char msg[96];
                std::snprintf(msg, sizeof(msg), "%s complete! Comparisons: %zu, Swaps: %zu.",
                              stats.name, stats.comparisons, stats.swaps);
                console.log(msg, StatusType::SUCCESS);
            }
        }
    }
}

void SortingView::draw() {
    float startX = SIDEBAR_WIDTH + 24.0f;
    float startY = 82.0f;
    float availW = WINDOW_WIDTH - SIDEBAR_WIDTH - 48.0f;

    // Controls container card
    Rectangle controlsCard = { startX, startY - 6.0f, availW, 46.0f };
    DrawCard(controlsCard, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    bubbleBtn.draw();
    selectBtn.draw();
    insertBtn.draw();
    playPauseBtn.draw();
    stepBtn.draw();
    resetBtn.draw();
    shuffleBtn.draw();
    size10Btn.draw();
    size15Btn.draw();
    size20Btn.draw();

    // Position Speed Slider
    speedSlider = Slider({ controlsCard.x + controlsCard.width - 160.0f, controlsCard.y + 20.0f, 140.0f, 16.0f },
                         0.2f, 3.0f, speedSlider.getValue(), "Speed");
    speedSlider.draw();

    // Canvas Card
    float canvasY = startY + 48.0f;
    float canvasH = 436.0f;
    Rectangle canvas = { startX, canvasY, availW, canvasH };
    DrawCard(canvas, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    // Canvas Header info
    DrawText("SORTING BAR VISUALIZATION", static_cast<int>(canvas.x + 20.0f), static_cast<int>(canvas.y + 14.0f), 12, COLOR_TEXT_DIM);

    // Current Step Reference
    const algorithms::SortStep* currentStep = nullptr;
    if (recorder.getCount() > 0 && currentStepIdx < recorder.getCount()) {
        currentStep = &recorder.getStep(currentStepIdx);
    }

    // Step Description Banner
    Rectangle descRect = { canvas.x + 20.0f, canvas.y + 36.0f, canvas.width - 40.0f, 38.0f };
    DrawCard(descRect, ColorAlpha(COLOR_PANEL_LIGHT, 0.7f), COLOR_PANEL_LIGHT, 0.15f);

    if (currentStep) {
        DrawCircle(static_cast<int>(descRect.x + 16.0f), static_cast<int>(descRect.y + 19.0f), 4.5f, COLOR_ACCENT_INFO);
        DrawText(currentStep->description, static_cast<int>(descRect.x + 28.0f), static_cast<int>(descRect.y + 12.0f), 13, COLOR_TEXT_PRIMARY);

        // Step index badge on right
        char stepNumStr[48];
        std::snprintf(stepNumStr, sizeof(stepNumStr), "Step %zu / %zu", currentStepIdx + 1, recorder.getCount());
        DrawText(stepNumStr, static_cast<int>(descRect.x + descRect.width - 120.0f), static_cast<int>(descRect.y + 12.0f), 12, COLOR_TEXT_DIM);
    }

    // Draw Bars
    const int* renderArr = originalArray;
    if (currentStep && currentStep->arraySnapshot) {
        renderArr = currentStep->arraySnapshot;
    }

    float barAreaX = canvas.x + 40.0f;
    float barAreaW = canvas.width - 80.0f;
    float barAreaY = canvas.y + 90.0f;
    float barAreaH = 260.0f;

    float barGap = 6.0f;
    float barW = (barAreaW - (arraySize - 1) * barGap) / arraySize;
    if (barW > 54.0f) barW = 54.0f;
    float totalBarsW = arraySize * barW + (arraySize - 1) * barGap;
    float startBarX = barAreaX + (barAreaW - totalBarsW) * 0.5f;

    // Baseline guide
    float baseY = barAreaY + barAreaH;
    DrawLineEx({ startBarX - 10.0f, baseY }, { startBarX + totalBarsW + 10.0f, baseY }, 2.0f, COLOR_PANEL_LIGHT);

    for (size_t i = 0; i < arraySize; ++i) {
        float valRatio = static_cast<float>(renderArr[i]) / 100.0f;
        if (valRatio < 0.08f) valRatio = 0.08f;
        if (valRatio > 1.0f) valRatio = 1.0f;

        float h = barAreaH * valRatio;
        float bx = startBarX + i * (barW + barGap);
        float by = baseY - h;

        Rectangle barRect = { bx, by, barW, h };

        // Determine bar color by step state
        Color barColor = COLOR_PANEL_LIGHT;
        Color barBorder = COLOR_ACCENT_PRIMARY;

        if (currentStep) {
            if (currentStep->type == algorithms::StepType::COMPARE &&
                (currentStep->indexA == static_cast<int>(i) || currentStep->indexB == static_cast<int>(i))) {
                barColor = ColorAlpha(COLOR_ACCENT_WARNING, 0.7f);
                barBorder = COLOR_ACCENT_WARNING;
            } else if ((currentStep->type == algorithms::StepType::SWAP || currentStep->type == algorithms::StepType::SHIFT) &&
                       (currentStep->indexA == static_cast<int>(i) || currentStep->indexB == static_cast<int>(i))) {
                barColor = ColorAlpha(COLOR_ACCENT_DANGER, 0.7f);
                barBorder = COLOR_ACCENT_DANGER;
            } else if (currentStep->type == algorithms::StepType::MARK_SORTED && currentStep->indexA == static_cast<int>(i)) {
                barColor = ColorAlpha(COLOR_ACCENT_SUCCESS, 0.7f);
                barBorder = COLOR_ACCENT_SUCCESS;
            } else if (currentStep->type == algorithms::StepType::FINISHED) {
                barColor = ColorAlpha(COLOR_ACCENT_SUCCESS, 0.65f);
                barBorder = COLOR_ACCENT_SUCCESS;
            }
        }

        DrawRectangleRounded(barRect, 0.15f, 4, barColor);
        DrawRectangleRoundedLines(barRect, 0.15f, 4, 1.2f, barBorder);

        // Value text centered on/above bar
        char valStr[16];
        std::snprintf(valStr, sizeof(valStr), "%d", renderArr[i]);
        int vw = MeasureText(valStr, 12);
        int textY = static_cast<int>(by - 16.0f);
        if (textY < barAreaY - 8.0f) textY = static_cast<int>(by + 6.0f);
        DrawText(valStr, static_cast<int>(bx + (barW - vw) * 0.5f), textY, 12, COLOR_TEXT_PRIMARY);

        // Index below baseline
        char idxStr[16];
        std::snprintf(idxStr, sizeof(idxStr), "%zu", i);
        int iw = MeasureText(idxStr, 10);
        DrawText(idxStr, static_cast<int>(bx + (barW - iw) * 0.5f), static_cast<int>(baseY + 6.0f), 10, COLOR_TEXT_DIM);
    }

    // Legend at bottom of canvas
    float legY = canvas.y + canvas.height - 30.0f;
    auto drawLegend = [&](const char* text, Color c, float x) {
        DrawCircle(static_cast<int>(x), static_cast<int>(legY + 6.0f), 4.5f, c);
        DrawText(text, static_cast<int>(x + 10.0f), static_cast<int>(legY), 11, COLOR_TEXT_MUTED);
    };
    drawLegend("Default", COLOR_PANEL_LIGHT, canvas.x + 30.0f);
    drawLegend("Comparing", COLOR_ACCENT_WARNING, canvas.x + 130.0f);
    drawLegend("Swapping / Shifting", COLOR_ACCENT_DANGER, canvas.x + 250.0f);
    drawLegend("Sorted Position", COLOR_ACCENT_SUCCESS, canvas.x + 420.0f);

    // Complexity and Status Console side-by-side at bottom
    float bottomY = canvasY + canvasH + 12.0f;
    float halfW = (availW - 16.0f) * 0.5f;

    complexity.draw({ startX, bottomY, halfW, 82.0f });
    console.draw({ startX + halfW + 16.0f, bottomY, halfW, 82.0f });
}

} // namespace gui

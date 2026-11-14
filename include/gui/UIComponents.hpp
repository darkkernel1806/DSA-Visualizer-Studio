#ifndef GUI_UI_COMPONENTS_HPP
#define GUI_UI_COMPONENTS_HPP

#include "raylib.h"
#include "gui/UICommon.hpp"
#include <cstddef>

namespace gui {

// Button Component
class Button {
private:
    Rectangle bounds;
    char label[48];
    Color baseColor;
    Color hoverColor;
    Color textColor;
    bool hovered;
    bool pressed;
    bool clickedThisFrame;
    bool disabled;

public:
    Button();
    Button(Rectangle bounds, const char* text, Color color = COLOR_ACCENT_PRIMARY);

    void setBounds(Rectangle newBounds);
    void setLabel(const char* text);
    void setColor(Color color);
    void setDisabled(bool isDisabled);
    bool isDisabled() const { return disabled; }

    void update();
    void draw() const;

    bool isClicked() const { return clickedThisFrame; }
    Rectangle getBounds() const { return bounds; }
};

// Text Input Field Component
class TextBox {
private:
    Rectangle bounds;
    char buffer[32];
    char placeholder[32];
    size_t length;
    size_t maxLength;
    bool focused;
    bool numbersOnly;
    float blinkTimer;

public:
    TextBox();
    TextBox(Rectangle bounds, const char* placeholderText, bool numbersOnly = true, size_t maxLen = 8);

    void setBounds(Rectangle newBounds);
    void update();
    void draw() const;

    bool getIntValue(int& outVal) const;
    const char* getText() const { return buffer; }
    void setText(const char* text);
    void clear();
    bool isFocused() const { return focused; }
    void setFocused(bool f) { focused = f; }
};

// Interactive Slider Component
class Slider {
private:
    Rectangle bounds;
    float minVal;
    float maxVal;
    float currentVal;
    char label[32];
    bool isDragging;

public:
    Slider();
    Slider(Rectangle bounds, float minV, float maxV, float initialV, const char* labelText);

    void update();
    void draw() const;

    float getValue() const { return currentVal; }
    void setValue(float v);
};

// Status message levels
enum class StatusType {
    INFO,
    SUCCESS,
    WARNING,
    DANGER
};

// Status Log Entry
struct StatusMessage {
    char text[160];
    StatusType type;
    float timestamp;
};

// Status Console & History Panel
class StatusConsole {
private:
    static constexpr size_t MAX_LOGS = 6;
    StatusMessage history[MAX_LOGS];
    size_t count;

public:
    StatusConsole();

    void log(const char* message, StatusType type = StatusType::INFO);
    void clear();
    void draw(Rectangle bounds) const;

    const char* getLatestMessage() const;
    StatusType getLatestType() const;
};

// Time & Space Complexity Display Card
class ComplexityCard {
private:
    const char* bestCase;
    const char* avgCase;
    const char* worstCase;
    const char* spaceComp;
    const char* notes;

public:
    ComplexityCard();
    ComplexityCard(const char* best, const char* avg, const char* worst,
                   const char* space, const char* noteText = "");

    void set(const char* best, const char* avg, const char* worst,
             const char* space, const char* noteText = "");
    void draw(Rectangle bounds) const;
};

} // namespace gui

#endif // GUI_UI_COMPONENTS_HPP

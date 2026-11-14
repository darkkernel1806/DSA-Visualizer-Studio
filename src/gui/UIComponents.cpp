#include "gui/UIComponents.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>

namespace gui {

// ==========================================
// Button Implementation
// ==========================================
Button::Button()
    : bounds{0, 0, 100, 36}, baseColor(COLOR_ACCENT_PRIMARY),
      hoverColor(COLOR_ACCENT_HOVER), textColor(COLOR_TEXT_PRIMARY),
      hovered(false), pressed(false), clickedThisFrame(false), disabled(false) {
    label[0] = '\0';
}

Button::Button(Rectangle b, const char* text, Color color)
    : bounds(b), baseColor(color), textColor(COLOR_TEXT_PRIMARY),
      hovered(false), pressed(false), clickedThisFrame(false), disabled(false) {
    hoverColor = ColorAlpha(color, 0.85f);
    std::strncpy(label, text, sizeof(label) - 1);
    label[sizeof(label) - 1] = '\0';
}

void Button::setBounds(Rectangle newBounds) {
    bounds = newBounds;
}

void Button::setLabel(const char* text) {
    std::strncpy(label, text, sizeof(label) - 1);
    label[sizeof(label) - 1] = '\0';
}

void Button::setColor(Color color) {
    baseColor = color;
    hoverColor = ColorAlpha(color, 0.85f);
}

void Button::setDisabled(bool isDisabled) {
    disabled = isDisabled;
}

void Button::update() {
    clickedThisFrame = false;
    if (disabled) {
        hovered = false;
        pressed = false;
        return;
    }

    Vector2 mouse = GetMousePosition();
    hovered = CheckCollisionPointRec(mouse, bounds);

    if (hovered) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            pressed = true;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && pressed) {
            clickedThisFrame = true;
            pressed = false;
        }
    } else {
        pressed = false;
    }
}

void Button::draw() const {
    Color bg = baseColor;
    if (disabled) {
        bg = COLOR_PANEL_LIGHT;
    } else if (pressed) {
        bg = ColorAlpha(baseColor, 0.7f);
    } else if (hovered) {
        bg = hoverColor;
    }

    Rectangle drawBounds = bounds;
    if (pressed) {
        drawBounds.y += 1.0f; // Tactile press effect
    }

    DrawRectangleRounded(drawBounds, 0.22f, 6, bg);
    DrawRectangleRoundedLines(drawBounds, 0.22f, 6, 1.2f,
                              disabled ? COLOR_TEXT_DIM : (hovered ? COLOR_TEXT_PRIMARY : ColorAlpha(baseColor, 0.9f)));

    int fontSize = 14;
    int textW = MeasureText(label, fontSize);
    int textX = static_cast<int>(drawBounds.x + (drawBounds.width - textW) * 0.5f);
    int textY = static_cast<int>(drawBounds.y + (drawBounds.height - fontSize) * 0.5f);

    Color textC = disabled ? COLOR_TEXT_DIM : textColor;
    DrawText(label, textX, textY, fontSize, textC);
}

// ==========================================
// TextBox Implementation
// ==========================================
TextBox::TextBox()
    : bounds{0, 0, 120, 36}, length(0), maxLength(8),
      focused(false), numbersOnly(true), blinkTimer(0.0f) {
    buffer[0] = '\0';
    placeholder[0] = '\0';
}

TextBox::TextBox(Rectangle b, const char* placeholderText, bool numOnly, size_t maxLen)
    : bounds(b), length(0), maxLength(maxLen > 31 ? 31 : maxLen),
      focused(false), numbersOnly(numOnly), blinkTimer(0.0f) {
    buffer[0] = '\0';
    std::strncpy(placeholder, placeholderText, sizeof(placeholder) - 1);
    placeholder[sizeof(placeholder) - 1] = '\0';
}

void TextBox::setBounds(Rectangle newBounds) {
    bounds = newBounds;
}

void TextBox::update() {
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        focused = CheckCollisionPointRec(mouse, bounds);
    }

    if (focused) {
        blinkTimer += GetFrameTime();
        if (blinkTimer >= 1.0f) blinkTimer -= 1.0f;

        // Character typing
        int key = GetCharPressed();
        while (key > 0) {
            bool valid = false;
            if (numbersOnly) {
                if (key >= '0' && key <= '9') valid = true;
                if (key == '-' && length == 0) valid = true;
            } else {
                if (key >= 32 && key <= 126) valid = true;
            }

            if (valid && length < maxLength) {
                buffer[length++] = static_cast<char>(key);
                buffer[length] = '\0';
            }
            key = GetCharPressed();
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
            if (length > 0) {
                length--;
                buffer[length] = '\0';
            }
        }

        // Enter key to finish editing
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
            focused = false;
        }
    } else {
        blinkTimer = 0.0f;
    }
}

void TextBox::draw() const {
    Color bg = focused ? ColorAlpha(COLOR_PANEL_DARK, 0.95f) : COLOR_PANEL_DARK;
    Color border = focused ? COLOR_ACCENT_PRIMARY : COLOR_PANEL_LIGHT;

    DrawRectangleRounded(bounds, 0.2f, 6, bg);
    DrawRectangleRoundedLines(bounds, 0.2f, 6, focused ? 2.0f : 1.2f, border);

    int fontSize = 14;
    int textY = static_cast<int>(bounds.y + (bounds.height - fontSize) * 0.5f);
    int textX = static_cast<int>(bounds.x + 10.0f);

    if (length == 0) {
        DrawText(placeholder, textX, textY, fontSize, COLOR_TEXT_DIM);
    } else {
        DrawText(buffer, textX, textY, fontSize, COLOR_TEXT_PRIMARY);
    }

    // Cursor
    if (focused && blinkTimer < 0.5f) {
        int cursorOffset = MeasureText(buffer, fontSize);
        DrawRectangle(textX + cursorOffset + 2, textY - 1, 2, fontSize + 2, COLOR_ACCENT_PRIMARY);
    }
}

bool TextBox::getIntValue(int& outVal) const {
    if (length == 0) return false;
    if (length == 1 && buffer[0] == '-') return false;
    outVal = std::atoi(buffer);
    return true;
}

void TextBox::setText(const char* text) {
    if (!text) return;
    std::strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    length = std::strlen(buffer);
}

void TextBox::clear() {
    buffer[0] = '\0';
    length = 0;
}

// ==========================================
// Slider Implementation
// ==========================================
Slider::Slider()
    : bounds{0, 0, 160, 24}, minVal(0.1f), maxVal(2.0f),
      currentVal(1.0f), isDragging(false) {
    label[0] = '\0';
}

Slider::Slider(Rectangle b, float minV, float maxV, float initialV, const char* labelText)
    : bounds(b), minVal(minV), maxVal(maxV), currentVal(initialV), isDragging(false) {
    std::strncpy(label, labelText, sizeof(label) - 1);
    label[sizeof(label) - 1] = '\0';
}

void Slider::setValue(float v) {
    if (v < minVal) v = minVal;
    if (v > maxVal) v = maxVal;
    currentVal = v;
}

void Slider::update() {
    Vector2 mouse = GetMousePosition();
    Rectangle hitArea = { bounds.x, bounds.y - 4.0f, bounds.width, bounds.height + 8.0f };

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, hitArea)) {
        isDragging = true;
    }

    if (isDragging) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float ratio = (mouse.x - bounds.x) / bounds.width;
            if (ratio < 0.0f) ratio = 0.0f;
            if (ratio > 1.0f) ratio = 1.0f;
            currentVal = minVal + ratio * (maxVal - minVal);
        } else {
            isDragging = false;
        }
    }
}

void Slider::draw() const {
    // Label & value text above slider
    char valBuf[48];
    std::snprintf(valBuf, sizeof(valBuf), "%s: %.1fx", label, currentVal);
    DrawText(valBuf, static_cast<int>(bounds.x), static_cast<int>(bounds.y - 18.0f), 12, COLOR_TEXT_MUTED);

    // Track background
    float trackHeight = 6.0f;
    Rectangle track = { bounds.x, bounds.y + (bounds.height - trackHeight) * 0.5f, bounds.width, trackHeight };
    DrawRectangleRounded(track, 0.5f, 4, COLOR_PANEL_LIGHT);

    // Active fill
    float ratio = (currentVal - minVal) / (maxVal - minVal);
    Rectangle fill = { bounds.x, track.y, bounds.width * ratio, trackHeight };
    DrawRectangleRounded(fill, 0.5f, 4, COLOR_ACCENT_PRIMARY);

    // Thumb Handle
    float handleX = bounds.x + bounds.width * ratio;
    float handleY = bounds.y + bounds.height * 0.5f;
    DrawCircle(static_cast<int>(handleX), static_cast<int>(handleY), 7.0f, COLOR_TEXT_PRIMARY);
    DrawCircle(static_cast<int>(handleX), static_cast<int>(handleY), 4.0f, COLOR_ACCENT_PRIMARY);
}

// ==========================================
// StatusConsole Implementation
// ==========================================
StatusConsole::StatusConsole() : count(0) {
    log("System initialized. Ready for operations.", StatusType::INFO);
}

void StatusConsole::log(const char* message, StatusType type) {
    if (!message) return;

    if (count < MAX_LOGS) {
        // Shift existing down
        for (size_t i = count; i > 0; --i) {
            history[i] = history[i - 1];
        }
        count++;
    } else {
        for (size_t i = MAX_LOGS - 1; i > 0; --i) {
            history[i] = history[i - 1];
        }
    }

    std::strncpy(history[0].text, message, sizeof(history[0].text) - 1);
    history[0].text[sizeof(history[0].text) - 1] = '\0';
    history[0].type = type;
    history[0].timestamp = static_cast<float>(GetTime());
}

void StatusConsole::clear() {
    count = 0;
    log("Console cleared.", StatusType::INFO);
}

const char* StatusConsole::getLatestMessage() const {
    if (count == 0) return "Ready";
    return history[0].text;
}

StatusType StatusConsole::getLatestType() const {
    if (count == 0) return StatusType::INFO;
    return history[0].type;
}

void StatusConsole::draw(Rectangle bounds) const {
    DrawCard(bounds, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.06f);

    // Header label
    DrawText("OPERATION STATUS & ACTIVITY LOG",
             static_cast<int>(bounds.x + 14.0f), static_cast<int>(bounds.y + 10.0f), 11, COLOR_TEXT_DIM);

    // Latest message prominence
    if (count > 0) {
        Color badgeColor = COLOR_ACCENT_INFO;
        const char* prefix = "[INFO]";
        switch (history[0].type) {
            case StatusType::SUCCESS:
                badgeColor = COLOR_ACCENT_SUCCESS;
                prefix = "[SUCCESS]";
                break;
            case StatusType::WARNING:
                badgeColor = COLOR_ACCENT_WARNING;
                prefix = "[WARNING]";
                break;
            case StatusType::DANGER:
                badgeColor = COLOR_ACCENT_DANGER;
                prefix = "[ERROR]";
                break;
            case StatusType::INFO:
            default:
                badgeColor = COLOR_ACCENT_INFO;
                prefix = "[INFO]";
                break;
        }

        // Indicator dot
        DrawCircle(static_cast<int>(bounds.x + 20.0f), static_cast<int>(bounds.y + 34.0f), 4.5f, badgeColor);

        // Prefix tag
        DrawText(prefix, static_cast<int>(bounds.x + 30.0f), static_cast<int>(bounds.y + 27.0f), 13, badgeColor);

        // Text
        int prefixW = MeasureText(prefix, 13);
        DrawText(history[0].text, static_cast<int>(bounds.x + 38.0f + prefixW), static_cast<int>(bounds.y + 27.0f), 13, COLOR_TEXT_PRIMARY);
    }

    // Previous logs list
    float startY = bounds.y + 48.0f;
    for (size_t i = 1; i < count && i < 3; ++i) {
        Color c = COLOR_TEXT_MUTED;
        DrawText("-", static_cast<int>(bounds.x + 20.0f), static_cast<int>(startY + (i - 1) * 16.0f), 11, COLOR_TEXT_DIM);
        DrawText(history[i].text, static_cast<int>(bounds.x + 30.0f), static_cast<int>(startY + (i - 1) * 16.0f), 11, c);
    }
}

// ==========================================
// ComplexityCard Implementation
// ==========================================
ComplexityCard::ComplexityCard()
    : bestCase("O(1)"), avgCase("O(1)"), worstCase("O(1)"), spaceComp("O(n)"), notes("") {}

ComplexityCard::ComplexityCard(const char* best, const char* avg, const char* worst,
                               const char* space, const char* noteText)
    : bestCase(best), avgCase(avg), worstCase(worst), spaceComp(space), notes(noteText) {}

void ComplexityCard::set(const char* best, const char* avg, const char* worst,
                         const char* space, const char* noteText) {
    bestCase = best;
    avgCase = avg;
    worstCase = worst;
    spaceComp = space;
    notes = noteText;
}

void ComplexityCard::draw(Rectangle bounds) const {
    DrawCard(bounds, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.08f);

    int textX = static_cast<int>(bounds.x + 14.0f);
    DrawText("THEORETICAL COMPLEXITY", textX, static_cast<int>(bounds.y + 10.0f), 11, COLOR_TEXT_DIM);

    // Complexity items grid
    float itemW = (bounds.width - 28.0f) * 0.25f;
    float cardY = bounds.y + 28.0f;

    auto drawMetric = [&](const char* lbl, const char* val, float x, Color accent) {
        DrawText(lbl, static_cast<int>(x), static_cast<int>(cardY), 10, COLOR_TEXT_MUTED);
        DrawText(val, static_cast<int>(x), static_cast<int>(cardY + 14.0f), 13, accent);
    };

    drawMetric("Best Time", bestCase, bounds.x + 14.0f, COLOR_ACCENT_SUCCESS);
    drawMetric("Avg Time", avgCase, bounds.x + 14.0f + itemW, COLOR_ACCENT_INFO);
    drawMetric("Worst Time", worstCase, bounds.x + 14.0f + itemW * 2.0f, COLOR_ACCENT_WARNING);
    drawMetric("Aux Space", spaceComp, bounds.x + 14.0f + itemW * 3.0f, COLOR_ACCENT_PURPLE);

    if (notes && notes[0] != '\0') {
        DrawText(notes, textX, static_cast<int>(bounds.y + bounds.height - 18.0f), 10, COLOR_TEXT_DIM);
    }
}

} // namespace gui

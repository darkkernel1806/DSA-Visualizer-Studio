#include "gui/UICommon.hpp"
#include <cmath>
#include <cstdio>

namespace gui {

void DrawCard(Rectangle bounds, Color bgColor, Color borderColor, float roundness) {
    DrawRectangleRounded(bounds, roundness, 8, bgColor);
    if (borderColor.a > 0) {
        DrawRectangleRoundedLines(bounds, roundness, 8, 1.5f, borderColor);
    }
}

void DrawHeaderBar(const char* title, const char* subtitle, Rectangle bounds) {
    DrawCard(bounds, COLOR_PANEL_DARK, COLOR_PANEL_LIGHT, 0.04f);

    // Left accent vertical indicator strip
    Rectangle strip = { bounds.x + 8.0f, bounds.y + 12.0f, 4.0f, bounds.height - 24.0f };
    DrawRectangleRounded(strip, 0.5f, 4, COLOR_ACCENT_PRIMARY);

    // Title & Subtitle text
    DrawText(title, static_cast<int>(bounds.x + 24.0f), static_cast<int>(bounds.y + 14.0f), 22, COLOR_TEXT_PRIMARY);
    if (subtitle && subtitle[0] != '\0') {
        DrawText(subtitle, static_cast<int>(bounds.x + 24.0f), static_cast<int>(bounds.y + 40.0f), 13, COLOR_TEXT_MUTED);
    }
}

void DrawArrow(Vector2 start, Vector2 end, Color color, float thickness, float headSize) {
    DrawLineEx(start, end, thickness, color);

    float angle = std::atan2(end.y - start.y, end.x - start.x);
    float angle1 = angle + 2.5f; // ~143 degrees
    float angle2 = angle - 2.5f;

    Vector2 p1 = { end.x + headSize * std::cos(angle1), end.y + headSize * std::sin(angle1) };
    Vector2 p2 = { end.x + headSize * std::cos(angle2), end.y + headSize * std::sin(angle2) };

    DrawTriangle(end, p1, p2, color);
}

void DrawCurvedArrow(Vector2 start, Vector2 end, Color color, float thickness) {
    // Draw Bezier spline
    Vector2 control = { (start.x + end.x) * 0.5f, start.y - 40.0f };
    DrawSplineSegmentBezierQuadratic(start, control, end, thickness, color);

    // Arrowhead at end
    float angle = std::atan2(end.y - control.y, end.x - control.x);
    float headSize = 9.0f;
    Vector2 p1 = { end.x + headSize * std::cos(angle + 2.5f), end.y + headSize * std::sin(angle + 2.5f) };
    Vector2 p2 = { end.x + headSize * std::cos(angle - 2.5f), end.y + headSize * std::sin(angle - 2.5f) };
    DrawTriangle(end, p1, p2, color);
}

void DrawBadge(const char* label, const char* value, Vector2 pos, Color badgeColor) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%s: %s", label, value);
    int textWidth = MeasureText(buf, 12);
    Rectangle rect = { pos.x, pos.y, static_cast<float>(textWidth + 16), 24.0f };

    Color bg = ColorAlpha(badgeColor, 0.2f);
    DrawRectangleRounded(rect, 0.3f, 4, bg);
    DrawRectangleRoundedLines(rect, 0.3f, 4, 1.0f, badgeColor);
    DrawText(buf, static_cast<int>(pos.x + 8.0f), static_cast<int>(pos.y + 6.0f), 12, COLOR_TEXT_PRIMARY);
}

} // namespace gui

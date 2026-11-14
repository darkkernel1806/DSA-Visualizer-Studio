#ifndef GUI_UI_COMMON_HPP
#define GUI_UI_COMMON_HPP

#include "raylib.h"
#include <cmath>

namespace gui {

// Application Window Constants
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 768;
constexpr int TARGET_FPS = 60;
constexpr float SIDEBAR_WIDTH = 240.0f;
constexpr float CONTENT_WIDTH = WINDOW_WIDTH - SIDEBAR_WIDTH;

// Professional Dark Palette Colors
inline const Color COLOR_BG_DARK        = Color{ 15,  23,  42, 255 }; // #0f172a Deep Slate Background
inline const Color COLOR_PANEL_DARK     = Color{ 30,  41,  59, 255 }; // #1e293b Card & Panel Background
inline const Color COLOR_PANEL_LIGHT    = Color{ 51,  65,  85, 255 }; // #334155 Panel Border & Separator
inline const Color COLOR_PANEL_HOVER    = Color{ 71,  85, 105, 255 }; // #475569 Subtle Hover

inline const Color COLOR_ACCENT_PRIMARY = Color{ 99, 102, 241, 255 }; // #6366f1 Indigo Accent
inline const Color COLOR_ACCENT_HOVER   = Color{129, 140, 248, 255 }; // #818cf8 Indigo Hover
inline const Color COLOR_ACCENT_SUCCESS = Color{ 16, 185, 129, 255 }; // #10b981 Emerald Green
inline const Color COLOR_ACCENT_DANGER  = Color{244,  63,  94, 255 }; // #f43f5e Rose Red
inline const Color COLOR_ACCENT_WARNING = Color{245, 158,  11, 255 }; // #f59e0b Amber Warning
inline const Color COLOR_ACCENT_INFO    = Color{ 14, 165, 233, 255 }; // #0ea5e9 Sky Blue
inline const Color COLOR_ACCENT_PURPLE  = Color{168,  85, 247, 255 }; // #a855f7 Royal Violet

inline const Color COLOR_TEXT_PRIMARY   = Color{248, 250, 252, 255 }; // #f8fafc Crisp White
inline const Color COLOR_TEXT_MUTED     = Color{148, 163, 184, 255 }; // #94a3b8 Secondary Text
inline const Color COLOR_TEXT_DIM       = Color{100, 116, 139, 255 }; // #64748b Darker Grey

// Common Drawing Utilities
void DrawCard(Rectangle bounds, Color bgColor, Color borderColor = COLOR_PANEL_LIGHT, float roundness = 0.08f);
void DrawHeaderBar(const char* title, const char* subtitle, Rectangle bounds);
void DrawArrow(Vector2 start, Vector2 end, Color color, float thickness = 3.0f, float headSize = 10.0f);
void DrawCurvedArrow(Vector2 start, Vector2 end, Color color, float thickness = 2.5f);
void DrawBadge(const char* label, const char* value, Vector2 pos, Color badgeColor);

// Math / Animation Utility
inline float LerpFloat(float a, float b, float t) {
    return a + (b - a) * t;
}

} // namespace gui

#endif // GUI_UI_COMMON_HPP

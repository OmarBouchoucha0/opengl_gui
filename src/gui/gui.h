#ifndef GUI_H
#define GUI_H

#include "../base/base_types.cpp"
#include "../base/base_math.cpp"
#include "../render/render.cpp"

enum ButtonState {
  BUTTON_IDLE,
  BUTTON_HOVERED,
  BUTTON_PRESSED,
  BUTTON_DOWN,
  BUTTON_RELEASED,
};
global_variable f32 BUTTON_WIDTH = 100;
global_variable f32 BUTTON_HEIGHT = 50;

global_variable Color BG_DEEP = {0x0d, 0x0d, 0x10, 0xff}; // window background
global_variable Color BG_SURFACE = {0x16, 0x16, 0x1b, 0xff}; // panels
global_variable Color BG_RAISED = {0x1f, 0x1f, 0x26, 0xff};  // buttons, inputs
global_variable Color BORDER = {0x2e, 0x2e, 0x38, 0xff}; // outlines, dividers

global_variable Color TEXT_PRIMARY = {0xe8, 0xe8, 0xee, 0xff};
global_variable Color TEXT_SECONDARY = {0x8a, 0x8a, 0x99, 0xff};
global_variable Color TEXT_DISABLED = {0x4a, 0x4a, 0x55, 0xff};

global_variable Color PRIMARY = {0xb4, 0xa7, 0xf5, 0xff}; // main accent
global_variable Color PRIMARY_HOVER = {0xc9, 0xbf, 0xf8,
                                       0xff}; // lighter on hover
global_variable Color PRIMARY_PRESSED = {0x9a, 0x8b, 0xe8,
                                         0xff}; // darker on press
global_variable Color ON_PRIMARY = {0x14, 0x12, 0x24,
                                    0xff}; // text drawn on top of PRIMARY

internal ButtonState guiButton(f32 posx, f32 posy, const char *text,
                               Font *font);
internal ButtonState guiButtonEx(Rectangle button, const char *text, Font *font,
                                 i32 fontSize, f32 rotation, f32 roundness,
                                 f32 borderThickness, Color buttonColor,
                                 Color textColor, Color borderColor);

#endif // GUI_H

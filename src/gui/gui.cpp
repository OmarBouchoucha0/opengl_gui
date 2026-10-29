#include "gui.h"

internal b32 checkCollisionBetweenMouseAndRect(Rectangle rect) {
  V2f mouse_position = getMousePosition();
  if (mouse_position.x > rect.x && mouse_position.x < rect.x + rect.width &&
      mouse_position.y > rect.y && mouse_position.y < rect.y + rect.height) {
    return true;
  }
  return false;
}

// internal ButtonState guiButton(Rectangle button, const char *text, Font
// *font,
//                                Color buttonColor, Color textColor) {
//
//   f32 textWidth = getTextWidth(text, font);
//   ButtonState buttonState = BUTTON_IDLE;
//   if (checkCollisionBetweenMouseAndRect(button)) {
//     if (isMouseButtonPressed(0)) {
//       buttonState = BUTTON_PRESSED;
//     } else {
//       buttonState = BUTTON_HOVERED;
//     }
//   }
//   V2f textPos = {
//       .x = button.x + button.width / 2 - textWidth / 2,
//       .y = button.y + button.height / 2,
//   };
//   drawRectangle(button, buttonColor);
//   drawText(textPos, text, font, textColor);
//   return buttonState;
// }

internal ButtonState guiButton(f32 posx, f32 posy, const char *text,
                               Font *font) {
  f32 textWidth = getTextWidth(text, font);
  ButtonState buttonState = BUTTON_IDLE;
  Rectangle button = {
      .x = posx,
      .y = posy,
      .width = BUTTON_WIDTH,
      .height = BUTTON_HEIGHT,
  };
  V2f textPos = {
      .x = button.x + (button.width - textWidth) / 2,
      .y = button.y + (button.height + font->ascent + font->descent) / 2,
  };

  if (checkCollisionBetweenMouseAndRect(button)) {
    if (isMouseButtonPressed(0)) {
      buttonState = BUTTON_PRESSED;
      drawRectangle(button, PRIMARY_PRESSED);
    } else {
      buttonState = BUTTON_HOVERED;
      drawRectangle(button, PRIMARY_HOVER);
    }
  } else {
    drawRectangle(button, PRIMARY);
  }
  drawRectangleOutline(button, 5.0f, BORDER);
  drawText(textPos, text, font, ON_PRIMARY);
  return buttonState;
}

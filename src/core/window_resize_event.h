#pragma once

#include "event.h"

struct WindowResizeEvent : public Event
{
  WindowResizeEvent(int old_width_, int old_height_, int width_, int height_)
    : width(width_), height(height_), old_width(old_width_), old_height(old_height_) {}
  int width;
  int height;
  int old_width;
  int old_height;
};
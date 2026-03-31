#pragma once 
#include "components/brain/brain.h"

struct Position { float x, y; };
struct Velocity { float dx, dy; };
struct Color { int r, g, b; };
struct Size { float radius; };
struct Energy { float energy; };
struct Dead {};
struct Creature {};
struct Plant {};
struct TimeOf { float birth, last_reproduction; };
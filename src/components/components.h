#pragma once 
#include "components/brain/brain.h"

struct Position { 
    float x, y; 
    Position(float x, float y) : x(x), y(y) {};
};
struct Velocity { float mag, dir, turn_rate; };
struct Color { int r, g, b; };
struct Size { float radius; };
struct CreatureEnergy { float energy; };
struct Dead {};
struct Creature {};
struct Plant {};
struct TimeOf { float birth, last_reproduction; };
struct VisionSensors { float dist_to_food, dir_to_food, dist_to_creature, dir_to_creature; };
#pragma once 
#include "components/brain/brain.h"

struct Position { 
    float x, y; 
    Position(float x, float y) : x(x), y(y) {};
};
struct OldPosition { float x, y; };
struct Velocity { float mag, dir, turn_rate; };
struct Color { int r, g, b; };
struct Size { float radius; };
struct CreatureEnergy { float energy, max; };
struct Dead {};
struct Creature {};
struct Plant {};
struct Carcass {};
struct VisionSensors { float dist_to_food, dir_to_food, dist_to_creature, dir_to_creature, dist_to_carcass, dir_to_carcass; };
struct FoodEnergy { float energy; };
struct Bite { float energy_per_sec; };
struct Stomach { float potential_energy, max; };
struct ChildEnergy { float energy; };
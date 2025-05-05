/*Defines the Fish class, a type of organism that moves randomly and reproduces occasionally.*/

#ifndef FISH_H 
#define FISH_H 

#include "Organism.h" // Include base class
#include "World.h" // Forward declare world (not used directly)
#include "emp/math/math.hpp" // For math operations

/* Class representing a fish organism */
class Fish : public Organism {
public:
/** Constructor: create a fish at a given position */
  Fish(int initial_x_position, int initial_y_position)
    : Organism(initial_x_position, initial_y_position) {}

  /** Draw the fish as a blue square on the canvas */
  void Draw(emp::web::Canvas& canvas, double cell_size) const override {
    canvas.Rect(x_position * cell_size, y_position * cell_size, cell_size, cell_size, "blue", "blue"); 
  }

  /** Identify this organism as a fish */
  bool IsFish() const override { return true; } 
};

#endif 
































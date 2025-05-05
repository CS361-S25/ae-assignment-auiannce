/* Defines the Shark class, a type of organism that hunts fish and loses energy over time.*/

#ifndef SHARK_H // Header guard
#define SHARK_H //

#include "Organism.h" // Include base class
#include "World.h" // Forward declare world
#include "emp/math/math.hpp" // For math operations

/* Class representing a shark organism */
class Shark : public Organism {
public:
/** Constructor: create a shark at a given position */
  Shark(int initial_x, int initial_y) : Organism(initial_x, initial_y) {}

  /** Draw the shark as a green square on the canvas */
  void Draw(emp::web::Canvas& canvas, double cell_size) const override {
    canvas.Rect(x_position * cell_size, y_position * cell_size, cell_size, cell_size, " green", " green"); 
  }

  /** Identify this organism as not a fish */
  bool IsFish() const override { return false; } 
};

#endif 




























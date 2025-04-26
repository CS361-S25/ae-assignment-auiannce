/* Defines the Shark class, a type of organism that hunts fish and loses energy over time.*/

#ifndef SHARK_H
#define SHARK_H

#include "Organism.h"
#include "World.h"
#include "emp/math/math.hpp"

/* Class representing a shark organism */
class Shark : public Organism {
public:
  /* Constructor: create a shark at a given position */
  Shark(int initial_x, int initial_y) : Organism(initial_x, initial_y) {}

  /* Draw the shark as a red square on the canvas */
  void Draw(emp::web::Canvas& canvas, double cell_size) const override {
    canvas.Rect(x_position * cell_size, y_position * cell_size, cell_size, cell_size, "red", "red"); // Draw shark
  }

  /* Identify this organism as not a fish */
  bool IsFish() const override { return false; }
};

#endif

























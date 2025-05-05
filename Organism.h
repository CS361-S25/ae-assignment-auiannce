/* Defines the abstract Organism class that serves as a base for Fish and Shark types.*/

#ifndef ORGANISM_H // Header guard start
#define ORGANISM_H //

#include "emp/web/Canvas.hpp" // Include for Canvas drawing
namespace emp { class Random; } // Forward declaration of Random

class World; // Forward declare World

/* Abstract class representing any organism (fish or shark) in the world */
class Organism {
protected:
  int x_position; // X position on grid
  int y_position; // Y position on grid
  double energy = 5.0; // Initial energy

public:
/** Constructor: create an organism at a given position */
  Organism(int initial_x_position, int initial_y_position)
    : x_position(initial_x_position), y_position(initial_y_position) {}

  virtual ~Organism() {} // Virtual destructor

/** Get the X position */
  int GetXPosition() const { return x_position; } 

/** Get the Y position */
  int GetYPosition() const { return y_position; } 

/** Get the current energy level */
  double GetEnergyLevel() const { return energy; } 

/** Set a new energy level */
  void SetEnergyLevel(double new_energy) { energy = new_energy; }

/** Change the energy level by a given amount */
  void ChangeEnergy(double delta) { energy += delta; } 

/** Set a new (X, Y) position */
  void SetPosition(int new_x_position, int new_y_position) {
    x_position = new_x_position; 
    y_position = new_y_position; 
  }

/** Draw the organism on the canvas (pure virtual) */
  virtual void Draw(emp::web::Canvas& canvas, double cell_size) const = 0; 

/** Check if organism is a fish (pure virtual) */
  virtual bool IsFish() const = 0;
};

#endif









  







#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
#include <memory>

class World; // Forward declaration to avoid circular dependency

/* Organism is a base class for entities in the world
   such as Fish and Shark. It provides shared behavior and defines
   a common interface for derived types. */
class Organism {
protected:
  int x;           // X position on the grid
  int y;           // Y position on the grid
  double energy = 5.0; // Initial energy level

public:
  /* Constructor to initialize organism's position */
  Organism(int startX, int startY)
    : x(startX), y(startY) {}

  /* Virtual destructor for base class */
  virtual ~Organism() {}

  /* Get X-coordinate of the organism */
  int GetX() const { return x; }

  /* Get Y-coordinate of the organism */
  int GetY() const { return y; }

  /* Get organism's current energy */
  double GetEnergy() const { return energy; }

  /* Set organism's energy directly */
  void SetEnergy(double newEnergy) { energy = newEnergy; }

  /* Modify organism's energy by a given amount */
  void ChangeEnergy(double amount) { energy += amount; }

  /* Update organism's position */
  void SetPosition(int newX, int newY) {
    x = newX;
    y = newY;
  }

  /* Return true if organism is a Fish (must be implemented in derived class) */
  virtual bool IsFish() const = 0;

  /* Clone the organism at a new position */
  virtual std::shared_ptr<Organism> CloneAt(int newX, int newY) const = 0;

  /* Reproduce based on internal conditions */
  virtual void Reproduce(World& world) = 0;

  /* Return organism's color for rendering */
  virtual std::string GetColor() const = 0;

protected:
  /* Spawn a clone of the organism in a nearby empty cell */
  void SpawnNear(World& world, const Organism& parent);
};

#endif

















  







#ifndef SHARK_H
#define SHARK_H

#include "Organism.h"

class World;

/* The Shark class represents a predator in the ecosystem.
   It inherits from Organism and overrides necessary functions. */
class Shark : public Organism {
public:
  /* Constructor initializes shark's position */
  Shark(int x, int y) : Organism(x, y) {}

  /* Indicates this organism is not a fish */
  bool IsFish() const override { return false; }

  /* Create a new Shark at a specified position */
  std::shared_ptr<Organism> CloneAt(int newX, int newY) const override {
    return std::make_shared<Shark>(newX, newY);
  }

  /* Reproduce if energy is sufficient by spawning a new Shark nearby */
  void Reproduce(World& world) override {
    if (energy >= 25.0) { // reproduction threshold
      SpawnNear(world, *this); // try to spawn in adjacent empty cell
      energy -= 25.0; // reproduction cost
    }
  }

  /* Return the color representation for display purposes */
  std::string GetColor() const override {
    return "green"; // sharks are displayed as green
  }
};

#endif






























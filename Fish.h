#ifndef FISH_H
#define FISH_H

#include "Organism.h"

class World;

/* The Fish class represents a prey organism in the ecosystem.
   It inherits from Organism and defines behavior like reproduction and identification. */
class Fish : public Organism {
public:
  /* Constructor initializes fish's position */
  Fish(int x, int y) : Organism(x, y) {}

  /* Indicates this organism is a fish */
  bool IsFish() const override { return true; }

  /* Create a new Fish at a specified position */
  std::shared_ptr<Organism> CloneAt(int newX, int newY) const override {
    return std::make_shared<Fish>(newX, newY);
  }

  /* Reproduce if energy is sufficient by spawning a new Fish nearby */
  void Reproduce(World& world) override {
    if (energy >= 5.0) { // reproduction threshold
      SpawnNear(world, *this); // attempt to spawn in a neighboring cell
      energy -= 4.0; // energy cost of reproduction
    }
  }

  /* Return the color representation for display purposes */
  std::string GetColor() const override {
    return "blue"; // fish are displayed as blue
  }
};

#endif








































#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "Organism.h"
#include "Fish.h"
#include "Shark.h"

extern emp::Random global_random;

/* The World class manages the 2D grid and organisms in the ecosystem */
class World {
private:
  int width;  // Width of the grid
  int height; // Height of the grid
  std::vector<std::vector<std::shared_ptr<Organism>>> grid; // 2D grid storing organisms

public:
  /* Constructor initializes grid with given dimensions */
  World(int w, int h) : width(w), height(h) {
    grid.resize(width, std::vector<std::shared_ptr<Organism>>(height));
  }

  /* Get the width of the grid */
  int GetGridWidth() const { return width; }

  /* Get the height of the grid */
  int GetGridHeight() const { return height; }

  /* Check if a cell is empty */
  bool IsCellEmpty(int x, int y) const {
    return !grid[x][y]; // true if nullptr
  }

  /* Check if a cell contains a fish */
  bool CellHasFish(int x, int y) const {
    return grid[x][y] && grid[x][y]->IsFish();
  }

  /* Return reference to internal grid */
  std::vector<std::vector<std::shared_ptr<Organism>>>& GetInternalGrid() {
    return grid;
  }

  /* Move organism from one cell to another */
  void MoveOrganism(int fromX, int fromY, int toX, int toY) {
    grid[toX][toY] = grid[fromX][fromY];
    grid[fromX][fromY] = nullptr;
  }

  /* Remove organism from the grid */
  void RemoveOrganism(int x, int y) {
    grid[x][y] = nullptr;
  }

  /* Populate the grid with initial fish and sharks */
  void PopulateInitial() {
    int total = width * height;
    int numFish = total / 40;   // 2.5% of grid
    int numSharks = total / 100; // 1% of grid

    for (int i = 0; i < numFish; ++i) {
      int x = global_random.GetInt(width);
      int y = global_random.GetInt(height);
      if (IsCellEmpty(x, y)) {
        grid[x][y] = std::make_shared<Fish>(x, y); // Spawn fish
      }
    }

    for (int i = 0; i < numSharks; ++i) {
      int x = global_random.GetInt(width);
      int y = global_random.GetInt(height);
      if (IsCellEmpty(x, y)) {
        grid[x][y] = std::make_shared<Shark>(x, y); // Spawn shark
      }
    }
  }

  /* Update all organisms in the grid */
  void UpdateOrganisms() {
    std::vector<std::shared_ptr<Organism>> all;

    // Collect all organisms
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        if (grid[x][y]) {
          all.push_back(grid[x][y]);
        }
      }
    }

    // Update fish and sharks separately
    for (auto& org : all) {
      if (!org) continue;

      if (org->IsFish()) {
        UpdateFish(org);
      } else {
        UpdateShark(org);
      }
    }

    // Shuffle organisms and call reproduction
    std::shuffle(all.begin(), all.end(), std::mt19937(global_random.GetUInt()));

    for (auto& org : all) {
      if (org) org->Reproduce(*this);
    }
  }

  /* Update behavior and state of a fish */
  void UpdateFish(std::shared_ptr<Organism> fish) {
    fish->ChangeEnergy(-0.05); // energy decay

    if (fish->GetEnergy() <= 0) {
      RemoveOrganism(fish->GetX(), fish->GetY()); // fish dies
      return;
    }

    if (global_random.P(0.7)) { // 70% chance to move
      int newX = emp::Mod(fish->GetX() + global_random.GetInt(-1, 2), width);  // wrap around
      int newY = emp::Mod(fish->GetY() + global_random.GetInt(-1, 2), height); // wrap around
      if (IsCellEmpty(newX, newY)) {
        MoveOrganism(fish->GetX(), fish->GetY(), newX, newY);
        fish->SetPosition(newX, newY);
      }
    }

    fish->ChangeEnergy(0.3); // regain energy from movement
  }

  /* Update behavior and state of a shark */
  void UpdateShark(std::shared_ptr<Organism> shark) {
    shark->ChangeEnergy(-0.2); // energy decay

    if (shark->GetEnergy() <= 0) {
      RemoveOrganism(shark->GetX(), shark->GetY()); // shark dies
      return;
    }

    bool foundFish = false;
    int tx = shark->GetX(), ty = shark->GetY();

    // Look for adjacent fish
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        int nx = emp::Mod(tx + dx, width);
        int ny = emp::Mod(ty + dy, height);
        if (CellHasFish(nx, ny)) {
          tx = nx;
          ty = ny;
          foundFish = true;
        }
      }
    }

    if (foundFish) {
      shark->ChangeEnergy(+1.5); // gain energy from eating
      RemoveOrganism(tx, ty);    // remove eaten fish
      MoveOrganism(shark->GetX(), shark->GetY(), tx, ty);
      shark->SetPosition(tx, ty);
    } else if (global_random.P(0.2)) { // 20% chance to move randomly
      int nx = emp::Mod(shark->GetX() + global_random.GetInt(-1, 2), width);
      int ny = emp::Mod(shark->GetY() + global_random.GetInt(-1, 2), height);
      if (IsCellEmpty(nx, ny)) {
        MoveOrganism(shark->GetX(), shark->GetY(), nx, ny);
        shark->SetPosition(nx, ny);
      }
    }

    shark->ChangeEnergy(-0.1); // additional cost per turn
  }
};

/* Spawns a new organism in a nearby empty cell */
 void Organism::SpawnNear(World& world, const Organism& parent) {
  int x = parent.GetX();
  int y = parent.GetY();

  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      int nx = emp::Mod(x + dx, world.GetGridWidth());
      int ny = emp::Mod(y + dy, world.GetGridHeight());
      if (world.IsCellEmpty(nx, ny)) {
        world.GetInternalGrid()[nx][ny] = parent.CloneAt(nx, ny);
        return;
      }
    }
  }
}

#endif



















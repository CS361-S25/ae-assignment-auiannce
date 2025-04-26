/* Defines the World class that manages the ecosystem simulation grid and all organism behaviors.*/
#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include "emp/math/math.hpp"
#include "Organism.h"
#include "Fish.h"
#include "Shark.h"

// External random number generator
extern emp::Random global_random;

/* Class that represents the world grid containing all organisms */
class World {
private:
  /* Width of the grid */
  int grid_width;

  /* Height of the grid */
  int grid_height;

  /* 2D grid holding organisms */
  std::vector<std::vector<std::shared_ptr<Organism>>> organism_grid;

public:
  /* Constructor: initialize the world with given width and height */
  World(int input_width, int input_height)
    : grid_width(input_width), grid_height(input_height) {
    // Resize the organism grid to match world dimensions
    organism_grid.resize(grid_width, std::vector<std::shared_ptr<Organism>>(grid_height));
  }

  /* Getter for grid width */
  int GetGridWidth() const { return grid_width; }

  /* Getter for grid height */
  int GetGridHeight() const { return grid_height; }

  /* Check if a cell is empty */
  bool IsCellEmpty(int x_position, int y_position) const {
    return !organism_grid[x_position][y_position];
  }

  /* Check if a cell contains a fish */
  bool DoesCellHaveFish(int x_position, int y_position) const {
    return organism_grid[x_position][y_position] && organism_grid[x_position][y_position]->IsFish();
  }

  /* Move an organism from one location to another */
  void MoveOrganismToNewLocation(int from_x, int from_y, int to_x, int to_y) {
    organism_grid[to_x][to_y] = organism_grid[from_x][from_y]; // Move organism to new spot
    organism_grid[from_x][from_y] = nullptr; // Clear old spot
  }

  /* Remove organism at a specific location */
  void RemoveOrganismAt(int x_position, int y_position) {
    organism_grid[x_position][y_position] = nullptr; // Remove organism by setting cell to nullptr
  }

  /* Spawn a new fish in an adjacent empty cell (if one exists) */
  void SpawnNewFishAt(int x_position, int y_position) {
    for (int offset_x = -1; offset_x <= 1; ++offset_x) {
      for (int offset_y = -1; offset_y <= 1; ++offset_y) {
        int new_x = emp::Mod(x_position + offset_x, grid_width); // Handle wrapping around grid edges
        int new_y = emp::Mod(y_position + offset_y, grid_height); // Handle wrapping around grid edges
        if (IsCellEmpty(new_x, new_y)) {
          organism_grid[new_x][new_y] = std::make_shared<Fish>(new_x, new_y); // Spawn fish
          return; // Done after first successful spawn
        }
      }
    }
  }

  /* Spawn a new shark in an adjacent empty cell (if one exists) */
  void SpawnNewSharkAt(int x_position, int y_position) {
    for (int offset_x = -1; offset_x <= 1; ++offset_x) {
      for (int offset_y = -1; offset_y <= 1; ++offset_y) {
        int new_x = emp::Mod(x_position + offset_x, grid_width); // Handle wrapping around grid edges
        int new_y = emp::Mod(y_position + offset_y, grid_height); // Handle wrapping around grid edges
        if (IsCellEmpty(new_x, new_y)) {
          organism_grid[new_x][new_y] = std::make_shared<Shark>(new_x, new_y); // Spawn shark
          return; // Done after first successful spawn
        }
      }
    }
  }

  /* Update a fish's behavior: move, reproduce, or die */
  void UpdateFish(std::shared_ptr<Organism> organism, emp::Random& random) {
    organism->ChangeEnergy(-0.1); // Decrease fish energy over time
    if (organism->GetEnergyLevel() <= 0.0) {
      RemoveOrganismAt(organism->GetXPosition(), organism->GetYPosition()); // Remove dead fish
      return;
    }

    if (random.P(0.5)) { // 50% chance to move
      int new_x = emp::Mod(organism->GetXPosition() + random.GetInt(-1, 2), grid_width);
      int new_y = emp::Mod(organism->GetYPosition() + random.GetInt(-1, 2), grid_height);
      if (IsCellEmpty(new_x, new_y)) {
        MoveOrganismToNewLocation(organism->GetXPosition(), organism->GetYPosition(), new_x, new_y); // Move fish
        organism->SetPosition(new_x, new_y); // Update fish coordinates
      }
    }

    if (random.P(0.05)) { // 5% chance to reproduce
      SpawnNewFishAt(organism->GetXPosition(), organism->GetYPosition());
    }
  }

  /* Update a shark's behavior: hunt, move, or die */
  void UpdateShark(std::shared_ptr<Organism> organism, emp::Random& random) {
    organism->ChangeEnergy(-0.1); // Decrease shark energy over time

    if (organism->GetEnergyLevel() <= 0.0) {
      RemoveOrganismAt(organism->GetXPosition(), organism->GetYPosition()); // Remove dead shark
      return;
    }

    bool found_fish = false;
    int fx = organism->GetXPosition();
    int fy = organism->GetYPosition();

    for (int dx = -1; dx <= 1; dx++) { // Check surrounding cells
      for (int dy = -1; dy <= 1; dy++) {
        int nx = emp::Mod(fx + dx, grid_width);
        int ny = emp::Mod(fy + dy, grid_height);

        if (DoesCellHaveFish(nx, ny)) {
          fx = nx; // Update fish location
          fy = ny;
          found_fish = true;
        }
      }
    }

    if (found_fish) {
      organism->ChangeEnergy(+8.0); // Gain energy by eating fish
      RemoveOrganismAt(fx, fy); // Remove eaten fish
      MoveOrganismToNewLocation(organism->GetXPosition(), organism->GetYPosition(), fx, fy); // Move to fish spot
      organism->SetPosition(fx, fy); // Update shark coordinates
    } else if (random.P(0.2)) { // 20% chance to move randomly
      int new_x = emp::Mod(organism->GetXPosition() + random.GetInt(-1, 2), grid_width);
      int new_y = emp::Mod(organism->GetYPosition() + random.GetInt(-1, 2), grid_height);

      if (IsCellEmpty(new_x, new_y)) {
        MoveOrganismToNewLocation(organism->GetXPosition(), organism->GetYPosition(), new_x, new_y); // Move shark
        organism->SetPosition(new_x, new_y); // Update shark coordinates
      }
    }
  }

  /* Populate the world initially with fish and sharks */
  void PopulateWithInitialOrganisms() {
    for (int i = 0; i < (grid_width * grid_height) / 30; i++) {
      int x = global_random.GetInt(grid_width);
      int y = global_random.GetInt(grid_height);
      if (IsCellEmpty(x, y)) {
        organism_grid[x][y] = std::make_shared<Fish>(x, y); // Place fish
      }
    }

    for (int i = 0; i < (grid_width * grid_height) / 30; i++) {
      int x = global_random.GetInt(grid_width);
      int y = global_random.GetInt(grid_height);
      if (IsCellEmpty(x, y)) {
        organism_grid[x][y] = std::make_shared<Shark>(x, y); // Place shark
      }
    }
  }

  /* Update all organisms for one simulation frame */
  void UpdateOrganisms() {
    std::vector<std::shared_ptr<Organism>> all_organisms;

    for (int i = 0; i < grid_width; i++) {
      for (int j = 0; j < grid_height; j++) {
        if (organism_grid[i][j]) {
          all_organisms.push_back(organism_grid[i][j]); // Collect all non-empty cells
        }
      }
    }

    for (auto& organism : all_organisms) {
      if (!organism) continue; // Skip empty slots
      if (organism->IsFish()) {
        UpdateFish(organism, global_random); // Update fish behavior
      } else {
        UpdateShark(organism, global_random); // Update shark behavior
      }
    }
  }

  /* Draw the world onto the given canvas */
  void DrawWorld(emp::web::Canvas& canvas) {
    canvas.Clear("black"); // Clear canvas background
    double cell_size = 8.0; // Size of each cell

    for (int i = 0; i < grid_width; i++) {
      for (int j = 0; j < grid_height; j++) {
        if (organism_grid[i][j]) {
          organism_grid[i][j]->Draw(canvas, cell_size); // Draw organism
        } else {
          canvas.Rect(i * cell_size, j * cell_size, cell_size, cell_size, "black", "black"); // Draw empty cell
        }
      }
    }
  }
};

#endif




#ifndef WORLD_H // Include guard to prevent double inclusion
#define WORLD_H // Include guard

#include <vector> // For using std::vector
#include <memory> // For using std::shared_ptr
#include "emp/math/math.hpp" // For modulus and math utilities
#include "emp/math/Random.hpp" // For emp::Random number generation
#include "Organism.h" // Include base class
#include "Fish.h" // Include Fish class
#include "Shark.h" // Include Shark class
#include <algorithm> // For std::shuffle
#include <random> // For random number generator

extern emp::Random global_random; // Global random number generator

class World { // Define the World class
private:
  int grid_width; // Width of the grid
  int grid_height; // Height of the grid
  std::vector<std::vector<std::shared_ptr<Organism>>> organism_grid; // 2D grid of organisms

public:
/** Constructor for the World class */
  World(int input_width, int input_height) // Initialize world size
    : grid_width(input_width), grid_height(input_height) { // Use initializer list
    organism_grid.resize(grid_width, std::vector<std::shared_ptr<Organism>>(grid_height)); // Resize grid
  }

/** Get the grid width */
  int GetGridWidth() const { return grid_width; } // Return width

/** Get the grid height */
  int GetGridHeight() const { return grid_height; } // Return height

/** Check if a specific grid cell is empty */
  bool IsCellEmpty(int x, int y) const {
    return !organism_grid[x][y]; // Return true if null
  }

/** Check if a cell has a fish */
  bool DoesCellHaveFish(int x, int y) const {
    return organism_grid[x][y] && organism_grid[x][y]->IsFish(); // Return true if it's a fish
  }

/** Move organism from one cell to another */
  void MoveOrganismToNewLocation(int from_x, int from_y, int to_x, int to_y) {
    organism_grid[to_x][to_y] = organism_grid[from_x][from_y]; // Move pointer
    organism_grid[from_x][from_y] = nullptr; // Clear old location
  }

/** Remove organism from a specific cell */
  void RemoveOrganismAt(int x, int y) {
    organism_grid[x][y] = nullptr; // Set pointer to null
  }

/** Spawn a new fish in a neighboring empty cell */
  void SpawnNewFishAt(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) { // Loop over 3x3 neighborhood
      for (int dy = -1; dy <= 1; ++dy) {
        int nx = emp::Mod(x + dx, grid_width); // Wrap x coordinate
        int ny = emp::Mod(y + dy, grid_height); // Wrap y coordinate
        if (IsCellEmpty(nx, ny)) {
          organism_grid[nx][ny] = std::make_shared<Fish>(nx, ny); // Create fish
          return; // Exit after placing
        }
      }
    }
  }

/** Spawn a new shark in a neighboring empty cell */
  void SpawnNewSharkAt(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        int nx = emp::Mod(x + dx, grid_width);
        int ny = emp::Mod(y + dy, grid_height);
        if (IsCellEmpty(nx, ny)) {
          organism_grid[nx][ny] = std::make_shared<Shark>(nx, ny); // Create shark
          return;
        }
      }
    }
  }

/** Update a fish: move, die, or reproduce */
  void UpdateFish(std::shared_ptr<Organism> organism, emp::Random& random) {
    organism->ChangeEnergy(-0.05); // Decay energy
    if (organism->GetEnergyLevel() <= 0.0) {
      RemoveOrganismAt(organism->GetXPosition(), organism->GetYPosition()); // Die if no energy
      return;
    }

    if (random.P(0.7)) { // 70% chance to move
      int new_x = emp::Mod(organism->GetXPosition() + random.GetInt(-1, 2), grid_width);
      int new_y = emp::Mod(organism->GetYPosition() + random.GetInt(-1, 2), grid_height);
      if (IsCellEmpty(new_x, new_y)) {
        MoveOrganismToNewLocation(organism->GetXPosition(), organism->GetYPosition(), new_x, new_y);
        organism->SetPosition(new_x, new_y); // Update internal coordinates
      }
    }

    if (random.P(0.05)) { // 5% chance to spawn new fish
      SpawnNewFishAt(organism->GetXPosition(), organism->GetYPosition());
    }
  }

/** Update a shark: hunt, move, or die */
  void UpdateShark(std::shared_ptr<Organism> organism, emp::Random& random) {
    organism->ChangeEnergy(-0.2); // Lose more energy per turn

    if (organism->GetEnergyLevel() <= 0.0) {
      RemoveOrganismAt(organism->GetXPosition(), organism->GetYPosition());
      return;
    }

    bool found_fish = false;
    int fx = organism->GetXPosition();
    int fy = organism->GetYPosition();

    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
        int nx = emp::Mod(fx + dx, grid_width);
        int ny = emp::Mod(fy + dy, grid_height);
        if (DoesCellHaveFish(nx, ny)) {
          fx = nx;
          fy = ny;
          found_fish = true;
        }
      }
    }

    if (found_fish) {
      organism->ChangeEnergy(+1.5); // Gain energy from eating
      RemoveOrganismAt(fx, fy); // Remove fish
      MoveOrganismToNewLocation(organism->GetXPosition(), organism->GetYPosition(), fx, fy);
      organism->SetPosition(fx, fy);
    } else if (random.P(0.2)) { // 20% chance to move
      int new_x = emp::Mod(organism->GetXPosition() + random.GetInt(-1, 2), grid_width);
      int new_y = emp::Mod(organism->GetYPosition() + random.GetInt(-1, 2), grid_height);
      if (IsCellEmpty(new_x, new_y)) {
        MoveOrganismToNewLocation(organism->GetXPosition(), organism->GetYPosition(), new_x, new_y);
        organism->SetPosition(new_x, new_y);
      }
    }
  }

/** Populate grid with initial fish and sharks */
  void PopulateWithInitialOrganisms() {
    int total_cells = grid_width * grid_height;
    int fish_count = total_cells / 40; // Density control
    int shark_count = total_cells / 100;

    for (int i = 0; i < fish_count; i++) {
      int x = global_random.GetInt(grid_width);
      int y = global_random.GetInt(grid_height);
      if (IsCellEmpty(x, y)) {
        organism_grid[x][y] = std::make_shared<Fish>(x, y);
      }
    }

    for (int i = 0; i < shark_count; i++) {
      int x = global_random.GetInt(grid_width);
      int y = global_random.GetInt(grid_height);
      if (IsCellEmpty(x, y)) {
        organism_grid[x][y] = std::make_shared<Shark>(x, y);
      }
    }
  }

/** Update every organism: move, act, reproduce */
  void UpdateOrganisms() {
    std::vector<std::shared_ptr<Organism>> all_organisms;

    for (int i = 0; i < grid_width; i++) {
      for (int j = 0; j < grid_height; j++) {
        if (organism_grid[i][j]) {
          all_organisms.push_back(organism_grid[i][j]);
        }
      }
    }

    for (auto& organism : all_organisms) {
      if (!organism) continue;
      if (organism->IsFish()) {
        UpdateFish(organism, global_random);
      } else {
        UpdateShark(organism, global_random);
      }

      if (organism->IsFish()) {
        organism->ChangeEnergy(+0.3); // Recover some energy
      } else {
        organism->ChangeEnergy(-0.1); // Slight penalty
      }
    }

    std::vector<size_t> schedule(all_organisms.size());
    std::iota(schedule.begin(), schedule.end(), 0);
    std::shuffle(schedule.begin(), schedule.end(), std::mt19937(global_random.GetUInt()));

    for (size_t k : schedule) {
      auto& organism = all_organisms[k];
      if (!organism) continue;
      int x = organism->GetXPosition();
      int y = organism->GetYPosition();

      if (organism->IsFish() && organism->GetEnergyLevel() >= 5.0) {
        SpawnNewFishAt(x, y);
        organism->ChangeEnergy(-4.0); // Reproduction cost
      } else if (!organism->IsFish() && organism->GetEnergyLevel() >= 25.0) {
        if (global_random.P(0.3)) {
          SpawnNewSharkAt(x, y);
          organism->ChangeEnergy(-25.0); // Costly reproduction
        }
      }
    }
  }

/** Render all organisms to the canvas */
  void DrawWorld(emp::web::Canvas& canvas) {
    canvas.Clear("black"); // Reset canvas
    double cell_size = 8.0;

    for (int i = 0; i < grid_width; i++) {
      for (int j = 0; j < grid_height; j++) {
        if (organism_grid[i][j]) {
          organism_grid[i][j]->Draw(canvas, cell_size);
        } else {
          canvas.Rect(i * cell_size, j * cell_size, cell_size, cell_size, "black", "black"); // Draw empty cell
        }
      }
    }
  }
};

#endif // End include guard











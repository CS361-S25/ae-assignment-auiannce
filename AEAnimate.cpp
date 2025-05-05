#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "emp/web/Canvas.hpp"
#include "emp/math/Random.hpp"
#include "World.h"

// Global random number generator
emp::Random global_random(1);

// HTML document interface
emp::web::Document doc("target");

/* Simulation class handles rendering and simulation updates each frame */
class Simulation : public emp::web::Animate {
  World world; // The simulated ecosystem world
  const double size = 8.0; // Size of each grid cell in pixels
  emp::web::Canvas canvas; // Canvas for drawing organisms

public:
  /* Constructor sets up the simulation environment and UI */
  Simulation()
  : world(120, 100), // Initialize a 120x100 world
    canvas(world.GetGridWidth() * size, world.GetGridHeight() * size, "canvas") {

    // Add descriptive title and text to the webpage
    doc << "<h2 style='color: black;'>Artificial Ecosystem Simulation</h2>";
    doc << "<p style='color: black;'>"
        << "This simulation shows an ecosystem with two species: blue fish and green sharks.<br>"
        << "Fish move randomly and reproduce when they have enough energy.<br>"
        << "Sharks lose energy over time, hunt nearby fish to survive, and reproduce when their energy is high.<br>"
        << "Both species die if their energy reaches zero.<br>"
        << "The world wraps around at the edges (toroidal space).<br>"
        << "Click Start to run the simulation continuously, or Step to advance one frame at a time."
        << "</p>";

    // Add the canvas and control buttons to the UI
    doc << canvas;
    doc << GetToggleButton("Start");
    doc << GetStepButton("Step");

    // Seed the world with initial organisms
    world.PopulateInitial();
  }

  /* Called every frame to update simulation and redraw canvas */
  void DoFrame() override {
    world.UpdateOrganisms(); // Update all fish and sharks
    canvas.Clear("black");   // Clear canvas for redrawing

    auto& grid = world.GetInternalGrid(); // Access internal grid

    // Draw each organism in the grid
    for (int x = 0; x < world.GetGridWidth(); ++x) {
      for (int y = 0; y < world.GetGridHeight(); ++y) {
        if (grid[x][y]) {
          canvas.Rect(x * size, y * size, size, size,
                      grid[x][y]->GetColor(), grid[x][y]->GetColor()); // Fill rectangle with organism's color
        }
      }
    }
  }
};

// Instantiate and run the simulation
Simulation app;

/* Entry point */
int main() {
  app.Step(); // Draw the first frame
  return 0;
}
































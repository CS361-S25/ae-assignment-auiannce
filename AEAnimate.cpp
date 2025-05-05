/*Creates the Animate application for simulating and visualizing the ecosystem in a web browser.*/

#include "emp/web/Animate.hpp" // For Animate class
#include "emp/web/web.hpp" // For DOM control
#include "emp/web/Canvas.hpp" // For rendering
#include "emp/math/Random.hpp" // For RNG
#include "World.h" // Simulation world

emp::Random global_random(1); // Global random seed

emp::web::Document web_document("target"); // HTML document anchor

/* Main application class for the ecosystem animation */
class AEAnimateApplication : public emp::web::Animate {
  World world; // World being simulated
  const double cell_size = 8.0; // Pixel size of each cell
  emp::web::Canvas canvas; // Canvas to draw grid

public:
/** Constructor: sets up world, canvas, buttons, and initial organisms */
  AEAnimateApplication()
    : world(120, 100),
      canvas(world.GetGridWidth() * cell_size, world.GetGridHeight() * cell_size, "canvas") {

    // Add title and description
    web_document << "<h2 style='color: black;'>Artificial Ecosystem Simulation</h2>";
    web_document << "<p style='color: black;'>"
                << "This simulation models two species: blue fish and red sharks.<br>"
                << "Fish move randomly and reproduce occasionally.<br>"
                << "Sharks hunt nearby fish and lose energy over time. If they run out of energy, they die.<br>"
                << "The simulation uses toroidal wrapping (edges wrap around).<br>"
                << "Click Start to begin, or Step to move one frame forward."
                << "</p>";

    web_document << canvas; // Add canvas
    web_document << GetToggleButton("Start"); // Add Start button
    web_document << GetStepButton("Step"); // Add Step button

    world.PopulateWithInitialOrganisms(); // Populate grid with organisms
  }

  /** One simulation frame update */
  void DoFrame() override {
    world.UpdateOrganisms(); // Update world state
    world.DrawWorld(canvas); // Draw updated world
  }
};

AEAnimateApplication animate_application; // Create application

int main() {
  animate_application.Step(); // Run one step immediately
  return 0;
}



























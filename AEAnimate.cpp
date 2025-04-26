
/*Creates the Animate application for simulating and visualizing the ecosystem in a web browser.*/

#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "emp/web/Canvas.hpp"
#include "emp/math/Random.hpp"
#include "World.h"

// Global random number generator
emp::Random global_random(1);

// Set up a document connected to the web page
emp::web::Document web_document("target");

/* Main application class for the ecosystem animation */
class AEAnimateApplication : public emp::web::Animate {
  /* The world grid being simulated */
  World world;

  /* Size of each grid cell when drawing */
  const double cell_size = 8.0;

  /* Canvas for drawing the world */
  emp::web::Canvas canvas;

public:
  /* Constructor: sets up world, canvas, buttons, and initial organisms */
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

    web_document << canvas; // Add canvas to document
    web_document << GetToggleButton("Start"); // Add Start button
    web_document << GetStepButton("Step"); // Add Step button

    world.PopulateWithInitialOrganisms(); // Populate initial organisms
  }

  /* One simulation frame update */
  void DoFrame() override {
    world.UpdateOrganisms(); // Update all organisms
    world.DrawWorld(canvas); // Draw updated world
  }
};

/* Create the application */
AEAnimateApplication animate_application;

/* Start simulation immediately with one frame */
int main() {
  animate_application.Step(); // Do initial step
  return 0;
}





















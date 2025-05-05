# Artificial Ecosystem Simulation

## Overview
This simulation models two species: blue fish and green sharks, interacting in a 2D toroidal world.
- **Fish** move randomly and regain energy after moving. They reproduce if they have enough energy.
- **Sharks** hunt nearby fish to survive and gain energy by eating them. If no fish are found, they may move randomly. Sharks lose energy each turn and die if it drops to zero.
- The world wraps around at the edges (toroidal space).
- Click **Start** to run the simulation continuously, or **Step** to advance one frame at a time.

## Organisms

### Fish (Blue Squares)
- Move randomly with a 70% probability each time step.
- Regain energy after moving.
- Reproduce when energy ≥ 5.0 (reproduction costs 4.0 energy).
- Lose a small amount of energy over time (0.05 per step).
- Die if energy reaches 0.

### Sharks (Green Squares)
- Search adjacent cells for fish to eat.
- Gain 1.5 energy by eating fish.
- Move randomly with a 20% chance if no fish are nearby.
- Lose 0.2 energy per step, and an additional 0.1 if they move.
- Reproduce when energy ≥ 25.0 (reproduction costs 25.0 energy).
- Die if energy reaches 0.

## Interaction
- This simulation models a **predator-prey dynamic**:
  - Fish reproduce independently and recover energy through movement.
  - Sharks rely on hunting fish to gain energy and reproduce.
- These rules create natural population fluctuations and survival dynamics.
- The world wraps around at the edges—organisms exiting one side appear on the opposite.

## Running the Simulation
1. **Build and run:**
   ```bash
   ./compile-run.sh



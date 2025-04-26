[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)

# Artificial Ecosystem Simulation

## Overview
This simulation models two species: blue fish and red sharks.
- Fish move randomly and reproduce occasionally.
- Sharks hunt nearby fish and lose energy over time. If they run out of energy, they die.
- The simulation uses toroidal wrapping (edges wrap around).
- Click **Start** to begin, or **Step** to move one frame forward.

## Organisms

### Fish (Blue Squares)
- Move randomly with a 50% chance per time step.
- Reproduce occasionally with a small probability (5%).
- Lose energy over time and die if their energy reaches zero.

### Sharks (Red Squares)
- Search for nearby fish to eat.
- Gain energy by eating fish.
- Lose energy each time step and die if it reaches zero.
- Move randomly with a small chance if no fish are nearby.

## Interaction
- This simulation models a predator-prey ecosystem.
- Fish reproduce occasionally without needing to hunt.
- Sharks depend on eating fish to survive, creating natural dynamics.
- The grid wraps toroidally: moving off one edge brings organisms to the opposite side.

## Running the Simulation
- Run `./compile-run.sh` to compile and open the simulation in a browser.
- Click **Start** to begin automatic simulation.
- Click **Step** to advance one frame manually.



# Monte Carlo Particle Detector Simulation

This repository contains a C++ simulation of particle motion and detection using Monte Carlo methods. The simulation involves generating particles of different types (Electron, Muon, and Pion) with random initial positions, velocities, and energies. These particles interact with a series of detectors, and their movements are tracked over time. The results, including particle data and their interactions with the detectors, are saved to a CSV file.

## Features

- **Particle Generation**: Random generation of particles with types including Electron, Muon, and Pion.
- **Particle Movement**: Particles move based on velocity and lose energy over time.
- **Energy Loss**: Particles lose energy randomly during the simulation, mimicking real-world physical processes.
- **Scattering**: Particles have a chance to scatter based on a normal distribution.
- **Detection**: Particles can be detected by one of three detectors with varying probabilities.
- **Data Export**: Particle data is exported to a CSV file for analysis, including information such as type, mass, charge, energy, position, and velocity.
- **Real-Time Console Output**: Particle movements and energy are printed at regular intervals for debugging and observation.

## Simulation Overview

1. **Particle Types**: 
    - **Electron**: Mass = 9.11e-31 kg, Charge = -1, Lifetime = 1e10 s
    - **Muon**: Mass = 1.88e-28 kg, Charge = -1, Lifetime = 2.2e-6 s
    - **Pion**: Mass = 2.49e-28 kg, Charge = 1, Lifetime = 2.6e-8 s

2. **Random Initialization**: Particles are initialized with random positions, velocities, and energies within specified ranges.
3. **Movement**: Particles move based on their velocities, and their positions are updated over small time intervals.
4. **Energy Loss**: Each particle loses a fraction of its energy at each time step.
5. **Scattering**: Particles have a chance to change their direction by scattering with a given angular deviation.
6. **Detection**: Particles are detected by one of the detectors if they enter a specific position range. The detection probability varies with each detector.
7. **Lifetime Check**: Particles that exceed their lifetime are considered decayed and are removed from the simulation.
8. **Data Export**: The detected particles are saved to a CSV file (`detected_particle.csv`) with detailed information.

## Files

- **Main Program**: `main.cpp`
  - Contains the simulation code that generates particles, tracks their movements, and checks for interactions with the detectors.
- **CSV Export**: `detected_particle.csv`
  - The output file containing the particle data (type, mass, charge, energy, position, velocity) for the particles that were detected by the detectors.

## Compilation and Running

### Requirements
- A C++ compiler (C++11 or later).
- `cmath`, `random`, `vector`, `fstream` libraries (standard in most C++ setups).

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/particle-detector-simulation.git
   cd particle-detector-simulation
2. Compile the program:
   ```bash
   g++ sim.cpp -o simulation -std=c++11
3. Run the simulation:
   ```bash
   ./simulation
4. Run the vizualizer:
   ```bash
   python visualize.py

### Contributing
Feel free to fork this repository, open issues, and submit pull requests. Contributions to improve the simulation's performance, add more features, or fix bugs are welcome!

### License
This project is licensed under the MIT License - see the LICENSE file for details.



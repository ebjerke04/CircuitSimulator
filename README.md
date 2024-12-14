My goal is to build a light-weight circuit simulator.
This project uses ImGui paired with GLFW to create an intuitive user interface. The plan is to allow users to assemble their own circuits, supply them with a DC/AC/noise source and measure voltage and current waveforms at various locations of the circuit.
The goal at the moment is to be able to assemble any RLC circuit you want and simulate it; however, I would love to go beyond that goal in time.

I plan to use a better build system eventually, but for now install as follows:
1) Clone the repository to a directory of your choosing.
2) Simply open the .sln file
3) Build
4) Run inside of Visual Studio

Disclaimer!
This project was written and built inside of Visual Studio 2022, so be cautious when trying to clone and build in other versions!!!

Current features:
- Assemble a circuit that contains a DC voltage source, and resistors. Assemble any number of resistors you want in any manner you want.
- Calculate the net resistance of the resistor network (Utilizes Eigen library to assemble a conduction matrix and injects a test current.)

In the works:
- Working on adding AC simulations.
- Graphing of output voltages/currents.
- Be able to delete components.
- Addition of inductors and capacitors.

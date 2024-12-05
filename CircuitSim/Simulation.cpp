#include "Simulation.h"

Simulation::Simulation(const Circuit& circuit) : m_Circuit(circuit) {}

float Simulation::CalculateNetResistance() const {
    return 0.0f;
}

void Simulation::Run() 
{
    m_Circuit.LogWires();
}

void Simulation::LogResults() const 
{
    std::cout << "Simulation Results:" << std::endl;
    for (const auto& [terminalName, voltage] : m_TerminalVoltages) 
    {
        std::cout << "Terminal " << terminalName << ": " << voltage << " V" << std::endl;
    }
}

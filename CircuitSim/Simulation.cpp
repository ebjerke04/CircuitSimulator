#include "Simulation.h"

Simulation::Simulation(const Circuit& circuit) : m_Circuit(circuit) {}

void Simulation::Run() 
{
    
}

void Simulation::LogResults() const 
{
    std::cout << "Simulation Results:" << std::endl;
    for (const auto& [terminalName, voltage] : m_TerminalVoltages) 
    {
        std::cout << "Terminal " << terminalName << ": " << voltage << " V" << std::endl;
    }
}

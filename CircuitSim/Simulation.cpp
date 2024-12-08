#include "Simulation.h"

Simulation::Simulation(const Circuit& circuit) : m_Circuit(circuit) {}

float Simulation::CalculateNetResistance() const {
    // find voltage source positive terminal...
    std::shared_ptr<Terminal> vs_positive_terminal = nullptr;
    std::shared_ptr<Terminal> vs_negative_terminal = nullptr;
    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<VoltageSource_DC*>(component.get()))
        {
            VoltageSource_DC* voltage_source = static_cast<VoltageSource_DC*>(component.get());
            vs_positive_terminal = voltage_source->GetPositiveTerminal();
            vs_negative_terminal = voltage_source->GetNegativeTerminal();
            break;
        }
    }

    std::function<const std::unique_ptr<Wire>& (const std::shared_ptr<Terminal>&)> getWireAtTerminal;
    getWireAtTerminal = [&](const std::shared_ptr<Terminal>& terminal) -> const std::unique_ptr<Wire>&
        {
            for (const std::unique_ptr<Wire>& wire : m_Circuit.GetWires())
            {
                for (const std::shared_ptr<Terminal>& wire_terminal : wire->GetConnectedTerminals())
                {
                    if (terminal == wire_terminal) return wire;
                }
            }

            return nullptr;
        };

    std::unordered_set<std::shared_ptr<Terminal>> visited;
    std::function<bool(const std::shared_ptr<Terminal>&)> notVisited;
    notVisited = [&](const std::shared_ptr<Terminal>& terminal) -> bool
        {
            if (visited.find(terminal) != visited.end()) return false;
            return true;
        };

    std::function<float(const std::shared_ptr<Terminal>&)> calculateResistance;
    calculateResistance = [&](const std::shared_ptr<Terminal>& currentTerminal) -> float
        {
            float total = 0.0f;

            if (notVisited(currentTerminal) == false)
            {
                return 0;
            }
            visited.insert(currentTerminal);

            const std::unique_ptr<Wire>& wire = getWireAtTerminal(currentTerminal);
            for (const std::shared_ptr<Terminal>& wire_terminal : wire->GetConnectedTerminals())
            {
                if (wire_terminal == vs_negative_terminal)
                {
                    std::cout << "Target terminal found" << std::endl;
                    return 0.0f;
                }
            }

            int other_terminals_count = wire->GetConnectedTerminals().size() - 1;

            if (other_terminals_count == 1)
            {
                // only one other terminal on wire (series connection)
                std::shared_ptr<Terminal> other_terminal = nullptr;
                for (const std::shared_ptr<Terminal>& terminal : wire->GetConnectedTerminals())
                {
                    if (terminal != currentTerminal)
                    {
                        other_terminal = terminal;
                        break;
                    }
                }

                Component* connected_component = other_terminal->GetComponent();
                if (dynamic_cast<Resistor*>(connected_component))
                {
                    Resistor* connected_resistor = static_cast<Resistor*>(connected_component);
                    float resistance = connected_resistor->GetResistance();
                    total += resistance;

                    for (const std::shared_ptr<Terminal>& resistor_terminal : connected_resistor->GetTerminals())
                    {
                        if (resistor_terminal != other_terminal)
                        {
                            other_terminal = resistor_terminal;
                            break;
                        }
                    }

                    total += calculateResistance(other_terminal);
                }
            }
            else if (other_terminals_count > 1)
            {
                // parallel branch detected
                std::vector<float> branch_resistances;
                for (const std::shared_ptr<Terminal>& terminal : wire->GetConnectedTerminals())
                {
                    if (terminal != currentTerminal)
                    {
                        Component* connected_component = terminal->GetComponent();
                        if (dynamic_cast<Resistor*>(connected_component))
                        {
                            Resistor* connected_resistor = static_cast<Resistor*>(connected_component);
                            float resistance = connected_resistor->GetResistance();

                            std::shared_ptr<Terminal> other_terminal = nullptr;
                            for (const std::shared_ptr<Terminal>& resistor_terminal : connected_resistor->GetTerminals())
                            {
                                if (resistor_terminal != terminal)
                                {
                                    other_terminal = resistor_terminal;
                                    break;
                                }
                            }

                            resistance += calculateResistance(other_terminal);
                            branch_resistances.push_back(resistance);
                        }

                    }
                }

                float conductance_sum = 0.0f;
                for (float branch_resistance : branch_resistances)
                {
                    if (branch_resistance > 0.0f) // Ensure no division by zero
                    {
                        float branch_conductance = 1.0f / branch_resistance;
                        conductance_sum += branch_conductance;
                    }
                    else
                    {
                        std::cerr << "Invalid branch resistance encountered: " << branch_resistance << std::endl;
                    }
                }

                if (conductance_sum > 0.0f)
                {
                    total += 1.0f / conductance_sum;
                }
                else
                {
                    std::cerr << "Parallel branch has no valid conductances." << std::endl;
                }
            }

            return total;
        };

    float net_resistance = calculateResistance(vs_positive_terminal);
    return net_resistance;
}

void Simulation::Run()
{
    m_Circuit.LogWires();

    std::cout << std::endl;
    std::cout << CalculateNetResistance() << std::endl;
}

void Simulation::LogResults() const
{
    std::cout << "Simulation Results:" << std::endl;
    for (const auto& [terminalName, voltage] : m_TerminalVoltages)
    {
        std::cout << "Terminal " << terminalName << ": " << voltage << " V" << std::endl;
    }
}
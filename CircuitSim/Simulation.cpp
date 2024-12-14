#include "Simulation.h"

Simulation::Simulation(const Circuit& circuit, Console& console) : m_Circuit(circuit), m_Console(console) {}

void Simulation::DrawSettingsCustomizer()
{
    if (m_DrawSettings)
    {
        ImGui::Begin("Simulation Settings", &m_DrawSettings);

        ImGui::Text("Duration:");
        ImGui::InputFloat("Duration (uS)", &m_DurationMicro);
        ImGui::Text("Time Step");
        ImGui::InputFloat("Time Step (uS)", &m_TimeStepMicro);

        ImGui::End();
    }
}

float Simulation::findEquivalentResistance() const 
{
    // Form blank conduction matrix, minus one because ground node gets removed.
    int wire_count = m_Circuit.GetWires().size();
    Eigen::MatrixXf conduction_matrix(wire_count, wire_count);
    conduction_matrix.setZero();
    
    // Get the positive terminal and negative terminal of voltage supply.
    Terminal* vs_positive_terminal = nullptr;
    Terminal* vs_negative_terminal = nullptr;
    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<VoltageSource_DC*>(component.get()))
        {
            VoltageSource_DC* voltage_source = static_cast<VoltageSource_DC*>(component.get());
            vs_positive_terminal = voltage_source->GetPositiveTerminal().get();
            vs_negative_terminal = voltage_source->GetNegativeTerminal().get();
            break;
        }
    }

    // Form an array of wires, first element being source positive node and last being ground node.
    std::vector<Wire*> wires_ordered;
    Wire* source_node = m_Circuit.GetWiringManager()->GetWireAtTerminal(vs_positive_terminal);
    Wire* ground_node = m_Circuit.GetWiringManager()->GetWireAtTerminal(vs_negative_terminal);
    wires_ordered.push_back(source_node);

    for (const std::unique_ptr<Wire>& wire : m_Circuit.GetWires())
    {
        Wire* wire_raw = wire.get();
        auto it = std::find(wires_ordered.begin(), wires_ordered.end(), wire_raw);
        
        if (it != wires_ordered.end())
        {
            continue;
        }
        if (wire_raw == ground_node) continue;
        if (wire_raw == source_node) continue;

        wires_ordered.push_back(wire_raw);
    }
    wires_ordered.push_back(ground_node);

    for (Wire* wire : wires_ordered)
    {
        wire->LogWire();
    }

    // Form the conduction matrix, solves for conductance between every node.
    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<Resistor*>(component.get()))
        {
            Resistor* resistor = static_cast<Resistor*>(component.get());

            Terminal* terminal1 = resistor->GetTerminals()[0].get();
            Terminal* terminal2 = resistor->GetTerminals()[1].get();
            Wire* terminal1_wire = m_Circuit.GetWiringManager()->GetWireAtTerminal(terminal1);
            Wire* terminal2_wire = m_Circuit.GetWiringManager()->GetWireAtTerminal(terminal2);

            if (terminal1_wire == nullptr || terminal2_wire == nullptr) continue;

            int terminal1_node = 999;
            int terminal2_node = 999;
            for (int i = 0; i < wires_ordered.size(); i++)
            {
                Wire* wire = wires_ordered[i];
                if (terminal1_wire == wire)
                {
                    terminal1_node = i;
                    break;
                }
            }
            for (int i = 0; i < wires_ordered.size(); i++)
            {
                Wire* wire = wires_ordered[i];
                if (terminal2_wire == wire)
                {
                    terminal2_node = i;
                    break;
                }
            }

            m_Console.PushMessage(resistor->GetName() + " connects nodes, " + std::to_string(terminal1_node) + " and " + std::to_string(terminal2_node));
            float resistor_conductance = (resistor->GetResistance() == 0.0f) ? 10000.0f : 1.0f / resistor->GetResistance();
            
            conduction_matrix(terminal1_node, terminal2_node) += -1.0f * resistor_conductance;
            conduction_matrix(terminal2_node, terminal1_node) += -1.0f * resistor_conductance;
            conduction_matrix(terminal1_node, terminal1_node) += resistor_conductance;
            conduction_matrix(terminal2_node, terminal2_node) += resistor_conductance;
        }
    }

    std::cout << conduction_matrix << std::endl;

    // Remove last row & column that corresponds to ground node.
    Eigen::MatrixXf sub_matrix = conduction_matrix.block(0, 0, conduction_matrix.rows() - 1, conduction_matrix.cols() - 1);

    // Set up vector that corresponds to a test current being injected where the source node is located.
    Eigen::VectorXf test_current(sub_matrix.rows());
    test_current.setZero();
    test_current(0) = 1.0f;

    // Solves for voltage vector, where G*V=I. Test current is 1 amp, so corresponding voltage in first position is the net resistance looking in from the source. 
    Eigen::VectorXf result = sub_matrix.colPivHouseholderQr().solve(test_current); //Essentially: sub_matrix.inverse() * test_current;
    std::cout << result << std::endl;

    return result(0);
}

void Simulation::Run()
{
    float source_voltage = 0.0f;
    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<VoltageSource_DC*>(component.get()))
        {
            VoltageSource_DC* voltage_source = static_cast<VoltageSource_DC*>(component.get());
            source_voltage = voltage_source->GetVoltage();
            break;
        }
    }

    float net_resistance = findEquivalentResistance();
    float source_current = source_voltage / net_resistance;

    m_Console.PushMessage("Net resistance of circuit: " + std::to_string(net_resistance) + " Ohms");
    
    data.SourceVoltage = source_voltage;
    data.SourceCurrent = source_current;
    //m_Circuit.LogWires();

    //m_Console.PushMessage(std::to_string(CalculateNetResistance()));
}

void Simulation::logResults() const
{
    std::cout << "Simulation Results:" << std::endl;
    for (const auto& [terminalName, voltage] : m_TerminalVoltages)
    {
        std::cout << "Terminal " << terminalName << ": " << voltage << " V" << std::endl;
    }
}
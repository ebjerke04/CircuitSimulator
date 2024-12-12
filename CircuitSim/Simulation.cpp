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

float Simulation::FindEquivalentResistance() const 
{
    // Form blank conduction matrix, minus one because ground node gets removed
    int wire_count = m_Circuit.GetWires().size();
    Eigen::MatrixXf conduction_matrix(wire_count, wire_count);
    conduction_matrix.setZero();
    
    // Get the positive terminal and negative terminal of voltage supply
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

    // Form an array of wires, first element being source positive node and last being ground node
    std::vector<Wire*> wire_vector;
    Wire* source_node = m_Circuit.GetWiringManager()->GetWireAtTerminal(vs_positive_terminal);
    Wire* ground_node = m_Circuit.GetWiringManager()->GetWireAtTerminal(vs_negative_terminal);
    wire_vector.push_back(source_node);

    for (const std::unique_ptr<Wire>& wire : m_Circuit.GetWires())
    {
        Wire* wire_raw = wire.get();
        auto it = std::find(wire_vector.begin(), wire_vector.end(), wire_raw);
        
        if (it != wire_vector.end())
        {
            continue;
        }
        if (wire_raw == ground_node) continue;
        if (wire_raw == source_node) continue;

        wire_vector.push_back(wire_raw);
    }
    wire_vector.push_back(ground_node);

    for (Wire* wire : wire_vector)
    {
        wire->LogWire();
    }

    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<Resistor*>(component.get()))
        {
            Resistor* resistor = static_cast<Resistor*>(component.get());

            Terminal* terminal1 = resistor->GetTerminals()[0].get();
            Terminal* terminal2 = resistor->GetTerminals()[1].get();
            Wire* terminal1_wire = m_Circuit.GetWiringManager()->GetWireAtTerminal(terminal1);
            Wire* terminal2_wire = m_Circuit.GetWiringManager()->GetWireAtTerminal(terminal2);

            int terminal1_node = 999;
            int terminal2_node = 999;
            for (int i = 0; i < wire_vector.size(); i++)
            {
                Wire* wire = wire_vector[i];
                if (terminal1_wire == wire)
                {
                    terminal1_node = i;
                    break;
                }
            }
            for (int i = 0; i < wire_vector.size(); i++)
            {
                Wire* wire = wire_vector[i];
                if (terminal2_wire == wire)
                {
                    terminal2_node = i;
                    break;
                }
            }

            m_Console.PushMessage(resistor->GetName() + " connects nodes, " + std::to_string(terminal1_node) + " and " + std::to_string(terminal2_node));
            float resistor_conductance = 1.0f / resistor->GetResistance();

            conduction_matrix(terminal1_node, terminal2_node) += -1.0f * resistor_conductance;
            conduction_matrix(terminal2_node, terminal1_node) += -1.0f * resistor_conductance;
            conduction_matrix(terminal1_node, terminal1_node) += resistor_conductance;
            conduction_matrix(terminal2_node, terminal2_node) += resistor_conductance;
        }
    }

    std::cout << conduction_matrix << std::endl;

    Eigen::MatrixXf sub_matrix = conduction_matrix.block(0, 0, conduction_matrix.rows() - 1, conduction_matrix.cols() - 1);
    Eigen::VectorXf test_current(sub_matrix.rows());
    test_current.setZero();
    test_current(0) = 1.0f;

    Eigen::VectorXf result = sub_matrix.inverse() * test_current;
    std::cout << result << std::endl;

    return result(0);
}

void Simulation::Run()
{
    float net_resistance = FindEquivalentResistance();
    m_Console.PushMessage("Net resistance of circuit: " + std::to_string(net_resistance) + " Ohms");
    
    //m_Circuit.LogWires();

    //m_Console.PushMessage(std::to_string(CalculateNetResistance()));
}

void Simulation::LogResults() const
{
    std::cout << "Simulation Results:" << std::endl;
    for (const auto& [terminalName, voltage] : m_TerminalVoltages)
    {
        std::cout << "Terminal " << terminalName << ": " << voltage << " V" << std::endl;
    }
}
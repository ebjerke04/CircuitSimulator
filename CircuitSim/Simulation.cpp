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

std::complex<float> Simulation::findEquivalentImpedance() const //TODO : Comment steps...
{
    int wire_count = m_Circuit.GetWires().size();
    Eigen::MatrixXcf admittance_matrix(wire_count, wire_count);
    admittance_matrix.setZero();

    Terminal* vs_positive_terminal = nullptr;
    Terminal* vs_negative_terminal = nullptr;
    float source_frequency = 0.0f;
    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<VoltageSource_AC*>(component.get())) {
            VoltageSource_AC* voltage_source = static_cast<VoltageSource_AC*>(component.get());
            vs_positive_terminal = voltage_source->GetPositiveRefTerminal().get();
            vs_negative_terminal = voltage_source->GetNegativeRefTerminal().get();
            source_frequency = voltage_source->GetFrequency();
            break;
        }
    }

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

    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        std::complex<float> impedance = std::complex<float>(0.0f, 0.0f);
        if (dynamic_cast<Resistor*>(component.get()))
        {
            Resistor* resistor = static_cast<Resistor*>(component.get());
            impedance = std::complex<float>(resistor->GetResistance(), 0.0f);
        }
        else if (dynamic_cast<Inductor*>(component.get()))
        {
            Inductor* inductor = static_cast<Inductor*>(component.get());
            float inductor_component = 2 * IM_PI * source_frequency * inductor->GetInductanceMicro() * pow(10, -6);
            impedance = std::complex<float>(0.0f, inductor_component);
        }
        else if (dynamic_cast<Capacitor*>(component.get()))
        {
            Capacitor* capacitor = static_cast<Capacitor*>(component.get());
            float capacitor_component = -1.0f / (2 * IM_PI * source_frequency * capacitor->GetCapacitanceMicro() * pow(10, -6));
            impedance = std::complex<float>(0.0f, capacitor_component);
        }
        else
        {
            continue;
        }

        Terminal* terminal1 = component->GetTerminals()[0].get();
        Terminal* terminal2 = component->GetTerminals()[1].get();
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

        m_Console.PushMessage(component->GetName() + " connects nodes, " + std::to_string(terminal1_node) + " and " + std::to_string(terminal2_node));
        std::complex<float> admittance = 1.0f / impedance;

        admittance_matrix(terminal1_node, terminal2_node) += -1.0f * admittance;
        admittance_matrix(terminal2_node, terminal1_node) += -1.0f * admittance;
        admittance_matrix(terminal1_node, terminal1_node) += admittance;
        admittance_matrix(terminal2_node, terminal2_node) += admittance;
    }

    Eigen::MatrixXcf sub_matrix = admittance_matrix.block(0, 0, admittance_matrix.rows() - 1, admittance_matrix.cols() - 1);
    std::cout << "----------- Admittance matrix minus ground node -----------" << std::endl;
    std::cout << sub_matrix << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    Eigen::VectorXcf test_current(sub_matrix.rows());
    test_current.setZero();
    test_current(0) = std::complex(1.0f, 0.0f);

    Eigen::VectorXcf result = sub_matrix.colPivHouseholderQr().solve(test_current);
    std::cout << "----------- Impedance vector -----------" << std::endl;
    std::cout << result << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    return result(0);
}

void Simulation::Run()
{
    m_Circuit.LogWires();

    std::complex<float> net_impedance = findEquivalentImpedance();
    m_Console.PushMessage("Net impedance seen by source: " + std::to_string(net_impedance.real()) + " + j(" + std::to_string(net_impedance.imag()) + ")");

    int data_points = m_DurationMicro / m_TimeStepMicro;

    float source_peak = 0.0f;
    float source_frequency = 0.0f;
    Terminal* voltage_positive = nullptr;
    for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
    {
        if (dynamic_cast<VoltageSource_AC*>(component.get())) {
            VoltageSource_AC* voltage_source = static_cast<VoltageSource_AC*>(component.get());
            source_peak = voltage_source->GetPeakVoltage();
            source_frequency = voltage_source->GetFrequency();
            voltage_positive = voltage_source->GetNegativeRefTerminal().get();
            break;
        }
    }

    std::vector<SimulationDataPoint> source_voltages;
    for (int i = 0; i < data_points; i++)
    {
        float time_micro = i * m_TimeStepMicro;
        float voltage = source_peak * sinf(2 * IM_PI * source_frequency * time_micro * pow(10, -6));
        source_voltages.push_back({ time_micro, voltage });
    }

    Wire* wire = m_Circuit.GetWiringManager()->GetWireAtTerminal(voltage_positive);
    SimulationDataset source_data = { "Source Voltage", source_voltages };
    m_NodeVoltages.insert({ wire, source_data });
}
#include "WiringManager.h"

WiringManager::WiringManager(std::vector<std::unique_ptr<Wire>>& wires) : m_Wires(wires)
{
    
}

void WiringManager::TerminalClicked(const std::shared_ptr<Terminal>& terminal)
{
    std::unique_ptr<Wire> wire_at_terminal = std::move(wireAtTerminal(terminal));
    if (!m_Wiring)
    {
        if (wire_at_terminal != nullptr)
        {
            m_CurrentWire = std::move(wire_at_terminal);
            std::cout << "Wire found at terminal: " << terminal->GetName() << " | wire set to current wire" << std::endl;
        }
        else
        {
            m_CurrentWire = std::make_unique<Wire>();
            m_CurrentWire->AddTerminal(terminal);
            std::cout << "New wire started at terminal: " << terminal->GetName() << std::endl;
        }
        m_Wiring = true;
    }
    else
    {
        if (wire_at_terminal != nullptr)
        {
            for (const std::shared_ptr<Terminal>& clicked_wire_terminal : wire_at_terminal->GetConnectedTerminals())
            {
                m_CurrentWire->AddTerminal(clicked_wire_terminal);
            }
            std::cout << "Wire found at terminal: " << terminal->GetName() << " | its terminals were added to the current wire" << std::endl;
        }
        else
        {
            m_CurrentWire->AddTerminal(terminal);
            std::cout << "Wire ended at terminal: " << terminal->GetName() << std::endl;
        }
        m_Wires.push_back(std::move(m_CurrentWire));
        m_CurrentWire = nullptr;
        m_Wiring = false;
    }
}

std::unique_ptr<Wire> WiringManager::wireAtTerminal(const std::shared_ptr<Terminal>&terminal)
{
    for (std::unique_ptr<Wire>& wire : m_Wires) {
        for (const std::shared_ptr<Terminal>& wire_terminal : wire->GetConnectedTerminals()) {
            if (terminal == wire_terminal) {
                std::unique_ptr<Wire> found_wire = std::move(wire);
                m_Wires.erase(
                    std::remove_if(
                        m_Wires.begin(),
                        m_Wires.end(),
                        [&](const std::unique_ptr<Wire>& wire) {
                            return wire == nullptr;
                        }),
                    m_Wires.end());
                return found_wire;
            }
        }
    }

    return nullptr;
}
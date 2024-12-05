#include "WiringManager.h"

WiringManager::WiringManager(std::vector<std::unique_ptr<Wire>>& wires) : m_Wires(wires)
{
    /*
    std::vector<std::unique_ptr<int>> vec;
    vec.push_back(std::make_unique<int>(10));
    vec.push_back(std::make_unique<int>(20));
    vec.push_back(std::make_unique<int>(30));

    // Display original vector
    std::cout << "Before move: ";
    for (const auto& ptr : vec) {
        std::cout << (ptr ? *ptr : 0) << " ";
    }
    std::cout << std::endl;

    // Move the second unique_ptr out of the vector
    std::unique_ptr<int> extracted = std::move(vec[1]);

    vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::unique_ptr<int>& ptr) { return ptr == nullptr; }), vec.end());

    // Display after move
    std::cout << "After move: ";
    for (const auto& ptr : vec) {
        std::cout << (ptr ? *ptr : 0) << " ";
    }
    std::cout << std::endl;

    // Use the extracted unique_ptr
    std::cout << "Extracted value: " << *extracted << std::endl;
    */
}

void WiringManager::TerminalClicked(const std::shared_ptr<Terminal>& terminal)
{
    if (!m_Wiring)
    {
        std::unique_ptr<Wire> wire_at_terminal = std::move(wireAtTerminal(terminal));
        if (wire_at_terminal != nullptr)
        {
            m_CurrentWire = std::move(wire_at_terminal);
        }
        else
        {
            
        }
    }
    else
    {

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
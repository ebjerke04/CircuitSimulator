#include "Circuit.h"

Circuit::Circuit()
{
	m_WiringManager = std::make_unique<WiringManager>(m_Wires);
}

Circuit::~Circuit()
{

}

const void Circuit::LogWires() const
{
	for (const std::unique_ptr<Wire>& wire : m_Wires)
	{
		std::cout << "Wire connects terminals: " << std::endl;
		for (const std::shared_ptr<Terminal>& wire_terminal : wire->GetConnectedTerminals())
		{
			std::cout << wire_terminal->GetName() << "  ";
		}
		std::cout << std::endl;
	}
}

void Circuit::PushComponent(std::unique_ptr<Component> component)
{
	m_Components.push_back(std::move(component));
}
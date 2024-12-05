#pragma once

#include <memory>
#include "Component.h"

#include "WiringManager.h"

class Circuit
{
public:
	Circuit();
	~Circuit();

	void PushComponent(std::unique_ptr<Component> component);
	void AddWire(std::unique_ptr<Wire> wire) 
	{ 
		std::string wire_description = "Wire connects terminals: ";
		for (const std::shared_ptr<Terminal>& terminal : wire->GetConnectedTerminals())
		{
			wire_description += (terminal->GetName() + ", ");
		}
		std::cout << wire_description << std::endl;
		m_Wires.push_back(std::move(wire)); 
	}
	const void LogWires() const;

	const std::unique_ptr<WiringManager>& GetWiringManager() const { return m_WiringManager; }
	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_Components; }
	const std::vector<std::unique_ptr<Wire>>& GetWires() const { return m_Wires; }
	const int& GetComponentCount() const { return m_Components.size(); }
private:
	std::unique_ptr<WiringManager> m_WiringManager;

	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<std::unique_ptr<Wire>> m_Wires;
};


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


#pragma once

#include <memory>
#include "Component.h"
#include "Wire.h"

class Circuit
{
public:
	Circuit();
	~Circuit();

	void PushComponent(std::unique_ptr<Component> component);
	void AddWire(std::unique_ptr<Wire> wire) { m_Wires.push_back(std::move(wire)); }

	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_Components; }
	const std::vector<std::unique_ptr<Wire>>& GetWires() const { return m_Wires; }
	const int& GetComponentCount() const { return m_Components.size(); }

	// TEMP
	void SetWiring(bool wiring) { m_Wiring = wiring; }
	const bool& IsWiring() const { return m_Wiring; }
	void MakeNewWire(const std::shared_ptr<Terminal>& startTerminal) {
		m_CurrentWire = std::make_unique<Wire>();
		m_CurrentWire->AddTerminal(startTerminal);
		m_Wiring = true;
	}
	void FinishWire(const std::shared_ptr<Terminal>& endTerminal) {
		m_CurrentWire->AddTerminal(endTerminal);
		AddWire(std::move(m_CurrentWire));
		m_CurrentWire = nullptr;
		m_Wiring = false;
	}
private:
	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<std::unique_ptr<Wire>> m_Wires;

	// TEMP
	bool m_Wiring = false;
	std::unique_ptr<Wire> m_CurrentWire = nullptr;
};


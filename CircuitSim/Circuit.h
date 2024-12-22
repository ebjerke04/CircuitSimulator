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

	template<typename T>
	inline std::string findAvailableName() const;
private:
	std::unique_ptr<WiringManager> m_WiringManager;

	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<std::unique_ptr<Wire>> m_Wires;
};

template<typename T>
inline std::string Circuit::findAvailableName() const
{
	bool name_found = false;
	int count = 1;
	std::string component_letter = Component::GetComponentLetter<T>();
	std::string name = "";

	while (!name_found)
	{
		std::string possible_name = component_letter + std::to_string(count);
		bool same_name_found = false;

		for (const std::unique_ptr<Component>& component : GetComponents())
		{
			if (possible_name == component->GetName()) same_name_found = true;
		}

		if (!same_name_found)
		{
			name = possible_name;
			name_found = true;
		}
		count++;
	}

	return name;
}
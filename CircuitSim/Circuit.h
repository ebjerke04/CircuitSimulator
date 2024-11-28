#pragma once

#include <memory>
#include <vector>
#include "Component.h"

class Circuit
{
public:
	Circuit();
	~Circuit();

	void PushComponent(std::unique_ptr<Component> component);

	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_Components; }
	const int& GetComponentCount() const { return m_Components.size(); }
private:
	std::vector<std::unique_ptr<Component>> m_Components;
};


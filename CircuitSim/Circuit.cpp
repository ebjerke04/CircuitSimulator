#include "Circuit.h"

Circuit::Circuit()
{
	m_WiringManager = std::make_unique<WiringManager>(m_Wires);
}

Circuit::~Circuit()
{

}

void Circuit::PushComponent(std::unique_ptr<Component> component)
{
	m_Components.push_back(std::move(component));
}
#include "Circuit.h"

Circuit::Circuit()
{

}

Circuit::~Circuit()
{

}

void Circuit::PushComponent(std::unique_ptr<Component> component)
{
	component->SetID(INT_MAX / (m_Components.size() + 1));
	m_Components.push_back(std::move(component));
}
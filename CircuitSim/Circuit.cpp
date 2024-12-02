#include "Circuit.h"

Circuit::Circuit()
{

}

Circuit::~Circuit()
{

}

void Circuit::PushComponent(std::unique_ptr<Component> component)
{
	m_Components.push_back(std::move(component));
}
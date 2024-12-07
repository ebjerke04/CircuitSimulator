#include "Wire.h"

Wire::Wire() {}

void Wire::AddTerminal(std::shared_ptr<Terminal> terminal)
{
	if (!IsTerminalConnected(terminal)) m_ConnectedTerminals.push_back(terminal);
}

void Wire::LogWire()
{
	for (const std::shared_ptr<Terminal>& connectedTerminal : m_ConnectedTerminals)
	{
		std::cout << connectedTerminal->GetName();
		if (connectedTerminal == m_ConnectedTerminals.back())
		{
			std::cout << std::endl;
			break;
		}
		std::cout << ",";
	}
}

bool Wire::IsTerminalConnected(const std::shared_ptr<Terminal>& terminal) const 
{
	for (const std::shared_ptr<Terminal>& connectedTerminal : m_ConnectedTerminals)
	{
		if (connectedTerminal == terminal) return true;
	}
	return false;
}
#pragma once

#include <iostream>
#include "Wire.h"
#include "Terminal.h"

class WiringManager
{
public:
	WiringManager(std::vector<std::unique_ptr<Wire>>& wires);

	void TerminalClicked(const std::shared_ptr<Terminal>& terminal);
private:
	std::unique_ptr<Wire> wireAtTerminal(const std::shared_ptr<Terminal>& terminal);

	bool m_Wiring = false;
	std::vector<std::unique_ptr<Wire>>& m_Wires;

	std::unique_ptr<Wire> m_CurrentWire;
};


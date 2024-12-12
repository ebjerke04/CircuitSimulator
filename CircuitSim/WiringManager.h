#pragma once

#include <iostream>
#include "Wire.h"

class WiringManager
{
public:
	WiringManager(std::vector<std::unique_ptr<Wire>>& wires);

	void TerminalClicked(const std::shared_ptr<Terminal>& terminal);
	void DrawCurrentWire(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom);

	Wire* GetWireAtTerminal(Terminal* terminal) const;
private:
	std::unique_ptr<Wire> wireAtTerminal(Terminal* terminal);

	bool m_Wiring = false;
	std::vector<std::unique_ptr<Wire>>& m_Wires;

	std::unique_ptr<Wire> m_CurrentWire;
};


#include "Wire.h"

#include <math.h>

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

void Wire::Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom, ImU32 color)
{
    const std::vector<std::shared_ptr<Terminal>>& wire_terminals = GetConnectedTerminals();
    if (wire_terminals.empty()) return;
    std::unordered_set<std::shared_ptr<Terminal>> handled_terminals;

    for (const std::shared_ptr<Terminal>& terminal_1 : wire_terminals)
    {
        ImVec2 terminal_1_pos = Component::GridPosToCanvasPos(terminal_1->GetActualGridPosition(), offset, gridSize, zoom);

        std::shared_ptr<Terminal> closest = nullptr;
        float smallest_distance = std::numeric_limits<float>::max();

        for (const std::shared_ptr<Terminal>& terminal_2 : wire_terminals)
        {
            if (terminal_1 == terminal_2 || handled_terminals.find(terminal_2) != handled_terminals.end())
                continue;

            ImVec2 terminal_2_pos = Component::GridPosToCanvasPos(
                terminal_2->GetActualGridPosition(), offset, gridSize, zoom);

            float distance_between = sqrtf(powf((terminal_2_pos.x - terminal_1_pos.x), 2.0f) + powf((terminal_2_pos.y - terminal_1_pos.y), 2));
            if (distance_between < smallest_distance)
            {
                closest = terminal_2;
                smallest_distance = distance_between;
            }
        }

        if (closest)
        {
            ImVec2 closest_pos = Component::GridPosToCanvasPos(closest->GetActualGridPosition(), offset, gridSize, zoom);
            drawList->AddLine(terminal_1_pos, closest_pos, color, 2.0f);
            handled_terminals.insert(terminal_1);
        }
    }
}
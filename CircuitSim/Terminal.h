#pragma once

#include <string>
#include "imgui/imgui.h"

class Terminal
{
public:
	Terminal(ImVec2 gridPosition);

	void Draw(ImDrawList* drawList, const ImVec2& posOnCanvas, const float& radius);
	bool IsHovered();

	const ImVec2& GetGridPosition() const { return m_GridPosition; }
private:
	std::string m_Name;
	ImVec2 m_GridPosition;
};


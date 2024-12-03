#pragma once

#include <string>
#include "imgui/imgui.h"
#include "math_helper.h"

class Terminal
{
public:
	Terminal(ImVec2 deltaGridPosition, const std::string& name);

	void Draw(ImDrawList* drawList, const ImVec2& posOnCanvas, const float& radius);
	void UpdateDeltaGrid(const ImVec2& deltaGrid) { m_DeltaGridPosition = deltaGrid; }

	bool IsHovered(const ImVec2& componentGridPosition, const ImVec2& offset, const float& gridSize, const float& zoom);

	const ImVec2& GetOriginalDeltaGridPosition() const { return m_OriginalDeltaGridPosition; }
	const ImVec2& GetDeltaGridPosition() const { return m_DeltaGridPosition; }
	const std::string& GetName() const { return m_Name; }

private:
	std::string m_Name;
	const ImVec2 m_OriginalDeltaGridPosition;
	ImVec2 m_DeltaGridPosition;
};


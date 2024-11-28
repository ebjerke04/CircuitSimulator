#pragma once

#include "imgui/imgui.h"
#include <iostream>

class Component
{
public:
	Component(ImVec2 gridPosition);

	virtual void Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom) = 0;
protected:
	ImVec2 m_GridPosition;

	ImVec2 GridPosToCanvasPos(const ImVec2& offset, const float& gridSize, const float& zoom);
};


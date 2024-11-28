#pragma once

#include "imgui/imgui.h"
#include <iostream>

class Component
{
public:
	Component(ImVec2 gridPosition);

	virtual void Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom) = 0;
	void SetID(int id) { m_ComponentID = id; }
protected:
	ImVec2 m_GridPosition;
	int m_ComponentID = 0;

	ImVec2 GridPosToCanvasPos(const ImVec2& offset, const float& gridSize, const float& zoom);
};


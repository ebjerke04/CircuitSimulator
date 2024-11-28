#pragma once

#include <iostream>
#include <vector>

#include "Terminal.h"

class Component
{
public:
	Component(ImVec2 gridPosition);

	virtual void Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom) = 0;
	void SetID(int id) { m_ComponentID = id; }

	const std::vector<std::unique_ptr<Terminal>>& GetTerminals() const{ return m_Terminals; }
protected:
	ImVec2 GridPosToCanvasPos(const ImVec2& gridPos, const ImVec2& offset, const float& gridSize, const float& zoom);

	ImVec2 m_GridPosition;
	int m_ComponentID = 0;

	std::vector<std::unique_ptr<Terminal>> m_Terminals;
};


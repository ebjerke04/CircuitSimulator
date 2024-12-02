#pragma once

#include <iostream>
#include <vector>

#include "Terminal.h"

class Component
{
public:
	Component(ImVec2 gridPosition, const std::string& name);

	virtual void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode) = 0;
	virtual void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) = 0;

	const std::vector<std::unique_ptr<Terminal>>& GetTerminals() const{ return m_Terminals; }
	static ImVec2 GridPosToCanvasPos(const ImVec2& gridPos, const ImVec2& offset, float gridSize, float zoom);
protected:
	virtual void DrawEditMenu() = 0;

	bool isHovered(const ImVec2& offset, float gridSize, float zoom);

	ImVec2 m_GridPosition;
	int m_Rotation = 0;
	bool m_DrawEditMenu = false;
	std::string m_Name;

	std::vector<std::unique_ptr<Terminal>> m_Terminals;
};


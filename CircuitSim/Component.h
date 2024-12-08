#pragma once

#include <iostream>
#include <vector>
#include "imgui/imgui.h"

class Terminal;
class Circuit;

class Component
{
public:
	Component(const ImVec2& gridPosition, const std::string& name, Circuit& circuit);

	virtual void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode) = 0;
	virtual void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) = 0;

	const std::vector<std::shared_ptr<Terminal>>& GetTerminals() const { return m_Terminals; }
	const std::string& GetName() const { return m_Name; }
	const bool& IsMoving() const { return m_Moving; }
	static ImVec2 GridPosToCanvasPos(const ImVec2& gridPos, const ImVec2& offset, float gridSize, float zoom);
	const ImVec2& GetGridPosition() const { return m_GridPosition; }
protected:
	virtual void drawEditMenu() = 0;

	void handleMoving(const ImVec2& offset, float gridSize, float zoom, int opMode);
	bool isHovered(const ImVec2& offset, float gridSize, float zoom);

	ImVec2 m_GridPosition;
	int m_Rotation = 0;
	bool m_DrawEditMenu = false;
	bool m_Moving = false;
	std::string m_Name;


	std::vector<std::shared_ptr<Terminal>> m_Terminals;

	Circuit& m_Circuit;
};


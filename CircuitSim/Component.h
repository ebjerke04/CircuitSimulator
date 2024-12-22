#pragma once

#include <iostream>
#include <vector>
#include "imgui/imgui.h"

class Circuit;
class Terminal;

class VoltageSource_AC;
class VoltageSource_DC;
class Resistor;
class Capacitor;
class Inductor;

class Component
{
public:
	static ImVec2 GridPosToCanvasPos(const ImVec2& gridPos, const ImVec2& offset, float gridSize, float zoom);
	template<typename T>
	static std::string GetComponentLetter();

	Component(const ImVec2& gridPosition, Circuit& circuit);

	virtual void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode) = 0;
	virtual void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) = 0;

	const std::vector<std::shared_ptr<Terminal>>& GetTerminals() const { return m_Terminals; }
	const std::string& GetName() const { return m_Name; }
	const bool& IsMoving() const { return m_Moving; }
	const ImVec2& GetGridPosition() const { return m_GridPosition; }
protected:
	virtual void drawEditMenu() = 0;

	void handleMoving(const ImVec2& offset, float gridSize, float zoom, int opMode);
	bool isHovered(const ImVec2& offset, float gridSize, float zoom);
	void setName(const std::string& name) { m_Name = name; }

	ImVec2 m_GridPosition;
	int m_Rotation = 0;
	bool m_DrawEditMenu = false;
	bool m_Moving = false;
	std::string m_Name;

	std::vector<std::shared_ptr<Terminal>> m_Terminals;
	Circuit& m_Circuit;
};

template<typename T>
inline std::string Component::GetComponentLetter()
{
	if (std::is_same<T, VoltageSource_AC>::value || std::is_same<T, VoltageSource_DC>::value)
	{
		return "V";
	}
	else if (std::is_same<T, Resistor>::value)
	{
		return "R";
	}
	else if (std::is_same<T, Inductor>::value)
	{
		return "L";
	}
	else if (std::is_same<T, Capacitor>::value)
	{
		return "C";
	}
	return "err";
}

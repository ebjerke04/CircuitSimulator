#include "Terminal.h"

#include "Component.h"

Terminal::Terminal(ImVec2 deltaGridPosition, const std::string& name, Component* parent) : m_OriginalDeltaGridPosition(deltaGridPosition), m_ParentComponent(parent)
{
	m_DeltaGridPosition = deltaGridPosition;
	m_Name = name;
}

void Terminal::Draw(ImDrawList* drawList, const ImVec2& posOnCanvas, const float& radius)
{
	drawList->AddCircleFilled(posOnCanvas, radius, IM_COL32(255, 0, 0, 255));
}

bool Terminal::IsHovered(const ImVec2& componentGridPosition, const ImVec2& offset, const float& gridSize, const float& zoom)
{
	ImVec2 terminal_canvas_pos = Component::GridPosToCanvasPos(Vec2Plus(componentGridPosition, m_DeltaGridPosition), offset, gridSize, zoom);

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 mouse_pos = ImGui::GetMousePos();

	if (mouse_pos.x <= terminal_canvas_pos.x + 5.0f && mouse_pos.x >= terminal_canvas_pos.x - 5.0f &&
		mouse_pos.y <= terminal_canvas_pos.y + 5.0f && mouse_pos.y >= terminal_canvas_pos.y - 5.0f) return true;

	return false;
}
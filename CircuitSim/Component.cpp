#include "Component.h"

Component::Component(ImVec2 gridPosition, const std::string& name)
{
	m_GridPosition = gridPosition;
	m_Name = name;
}

ImVec2 Component::GridPosToCanvasPos(const ImVec2& gridPos, const ImVec2& offset, const float& gridSize, const float& zoom)
{
	int min_grid_x = (offset.x > 0) ? floor(offset.x / gridSize / zoom) : ceil(offset.x / gridSize / zoom);
	int min_grid_y = (offset.y > 0) ? floor(offset.y / gridSize / zoom) : ceil(offset.y / gridSize / zoom);

	float offset_remainder_x = fmodf(offset.x, gridSize * zoom);
	float offset_remainder_y = fmodf(offset.y, gridSize * zoom);

	return ImVec2(offset_remainder_x + (gridPos.x + min_grid_x) * gridSize * zoom, offset_remainder_y + (gridPos.y + min_grid_y) * gridSize * zoom);
}

bool Component::isHovered(const ImVec2& offset, const float& gridSize, const float& zoom)
{
	ImVec2 component_canvas_pos = GridPosToCanvasPos(m_GridPosition, offset, gridSize, zoom);

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 mouse_pos = ImGui::GetMousePos();

	if (mouse_pos.x <= component_canvas_pos.x + gridSize * 2.0f * zoom && mouse_pos.x >= component_canvas_pos.x - gridSize * 2.0f * zoom &&
		mouse_pos.y <= component_canvas_pos.y + gridSize * 2.0f * zoom && mouse_pos.y >= component_canvas_pos.y - gridSize * 2.0f * zoom) return true;

	return false;
}

#include "Component.h"

Component::Component(ImVec2 gridPosition)
{
	m_GridPosition = gridPosition;
}

ImVec2 Component::GridPosToCanvasPos(const ImVec2& offset, const float& gridSize, const float& zoom)
{
	int min_grid_x = (offset.x > 0) ? floor(offset.x / gridSize / zoom) : ceil(offset.x / gridSize / zoom);
	int min_grid_y = (offset.y > 0) ? floor(offset.y / gridSize / zoom) : ceil(offset.y / gridSize / zoom);

	float offset_remainder_x = fmodf(offset.x, gridSize * zoom);
	float offset_remainder_y = fmodf(offset.y, gridSize * zoom);

	return ImVec2(offset_remainder_x + (m_GridPosition.x + min_grid_x) * gridSize * zoom, offset_remainder_y + (m_GridPosition.y + min_grid_y) * gridSize * zoom);
}

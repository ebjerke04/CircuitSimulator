#include "VoltageSource.h"

VoltageSource::VoltageSource(ImVec2 gridPosition) : Component(gridPosition)
{
	
}

void VoltageSource::Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom)
{
	ImVec2 canvas_pos = GridPosToCanvasPos(offset, gridSize, zoom);

	drawList->AddCircle(canvas_pos, gridSize * zoom * 2.0f, IM_COL32(0, 0, 0, 255), 0, 2.0f);

	// draw positive marking...
	drawList->AddLine(ImVec2(canvas_pos.x, canvas_pos.y - gridSize * zoom * 1.5f), ImVec2(canvas_pos.x, canvas_pos.y - gridSize * zoom * 0.5f), IM_COL32(0, 0, 0, 255), 2.0f);
	drawList->AddLine(ImVec2(canvas_pos.x - gridSize * zoom * 0.5f, canvas_pos.y - gridSize * zoom), ImVec2(canvas_pos.x + gridSize * zoom * 0.5f, canvas_pos.y - gridSize * zoom), IM_COL32(0, 0, 0, 255), 2.0f);

	// draw negative marking...
	drawList->AddLine(ImVec2(canvas_pos.x - gridSize * zoom * 0.5f, canvas_pos.y + gridSize * zoom), ImVec2(canvas_pos.x + gridSize * zoom * 0.5f, canvas_pos.y + gridSize * zoom), IM_COL32(0, 0, 0, 255), 2.0f);

	// draw terminals...
	drawList->AddLine(ImVec2(canvas_pos.x, canvas_pos.y - gridSize * zoom * 2.0f), ImVec2(canvas_pos.x, canvas_pos.y - gridSize * zoom * 3.0f), IM_COL32(0, 0, 0, 255), 2.0f);
	drawList->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + gridSize * zoom * 2.0f), ImVec2(canvas_pos.x, canvas_pos.y + gridSize * zoom * 3.0f), IM_COL32(0, 0, 0, 255), 2.0f);
}
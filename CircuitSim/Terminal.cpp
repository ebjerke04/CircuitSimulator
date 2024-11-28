#include "Terminal.h"

Terminal::Terminal(ImVec2 gridPosition)
{
	m_GridPosition = gridPosition;
}

void Terminal::Draw(ImDrawList* drawList, const ImVec2& posOnCanvas, const float& radius)
{
	drawList->AddCircleFilled(posOnCanvas, radius, IM_COL32(255, 0, 0, 255));
}

bool Terminal::IsHovered()
{
	return true;
}
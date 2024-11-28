#include "VoltageSource_DC.h"

VoltageSource_DC::VoltageSource_DC(ImVec2 gridPosition) : Component(gridPosition)
{
	m_Terminals.push_back(std::make_unique<Terminal>(ImVec2(gridPosition.x, gridPosition.y - 3)));
	m_Terminals.push_back(std::make_unique<Terminal>(ImVec2(gridPosition.x, gridPosition.y + 3)));
}

void VoltageSource_DC::Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom)
{
	ImVec2 pos_on_canvas = GridPosToCanvasPos(m_GridPosition, offset, gridSize, zoom);

	drawList->AddCircle(pos_on_canvas, gridSize * zoom * 2.0f, IM_COL32(0, 0, 0, 255), 0, 2.0f);

	// draw positive marking...
	drawList->AddLine(ImVec2(pos_on_canvas.x, pos_on_canvas.y - gridSize * zoom * 1.5f), ImVec2(pos_on_canvas.x, pos_on_canvas.y - gridSize * zoom * 0.5f), IM_COL32(0, 0, 0, 255), 2.0f);
	drawList->AddLine(ImVec2(pos_on_canvas.x - gridSize * zoom * 0.5f, pos_on_canvas.y - gridSize * zoom), ImVec2(pos_on_canvas.x + gridSize * zoom * 0.5f, pos_on_canvas.y - gridSize * zoom), IM_COL32(0, 0, 0, 255), 2.0f);

	// draw negative marking...
	drawList->AddLine(ImVec2(pos_on_canvas.x - gridSize * zoom * 0.5f, pos_on_canvas.y + gridSize * zoom), ImVec2(pos_on_canvas.x + gridSize * zoom * 0.5f, pos_on_canvas.y + gridSize * zoom), IM_COL32(0, 0, 0, 255), 2.0f);

	// draw terminals...
	drawList->AddLine(ImVec2(pos_on_canvas.x, pos_on_canvas.y - gridSize * zoom * 2.0f), ImVec2(pos_on_canvas.x, pos_on_canvas.y - gridSize * zoom * 3.0f), IM_COL32(0, 0, 0, 255), 2.0f);
	drawList->AddLine(ImVec2(pos_on_canvas.x, pos_on_canvas.y + gridSize * zoom * 2.0f), ImVec2(pos_on_canvas.x, pos_on_canvas.y + gridSize * zoom * 3.0f), IM_COL32(0, 0, 0, 255), 2.0f);

	ImVec2 window_pos = ImGui::GetWindowPos(); // Get the top-left corner of the window
	ImVec2 voltageSourcePosInLocalWindowSpace = ImVec2(
		pos_on_canvas.x - window_pos.x - (gridSize * zoom * 2.0f),
		pos_on_canvas.y - window_pos.y - (gridSize * zoom * 2.0f)
	);

	for (const std::unique_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->IsHovered())
		{
			ImVec2 grid_pos_difference = ImVec2(m_GridPosition.x - terminal->GetGridPosition().x, m_GridPosition.y - terminal->GetGridPosition().y);
			ImVec2 terminal_pos_on_canvas = GridPosToCanvasPos(ImVec2(m_GridPosition.x + grid_pos_difference.x, m_GridPosition.y + grid_pos_difference.y), offset, gridSize, zoom);
			
			terminal->Draw(drawList, terminal_pos_on_canvas, 5.0f);
		}
	}

	ImGui::SetCursorPos(voltageSourcePosInLocalWindowSpace);
	std::string id = "VoltageSource" + std::to_string(m_ComponentID);
	ImGui::InvisibleButton(id.c_str(), ImVec2(gridSize * zoom * 4.0f, gridSize * zoom * 4.0f));

	id = "VoltageSourceContextMenu" + std::to_string(m_ComponentID);
	if (ImGui::BeginPopupContextItem(id.c_str())) {
		ImGui::Text("Edit Voltage Source");
		ImGui::Separator();

		ImGui::SliderFloat("Voltage", &m_Voltage, 0.0f, 20.0f, "%.1f V");
		ImGui::EndPopup();
	}
}
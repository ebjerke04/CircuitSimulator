#include "VoltageSource_DC.h"
#include "math_helper.h"

VoltageSource_DC::VoltageSource_DC(ImVec2 gridPosition) : Component(gridPosition)
{
	m_Terminals.push_back(std::make_unique<Terminal>(ImVec2(gridPosition.x, gridPosition.y - 3)));
	m_Terminals.push_back(std::make_unique<Terminal>(ImVec2(gridPosition.x, gridPosition.y + 3)));
}

void VoltageSource_DC::HandleInput(const ImVec2& offset, const float& gridSize, const float& zoom, const int& opMode)
{
	ImVec2 pos_on_canvas = GridPosToCanvasPos(m_GridPosition, offset, gridSize, zoom);

	ImVec2 window_pos = ImGui::GetWindowPos(); // Get the top-left corner of the window
	ImVec2 voltageSourcePosInLocalWindowSpace = ImVec2(
		pos_on_canvas.x - window_pos.x - (gridSize * zoom * 2.0f),
		pos_on_canvas.y - window_pos.y - (gridSize * zoom * 2.0f)
	);

	if (isHovered(offset, gridSize, zoom))
	{
		if (ImGui::IsMouseClicked(1))
		{
			if (opMode == OpMode::CONSTRUCT)
			{
				std::cout << "now editing" << std::endl;
			}
			else if (opMode == OpMode::MOVE)
			{
				std::cout << "now moving" << std::endl;
			}
		}
	}

	for (const std::unique_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->IsHovered(offset, gridSize, zoom))
		{
			if (ImGui::IsMouseClicked(0))
			{
				// terminal clicked
			}
		}
	}

	ImVec2 toReset = ImGui::GetCursorPos();
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

	ImGui::SetCursorPos(toReset);
}

void VoltageSource_DC::Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom)
{
	ImVec2 pos_on_canvas = GridPosToCanvasPos(m_GridPosition, offset, gridSize, zoom);

	drawList->AddCircle(pos_on_canvas, gridSize * zoom * 2.0f, IM_COL32(0, 0, 0, 255), 0, 2.0f);

	float half_grid = gridSize * zoom * 0.5f;
	float one_grid = gridSize * zoom;
	float one_and_half_grid = gridSize * zoom * 1.5f;
	float two_grid = gridSize * zoom * 2.0f;
	float three_grid = gridSize * zoom * 3.0f;
	float theta = m_Rotation * (3.141592 / 2.0f); // rotation is 0, 1, 2, or 3

	// Define points relative to the center
	ImVec2 plus1 = Vec2Plus(Rotate(0, -one_and_half_grid, theta), pos_on_canvas);
	ImVec2 plus2 = Vec2Plus(Rotate(0, -half_grid, theta), pos_on_canvas);

	ImVec2 minus1 = Vec2Plus(Rotate(-half_grid, -one_grid, theta), pos_on_canvas);
	ImVec2 minus2 = Vec2Plus(Rotate(half_grid, -one_grid, theta), pos_on_canvas);

	ImVec2 minus3 = Vec2Plus(Rotate(-half_grid, one_grid, theta), pos_on_canvas);
	ImVec2 minus4 = Vec2Plus(Rotate(half_grid, one_grid, theta), pos_on_canvas);

	ImVec2 terminal1 = Vec2Plus(Rotate(0, -two_grid, theta), pos_on_canvas);
	ImVec2 terminal2 = Vec2Plus(Rotate(0, -three_grid, theta), pos_on_canvas);

	ImVec2 terminal3 = Vec2Plus(Rotate(0, two_grid, theta), pos_on_canvas);
	ImVec2 terminal4 = Vec2Plus(Rotate(0, three_grid, theta), pos_on_canvas);

	// Draw lines
	drawList->AddLine(plus1, plus2, IM_COL32(0, 0, 0, 255), 2.0f); // Vertical line
	drawList->AddLine(minus1, minus2, IM_COL32(0, 0, 0, 255), 2.0f); // Upper minus
	drawList->AddLine(minus3, minus4, IM_COL32(0, 0, 0, 255), 2.0f); // Lower minus
	drawList->AddLine(terminal1, terminal2, IM_COL32(0, 0, 0, 255), 2.0f); // Top terminal
	drawList->AddLine(terminal3, terminal4, IM_COL32(0, 0, 0, 255), 2.0f);

	for (const std::unique_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->IsHovered(offset, gridSize, zoom))
		{
			ImVec2 terminal_pos_on_canvas = GridPosToCanvasPos(terminal->GetGridPosition(), offset, gridSize, zoom);
			terminal->Draw(drawList, terminal_pos_on_canvas, 5.0f);
		}
	}
}
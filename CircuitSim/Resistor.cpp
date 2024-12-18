#include "Resistor.h"

Resistor::Resistor(const ImVec2& gridPosition, const std::string& name, Circuit& circuit) : Component(gridPosition, name, circuit)
{
	m_Terminals.push_back(std::make_shared<Terminal>(ImVec2(0, -3), name + "_1", this));
	m_Terminals.push_back(std::make_shared<Terminal>(ImVec2(0,  3), name + "_2", this));
}

void Resistor::HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode)
{
	if (opMode != OpMode::EDIT) m_DrawEditMenu = false;
	if (m_DrawEditMenu) drawEditMenu();

	if (isHovered(offset, gridSize, zoom))
	{
		if (ImGui::IsMouseClicked(1))
		{
			if (opMode == OpMode::EDIT)
			{
				m_DrawEditMenu = true;
			}
		}
	}

	handleMoving(offset, gridSize, zoom, opMode);

	for (const std::shared_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->IsHovered(m_GridPosition, offset, gridSize, zoom))
		{
			if (ImGui::IsMouseClicked(0) && opMode == OpMode::WIRING)
			{
				m_Circuit.GetWiringManager()->TerminalClicked(terminal);
			}
		}
	}
}

void Resistor::Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom)
{
	ImVec2 pos_on_canvas = GridPosToCanvasPos(m_GridPosition, offset, gridSize, zoom);

	float one_grid = gridSize * zoom;
	float two_grid = gridSize * zoom * 2.0f;
	float three_grid = gridSize * zoom * 3.0f;
	float theta = m_Rotation * (3.141592 / 2.0f); // rotation is 0, 1, 2, or 3

	ImVec2 terminal1 = Vec2Plus(Rotate(0, -two_grid, theta), pos_on_canvas);
	ImVec2 terminal2 = Vec2Plus(Rotate(0, -three_grid, theta), pos_on_canvas);

	ImVec2 terminal3 = Vec2Plus(Rotate(0, two_grid, theta), pos_on_canvas);
	ImVec2 terminal4 = Vec2Plus(Rotate(0, three_grid, theta), pos_on_canvas);

	ImVec2 rect1 = Vec2Plus(Rotate(-one_grid, -two_grid, theta), pos_on_canvas);
	ImVec2 rect2 = Vec2Plus(Rotate(one_grid, two_grid, theta), pos_on_canvas);

	drawList->AddRect(rect1, rect2, IM_COL32(0, 0, 0, 255), 0, 0, 2.0f);

	// draw terminals
	drawList->AddLine(terminal1, terminal2, IM_COL32(0, 0, 0, 255), 2.0f);
	drawList->AddLine(terminal3, terminal4, IM_COL32(0, 0, 0, 255), 2.0f);

	for (const std::shared_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->IsHovered(m_GridPosition, offset, gridSize, zoom))
		{
			ImVec2 terminal_pos_on_canvas = GridPosToCanvasPos(Vec2Plus(m_GridPosition, terminal->GetDeltaGridPosition()), offset, gridSize, zoom);
			terminal->Draw(drawList, terminal_pos_on_canvas, 5.0f);
		}
	}
}

void Resistor::drawEditMenu()
{
	std::string window_name = std::string("Resistor (").append(m_Name).append(")");
	ImGui::Begin(window_name.c_str(), &m_DrawEditMenu);

	ImGui::InputFloat("Resistance", &m_Resistance, 0.01f, 0.0f);

	ImGui::End();
}
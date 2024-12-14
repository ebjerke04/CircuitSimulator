#include "VoltageSource_AC.h"

VoltageSource_AC::VoltageSource_AC(const ImVec2& gridPosition, const std::string& name, Circuit& circuit) : Component(gridPosition, name, circuit)
{
	m_Terminals.push_back(std::make_unique<Terminal>(ImVec2(0, -3), "AC_POS", this));
	m_Terminals.push_back(std::make_unique<Terminal>(ImVec2(0, 3), "AC_NEG", this));
}

void VoltageSource_AC::HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode)
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

void VoltageSource_AC::Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom)
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
	ImVec2 curve1 = Vec2Plus(Rotate(-one_grid, 0, theta), pos_on_canvas);
	ImVec2 curve2 = Vec2Plus(Rotate(-half_grid, -half_grid, theta), pos_on_canvas);
	ImVec2 curve3 = Vec2Plus(Rotate(0, 0, theta), pos_on_canvas);
	ImVec2 curve4 = Vec2Plus(Rotate(0, 0, theta), pos_on_canvas);
	ImVec2 curve5 = Vec2Plus(Rotate(half_grid, half_grid, theta), pos_on_canvas);
	ImVec2 curve6 = Vec2Plus(Rotate(one_grid, 0, theta), pos_on_canvas);

	ImVec2 terminal1 = Vec2Plus(Rotate(0, -two_grid, theta), pos_on_canvas);
	ImVec2 terminal2 = Vec2Plus(Rotate(0, -three_grid, theta), pos_on_canvas);

	ImVec2 terminal3 = Vec2Plus(Rotate(0, two_grid, theta), pos_on_canvas);
	ImVec2 terminal4 = Vec2Plus(Rotate(0, three_grid, theta), pos_on_canvas);

	// Draw lines
	drawList->AddBezierQuadratic(curve1, curve2, curve3, IM_COL32(0, 0, 0, 255), 2.0f);
	drawList->AddBezierQuadratic(curve4, curve5, curve6, IM_COL32(0, 0, 0, 255), 2.0f);
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

void VoltageSource_AC::drawEditMenu()
{
	std::string window_name = std::string("AC Voltage Source (").append(m_Name).append(")");
	ImGui::Begin(window_name.c_str(), &m_DrawEditMenu);

	ImGui::SliderFloat("Peak Voltage", &m_PeakVoltage, 0.0f, 20.0f, "%.1f V");

	ImGui::End();
}

const std::shared_ptr<Terminal>& VoltageSource_AC::GetPositiveRefTerminal() const
{
	for (const std::shared_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->GetName() == "AC_POS") return terminal;
	}
}

const std::shared_ptr<Terminal>& VoltageSource_AC::GetNegativeRefTerminal() const
{
	for (const std::shared_ptr<Terminal>& terminal : m_Terminals)
	{
		if (terminal->GetName() == "AC_NEG") return terminal;
	}
}
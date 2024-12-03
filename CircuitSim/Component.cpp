#include "Component.h"
#include "Application.h"

Component::Component(const ImVec2& gridPosition, const std::string& name, const Circuit& circuit) : m_Circuit(circuit)
{
	m_GridPosition = gridPosition;
	m_Name = name;
}

ImVec2 Component::GridPosToCanvasPos(const ImVec2& gridPos, const ImVec2& offset, float gridSize, float zoom)
{
	int min_grid_x = (offset.x > 0) ? floor(offset.x / gridSize / zoom) : ceil(offset.x / gridSize / zoom);
	int min_grid_y = (offset.y > 0) ? floor(offset.y / gridSize / zoom) : ceil(offset.y / gridSize / zoom);

	float offset_remainder_x = fmodf(offset.x, gridSize * zoom);
	float offset_remainder_y = fmodf(offset.y, gridSize * zoom);

	return ImVec2(offset_remainder_x + (gridPos.x + min_grid_x) * gridSize * zoom, offset_remainder_y + (gridPos.y + min_grid_y) * gridSize * zoom);
}

void Component::handleMoving(const ImVec2& offset, float gridSize, float zoom, int opMode)
{
	if (opMode != OpMode::MOVE)
	{
		m_Moving = false;
		return;
	}

	if (ImGui::IsMouseClicked(1))
	{
		if (m_Moving) {
			m_Moving = false;
			return;
		}

		if (isHovered(offset, gridSize, zoom))
		{
			bool component_moving = false;
			for (const std::unique_ptr<Component>& component : m_Circuit.GetComponents())
			{
				if (component->IsMoving())
				{
					component_moving = true;
					break;
				}
			}

			if (!component_moving)
			{
				m_Moving = true;
			}
		}
	}

	if (m_Moving)
	{
		if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R))
		{
			m_Rotation++;
			m_Rotation = (m_Rotation > 3) ? m_Rotation = 0 : m_Rotation;

			float theta = m_Rotation * (3.141592 / 2.0f);
			for (const std::unique_ptr<Terminal>& terminal : m_Terminals)
			{
				ImVec2 original_delta_grid = terminal->GetOriginalDeltaGridPosition();
				ImVec2 updated_delta_grid = Rotate(original_delta_grid.x, original_delta_grid.y, theta);
				updated_delta_grid.x = round(updated_delta_grid.x);
				updated_delta_grid.y = round(updated_delta_grid.y);

				terminal->UpdateDeltaGrid(updated_delta_grid);
			}
		}

		ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
		ImVec2 mouse_pos = ImGui::GetMousePos();
		ImVec2 canvas_mouse_pos = ImVec2(mouse_pos.x - offset.x, mouse_pos.y - offset.y);

		m_GridPosition = ImVec2(round(canvas_mouse_pos.x / (gridSize * zoom)), round(canvas_mouse_pos.y / (gridSize * zoom)));
	}
}

bool Component::isHovered(const ImVec2& offset, float gridSize, float zoom)
{
	ImVec2 component_canvas_pos = GridPosToCanvasPos(m_GridPosition, offset, gridSize, zoom);

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 mouse_pos = ImGui::GetMousePos();

	if (mouse_pos.x <= component_canvas_pos.x + gridSize * 2.0f * zoom && mouse_pos.x >= component_canvas_pos.x - gridSize * 2.0f * zoom &&
		mouse_pos.y <= component_canvas_pos.y + gridSize * 2.0f * zoom && mouse_pos.y >= component_canvas_pos.y - gridSize * 2.0f * zoom) return true;

	return false;
}

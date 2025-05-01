#pragma once

#include "Component.h"
#include "Application.h"

class NameTag : public Component
{
public:
	NameTag(const ImVec2& gridPosition, Circuit& circuit);

	void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode) override;
	void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) override;

	const std::string& GetTagText() const { return m_TagText; }
private:
	void drawEditMenu() override;

	std::string m_TagText;
};
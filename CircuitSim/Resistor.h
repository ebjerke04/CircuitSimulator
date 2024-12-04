#pragma once
#include "Component.h"
#include "math_helper.h"
#include "Application.h"

class Resistor :
    public Component
{
public:
    Resistor(const ImVec2& gridPositon, const std::string& name, Circuit& circuit);

    void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode);
    void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) override;

    const float& GetResistance() const { return m_Resistance; }
private:
    void drawEditMenu();

    float m_Resistance = 1000.0f;
};


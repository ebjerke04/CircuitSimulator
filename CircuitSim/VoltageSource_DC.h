#pragma once
#include "Component.h"
#include "math_helper.h"
#include "Application.h"

class VoltageSource_DC :
    public Component
{
public:
    VoltageSource_DC(const ImVec2& gridPositon, const std::string& name, const Circuit& circuit);

    void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode);
    void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) override;
private:
    void drawEditMenu();

    float m_Voltage = 5.0f;
};


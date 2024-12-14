#pragma once
#include "Component.h"
#include "math_helper.h"
#include "Application.h"
#include "imgui/imgui.h"

class Capacitor :
    public Component
{
public:
    Capacitor(const ImVec2& gridPositon, const std::string& name, Circuit& circuit);

    void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode);
    void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) override;

    const float& GetCapacitanceMicro() const { return m_CapacitanceMicro; }
private:
    void drawEditMenu();

    float m_CapacitanceMicro = 100.0f;
};


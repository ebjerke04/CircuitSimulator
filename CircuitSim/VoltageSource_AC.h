#pragma once
#include "Component.h"
#include "math_helper.h"
#include "Application.h"
#include "imgui/imgui.h"

class VoltageSource_AC :
    public Component
{
public:
    VoltageSource_AC(const ImVec2& gridPositon, const std::string& name, Circuit& circuit);

    void HandleInput(const ImVec2& offset, float gridSize, float zoom, int opMode);
    void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom) override;

    const float& GetPeakVoltage() const { return m_PeakVoltage; }
    const float& GetFrequency() const { return m_Frequency; }
    const std::shared_ptr<Terminal>& GetPositiveRefTerminal() const;
    const std::shared_ptr<Terminal>& GetNegativeRefTerminal() const;
private:
    void drawEditMenu();

    float m_PeakVoltage = 5.0f;
    float m_Frequency = 100000.0f;
};


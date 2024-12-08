#pragma once

#include "Circuit.h"

#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <functional>

#include "Resistor.h"
#include "VoltageSource_DC.h"
#include "VoltageSource_AC.h"

#include "implot/implot.h"

class Simulation
{
public:
    Simulation(const Circuit& circuit);

    void SetSettingsVisible() { m_DrawSettings = true; }
    void DrawSettingsCustomizer();

    void Run(); // Run the simulation
    void LogResults() const; // Log terminal voltages

    void TestPlots()
    {
        ImGui::Begin("Test plot");
        static double xs2[1000], ys2[1000];
        for (int i = 0; i < 1000; ++i) {
            xs2[i] = i * 1 / 19.0f;
            ys2[i] = xs2[i] * xs2[i];
        }
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotLine("My Line Plot", xs2, ys2, 1000);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    float CalculateNetResistance() const;
private:
    const Circuit& m_Circuit;
    std::unordered_map<std::string, float> m_TerminalVoltages;

    bool m_DrawSettings = false;

    // Simulation Settings
    float m_DurationMicro = 1000.0f;
    float m_TimeStepMicro = 5.0f;
};

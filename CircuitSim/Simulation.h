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
#include "Console.h"

#include "implot/implot.h"

#include "Eigen/Dense"

struct DCSimData
{
    float SourceVoltage = 0.0f;
    float SourceCurrent = 0.0f;
};

class Simulation
{
public:
    Simulation(const Circuit& circuit, Console& console);

    void SetSettingsVisible() { m_DrawSettings = true; }
    void DrawSettingsCustomizer();

    void Run(); // Run the simulation

    void TestPlots()
    {
        ImGui::Begin("Simulation Data");

        float data_points = m_DurationMicro / m_TimeStepMicro;
        float* x_data = new float[data_points];
        float* y_data1 = new float[data_points];
        float* y_data2 = new float[data_points];

        for (int i = 0; i < data_points; i++)
        {
            x_data[i]  = i * m_TimeStepMicro;
            y_data1[i] = data.SourceVoltage;
            y_data2[i] = data.SourceCurrent;
        }

        ImPlot::SetNextAxesToFit();
        if (ImPlot::BeginPlot("Voltage")) {
            ImPlot::PlotLine("Source Voltage", x_data, y_data1, data_points);
            ImPlot::EndPlot();
        }

        ImPlot::SetNextAxesToFit();
        if (ImPlot::BeginPlot("Current")) {
            ImPlot::PlotLine("Source Current", x_data, y_data2, data_points);
            ImPlot::EndPlot();
        }

        ImGui::End();

        delete[] x_data;
        delete[] y_data1;
        delete[] y_data2;
    }

private:
    void logResults() const;
    float findEquivalentResistance() const;

    const Circuit& m_Circuit;
    Console& m_Console;
    std::unordered_map<std::string, float> m_TerminalVoltages;

    bool m_DrawSettings = false;

    // Simulation Settings
    float m_DurationMicro = 1000.0f;
    float m_TimeStepMicro = 0.25f;

    DCSimData data = { 0.0f, 0.0f };
};

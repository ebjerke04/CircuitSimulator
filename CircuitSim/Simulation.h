#pragma once

#include "Circuit.h"

#include <iostream>
#include <queue>
#include <map>
#include <unordered_map>
#include <stdexcept>
#include <functional>
#include <complex>

#include "Resistor.h"
#include "VoltageSource_DC.h"
#include "VoltageSource_AC.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "Console.h"

#include "implot/implot.h"

#include "Eigen/Dense"

struct SimulationDataPoint
{
    float TimeMicro;
    float Value;
};

struct SimulationDataset
{
    const char* MeasurementName;
    std::vector<SimulationDataPoint> Data;
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

        float points = m_DurationMicro / m_TimeStepMicro;
        float* x_data = new float[points];
        float* y_data = new float[points];

        for (const auto& pair : m_NodeVoltages)
        {
            for (int i = 0; i < points; i++)
            {
                x_data[i] = pair.second.Data[i].TimeMicro;
                y_data[i] = pair.second.Data[i].Value;
            }
        }

        ImPlot::SetNextAxesToFit();
        if (ImPlot::BeginPlot("Voltage")) {
            ImPlot::PlotLine("Source Voltage", x_data, y_data, points);
            ImPlot::EndPlot();
        }

        //ImPlot::SetNextAxesToFit();
        if (ImPlot::BeginPlot("Current")) {
            //ImPlot::PlotLine("Source Current", x_data, y_data2, data_points);
            ImPlot::EndPlot();
        }

        ImGui::End();

        delete[] x_data;
        delete[] y_data;
    }

private:
    // only works for resistor only circuits connected to VoltageSource_DC.
    // this configuration is set up pretty much as a proof of concept for the resistance solving algorithm.
    // we be made more modular in the future.
    float findEquivalentResistance() const;

    std::complex<float> findEquivalentImpedance() const;

    const Circuit& m_Circuit;
    Console& m_Console;
    std::unordered_map<const Wire*, SimulationDataset> m_NodeVoltages;

    bool m_DrawSettings = false;

    // Simulation Settings
    float m_DurationMicro = 20.0f;
    float m_TimeStepMicro = 0.001;
};

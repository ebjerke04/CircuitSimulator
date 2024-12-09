#ifndef APPLICATION_H
#define APPLICATION_H

#include "VoltageSource_DC.h"
#include "VoltageSource_AC.h"
#include "Resistor.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>

#include "Circuit.h"
#include "Console.h"

#include "implot/implot.h"

enum OpMode
{
    EDIT = 0,
    MOVE = 1,
    WIRING = 2,
    SIMULATE = 3
};

class Simulation;

class Application {
public:
    Application();
    ~Application();

    void Begin();
private:
    static void error_callback(int error, const char* description);

    void pollUserInput();
    void drawMenuBar();
    void drawViewCustomizer();
    void drawCircuitCanvas();
    void drawAndHandleCircuit();

    void handleImGui();
    void cleanup();

    GLFWwindow* m_Window;
    ImFont* m_MainFont;
    OpMode m_OperationMode = EDIT;

    Circuit circuit;
    Console m_Console;
    std::unique_ptr<Simulation> m_Simulation;

    float m_Zoom = 1.0f;
    float m_GridSize = 10.0f;
    float m_xOffset = 0.0f;
    float m_yOffset = 0.0f;

    bool m_DrawViewCustomizer = false;
    bool m_DrawConsole = true;
    ImVec4 m_CanvasColor = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    ImVec4 m_GridColor = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
};

#endif // APPLICATION_H

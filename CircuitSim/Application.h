#ifndef APPLICATION_H
#define APPLICATION_H

#include "VoltageSource_DC.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Circuit.h"

class Application {
public:
    Application();
    ~Application();

    void Begin();
private:
    static void error_callback(int error, const char* description);

    void drawMenuBar();
    void drawViewCustomizer();
    void drawCircuitCanvas();
    void drawAndHandleCircuit();

    void drawImGui();
    void cleanup();

    GLFWwindow* m_Window;
    ImFont* m_MainFont;

    Circuit circuit;

    float m_Zoom = 1.0f;
    float m_GridSize = 10.0f;
    float m_xOffset = 0.0f;
    float m_yOffset = 0.0f;

    bool m_DrawViewCustomizer = false;
    ImVec4 m_CanvasColor = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    ImVec4 m_GridColor = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
};

#endif // APPLICATION_H

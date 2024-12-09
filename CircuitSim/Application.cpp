#include "Application.h"

#include "Simulation.h"

template<typename T>
inline T ImClamp(T v, T mn, T mx) 
{
    return (v < mn) ? mn : (v > mx) ? mx : v;
}

Application::Application() : m_Window(nullptr)
{
    // Set error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit()) 
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Create GLFW window
    m_Window = glfwCreateWindow(1280, 720, "Circuit Simulator", nullptr, nullptr);
    if (!m_Window) 
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    m_MainFont = io.Fonts->AddFontFromFileTTF("resources/font.ttf", 20.0f);

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    circuit.PushComponent(std::make_unique<VoltageSource_DC>(ImVec2(20.0f, 20.0f), "V1", circuit));
    circuit.PushComponent(std::make_unique<Resistor>(ImVec2(25.0f, 16.0f), "R1", circuit));
    circuit.PushComponent(std::make_unique<Resistor>(ImVec2(28.0f, 16.0f), "R2", circuit));
    circuit.PushComponent(std::make_unique<Resistor>(ImVec2(25.0f, 24.0f), "R3", circuit));
    circuit.PushComponent(std::make_unique<Resistor>(ImVec2(28.0f, 24.0f), "R4", circuit));

    m_Simulation = std::make_unique<Simulation>(circuit);
}

Application::~Application() 
{
    cleanup();
}

void Application::Begin() 
{
    while (!glfwWindowShouldClose(m_Window)) 
    {
        glfwPollEvents();

        // Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui content
        handleImGui();
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_Window);
    }
}

void Application::pollUserInput()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl) 
    {
        if (ImGui::IsKeyPressed(ImGuiKey_Equal)) 
        { // Zoom in
            m_Zoom = ImClamp(m_Zoom * 1.1f, 0.7f, 3.5f);
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Minus)) 
        { // Zoom out
            m_Zoom = ImClamp(m_Zoom / 1.1f, 0.7f, 3.5f);
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        if (m_OperationMode == OpMode::MOVE)
        {
            m_OperationMode = OpMode::EDIT;
        }
        else if (m_OperationMode == OpMode::WIRING)
        {
            m_OperationMode = OpMode::EDIT;
        }
    }
}

void Application::drawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                
            }
            if (ImGui::MenuItem("Open"))
            {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Customize View"))
            {
                m_DrawViewCustomizer = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Wire"))
            {
                m_OperationMode = WIRING;
            }
            if (ImGui::MenuItem("Move"))
            {
                m_OperationMode = MOVE;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Components"))
        {
            if (ImGui::BeginMenu("Sources"))
            {
                if (ImGui::MenuItem("DC Voltage Source"))
                {
                    circuit.PushComponent(std::make_unique<VoltageSource_DC>(ImVec2(40.0f, 40.0f), "V2", circuit));
                }
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Passives"))
            {
                if (ImGui::MenuItem("Resistor"))
                {
                    circuit.PushComponent(std::make_unique<Resistor>(ImVec2(35.0f, 35.0f), "R3", circuit));
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Simulate"))
        {
            if (ImGui::MenuItem("Settings"))
            {
                m_Simulation->SetSettingsVisible();
            }
            if (ImGui::MenuItem("Run"))
            {
                m_Simulation->Run();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Application::drawViewCustomizer()
{
    if (m_DrawViewCustomizer)
    {
        ImGui::Begin("View Customizer", &m_DrawViewCustomizer);

        ImGui::Text("Canvas Color:");
        ImGui::ColorEdit4("Canvas", (float*)&m_CanvasColor);
        ImGui::Text("Grid Color:");
        ImGui::ColorEdit4("Grid", (float*)&m_GridColor);

        ImGui::End();
    }
}

void Application::drawCircuitCanvas()
{
    // Get canvas position and size
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // Top-left corner of the canvas
    ImVec2 canvas_size = ImGui::GetContentRegionAvail(); // Size of the canvas
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 mouse_pos = ImGui::GetMousePos();
    ImVec2 canvas_mouse_pos = ImVec2(mouse_pos.x - canvas_pos.x, mouse_pos.y - canvas_pos.y);

    // Draw background
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(m_CanvasColor));

    // Draw info to tell user that editor is in move mode.
    if (m_OperationMode == OpMode::MOVE)
    {
        draw_list->AddText(Vec2Plus(canvas_pos, ImVec2(m_GridSize * m_Zoom, m_GridSize * m_Zoom)), IM_COL32(255, 0, 0, 255), "Move Components On - ESC to Exit");
    }
    else if (m_OperationMode == OpMode::WIRING)
    {
        draw_list->AddText(Vec2Plus(canvas_pos, ImVec2(m_GridSize * m_Zoom, m_GridSize * m_Zoom)), IM_COL32(255, 0, 0, 255), "Wiring Components Mode - ESC to Exit");
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) && m_OperationMode == OpMode::EDIT)
    {
        ImVec2 drag_delta = ImGui::GetIO().MouseDelta;
        m_xOffset += drag_delta.x;
        m_yOffset += drag_delta.y;
        ImGui::ResetMouseDragDelta();
    }

    for (float x = fmodf(m_xOffset, m_GridSize * m_Zoom); x < canvas_size.x; x += m_GridSize * m_Zoom)
    {
        draw_list->AddLine(ImVec2(canvas_pos.x + x, canvas_pos.y), ImVec2(canvas_pos.x + x, canvas_pos.y + canvas_size.y), ImColor(m_GridColor));
    }
    for (float y = fmodf(m_yOffset, m_GridSize * m_Zoom); y < canvas_size.y; y += m_GridSize * m_Zoom)
    {
        draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + y), ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + y), ImColor(m_GridColor));
    }
}

void Application::drawAndHandleCircuit()
{
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // Top-left corner of the canvas
    ImVec2 canvas_size = ImGui::GetContentRegionAvail(); // Size of the canvas
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (const std::unique_ptr<Component>& component : circuit.GetComponents())
    {
        component->Draw(draw_list, ImVec2(canvas_pos.x + m_xOffset, canvas_pos.y + m_yOffset), m_GridSize, m_Zoom);
        component->HandleInput(ImVec2(canvas_pos.x + m_xOffset, canvas_pos.y + m_yOffset), m_GridSize, m_Zoom, m_OperationMode);
    }

    for (const std::unique_ptr<Wire>& wire : circuit.GetWires())
    {
        wire->Draw(draw_list, ImVec2(canvas_pos.x + m_xOffset, canvas_pos.y + m_yOffset), m_GridSize, m_Zoom);
    }
}

void Application::handleImGui() 
{
    pollUserInput();
    drawMenuBar();
    drawViewCustomizer();

    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));

    ImGui::Begin("Canvas", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    drawCircuitCanvas();
    drawAndHandleCircuit();

    ImGui::End();

    m_Simulation->DrawSettingsCustomizer();
    m_Simulation->TestPlots();
}

void Application::cleanup() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    glfwTerminate();
}

void Application::error_callback(int error, const char* description) 
{
    std::cerr << "Error: " << description << std::endl;
}
    
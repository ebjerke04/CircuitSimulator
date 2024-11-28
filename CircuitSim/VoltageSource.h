#pragma once
#include "Component.h"
#include "Application.h"

class VoltageSource :
    public Component
{
public:
    VoltageSource(ImVec2 gridPositon);

    void Draw(ImDrawList* drawList, const ImVec2& offset, const float& gridSize, const float& zoom) override;
};


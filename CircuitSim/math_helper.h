#pragma once

#include "imgui/imgui.h"

ImVec2 Rotate(const float& x, const float& y, const float& theta) {
	return ImVec2(
		x * cos(theta) - y * sin(theta),
		x * sin(theta) + y * cos(theta)
	);
}

ImVec2 Vec2Plus(const ImVec2& add1, const ImVec2& add2) {
	return ImVec2(add1.x + add2.x, add1.y + add2.y);
}
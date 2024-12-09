#pragma once

#include <vector>
#include <string>

#include "imgui.h"

class Console
{
public:
	Console();

	void Clear() { m_Messages.clear(); }
	void PushMessage(const std::string& message);
	void Draw(bool* consoleOpen);
private:
	std::vector<std::string> m_Messages;
	bool m_ScrollToBottom = false;
};


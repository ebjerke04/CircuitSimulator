#include "Console.h"

Console::Console() {}

void Console::PushMessage(const std::string& message)
{
	m_Messages.emplace_back(message);
	m_ScrollToBottom = true;
}

void Console::Draw(bool* consoleOpen)
{
	ImGui::Begin("Console", consoleOpen);

	if (ImGui::Button("Clear"))
	{
		Clear();
	}
	ImGui::SameLine();
	if (ImGui::Button("Copy"))
	{
		ImGui::LogToClipboard();
		for (const std::string& message : m_Messages)
		{
			ImGui::LogText("%s\n", message.c_str());
		}
		ImGui::LogFinish();
	}

	ImGui::Separator();

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (const std::string& message : m_Messages)
	{
		ImGui::TextUnformatted(message.c_str());
	}
	if (m_ScrollToBottom)
	{
		ImGui::SetScrollHereY(1.0f);
		m_ScrollToBottom = false;
	}
	ImGui::EndChild();

	ImGui::End();
}
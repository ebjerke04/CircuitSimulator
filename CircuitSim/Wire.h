#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "Terminal.h"
#include "Component.h"

class Wire
{
public:
	Wire();

	void AddTerminal(std::shared_ptr<Terminal> terminal);
	void LogWire();
	bool IsTerminalConnected(const std::shared_ptr<Terminal>& terminal) const;
	void Draw(ImDrawList* drawList, const ImVec2& offset, float gridSize, float zoom);

	const std::vector<std::shared_ptr<Terminal>>& GetConnectedTerminals() const { return m_ConnectedTerminals; }
private:
	std::vector<std::shared_ptr<Terminal>> m_ConnectedTerminals;
};


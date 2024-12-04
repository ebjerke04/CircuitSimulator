#pragma once

#include <vector>
#include <memory>
#include "Terminal.h"

class Wire
{
public:
	Wire();

	void AddTerminal(std::shared_ptr<Terminal> terminal);
	bool IsTerminalConnected(const std::shared_ptr<Terminal>& terminal) const;

	const std::vector<std::shared_ptr<Terminal>>& GetConnectedTerminals() const { return m_ConnectedTerminals; }
private:
	std::vector<std::shared_ptr<Terminal>> m_ConnectedTerminals;
};


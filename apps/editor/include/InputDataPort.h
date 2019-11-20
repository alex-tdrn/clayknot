#pragma once
#include "InputPort.h"

namespace rshp::base
{
	class DataPort;
}

class InputDataPort final : public InputPort
{
private:
	rshp::base::DataPort* port;
	int dataRequests = 0;
	bool dataRequested = false;

public:
	InputDataPort() = default;
	InputDataPort(rshp::base::DataPort* port);
	InputDataPort(InputDataPort&&) = default;
	InputDataPort(InputDataPort const&) = delete;
	InputDataPort& operator=(InputDataPort&&) = default;
	InputDataPort& operator=(InputDataPort const&) = delete;
	~InputDataPort() = default;

public:
	void draw() final override;
	ImVec2 calculateSize() const final override;
	void drawLink();

};
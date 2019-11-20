#pragma once
#include "OutputPort.h"
#include "OutputDataPortDelegate.h"

#include <memory>

namespace rshp::base
{
	class DataPort;
}
class OutputDataPortDelegate;

class OutputDataPort final : public OutputPort
{
private:
	rshp::base::DataPort* port;
	std::unique_ptr<OutputDataPortDelegate> delegate;

public:
	OutputDataPort() = default;
	OutputDataPort(rshp::base::DataPort* port);
	OutputDataPort(OutputDataPort&&) = default;
	OutputDataPort(OutputDataPort const&) = delete;
	OutputDataPort& operator=(OutputDataPort&&) = default;
	OutputDataPort& operator=(OutputDataPort const&) = delete;
	~OutputDataPort() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;

};
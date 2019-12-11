#pragma once

#include <memory>
#include <string>

namespace rsp
{
class DataPort;
}

namespace rsp::gui
{
class OutputDataPortDelegate
{
public:
	static std::unique_ptr<OutputDataPortDelegate> create(rsp::DataPort* port, std::string name);

	virtual void draw() const = 0;
};

} // namespace rsp::gui
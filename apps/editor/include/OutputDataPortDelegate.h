#pragma once
#include <memory>

namespace rshp::base
{
	class DataPort;
}

class OutputDataPortDelegate
{
public:
	static std::unique_ptr<OutputDataPortDelegate> create(rshp::base::DataPort* port);

	virtual void draw() const = 0;
};
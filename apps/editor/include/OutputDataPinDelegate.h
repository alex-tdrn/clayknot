#pragma once
#include <memory>

namespace rshp::base
{
	class AbstractDataPort;
}

class OutputDataPinDelegate
{
public:
	static std::unique_ptr<OutputDataPinDelegate> create(rshp::base::AbstractDataPort* port);

	virtual void draw() const = 0;
};
#pragma once
#include "rshp/gui/nodes/OutputPort.h"
#include "rshp/gui/nodes/OutputDataPortDelegate.h"

#include <memory>

namespace rshp::base
{
	class DataPort;
}

namespace rshp::gui
{

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
}
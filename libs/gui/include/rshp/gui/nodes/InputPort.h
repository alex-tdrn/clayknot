#pragma once
#include "rshp/gui/nodes/AbstractPort.hpp"

namespace rshp::gui
{

	class OutputPort;

	class InputPort : public AbstractPort
	{
	protected:
		OutputPort* connection = nullptr;
		ax::NodeEditor::LinkId linkID = -1;

	public:
		InputPort() = default;
		InputPort(rshp::base::Port* port);
		InputPort(InputPort&&) = default;
		InputPort(InputPort const&) = delete;
		InputPort& operator=(InputPort&&) = default;
		InputPort& operator=(InputPort const&) = delete;
		virtual ~InputPort() = default;

	protected:
		ImVec2 calculateAnchorPosition() const override;

	public:
		virtual void drawLink() = 0;
		bool canConnect(AbstractPort* outputPort) final override;
		void connect(AbstractPort* outputPort) final override;

	};

}
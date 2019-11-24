#pragma once
#include "rshp/gui/nodes/InputPort.h"

namespace rshp::base
{
	class InputEventPort;
}

namespace rshp::gui
{

	class InputEventPort final : public InputPort
	{
	private:
		rshp::base::InputEventPort* port;
		int triggerCount = 0;
		bool justTriggered = false;

	public:
		InputEventPort() = default;
		InputEventPort(rshp::base::InputEventPort* port);
		InputEventPort(InputEventPort&&) = default;
		InputEventPort(InputEventPort const&) = delete;
		InputEventPort& operator=(InputEventPort&&) = default;
		InputEventPort& operator=(InputEventPort const&) = delete;
		~InputEventPort() = default;

	public:
		void draw() final override;
		ImVec2 calculateSize() const final override;
		void drawLink();

	};

}
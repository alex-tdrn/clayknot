#pragma once

#include "rsp/base/node/Node.hpp"

#include <array>

namespace rsp::gui
{
class FrameControllerNode final : public rsp::Node<FrameControllerNode, rsp::InputList<>, rsp::OutputList<>>
{
public:
	struct OutputEvents
	{
		enum
		{
			NewFrame = AbstractNode::OutputEvents::UserEvents
		};
	};

public:
	static inline std::string const name = registerNode<FrameControllerNode>("Frame Controller Node");

protected:
	void registerOutputEvents() override;

public:
	void update() override;
	void newFrame();
};
} // namespace rsp::gui
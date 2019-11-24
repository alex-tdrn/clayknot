#pragma once

#include "rshp/base/node/Node.hpp"

#include <array>

namespace rshp::gui
{
class FrameControllerNode final
	: public rshp::base::Node<FrameControllerNode, rshp::base::InputList<>, rshp::base::OutputList<>>
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
} // namespace rshp::gui
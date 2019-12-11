#pragma once

#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/node/Node.hpp"

#include <array>

namespace rsp::nodes
{
class GrayscaleColorNode
	: public rsp::Node<GrayscaleColorNode, rsp::InputList<rsp::ColorRGB>, rsp::OutputList<rsp::ColorRGB>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Color"};
		enum
		{
			Color
		};
	};

	struct OutputPorts
	{
		static inline std::array names = {"Color"};
		enum
		{
			Color
		};
	};

public:
	static inline std::string const name = registerNode<GrayscaleColorNode>("Grayscale Color Node");

public:
	void update() override;
};

} // namespace rsp::nodes
#pragma once

#include "rshp/base/ColorRGB.hpp"
#include "rshp/base/node/Node.hpp"

#include <array>

namespace rshp::nodes
{
class GrayscaleColorNode
	: public rshp::base::Node<GrayscaleColorNode, rshp::base::InputList<rshp::base::ColorRGB>,
		  rshp::base::OutputList<rshp::base::ColorRGB>>
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

} // namespace rshp::nodes
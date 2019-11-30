#pragma once

#include "rshp/base/ColorRGB.hpp"
#include "rshp/base/node/Node.hpp"

#include <array>

namespace rshp::nodes
{
class DecomposeColor
	: public rshp::base::Node<DecomposeColor, rshp::base::InputList<rshp::base::ColorRGB>,
		  rshp::base::OutputList<float, float, float>>
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
		static inline std::array names = {"R", "G", "B"};
		enum
		{
			R,
			G,
			B
		};
	};

public:
	static inline std::string const name = registerNode<DecomposeColor>("Decompose Colors");

public:
	void update() override;
};

} // namespace rshp::nodes
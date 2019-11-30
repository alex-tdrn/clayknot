#pragma once

#include "rshp/base/ColorRGB.hpp"
#include "rshp/base/node/Node.hpp"

#include <array>

namespace rshp::nodes
{
class MixColors
	: public rshp::base::Node<MixColors, rshp::base::InputList<float, rshp::base::ColorRGB, rshp::base::ColorRGB>,
		  rshp::base::OutputList<rshp::base::ColorRGB>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Factor", "Color A", "Color B"};
		enum
		{
			Factor,
			ColorA,
			ColorB
		};
	};

	struct OutputPorts
	{
		static inline std::array names = {"Mixed Color"};
		enum
		{
			MixedColor
		};
	};

public:
	static inline std::string const name = registerNode<MixColors>("Mix Colors");

public:
	void update() override;
};

} // namespace rshp::nodes
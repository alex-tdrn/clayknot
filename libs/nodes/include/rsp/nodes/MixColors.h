#pragma once

#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/node/Node.hpp"

#include <array>

namespace rsp::nodes
{
class MixColors
	: public rsp::Node<MixColors, rsp::InputList<float, rsp::ColorRGB, rsp::ColorRGB>, rsp::OutputList<rsp::ColorRGB>>
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

} // namespace rsp::nodes
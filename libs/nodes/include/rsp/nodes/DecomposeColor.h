#pragma once

#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/node/Node.hpp"

#include <array>

namespace rsp::nodes
{
class DecomposeColor
	: public rsp::Node<DecomposeColor, rsp::InputList<rsp::ColorRGB>, rsp::OutputList<float, float, float>>
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

} // namespace rsp::nodes
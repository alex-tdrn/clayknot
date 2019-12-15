#pragma once

#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/node/Node.hpp"

#include <array>

namespace rsp::nodes
{
class ValueToColor : public rsp::Node<ValueToColor, rsp::InputList<float>, rsp::OutputList<rsp::ColorRGB>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Value"};
		enum
		{
			Value
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
	static inline std::string const name = registerNode<ValueToColor>("Value To Color");

public:
	void run() override;
};

} // namespace rsp::nodes
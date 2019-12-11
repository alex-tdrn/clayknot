#pragma once

#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/node/Source.hpp"

#include <array>

namespace rsp::nodes
{
class RandomColorSource : public rsp::Source<RandomColorSource, rsp::OutputList<rsp::ColorRGB>>
{
public:
	struct OutputPorts
	{
		static inline std::array names = {"Color"};
		enum
		{
			Color
		};
	};

public:
	static inline std::string const name = registerNode<RandomColorSource>("Random Color Source");

protected:
	void update() override;
};

} // namespace rsp::nodes
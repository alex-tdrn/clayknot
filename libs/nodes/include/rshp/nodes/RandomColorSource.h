#pragma once

#include "rshp/base/ColorRGB.hpp"
#include "rshp/base/node/Source.hpp"

#include <array>

namespace rshp::nodes
{
class RandomColorSource : public rshp::base::Source<RandomColorSource, rshp::base::OutputList<rshp::base::ColorRGB>>
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

} // namespace rshp::nodes
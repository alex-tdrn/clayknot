#pragma once

#include "rshp/base/node/Source.hpp"

#include <array>
#include <glm/glm.hpp>

namespace rshp::nodes
{
class RandomColorSource : public rshp::base::Source<RandomColorSource, rshp::base::OutputList<glm::vec3>>
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
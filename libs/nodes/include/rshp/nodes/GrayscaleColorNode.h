#pragma once

#include "rshp/base/node/Node.hpp"

#include <array>
#include <glm/glm.hpp>

namespace rshp::nodes
{
class GrayscaleColorNode
	: public rshp::base::Node<GrayscaleColorNode, rshp::base::InputList<glm::vec3>, rshp::base::OutputList<glm::vec3>>
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
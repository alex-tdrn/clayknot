#include "rsp/algorithms/GrayscaleColorNode.h"

#include <glm/glm.hpp>

namespace rsp::algorithms
{
GrayscaleColorNode::GrayscaleColorNode()
{
	registerPort(inputColor);
	registerPort(outputColor);
}

auto GrayscaleColorNode::getName() const noexcept -> std::string const&
{
	static std::string name = "Grayscale Color Node";
	return name;
}

void GrayscaleColorNode::update()
{
	const glm::vec3 linearGrayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	*outputColor = rsp::ColorRGB(glm::dot(linearGrayscale, inputColor->vector()));
}
} // namespace rsp::algorithms
#include "rsp/nodes/GrayscaleColorNode.h"

#include <glm/glm.hpp>

void rsp::nodes::GrayscaleColorNode::update()
{
	const glm::vec3 linearGrayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	*outputColor = rsp::ColorRGB(glm::dot(linearGrayscale, inputColor->vector()));
}

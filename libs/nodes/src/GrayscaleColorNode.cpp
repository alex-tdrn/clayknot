#include "rsp/nodes/GrayscaleColorNode.h"

#include <glm/glm.hpp>

void rsp::nodes::GrayscaleColorNode::update()
{
	auto const& inputColor = getInputData<InputPorts::Color>();
	auto& outputColor = getOutputData<OutputPorts::Color>();
	const glm::vec3 linearGrayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	outputColor = rsp::ColorRGB(glm::dot(linearGrayscale, inputColor.vector()));
}

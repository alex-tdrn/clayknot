#include "rendershop/pipes/GrayscaleColorPipe.h"

void GrayscaleColorPipe::update()
{
	auto const& inputColor = getInputData<InputPorts::Color>();
	auto& outputColor = getOutputData<OutputPorts::Color>();
	const glm::vec3 linearGrayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	outputColor = glm::vec3(glm::dot(linearGrayscale, inputColor));
}

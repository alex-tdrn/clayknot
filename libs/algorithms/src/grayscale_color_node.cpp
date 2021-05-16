#include "rsp/algorithms/grayscale_color_node.h"

#include <glm/glm.hpp>

namespace rsp::algorithms
{
grayscale_color_node::grayscale_color_node()
{
	register_port(input_color);
	register_port(output_color);
}

auto grayscale_color_node::get_name() const noexcept -> std::string const&
{
	static std::string name = "Grayscale Color Node";
	return name;
}

void grayscale_color_node::update()
{
	const glm::vec3 linear_grayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	*output_color = rsp::color_rgb(glm::dot(linear_grayscale, input_color->vector()));
}
} // namespace rsp::algorithms
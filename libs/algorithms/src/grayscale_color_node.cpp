#include "clk/algorithms/grayscale_color_node.hpp"

#include <glm/glm.hpp>

namespace clk::algorithms
{
grayscale_color_node::grayscale_color_node()
{
	register_port(_input_color);
	register_port(_output_color);
}

auto grayscale_color_node::name() const noexcept -> std::string const&
{
	static std::string name = "Grayscale Color Node";
	return name;
}

void grayscale_color_node::update()
{
	const glm::vec3 linear_grayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	*_output_color = clk::color_rgb(glm::dot(linear_grayscale, _input_color->vector()));
}
} // namespace clk::algorithms
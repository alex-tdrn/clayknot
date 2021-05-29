#include "clk/algorithms/color.hpp"

namespace clk::algorithms
{
decompose_color::decompose_color()
{
	register_port(_color);
	register_port(_red_component);
	register_port(_green_component);
	register_port(_blue_component);
}

void decompose_color::update()
{
	*_red_component = _color->r() * 100;
	*_green_component = _color->g() * 100;
	*_blue_component = _color->b() * 100;
}

grayscale_color_node::grayscale_color_node()
{
	register_port(_input_color);
	register_port(_output_color);
}

void grayscale_color_node::update()
{
	const glm::vec3 linear_grayscale = glm::vec3(0.2126, 0.7152, 0.0722);
	*_output_color = clk::color_rgb(glm::dot(linear_grayscale, _input_color->vector()));
}

mix_colors::mix_colors()
{
	register_port(_factor);
	register_port(_color_a);
	register_port(_color_b);
	register_port(_mixed_color);
}

void mix_colors::update()
{
	float f = _factor.data() / 100.0f;
	*_mixed_color = f * _color_a.data() + (1 - f) * _color_b.data();
}

random_color_source::random_color_source()
{
	register_port(_color);
}

void random_color_source::update()
{
	std::mt19937 generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> dis(0, 1);

	*_color = clk::color_rgb{dis(generator), dis(generator), dis(generator)};
}

value_to_color::value_to_color()
{
	register_port(_value);
	register_port(_color);
}

void value_to_color::update()
{
	*_color = clk::color_rgb{*_value / 100.0f};
}
} // namespace clk::algorithms
#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

namespace clk::algorithms
{
class decompose_color final : public clk::algorithm_builder<decompose_color>
{
public:
	static constexpr std::string_view name = "Decompose Colors";

	decompose_color();

private:
	clk::input_of<clk::color_rgb> _color{"Color"};
	clk::output_of<float> _red_component{"R"};
	clk::output_of<float> _green_component{"G"};
	clk::output_of<float> _blue_component{"B"};

	void update() final;
};

class grayscale_color_node final : public clk::algorithm_builder<grayscale_color_node>
{
public:
	static constexpr std::string_view name = "Grayscale Color Node";

	grayscale_color_node();

private:
	clk::input_of<clk::color_rgb> _input_color{"Color"};
	clk::output_of<clk::color_rgb> _output_color{"Color"};

	void update() override;
};
class mix_colors final : public clk::algorithm_builder<mix_colors>
{
public:
	static constexpr std::string_view name = "Mix Colors";

	mix_colors();

private:
	clk::input_of<float> _factor{"Factor"};
	clk::input_of<clk::color_rgb> _color_a{"Color A"};
	clk::input_of<clk::color_rgb> _color_b{"Color B"};
	clk::output_of<clk::color_rgb> _mixed_color{"Mixed Color"};

	void update() override;
};

class random_color_source final : public clk::algorithm_builder<random_color_source>
{
public:
	static constexpr std::string_view name = "Random Color Source";

	random_color_source();

private:
	clk::output_of<clk::color_rgb> _color{"Color"};

	void update() override;
};

class value_to_color final : public clk::algorithm_builder<value_to_color>
{
public:
	static constexpr std::string_view name = "Value To Color";

	value_to_color();

private:
	clk::input_of<float> _value{"Value"};
	clk::output_of<clk::color_rgb> _color{"Color"};

	void update() override;
};
} // namespace clk::algorithms
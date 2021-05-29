#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

namespace clk::algorithms
{
class add_colors final : public clk::algorithm_builder<add_colors>
{
public:
	static constexpr std::string_view name = "Add Colors";

	add_colors();

private:
	clk::input_of<clk::color_rgb> _color_a{"Color A"};
	clk::input_of<clk::color_rgb> _color_b{"Color B"};
	clk::output_of<clk::color_rgb> _result{"Result"};

	void update() override;
};

class subtract_colors final : public clk::algorithm_builder<subtract_colors>
{
public:
	static constexpr std::string_view name = "Subtract Colors";

	subtract_colors();

private:
	clk::input_of<clk::color_rgb> _color_a{"Color A"};
	clk::input_of<clk::color_rgb> _color_b{"Color B"};
	clk::output_of<clk::color_rgb> _result{"Result"};

	void update() override;
};

class multiply_colors final : public clk::algorithm_builder<multiply_colors>
{
public:
	static constexpr std::string_view name = "Multiply Colors";

	multiply_colors();

private:
	clk::input_of<clk::color_rgb> _color_a{"Color A"};
	clk::input_of<clk::color_rgb> _color_b{"Color B"};
	clk::output_of<clk::color_rgb> _result{"Result"};

	void update() override;
};

class divide_colors final : public clk::algorithm_builder<divide_colors>
{
public:
	static constexpr std::string_view name = "Divide Colors";

	divide_colors();

private:
	clk::input_of<clk::color_rgb> _color_a{"Color A"};
	clk::input_of<clk::color_rgb> _color_b{"Color B"};
	clk::output_of<clk::color_rgb> _result{"Result"};

	void update() override;
};

class compose_color final : public clk::algorithm_builder<compose_color>
{
public:
	static constexpr std::string_view name = "Compose Colors";

	compose_color();

private:
	clk::input_of<float> _red_component{"R"};
	clk::input_of<float> _green_component{"G"};
	clk::input_of<float> _blue_component{"B"};
	clk::output_of<clk::color_rgb> _color{"Color"};

	void update() final;
};

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

class grayscale final : public clk::algorithm_builder<grayscale>
{
public:
	static constexpr std::string_view name = "Grayscale";

	grayscale();

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

class random_color final : public clk::algorithm_builder<random_color>
{
public:
	static constexpr std::string_view name = "Random Color";

	random_color();

private:
	clk::input_of<clk::color_rgb> _from{"From"};
	clk::input_of<clk::color_rgb> _to{"To"};
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

class apply_gamma final : public clk::algorithm_builder<apply_gamma>
{
public:
	static constexpr std::string_view name = "Apply Gamma";

	apply_gamma();

private:
	clk::input_of<clk::color_rgb> _linear{"Linear"};
	clk::output_of<clk::color_rgb> _gamma_corrected{"Gamma Corrected"};

	void update() override;
};

class remove_gamma final : public clk::algorithm_builder<remove_gamma>
{
public:
	static constexpr std::string_view name = "Remove Gamma";

	remove_gamma();

private:
	clk::input_of<clk::color_rgb> _gamma_corrected{"Gamma Corrected"};
	clk::output_of<clk::color_rgb> _linear{"Linear"};

	void update() override;
};

class tonemap_reinhard final : public clk::algorithm_builder<tonemap_reinhard>
{
public:
	static constexpr std::string_view name = "Tonemap Reinhard";

	tonemap_reinhard();

private:
	clk::input_of<clk::color_rgb> _input_color{"Input Color"};
	clk::output_of<clk::color_rgb> _tonemapped_color{"Tonemapped Color"};

	void update() override;
};

class tonemap_filmic_aces final : public clk::algorithm_builder<tonemap_filmic_aces>
{
public:
	static constexpr std::string_view name = "Tonemap Filmic ACES";

	tonemap_filmic_aces();

private:
	clk::input_of<clk::color_rgb> _input_color{"Input Color"};
	clk::output_of<clk::color_rgb> _tonemapped_color{"Tonemapped Color"};

	void update() override;
};

} // namespace clk::algorithms
#include "clk/algorithms/color.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <random>
#include <stdexcept>
#include <utility>

namespace clk::algorithms
{
add_colors::add_colors()
{
    register_port(_color_a);
    register_port(_color_b);
    register_port(_result);
}

void add_colors::update()
{
    *_result = *_color_a + *_color_b;
}

subtract_colors::subtract_colors()
{
    register_port(_color_a);
    register_port(_color_b);
    register_port(_result);
}

void subtract_colors::update()
{
    *_result = *_color_a - *_color_b;
}

multiply_colors::multiply_colors()
{
    register_port(_color_a);
    register_port(_color_b);
    register_port(_result);
}

void multiply_colors::update()
{
    *_result = *_color_a * *_color_b;
}

divide_colors::divide_colors()
{
    register_port(_color_a);
    register_port(_color_b);
    register_port(_result);
}

void divide_colors::update()
{
    if(_color_b->r() == 0)
    {
        throw std::runtime_error("B's red component is zero!");
    }

    if(_color_b->g() == 0)
    {
        throw std::runtime_error("B's green component is zero!");
    }

    if(_color_b->b() == 0)
    {
        throw std::runtime_error("B's blue component is zero!");
    }

    *_result = *_color_a / *_color_b;
}

compose_color::compose_color()
{
    register_port(_red_component);
    register_port(_green_component);
    register_port(_blue_component);
    register_port(_color);
}

void compose_color::update()
{
    *_color = clk::color_rgb(*_red_component, *_green_component, *_blue_component);
}

decompose_color::decompose_color()
{
    register_port(_color);
    register_port(_red_component);
    register_port(_green_component);
    register_port(_blue_component);
}

void decompose_color::update()
{
    *_red_component = _color->r();
    *_green_component = _color->g();
    *_blue_component = _color->b();
}

grayscale::grayscale()
{
    register_port(_input_color);
    register_port(_output_color);
}

void grayscale::update()
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

random_color::random_color()
{
    register_port(_from);
    register_port(_to);
    register_port(_color);

    *_from.default_port() = clk::color_rgb(0.0f, 0.0f, 0.0f);
    *_to.default_port() = clk::color_rgb(1.0f, 1.0f, 1.0f);
}

void random_color::update()
{
    std::mt19937 generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<float> dis_r(std::min(_from->r(), _to->r()), std::max(_from->r(), _to->r()));
    std::uniform_real_distribution<float> dis_g(std::min(_from->g(), _to->g()), std::max(_from->g(), _to->g()));
    std::uniform_real_distribution<float> dis_b(std::min(_from->b(), _to->b()), std::max(_from->b(), _to->b()));

    *_color = clk::color_rgb{dis_r(generator), dis_g(generator), dis_b(generator)};
}

value_to_color::value_to_color()
{
    register_port(_value);
    register_port(_color);
}

void value_to_color::update()
{
    *_color = clk::color_rgb{*_value};
}

apply_gamma::apply_gamma()
{
    register_port(_linear);
    register_port(_gamma_corrected);
}

void apply_gamma::update()
{
    *_gamma_corrected = clk::color_rgb(glm::pow(_linear->vector(), glm::vec3(1 / 2.2f)));
}

remove_gamma::remove_gamma()
{
    register_port(_gamma_corrected);
    register_port(_linear);
}

void remove_gamma::update()
{
    *_linear = clk::color_rgb(glm::pow(_gamma_corrected->vector(), glm::vec3(2.2f)));
}

tonemap_reinhard::tonemap_reinhard()
{
    register_port(_input_color);
    register_port(_tonemapped_color);
}

void tonemap_reinhard::update()
{
    *_tonemapped_color = *_input_color / clk::color_rgb(*_input_color + glm::vec3(1.0f));
}

tonemap_filmic_aces::tonemap_filmic_aces()
{
    register_port(_input_color);
    register_port(_tonemapped_color);
}

void tonemap_filmic_aces::update()
{
    auto c = 2.43f;
    auto d = 0.59f;
    auto e = 0.14f;
    auto a = 2.51f;
    auto b = 0.03f;

    *_tonemapped_color = (*_input_color * (a * *_input_color + b)) / *_input_color * (c * *_input_color + d) + e;
}

} // namespace clk::algorithms

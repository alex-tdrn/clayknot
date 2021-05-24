#include "clk/algorithms/random_color_source.hpp"

#include <chrono>
#include <random>

namespace clk::algorithms
{
random_color_source::random_color_source()
{
	register_port(_color);
}

auto random_color_source::name() const noexcept -> std::string const&
{
	static std::string name = "Random Color Source";
	return name;
}

void random_color_source::update()
{
	std::mt19937 generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> dis(0, 1);

	*_color = clk::color_rgb{dis(generator), dis(generator), dis(generator)};
}
} // namespace clk::algorithms
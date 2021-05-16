#include "rsp/algorithms/random_color_source.h"

#include <chrono>
#include <random>

namespace rsp::algorithms
{
random_color_source::random_color_source()
{
	register_port(color);
}

auto random_color_source::get_name() const noexcept -> std::string const&
{
	static std::string name = "Random Color Source";
	return name;
}

void random_color_source::update()
{
	std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> dis(0, 1);

	*color = rsp::color_rgb{dis(generator), dis(generator), dis(generator)};
}
} // namespace rsp::algorithms
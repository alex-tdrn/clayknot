#include "rsp/algorithms/mix_colors.h"

namespace rsp::algorithms
{
mix_colors::mix_colors()
{
	register_port(factor);
	register_port(color_a);
	register_port(color_b);
	register_port(mixed_color);
}

auto mix_colors::get_name() const noexcept -> std::string const&
{
	static std::string name = "Mix Colors";
	return name;
}

void mix_colors::update()
{
	float f = factor.get() / 100.0f;
	*mixed_color = f * color_a.get() + (1 - f) * color_b.get();
}
} // namespace rsp::algorithms
#include "clk/algorithms/init.hpp"
#include "clk/algorithms/color.hpp"

namespace clk::algorithms
{
void init()
{
	clk::algorithm::register_factory<add_colors>();
	clk::algorithm::register_factory<subtract_colors>();
	clk::algorithm::register_factory<multiply_colors>();
	clk::algorithm::register_factory<divide_colors>();
	clk::algorithm::register_factory<decompose_color>();
	clk::algorithm::register_factory<grayscale>();
	clk::algorithm::register_factory<mix_colors>();
	clk::algorithm::register_factory<random_color>();
	clk::algorithm::register_factory<value_to_color>();
	clk::algorithm::register_factory<apply_gamma>();
	clk::algorithm::register_factory<remove_gamma>();
	clk::algorithm::register_factory<tonemap_reinhard>();
	clk::algorithm::register_factory<tonemap_filmic_aces>();
}
} // namespace clk::algorithms
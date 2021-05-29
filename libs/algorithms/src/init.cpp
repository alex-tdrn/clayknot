#include "clk/algorithms/init.hpp"
#include "clk/algorithms/color.hpp"

namespace clk::algorithms
{
void init()
{
	clk::algorithm::register_factory<decompose_color>();
	clk::algorithm::register_factory<grayscale_color_node>();
	clk::algorithm::register_factory<mix_colors>();
	clk::algorithm::register_factory<random_color_source>();
	clk::algorithm::register_factory<value_to_color>();
}
} // namespace clk::algorithms
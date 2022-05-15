#include "clk/algorithms/init.hpp"
#include "clk/algorithms/boolean.hpp"
#include "clk/algorithms/color.hpp"
#include "clk/algorithms/math.hpp"
#include "clk/algorithms/text.hpp"
#include "clk/base/algorithm.hpp"

namespace clk::algorithms
{
void init()
{
	clk::algorithm::register_factory<random_integer>();
	clk::algorithm::register_factory<random_float>();
	clk::algorithm::register_factory<add_integers>();
	clk::algorithm::register_factory<subtract_integers>();
	clk::algorithm::register_factory<multiply_integers>();
	clk::algorithm::register_factory<divide_integers>();
	clk::algorithm::register_factory<modulo>();
	clk::algorithm::register_factory<add_floats>();
	clk::algorithm::register_factory<subtract_floats>();
	clk::algorithm::register_factory<multiply_floats>();
	clk::algorithm::register_factory<divide_floats>();
	clk::algorithm::register_factory<pow>();
	clk::algorithm::register_factory<nth_root>();
	clk::algorithm::register_factory<rad_to_deg>();
	clk::algorithm::register_factory<deg_to_rad>();
	clk::algorithm::register_factory<sin>();
	clk::algorithm::register_factory<cos>();
	clk::algorithm::register_factory<is_even>();
	clk::algorithm::register_factory<is_odd>();
	clk::algorithm::register_factory<integer_equal_to>();
	clk::algorithm::register_factory<integer_not_equal_to>();
	clk::algorithm::register_factory<integer_less_than>();
	clk::algorithm::register_factory<integer_less_than_or_equal_to>();
	clk::algorithm::register_factory<integer_greater_than>();
	clk::algorithm::register_factory<integer_greater_than_or_equal_to>();
	clk::algorithm::register_factory<float_equal_to>();
	clk::algorithm::register_factory<float_not_equal_to>();
	clk::algorithm::register_factory<float_less_than>();
	clk::algorithm::register_factory<float_less_than_or_equal_to>();
	clk::algorithm::register_factory<float_greater_than>();
	clk::algorithm::register_factory<float_greater_than_or_equal_to>();
	clk::algorithm::register_factory<integer_to_float>();
	clk::algorithm::register_factory<float_to_integer>();

	clk::algorithm::register_factory<add_colors>();
	clk::algorithm::register_factory<subtract_colors>();
	clk::algorithm::register_factory<multiply_colors>();
	clk::algorithm::register_factory<divide_colors>();
	clk::algorithm::register_factory<compose_color>();
	clk::algorithm::register_factory<decompose_color>();
	clk::algorithm::register_factory<grayscale>();
	clk::algorithm::register_factory<mix_colors>();
	clk::algorithm::register_factory<random_color>();
	clk::algorithm::register_factory<value_to_color>();
	clk::algorithm::register_factory<apply_gamma>();
	clk::algorithm::register_factory<remove_gamma>();
	clk::algorithm::register_factory<tonemap_reinhard>();
	clk::algorithm::register_factory<tonemap_filmic_aces>();

	clk::algorithm::register_factory<boolean_not>();
	clk::algorithm::register_factory<boolean_and>();
	clk::algorithm::register_factory<boolean_nand>();
	clk::algorithm::register_factory<boolean_or>();
	clk::algorithm::register_factory<boolean_nor>();
	clk::algorithm::register_factory<boolean_xor>();
	clk::algorithm::register_factory<boolean_xnor>();
	clk::algorithm::register_factory<integer_to_boolean>();
	clk::algorithm::register_factory<boolean_to_integer>();

	clk::algorithm::register_factory<uppercase>();
	clk::algorithm::register_factory<lowercase>();
}
} // namespace clk::algorithms
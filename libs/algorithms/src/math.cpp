#include "clk/algorithms/math.hpp"

#include <chrono>
#include <cmath>
#include <random>
#include <utility>

namespace clk::algorithms
{
random_integer::random_integer()
{
	register_port(_from);
	register_port(_to);
	register_port(_number);

	*_from.default_port() = 0;
	*_to.default_port() = 100;
}

void random_integer::update()
{
	auto min = *_from;
	auto max = *_to;
	if(min > max)
		std::swap(min, max);
	std::mt19937 generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> dis(min, max);

	*_number = dis(generator);
}

random_float::random_float()
{
	register_port(_from);
	register_port(_to);
	register_port(_number);

	*_from.default_port() = 0.0f;
	*_to.default_port() = 1.0f;
}

void random_float::update()
{
	auto min = *_from;
	auto max = *_to;
	if(min > max)
		std::swap(min, max);
	std::mt19937 generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> dis(min, max);

	*_number = dis(generator);
}

add_integers::add_integers()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void add_integers::update()
{
	*_result = *_number_a + *_number_b;
}

subtract_integers::subtract_integers()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void subtract_integers::update()
{
	*_result = *_number_a - *_number_b;
}

multiply_integers::multiply_integers()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void multiply_integers::update()
{
	*_result = *_number_a * *_number_b;
}

divide_integers::divide_integers()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void divide_integers::update()
{
	if(*_number_b != 0)
		*_result = *_number_a / *_number_b;
}

modulo::modulo()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void modulo::update()
{
	if(*_number_b != 0)
		*_result = *_number_a % *_number_b;
}

add_floats::add_floats()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void add_floats::update()
{
	*_result = *_number_a + *_number_b;
}

subtract_floats::subtract_floats()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void subtract_floats::update()
{
	*_result = *_number_a - *_number_b;
}

multiply_floats::multiply_floats()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void multiply_floats::update()
{
	*_result = *_number_a * *_number_b;
}

divide_floats::divide_floats()
{
	register_port(_number_a);
	register_port(_number_b);
	register_port(_result);
}

void divide_floats::update()
{
	if(*_number_b != 0)
		*_result = *_number_a / *_number_b;
}

pow::pow()
{
	register_port(_number);
	register_port(_exponent);
	register_port(_result);

	*_exponent.default_port() = 2.0f;
}

void pow::update()
{
	*_result = std::pow(*_number, *_exponent);
}

nth_root::nth_root()
{
	register_port(_number);
	register_port(_root_degree);
	register_port(_result);

	*_root_degree.default_port() = 2.0f;
}

void nth_root::update()
{
	if(*_root_degree != 0.0f && *_number >= 0.0f)
		*_result = std::pow(*_number, 1.0f / *_root_degree);
}

rad_to_deg::rad_to_deg()
{
	register_port(_radians);
	register_port(_degrees);
}

void rad_to_deg::update()
{
	*_degrees = *_radians * 180.0f / 3.14159265f;
}

deg_to_rad::deg_to_rad()
{
	register_port(_degrees);
	register_port(_radians);
}

void deg_to_rad::update()
{
	*_radians = *_degrees * 3.14159265f / 180.0f;
}

sin::sin()
{
	register_port(_angle);
	register_port(_sin);
}

void sin::update()
{
	*_sin = std::sin(*_angle);
}

cos::cos()
{
	register_port(_angle);
	register_port(_cos);
}

void cos::update()
{
	*_cos = std::cos(*_angle);
}

is_even::is_even()
{
	register_port(_number);
	register_port(_result);
}

void is_even::update()
{
	*_result = *_number % 2 == 0;
}

is_odd::is_odd()
{
	register_port(_number);
	register_port(_result);
}

void is_odd::update()
{
	*_result = *_number % 2 == 1;
}

integer_equal_to::integer_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void integer_equal_to::update()
{
	*_result = *_a == *_b;
}

integer_not_equal_to::integer_not_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void integer_not_equal_to::update()
{
	*_result = *_a != *_b;
}

integer_less_than::integer_less_than()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void integer_less_than::update()
{
	*_result = *_a < *_b;
}

integer_less_than_or_equal_to::integer_less_than_or_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void integer_less_than_or_equal_to::update()
{
	*_result = *_a <= *_b;
}

integer_greater_than::integer_greater_than()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void integer_greater_than::update()
{
	*_result = *_a > *_b;
}

integer_greater_than_or_equal_to::integer_greater_than_or_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void integer_greater_than_or_equal_to::update()
{
	*_result = *_a >= *_b;
}

float_equal_to::float_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void float_equal_to::update()
{
	*_result = *_a == *_b;
}

float_not_equal_to::float_not_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void float_not_equal_to::update()
{
	*_result = *_a != *_b;
}

float_less_than::float_less_than()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void float_less_than::update()
{
	*_result = *_a < *_b;
}

float_less_than_or_equal_to::float_less_than_or_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void float_less_than_or_equal_to::update()
{
	*_result = *_a <= *_b;
}

float_greater_than::float_greater_than()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void float_greater_than::update()
{
	*_result = *_a > *_b;
}

float_greater_than_or_equal_to::float_greater_than_or_equal_to()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void float_greater_than_or_equal_to::update()
{
	*_result = *_a >= *_b;
}

integer_to_float::integer_to_float()
{
	register_port(_integer);
	register_port(_float);
}

void integer_to_float::update()
{
	*_float = static_cast<float>(*_integer);
}

float_to_integer::float_to_integer()
{
	register_port(_float);
	register_port(_integer);
}

void float_to_integer::update()
{
	*_integer = static_cast<int>(*_float);
}

} // namespace clk::algorithms
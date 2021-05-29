#include "clk/algorithms/boolean.hpp"

namespace clk::algorithms
{
boolean_not::boolean_not()
{
	register_port(_a);
	register_port(_result);
}

void boolean_not::update()
{
	*_result = !*_a;
}

boolean_and::boolean_and()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void boolean_and::update()
{
	*_result = *_a && *_b;
}

boolean_nand::boolean_nand()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void boolean_nand::update()
{
	*_result = !(*_a && *_b);
}

boolean_or::boolean_or()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void boolean_or::update()
{
	*_result = *_a || *_b;
}

boolean_nor::boolean_nor()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void boolean_nor::update()
{
	*_result = !(*_a || *_b);
}

boolean_xor::boolean_xor()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void boolean_xor::update()
{
	*_result = *_a != *_b;
}

boolean_xnor::boolean_xnor()
{
	register_port(_a);
	register_port(_b);
	register_port(_result);
}

void boolean_xnor::update()
{
	*_result = *_a == *_b;
}

integer_to_boolean::integer_to_boolean()
{
	register_port(_integer);
	register_port(_boolean);
}

void integer_to_boolean::update()
{
	*_boolean = static_cast<bool>(*_integer);
}

boolean_to_integer::boolean_to_integer()
{
	register_port(_boolean);
	register_port(_integer);
}

void boolean_to_integer::update()
{
	*_integer = static_cast<int>(*_boolean);
}
} // namespace clk::algorithms
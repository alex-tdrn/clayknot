#pragma once

#include "clk/base/algorithm.hpp"

namespace clk::algorithms
{
class random_integer final : public clk::algorithm_builder<random_integer>
{
public:
	static constexpr std::string_view name = "Random Integer";

	random_integer();

private:
	clk::input_of<int> _from{"From"};
	clk::input_of<int> _to{"To"};
	clk::output_of<int> _number{"Number"};

	void update() override;
};

class random_float final : public clk::algorithm_builder<random_float>
{
public:
	static constexpr std::string_view name = "Random Float";

	random_float();

private:
	clk::input_of<float> _from{"From"};
	clk::input_of<float> _to{"To"};
	clk::output_of<float> _number{"Number"};

	void update() override;
};

class add_integers final : public clk::algorithm_builder<add_integers>
{
public:
	static constexpr std::string_view name = "Add Integers";

	add_integers();

private:
	clk::input_of<int> _number_a{"Number A"};
	clk::input_of<int> _number_b{"Number B"};
	clk::output_of<int> _result{"Result"};

	void update() override;
};

class subtract_integers final : public clk::algorithm_builder<subtract_integers>
{
public:
	static constexpr std::string_view name = "Subtract Integers";

	subtract_integers();

private:
	clk::input_of<int> _number_a{"Number A"};
	clk::input_of<int> _number_b{"Number B"};
	clk::output_of<int> _result{"Result"};

	void update() override;
};

class multiply_integers final : public clk::algorithm_builder<multiply_integers>
{
public:
	static constexpr std::string_view name = "Multiply Integers";

	multiply_integers();

private:
	clk::input_of<int> _number_a{"Number A"};
	clk::input_of<int> _number_b{"Number B"};
	clk::output_of<int> _result{"Result"};

	void update() override;
};

class divide_integers final : public clk::algorithm_builder<divide_integers>
{
public:
	static constexpr std::string_view name = "Divide Integers";

	divide_integers();

private:
	clk::input_of<int> _number_a{"Number A"};
	clk::input_of<int> _number_b{"Number B"};
	clk::output_of<int> _result{"Result"};

	void update() override;
};

class modulo final : public clk::algorithm_builder<modulo>
{
public:
	static constexpr std::string_view name = "Modulo";

	modulo();

private:
	clk::input_of<int> _number_a{"Number A"};
	clk::input_of<int> _number_b{"Number B"};
	clk::output_of<int> _result{"Result"};

	void update() override;
};

class add_floats final : public clk::algorithm_builder<add_floats>
{
public:
	static constexpr std::string_view name = "Add Floats";

	add_floats();

private:
	clk::input_of<float> _number_a{"Number A"};
	clk::input_of<float> _number_b{"Number B"};
	clk::output_of<float> _result{"Result"};

	void update() override;
};

class subtract_floats final : public clk::algorithm_builder<subtract_floats>
{
public:
	static constexpr std::string_view name = "Subtract Floats";

	subtract_floats();

private:
	clk::input_of<float> _number_a{"Number A"};
	clk::input_of<float> _number_b{"Number B"};
	clk::output_of<float> _result{"Result"};

	void update() override;
};

class multiply_floats final : public clk::algorithm_builder<multiply_floats>
{
public:
	static constexpr std::string_view name = "Multiply Floats";

	multiply_floats();

private:
	clk::input_of<float> _number_a{"Number A"};
	clk::input_of<float> _number_b{"Number B"};
	clk::output_of<float> _result{"Result"};

	void update() override;
};

class divide_floats final : public clk::algorithm_builder<divide_floats>
{
public:
	static constexpr std::string_view name = "Divide Floats";

	divide_floats();

private:
	clk::input_of<float> _number_a{"Number A"};
	clk::input_of<float> _number_b{"Number B"};
	clk::output_of<float> _result{"Result"};

	void update() override;
};

class pow final : public clk::algorithm_builder<pow>
{
public:
	static constexpr std::string_view name = "Pow";

	pow();

private:
	clk::input_of<float> _number{"Number"};
	clk::input_of<float> _exponent{"Exponent"};
	clk::output_of<float> _result{"Result"};

	void update() override;
};

class nth_root final : public clk::algorithm_builder<nth_root>
{
public:
	static constexpr std::string_view name = "Nth Root";

	nth_root();

private:
	clk::input_of<float> _number{"Number"};
	clk::input_of<float> _root_degree{"Root Degree"};
	clk::output_of<float> _result{"Result"};

	void update() override;
};

class rad_to_deg final : public clk::algorithm_builder<rad_to_deg>
{
public:
	static constexpr std::string_view name = "Radians to Degrees";

	rad_to_deg();

private:
	clk::input_of<float> _radians{"Radians"};
	clk::output_of<float> _degrees{"Degrees"};

	void update() override;
};

class deg_to_rad final : public clk::algorithm_builder<deg_to_rad>
{
public:
	static constexpr std::string_view name = "Degrees to Radians";

	deg_to_rad();

private:
	clk::input_of<float> _degrees{"Degrees"};
	clk::output_of<float> _radians{"Radians"};

	void update() override;
};

class sin final : public clk::algorithm_builder<sin>
{
public:
	static constexpr std::string_view name = "Sine";

	sin();

private:
	clk::input_of<float> _angle{"Angle"};
	clk::output_of<float> _sin{"Sine"};

	void update() override;
};

class cos final : public clk::algorithm_builder<cos>
{
public:
	static constexpr std::string_view name = "Cosine";

	cos();

private:
	clk::input_of<float> _angle{"Angle"};
	clk::output_of<float> _cos{"Cosine"};

	void update() override;
};

} // namespace clk::algorithms
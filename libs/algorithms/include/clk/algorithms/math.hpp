#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"
#include "clk/util/timestamp.hpp"

#include <cstddef>
#include <memory>
#include <range/v3/view/view.hpp>
#include <string_view>
#include <unordered_set>

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
    static constexpr std::string_view name = "Radians To Degrees";

    rad_to_deg();

private:
    clk::input_of<float> _radians{"Radians"};
    clk::output_of<float> _degrees{"Degrees"};

    void update() override;
};

class deg_to_rad final : public clk::algorithm_builder<deg_to_rad>
{
public:
    static constexpr std::string_view name = "Degrees To Radians";

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

class is_even final : public clk::algorithm_builder<is_even>
{
public:
    static constexpr std::string_view name = "Is Even";

    is_even();

private:
    clk::input_of<int> _number{"Number"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class is_odd final : public clk::algorithm_builder<is_odd>
{
public:
    static constexpr std::string_view name = "Is Odd";

    is_odd();

private:
    clk::input_of<int> _number{"Number"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class integer_equal_to final : public clk::algorithm_builder<integer_equal_to>
{
public:
    static constexpr std::string_view name = "Integer Equal To";

    integer_equal_to();

private:
    clk::input_of<int> _a{"A"};
    clk::input_of<int> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class integer_not_equal_to final : public clk::algorithm_builder<integer_not_equal_to>
{
public:
    static constexpr std::string_view name = "Integer Not Equal To";

    integer_not_equal_to();

private:
    clk::input_of<int> _a{"A"};
    clk::input_of<int> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class integer_less_than final : public clk::algorithm_builder<integer_less_than>
{
public:
    static constexpr std::string_view name = "Integer Less Than";

    integer_less_than();

private:
    clk::input_of<int> _a{"A"};
    clk::input_of<int> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class integer_less_than_or_equal_to final : public clk::algorithm_builder<integer_less_than_or_equal_to>
{
public:
    static constexpr std::string_view name = "Integer Less Than Or Equal To";

    integer_less_than_or_equal_to();

private:
    clk::input_of<int> _a{"A"};
    clk::input_of<int> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};
class integer_greater_than final : public clk::algorithm_builder<integer_greater_than>
{
public:
    static constexpr std::string_view name = "Integer Greater Than";

    integer_greater_than();

private:
    clk::input_of<int> _a{"A"};
    clk::input_of<int> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class integer_greater_than_or_equal_to final : public clk::algorithm_builder<integer_greater_than_or_equal_to>
{
public:
    static constexpr std::string_view name = "Integer Greater Than Or Equal To";

    integer_greater_than_or_equal_to();

private:
    clk::input_of<int> _a{"A"};
    clk::input_of<int> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class float_equal_to final : public clk::algorithm_builder<float_equal_to>
{
public:
    static constexpr std::string_view name = "Float Equal To";

    float_equal_to();

private:
    clk::input_of<float> _a{"A"};
    clk::input_of<float> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class float_not_equal_to final : public clk::algorithm_builder<float_not_equal_to>
{
public:
    static constexpr std::string_view name = "Float Not Equal To";

    float_not_equal_to();

private:
    clk::input_of<float> _a{"A"};
    clk::input_of<float> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class float_less_than final : public clk::algorithm_builder<float_less_than>
{
public:
    static constexpr std::string_view name = "Float Less Than";

    float_less_than();

private:
    clk::input_of<float> _a{"A"};
    clk::input_of<float> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class float_less_than_or_equal_to final : public clk::algorithm_builder<float_less_than_or_equal_to>
{
public:
    static constexpr std::string_view name = "Float Less Than Or Equal To";

    float_less_than_or_equal_to();

private:
    clk::input_of<float> _a{"A"};
    clk::input_of<float> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};
class float_greater_than final : public clk::algorithm_builder<float_greater_than>
{
public:
    static constexpr std::string_view name = "Float Greater Than";

    float_greater_than();

private:
    clk::input_of<float> _a{"A"};
    clk::input_of<float> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class float_greater_than_or_equal_to final : public clk::algorithm_builder<float_greater_than_or_equal_to>
{
public:
    static constexpr std::string_view name = "Float Greater Than Or Equal To";

    float_greater_than_or_equal_to();

private:
    clk::input_of<float> _a{"A"};
    clk::input_of<float> _b{"B"};
    clk::output_of<bool> _result{"Result"};

    void update() override;
};

class integer_to_float final : public clk::algorithm_builder<integer_to_float>
{
public:
    static constexpr std::string_view name = "Integer To Float";

    integer_to_float();

private:
    clk::input_of<int> _integer{"Integer"};
    clk::output_of<float> _float{"Float"};

    void update() override;
};

class float_to_integer final : public clk::algorithm_builder<float_to_integer>
{
public:
    static constexpr std::string_view name = "Float To Integer";

    float_to_integer();

private:
    clk::input_of<float> _float{"Float"};
    clk::output_of<int> _integer{"Integer"};

    void update() override;
};

} // namespace clk::algorithms

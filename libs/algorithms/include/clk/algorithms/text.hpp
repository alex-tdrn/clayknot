#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"

#include <string>
#include <string_view>

namespace clk::algorithms
{
class uppercase final : public clk::algorithm_builder<uppercase>
{
public:
    static constexpr std::string_view name = "Uppercase";

    uppercase();

private:
    clk::input_of<std::string> _in{"In"};
    clk::output_of<std::string> _out{"Out"};

    void update() override;
};

class lowercase final : public clk::algorithm_builder<lowercase>
{
public:
    static constexpr std::string_view name = "Lowercase";

    lowercase();

private:
    clk::input_of<std::string> _in{"In"};
    clk::output_of<std::string> _out{"Out"};

    void update() override;
};

} // namespace clk::algorithms

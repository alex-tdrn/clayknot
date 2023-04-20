#include "clk/base/algorithm.hpp"

#include <range/v3/algorithm/find.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/range/dangling.hpp>
#include <utility>

namespace clk
{
auto algorithm::create(std::string_view name) -> std::unique_ptr<algorithm>
{
    auto it = factories_map().find(name);
    if(it == factories_map().end())
        throw std::runtime_error("Algorithm not registered");
    return it->second();
}

auto algorithm::factories() -> std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>> const&
{
    return factories_map();
}

auto algorithm::inputs() const noexcept -> std::vector<clk::input*> const&
{
    return _inputs;
}

auto algorithm::outputs() const noexcept -> std::vector<clk::output*> const&
{
    return _outputs;
}

void algorithm::register_port(clk::input& input)
{
    if(ranges::find(_inputs, &input) != _inputs.end())
        return;
    _inputs.emplace_back(&input);
}

void algorithm::register_port(clk::output& output)
{
    if(ranges::find(_outputs, &output) != _outputs.end())
        return;
    _outputs.emplace_back(&output);
}

auto algorithm::factories_map() -> std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>>&
{
    static std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>> factories_map;
    return factories_map;
}

} // namespace clk

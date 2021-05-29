#pragma once

#include "clk/base/input.hpp"
#include "clk/base/output.hpp"

#include <map>
#include <memory>
#include <range/v3/algorithm.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace clk
{
class algorithm
{
public:
	template <typename algorithm_implementation>
	static void register_factory();
	static auto create(std::string_view name) -> std::unique_ptr<algorithm>;
	static auto factories() -> std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>> const&;

	algorithm(const algorithm&) = delete;
	algorithm(algorithm&&) = delete;
	auto operator=(const algorithm&) = delete;
	auto operator=(algorithm&&) = delete;
	virtual ~algorithm() = default;

	virtual auto name() const noexcept -> std::string_view = 0;
	virtual void update() = 0;
	auto inputs() const noexcept -> std::vector<clk::input*> const&;
	auto outputs() const noexcept -> std::vector<clk::output*> const&;

protected:
	algorithm() = default;

	void register_port(clk::input& input);
	void register_port(clk::output& output);

private:
	static auto factories_map() -> std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>>&;

	std::vector<clk::input*> _inputs;
	std::vector<clk::output*> _outputs;
};

template <typename algorithm_implementation>
class algorithm_builder : public algorithm
{
public:
	auto name() const noexcept -> std::string_view final;
};

template <typename algorithm_implementation>
void algorithm::register_factory()
{
	if(factories_map().find(algorithm_implementation::name) != factories_map().end())
		throw std::exception("Algorithm already registered!");

	factories_map()[std::string(algorithm_implementation::name)] = []() -> std::unique_ptr<algorithm> {
		return std::make_unique<algorithm_implementation>();
	};
}

auto inline algorithm::create(std::string_view name) -> std::unique_ptr<algorithm>
{
	auto it = factories_map().find(name);
	if(it == factories_map().end())
		throw std::exception("Algorithm not registered");
	return it->second();
}

inline auto algorithm::factories() -> std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>> const&
{
	return factories_map();
}

inline auto algorithm::inputs() const noexcept -> std::vector<clk::input*> const&
{
	return _inputs;
}

inline auto algorithm::outputs() const noexcept -> std::vector<clk::output*> const&
{
	return _outputs;
}

inline void algorithm::register_port(clk::input& input)
{
	if(ranges::find(_inputs, &input) != _inputs.end())
		return;
	_inputs.emplace_back(&input);
}

inline void algorithm::register_port(clk::output& output)
{
	if(ranges::find(_outputs, &output) != _outputs.end())
		return;
	_outputs.emplace_back(&output);
}

inline auto algorithm::factories_map() -> std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>>&
{
	static std::map<std::string, std::unique_ptr<algorithm> (*)(), std::less<>> factories_map;
	return factories_map;
}

template <typename algorithm_implementation>
auto algorithm_builder<algorithm_implementation>::name() const noexcept -> std::string_view
{
	return algorithm_implementation::name;
}

} // namespace clk
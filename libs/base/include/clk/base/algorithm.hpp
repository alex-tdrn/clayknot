#pragma once

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace clk
{
class input;
class output;

class algorithm
{
public:
	template <typename AlgorithmImplementation>
	static void register_factory()
	{
		if(factories_map().find(AlgorithmImplementation::name) != factories_map().end())
			throw std::runtime_error("Algorithm already registered!");

		factories_map()[std::string(AlgorithmImplementation::name)] = []() -> std::unique_ptr<algorithm> {
			return std::make_unique<AlgorithmImplementation>();
		};
	}

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

template <typename AlgorithmImplementation>
class algorithm_builder : public algorithm
{
public:
	auto name() const noexcept -> std::string_view final
	{
		return AlgorithmImplementation::name;
	}
};

} // namespace clk
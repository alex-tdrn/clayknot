#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class DecomposeColor final : public Algorithm
{
public:
	DecomposeColor();
	auto getName() const noexcept -> std::string const& final;

private:
	InputPortOf<ColorRGB> color{"Color"};
	OutputPortOf<float> redComponent{"R"};
	OutputPortOf<float> greenComponent{"G"};
	OutputPortOf<float> blueComponent{"B"};

	void update() final;
};

// class DecomposeColor final : public rsp::Algorithm<DecomposeColor>
// {
// public:
// 	static inline std::string name = "DecomposeColor";
// 	static inline std::array<std::string, 1> inputNames = {"Color"};
// 	static inline std::array<std::string, 3> outputNames = {"R", "G", "B"};

// 	void run(ColorRGB const& color)
// 	{
// 		redComponent = color->r();
// 		greenComponent = color->g();
// 		blueComponent = color->b();
// 	}

// 	auto result() const noexcept -> std::tuple<float const&, float const&, float const&>
// 	{
// 		return std::tie(redComponent, greenComponent, blueComponent);
// 	}

// private:
// 	float redComponent;
// 	float greenComponent;
// 	float blueComponent;
// };

} // namespace rsp::algorithms
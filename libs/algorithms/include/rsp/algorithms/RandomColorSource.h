#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class RandomColorSource final : public Algorithm
{
public:
	RandomColorSource();

	auto getName() const noexcept -> std::string const& override;

private:
	OutputPortOf<ColorRGB> color{"Color"};

	void update() override;
};

} // namespace rsp::algorithms
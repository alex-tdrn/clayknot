#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::nodes
{
class ValueToColor final : public Algorithm
{
public:
	ValueToColor();

	auto getName() const noexcept -> std::string const& override;

private:
	InputPortOf<float> value{"Value"};
	OutputPortOf<ColorRGB> color{"Color"};

	void update() override;
};

} // namespace rsp::nodes
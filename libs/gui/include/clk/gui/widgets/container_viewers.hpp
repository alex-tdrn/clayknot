#pragma once

#include "viewer.hpp"

#include <vector>

namespace clk::gui
{
template <typename StoredDataType>
class viewer_of<std::vector<StoredDataType>> : public viewer
{
public:
	using viewer::viewer;
	viewer_of() = delete;
	viewer_of(viewer_of const&) = delete;
	viewer_of(viewer_of&&) = delete;
	auto operator=(viewer_of const&) -> viewer_of& = delete;
	auto operator=(viewer_of&&) -> viewer_of& = delete;
	~viewer_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override
	{
		auto clone = std::make_unique<viewer_of<std::vector<StoredDataType>>>(get_widget_factory(), name());
		clone->copy(*this);
		return clone;
	}

	void copy(widget const& other) override
	{
		viewer::copy(other);
	}

private:
	void draw_contents() const final
	{
		ImGui::Text("Vector VIEWER");
	}
};

} // namespace clk::gui
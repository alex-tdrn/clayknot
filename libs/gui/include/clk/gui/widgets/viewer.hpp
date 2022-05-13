#pragma once

#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/data_widget.hpp"

#include <imgui.h>

namespace clk::gui
{
class viewer : public data_widget
{
public:
	using data_widget::data_widget;
	viewer() = delete;
	viewer(viewer const&) = delete;
	viewer(viewer&&) = delete;
	auto operator=(viewer const&) -> viewer& = delete;
	auto operator=(viewer&&) -> viewer& = delete;
	~viewer() override = default;
};

template <typename data_type>
class viewer_of : public viewer
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
		auto clone = std::make_unique<viewer_of<data_type>>(get_widget_factory(), name());
		clone->copy(*this);
		return clone;
	}

	void copy(widget const& other) override
	{
		auto const& casted = dynamic_cast<viewer_of<data_type> const&>(other);
		_data = casted._data;
		viewer::copy(other);
	}

	void set_data_reader(data_reader<data_type> data)
	{
		_data = std::move(data);
	}

	virtual void draw_contents(data_type const& /*data*/) const
	{
		ImGui::Text("NO VIEWER IMPLEMENTATION");
	}

private:
	data_reader<data_type> _data;

	void draw_contents() const final
	{
		if(auto const* data = _data.read(); data != nullptr)
		{
			draw_contents(*data);
		}
		else
		{
			ImGui::Text("NO DATA");
		}
	}
};

template <>
class viewer_of<void> : public viewer
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
		auto clone = std::make_unique<viewer_of<void>>(get_widget_factory(), name());
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
		ImGui::Text("VOID VIEWER");
	}
};

} // namespace clk::gui

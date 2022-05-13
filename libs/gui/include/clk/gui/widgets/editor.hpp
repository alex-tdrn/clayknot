#pragma once

#include "clk/gui/widgets/data_widget.hpp"
#include "clk/gui/widgets/data_writer.hpp"

#include <imgui.h>

namespace clk::gui
{
class editor : public data_widget
{
public:
	using data_widget::data_widget;
	editor() = delete;
	editor(editor const&) = delete;
	editor(editor&&) = delete;
	auto operator=(editor const&) -> editor& = delete;
	auto operator=(editor&&) -> editor& = delete;
	~editor() override = default;
};

template <typename data_type>
class editor_of : public editor
{
public:
	using editor::editor;
	editor_of() = delete;
	editor_of(editor_of const&) = delete;
	editor_of(editor_of&&) = delete;
	auto operator=(editor_of const&) -> editor_of& = delete;
	auto operator=(editor_of&&) -> editor_of& = delete;
	~editor_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override
	{
		auto clone = std::make_unique<editor_of<data_type>>(get_widget_factory(), name());
		clone->copy(*this);
		return clone;
	}

	void copy(widget const& other) override
	{
		auto const& casted = dynamic_cast<editor_of<data_type> const&>(other);
		_data = casted._data;
		editor::copy(other);
	}

	void set_data_writer(data_writer<data_type> data)
	{
		_data = std::move(data);
	}

	virtual auto draw_contents(data_type& /*data*/) const -> bool
	{
		ImGui::Text("NO EDITOR IMPLEMENTATION");
		return false;
	}

private:
	data_writer<data_type> _data;

	void draw_contents() const final
	{
		if(auto* data = _data.read(); data != nullptr)
		{
			if(draw_contents(*data))
			{
				_data.write(data); // TODO pass in new data
			}
		}
		else
		{
			ImGui::Text("NO DATA");
		}
	}
};

template <>
class editor_of<void> : public editor
{
public:
	using editor::editor;
	editor_of() = delete;
	editor_of(editor_of const&) = delete;
	editor_of(editor_of&&) = delete;
	auto operator=(editor_of const&) -> editor_of& = delete;
	auto operator=(editor_of&&) -> editor_of& = delete;
	~editor_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override
	{
		auto clone = std::make_unique<editor_of<void>>(get_widget_factory(), name());
		clone->copy(*this);
		return clone;
	}

	void copy(widget const& other) override
	{
		editor::copy(other);
	}

private:
	void draw_contents() const final
	{
		ImGui::Text("VOID EDITOR");
	}
};

} // namespace clk::gui
#include "clk/gui/panel.hpp"
#include "clk/gui/widgets/widget_group.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <range/v3/algorithm.hpp>

namespace clk::gui
{
void panel::orphan(panel&& panel)
{
	panel._orphan = true;
	_orphaned_panels.emplace_back(std::move(panel));
}

panel::panel() : _title("Unnamed panel")
{
	update_title_with_id();
	register_self();
}

panel::panel(std::unique_ptr<clk::gui::widget>&& widget) : _widget(std::move(widget)), _title(_widget->name())
{
	update_title_with_id();
	register_self();
	_widget->disable_title();
}

panel::panel(std::unique_ptr<clk::gui::widget>&& widget, std::string_view title)
	: _widget(std::move(widget)), _title(std::string(title))
{
	update_title_with_id();
	register_self();
	_widget->disable_title();
}

panel::panel(panel const& other)
{
	*this = other;
	update_title_with_id();
	register_self();
}

panel::panel(panel&& other) noexcept : _id(other._id)
{
	*this = std::move(other);
	update_title_with_id();
	register_self();
}

auto panel::operator=(panel const& other) -> panel&
{
	if(this != &other)
	{
		_title = "Copy of " + other._title;
		_visible = other._visible;
		_flags = other._flags;
		_widget = other._widget->clone();
		update_title_with_id();
	}
	return *this;
}

panel::~panel()
{
	deregister_self();
}

auto panel::operator==(panel const& other) const noexcept -> bool
{
	return this->_id == other._id;
}

void panel::queue(action_type action_type) const
{
	_queued_actions.emplace_back(_id, action_type);
}

void panel::set_widget(std::unique_ptr<clk::gui::widget>&& widget) noexcept
{
	_widget = std::move(widget);
	_widget->disable_title();
}

auto panel::widget() const -> clk::gui::widget const*
{
	return _widget.get();
}

void panel::add_child_panel(panel&& child)
{
	_child_panels.push_back(std::move(child));
}

void panel::draw()
{
	if(!_visible || _widget == nullptr)
		return;
	ImGui::PushID(this);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, _opacity);
	if(ImGui::Begin(_title_with_id.c_str(), &_visible, _flags))
	{
		handle_context_menu();
		if(!_interactive && !ImGui::GetIO().KeyShift)
		{
			auto* current_window = ImGui::GetCurrentWindow();
			ImGui::SetWindowHitTestHole(current_window, current_window->Pos, current_window->Size);
		}

		_widget->set_interactivity(_interactive);
		_widget->draw();
		if(ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::GetIO().KeyShift &&
			ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Context menu");
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopID();
}

void panel::set_title(std::string_view title)
{
	if(title.empty() && _widget != nullptr)
		_title = _widget->name();
	else
		_title = std::string(title);
}

auto panel::title() const noexcept -> std::string_view
{
	return _title;
}

auto panel::visible() const noexcept -> bool
{
	return _visible;
}

void panel::toggle_visibility() noexcept
{
	_visible = !_visible;
}

void panel::show() noexcept
{
	_visible = true;
}

void panel::hide() noexcept
{
	_visible = false;
}

void panel::set_title_bar_visibility(bool visible) noexcept
{
	if(visible)
		_flags &= ~ImGuiWindowFlags_NoTitleBar;
	else
		_flags |= ImGuiWindowFlags_NoTitleBar;
}

auto panel::title_bar_is_visible() const noexcept -> bool
{
	return (_flags & ImGuiWindowFlags_NoTitleBar) == 0;
}

void panel::set_resizability(resizability mode) noexcept
{
	switch(mode)
	{
		case resizability::off:
			_flags |= ImGuiWindowFlags_NoResize;
			_flags &= ~ImGuiWindowFlags_AlwaysAutoResize;
			break;
		case resizability::manual:
			_flags &= ~ImGuiWindowFlags_NoResize;
			_flags &= ~ImGuiWindowFlags_AlwaysAutoResize;
			break;
		case resizability::automatic:
			_flags |= ImGuiWindowFlags_NoResize;
			_flags |= ImGuiWindowFlags_AlwaysAutoResize;
			break;
	}
}

auto panel::resizability_mode() const noexcept -> resizability
{
	bool no_resize = (_flags & ImGuiWindowFlags_NoResize) != 0;
	bool auto_resize = (_flags & ImGuiWindowFlags_AlwaysAutoResize) != 0;

	if(auto_resize)
		return resizability::automatic;
	if(!no_resize)
		return resizability::manual;

	return resizability::off;
}

void panel::set_movability(bool movable) noexcept
{
	if(movable)
		_flags &= ~ImGuiWindowFlags_NoMove;
	else
		_flags |= ImGuiWindowFlags_NoMove;
}

auto panel::is_movable() const noexcept -> bool
{
	return (_flags & ImGuiWindowFlags_NoMove) == 0;
}

void panel::set_docking(bool enabled) noexcept
{
	if(enabled)
		_flags &= ~ImGuiWindowFlags_NoDocking;
	else
		_flags |= ImGuiWindowFlags_NoDocking;
}

auto panel::docking_is_enabled() const noexcept -> bool
{
	return (_flags & ImGuiWindowFlags_NoDocking) == 0;
}

void panel::set_interactivity(bool interactive) noexcept
{
	_interactive = interactive;
}

auto panel::is_interactive() const noexcept -> bool
{
	return _interactive;
}

void panel::set_opactiy(float opacity) noexcept
{
	_opacity = opacity;
}

auto panel::opacity() const noexcept -> float
{
	return _opacity;
}

auto panel::generate_id() noexcept -> int
{
	static int next_available_id = 0;
	return next_available_id++;
}

void panel::register_self()
{
	_all_panels.push_back(this);
}

void panel::deregister_self() noexcept
{
	_all_panels.erase(ranges::remove(_all_panels, this), _all_panels.end());
}

void panel::update_title_with_id()
{
	_title_with_id = _title + "###" + std::to_string(_id);
}

void panel::handle_context_menu()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

	if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Context menu");
	}

	if(ImGui::BeginPopup("Context menu"))
	{
		ImGui::InputText(
			"Rename", &_title, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackAlways,
			[](ImGuiInputTextCallbackData* data) -> int {
				auto* panel = static_cast<clk::gui::panel*>(data->UserData);
				panel->update_title_with_id();
				return 0;
			},
			this);
		ImGui::SliderFloat("Opacity", &_opacity, 0.05f, 1.0f, "%.2f");

		if(auto flag = title_bar_is_visible(); ImGui::Checkbox("Title Bar", &flag))
			set_title_bar_visibility(flag);

		if(int value = static_cast<int>(resizability_mode());
			ImGui::SliderInt("Resizability", &value, 0, 2, value == 0 ? "off" : (value == 1 ? "manual" : "automatic")))
		{
			set_resizability(static_cast<resizability>(value));
		}

		if(auto flag = is_movable(); ImGui::Checkbox("Movable", &flag))
			set_movability(flag);

		if(auto flag = docking_is_enabled(); ImGui::Checkbox("Docking", &flag))
			set_docking(flag);

		if(auto flag = is_interactive(); ImGui::Checkbox("Interactive", &flag))
			set_interactivity(flag);

		if(ImGui::MenuItem("Duplicate"))
			queue(action_type::duplicate);

		if(_orphan && ImGui::MenuItem("Delete"))
			queue(action_type::remove);
		if(auto const* settings = _widget->get_settings(); settings != nullptr)
		{
			ImGui::Separator();
			ImGui::Text("Settings");
			ImGui::SameLine();
			if(ImGui::SmallButton("Extract to panel"))
				queue(action_type::extract_widget_settings);

			settings->draw();
		}
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
}

} // namespace clk::gui
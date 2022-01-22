#include "clk/gui/init.hpp"

#include "clk/gui/panel.hpp"
#include "clk/gui/widgets/composite_viewer.hpp"
#include "clk/gui/widgets/default_editors.hpp"
#include "clk/gui/widgets/default_viewers.hpp"
#include "clk/gui/widgets/graph_editor.hpp"
#include "clk/gui/widgets/graph_viewer.hpp"
#include "clk/gui/widgets/profiler_viewer.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/util/type_list.hpp"

#include <range/v3/algorithm.hpp>

namespace clk::gui
{
auto create_default_factory() -> std::shared_ptr<widget_factory>
{
	auto factory = std::make_shared<widget_factory>();

	using default_types = meta::type_list<bool, int, float, glm::vec2, glm::vec3, glm::vec4, clk::bounded<int>,
		clk::bounded<float>, clk::bounded<glm::vec2>, clk::bounded<glm::vec3>, clk::bounded<glm::vec4>, clk::color_rgb,
		clk::color_rgba, std::chrono::nanoseconds>;

	default_types::for_each([&](auto* dummy) {
		using current_type = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;
		factory->register_viewer<current_type, viewer_of<current_type>>();
		factory->register_editor<current_type, editor_of<current_type>>();
	});

	factory->register_viewer<clk::graph, graph_viewer>();
	factory->register_viewer<clk::profiler, profiler_viewer>();
	factory->register_editor<clk::graph, graph_editor>();

	return factory;
}

void draw()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	window_flags |= ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Main Window", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("Main Window Dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("View"))
		{
			for(auto* panel : clk::gui::panel::_all_panels)
				if(ImGui::MenuItem(panel->title().data(), "", panel->visible()))
					panel->toggle_visibility();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
	for(auto* panel : panel::_all_panels)
		panel->draw();

	for(auto [panel_id, action] : panel::_queued_actions)
	{
		auto it = ranges::find(panel::_all_panels, panel_id, &panel::_id);
		if(it == panel::_all_panels.end())
			continue;
		auto const* panel = *it;

		switch(action)
		{
			case panel::action_type::duplicate:
			{
				auto copy = *panel;
				panel::orphan(std::move(copy));
				break;
			}
			case panel::action_type::remove:
			{
				panel::_orphaned_panels.erase(ranges::remove_if(panel::_orphaned_panels,
												  [panel](auto const& other) {
													  return other._id == panel->_id;
												  }),
					panel::_orphaned_panels.end());
				break;
			}
		}
	}

	panel::_queued_actions.clear();
}
} // namespace clk::gui
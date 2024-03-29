#include "clk/gui/init.hpp"
#include "clk/base/graph.hpp"
#include "clk/gui/panel.hpp"
#include "clk/gui/widgets/default_editors.hpp"
#include "clk/gui/widgets/default_viewers.hpp"
#include "clk/gui/widgets/graph_editor.hpp"
#include "clk/gui/widgets/graph_viewer.hpp"
#include "clk/gui/widgets/profiler_editor.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/gui/widgets/widget_tree.hpp"
#include "clk/util/profiler.hpp"
#include "clk/util/type_list.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <imgui.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/remove_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace clk
{
class color_rgb;
class color_rgba;
template <typename T>
class bounded;
} // namespace clk

namespace clk::gui
{

template <typename DataType>
class editor_of;
template <typename DataType>
class viewer_of;

auto create_default_factory() -> std::shared_ptr<widget_factory>
{
    auto factory = std::make_shared<widget_factory>();

    using default_types = meta::type_list<bool, signed char, unsigned char, short int, unsigned short int, int,
        unsigned int, long int, unsigned long int, long long int, unsigned long long int, float, double, glm::vec2,
        glm::vec3, glm::vec4, clk::bounded<int>, clk::bounded<float>, clk::bounded<glm::vec2>, clk::bounded<glm::vec3>,
        clk::bounded<glm::vec4>, clk::color_rgb, clk::color_rgba, std::chrono::nanoseconds, char, std::string>;

    default_types::for_each([&](auto* dummy) {
        using current_type = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;
        factory->register_viewer<current_type, viewer_of<current_type>>();
        factory->register_editor<current_type, editor_of<current_type>>();
    });

    factory->register_viewer<clk::graph, graph_viewer>();
    factory->register_editor<clk::profiler, profiler_editor>();
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
        auto* panel = *it;

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
            case panel::action_type::extract_widget_settings:
            {
                if(panel->widget() != nullptr)
                {
                    if(auto const* settings = panel->widget()->get_settings(); settings != nullptr)
                    {
                        auto clone = settings->clone();
                        static_cast<widget_tree*>(clone.get())->set_draw_mode(widget_tree::draw_mode::tree_nodes);
                        panel->add_child_panel(clk::gui::panel(std::move(clone)));
                    }
                }
                break;
            }
        }
    }

    panel::_queued_actions.clear();
}
} // namespace clk::gui

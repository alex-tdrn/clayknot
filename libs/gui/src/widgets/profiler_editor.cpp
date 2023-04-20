#include "clk/gui/widgets/profiler_editor.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/gui/widgets/widget_tree.hpp"

#include <cstddef>
#include <utility>

namespace clk::gui
{

profiler_editor::profiler_editor(std::shared_ptr<widget_factory const> factory, std::string_view name)
    : editor_of<clk::profiler>(std::move(factory), name)
{
    auto const& f = get_widget_factory();
    auto& plot_settings = settings().get_subtree("Plot");
    plot_settings.add(f->create(_plot_height, "Height"));
    plot_settings.add(f->create(_plot_width, "Width"));
    plot_settings.add(f->create(_plot_alpha, "Fill opacity"));
}

auto profiler_editor::clone() const -> std::unique_ptr<widget>
{
    auto clone = std::make_unique<profiler_editor>(this->get_widget_factory(), this->name());
    clone->copy(*this);
    return clone;
}

void profiler_editor::copy(widget const& other)
{
    auto const& casted = dynamic_cast<profiler_editor const&>(other);
    _plot_height = casted._plot_height;
    editor_of<clk::profiler>::copy(other);
}

void profiler_editor::set_plot_height(float height)
{
    _plot_height = height;
}

auto profiler_editor::draw_contents(clk::profiler& profiler) const -> bool
{
    bool modified = false;

    if(auto active = profiler.is_active(); ImGui::Checkbox("Active", &active))
    {
        profiler.set_active(active);
        modified = true;
    }
    ImGui::SameLine();

    ImGui::Text("Sample count");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1);
    if(auto sample_count = static_cast<int>(profiler.samples().second.size());
        ImGui::DragInt("###Sample count", &sample_count, 1.0f, 2))
    {
        profiler.set_sample_count(static_cast<std::size_t>(std::max(sample_count, 2)));
        modified = true;
    }

    draw_helper<std::nano>(profiler);

    return modified;
}
} // namespace clk::gui

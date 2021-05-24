#include "clk/gui/init.hpp"

#include "clk/gui/widgets/graph_editor.hpp"
#include "clk/gui/widgets/graph_viewer.hpp"

namespace clk::gui
{
void init()
{
	editor::register_factory<clk::graph, graph_editor>();
	viewer::register_factory<clk::graph, graph_viewer>();
}
} // namespace clk::gui
#include "clk/gui/init.h"

#include "clk/gui/widgets/graph_editor.h"
#include "clk/gui/widgets/graph_viewer.h"

namespace clk::gui
{
void init()
{
	editor::register_factory<clk::graph, graph_editor>();
	viewer::register_factory<clk::graph, graph_viewer>();
}
} // namespace clk::gui
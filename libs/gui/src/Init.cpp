#include "rsp/gui/init.h"

#include "rsp/gui/widgets/graph_editor.h"
#include "rsp/gui/widgets/graph_viewer.h"

namespace rsp::gui
{
void init()
{
	editor::register_factory<rsp::graph, graph_editor>();
	viewer::register_factory<rsp::graph, graph_viewer>();
}
} // namespace rsp::gui
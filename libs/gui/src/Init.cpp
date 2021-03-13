#include "rsp/gui/Init.h"

#include "rsp/gui/widgets/GraphEditor.h"
#include "rsp/gui/widgets/GraphViewer.h"

namespace rsp::gui
{
void init()
{
	Editor::registerFactory<Graph, GraphEditor>();
	Viewer::registerFactory<Graph, GraphViewer>();
}
} // namespace rsp::gui
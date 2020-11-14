#include "rsp/nodes/DecomposeColor.h"

void rsp::nodes::DecomposeColor::update()
{
	*redComponent = color->r();
	*greenComponent = color->g();
	*blueComponent = color->b();
}

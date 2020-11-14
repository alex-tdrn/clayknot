#include "rsp/nodes/ValueToColor.h"

void rsp::nodes::ValueToColor::update()
{
	*color = rsp::ColorRGB{*value};
}

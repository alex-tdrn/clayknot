#include "rsp/nodes/MixColors.h"

void rsp::nodes::MixColors::update()
{
	*mixedColor = factor.get() * colorA.get() + (1 - factor.get()) * colorB.get();
}

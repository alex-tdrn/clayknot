#pragma once

namespace rsp::gui
{
inline int uniqueID()
{
	static int lastID = 1;
	return lastID++;
}

} // namespace rsp::gui
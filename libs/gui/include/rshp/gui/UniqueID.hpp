#pragma once

namespace rshp::gui
{
inline int uniqueID()
{
	static int lastID = 1;
	return lastID++;
}

} // namespace rshp::gui
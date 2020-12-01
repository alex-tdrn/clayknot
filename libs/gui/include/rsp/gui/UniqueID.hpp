#pragma once

namespace rsp::gui
{
inline auto uniqueID() -> int
{
	static int lastID = 1;
	return lastID++;
}

} // namespace rsp::gui
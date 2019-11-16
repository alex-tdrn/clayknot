#pragma once

inline int uniqueID()
{
	static int lastID = 1;
	return lastID++;
}
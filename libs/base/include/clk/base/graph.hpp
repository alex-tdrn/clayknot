#pragma once

#include "clk/base/node.hpp"

#include <memory>
#include <vector>

namespace clk
{
// TODO develop this into a useful class
using graph = std::vector<std::unique_ptr<clk::node>>;
} // namespace clk
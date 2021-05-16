#pragma once

#include "rsp/base/node.hpp"

#include <memory>
#include <vector>

namespace rsp
{
// TODO develop this into a useful class
using graph = std::vector<std::unique_ptr<rsp::node>>;
} // namespace rsp
#pragma once

#include "rsp/base/Node.hpp"

#include <memory>
#include <vector>

namespace rsp
{
// TODO develop this into a useful class
using Graph = std::vector<std::unique_ptr<Node>>;
} // namespace rsp
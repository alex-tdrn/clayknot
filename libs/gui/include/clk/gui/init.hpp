#pragma once

#include <memory>

namespace clk::gui
{
class widget_factory;

auto create_default_factory() -> std::shared_ptr<widget_factory>;
void draw();
} // namespace clk::gui
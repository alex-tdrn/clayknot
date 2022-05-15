#include "clk/clkvk/init.hpp"
#include "clk/clkvk/instance.hpp"
#include "clk/clkvk/window.hpp"

namespace clk::clkvk
{
void init()
{
	clk::algorithm::register_factory<clkvk::window>();
	clk::algorithm::register_factory<clkvk::instance>();
}
} // namespace clk::clkvk
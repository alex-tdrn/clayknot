#include "clk/clkvk/init.hpp"
#include "clk/clkvk/application_info.hpp"
#include "clk/clkvk/instance.hpp"
#include "clk/clkvk/window.hpp"

namespace clk::clkvk
{
void init()
{
	clk::algorithm::register_factory<clkvk::application_info>();
	clk::algorithm::register_factory<clkvk::instance>();
	clk::algorithm::register_factory<clkvk::window>();
}
} // namespace clk::clkvk
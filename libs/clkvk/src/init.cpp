#include "clk/clkvk/init.hpp"
#include "clk//clkvk/required_windowing_extensions.hpp"
#include "clk/base/algorithm.hpp"
#include "clk/clkvk/application_info.hpp"
#include "clk/clkvk/enumerate_instance_extensions.hpp"
#include "clk/clkvk/instance.hpp"
#include "clk/clkvk/window.hpp"

namespace clk::clkvk
{
void init()
{
	clk::algorithm::register_factory<clkvk::application_info>();
	clk::algorithm::register_factory<clkvk::instance>();
	clk::algorithm::register_factory<clkvk::window>();
	clk::algorithm::register_factory<clkvk::required_windowing_extensions>();
	clk::algorithm::register_factory<clkvk::enumerate_instance_extensions>();
}
} // namespace clk::clkvk
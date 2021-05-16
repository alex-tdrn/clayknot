add_library(rendershop-util INTERFACE)

target_sources(
	rendershop-util
	INTERFACE "include/rsp/util/bounded.hpp"
				"include/rsp/util/color_rgb.hpp"
				"include/rsp/util/color_rgba.hpp"
				"include/rsp/util/data_type_name.hpp"
				"include/rsp/util/timestamp.hpp"
				"include/rsp/util/time_unit.hpp"
				"include/rsp/util/type_list.hpp"
)

target_include_directories(rendershop-util INTERFACE "include")
target_link_libraries(rendershop-util INTERFACE glm::glm)

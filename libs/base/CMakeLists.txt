add_library(rendershop-base INTERFACE)

target_sources(rendershop-base
	INTERFACE
	"include/rsp/base/Node.hpp"
	"include/rsp/base/Data.hpp"
	"include/rsp/base/Port.hpp"
)

target_include_directories(rendershop-base
	INTERFACE
	"include"
)

target_link_libraries(rendershop-base
	INTERFACE
	rendershop-util
)
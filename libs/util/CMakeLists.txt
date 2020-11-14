add_library(rendershop-util INTERFACE)

target_sources(rendershop-util
	INTERFACE
	"include/rsp/util/Bounded.hpp"
	"include/rsp/util/ColorRGB.hpp"
	"include/rsp/util/ColorRGBA.hpp"
	"include/rsp/util/DataTypeName.hpp"
	"include/rsp/util/Meta.hpp"
	"include/rsp/util/Observable.hpp"
	"include/rsp/util/Timestamp.hpp"
	"include/rsp/util/TimeUnit.hpp"
	"include/rsp/util/TypeList.hpp"
)

target_include_directories(rendershop-util
	INTERFACE
	"include"
)
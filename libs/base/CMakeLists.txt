add_library(rendershop-base
	"src/port/InputEventPort.cpp"
	"include/rsp/base/port/InputEventPort.h"

	"src/port/OutputEventPort.cpp"
	"include/rsp/base/port/OutputEventPort.h"
	
	"include/rsp/base/port/DataPort.hpp"
	"include/rsp/base/node/AbstractNode.hpp"
	"include/rsp/base/port/Port.hpp"
	"include/rsp/base/node/AbstractSink.hpp"
	"include/rsp/base/node/AbstractSource.hpp"
	"include/rsp/base/node/EventNode.hpp"
	"include/rsp/base/port/InputDataPort.hpp"
	"include/rsp/base/port/InputPort.hpp"
	"include/rsp/base/port/OutputDataPort.hpp"
	"include/rsp/base/port/OutputPort.hpp"
	"include/rsp/base/node/Node.hpp"
	"include/rsp/base/node/Sink.hpp"
	"include/rsp/base/node/Source.hpp"
	"include/rsp/base/Timestamp.hpp"
	"include/rsp/base/Utility.hpp"
	"include/rsp/base/ColorRGB.hpp"
	"include/rsp/base/ColorRGBA.hpp"
	"include/rsp/base/Bounded.hpp"
	"include/rsp/base/TimeUnit.hpp"
	"include/rsp/base/Observable.hpp"
	"include/rsp/base/port/EventPort.hpp"
)

target_include_directories(rendershop-base
	PUBLIC
	"include"
	"../../external/glm/include"
)
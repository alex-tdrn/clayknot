add_library(rendershop-base
	"src/port/InputEventPort.cpp"
	"include/rshp/base/port/InputEventPort.h"

	"src/port/OutputEventPort.cpp"
	"include/rshp/base/port/OutputEventPort.h"
	
	"include/rshp/base/port/DataPort.hpp"
	"include/rshp/base/node/AbstractNode.hpp"
	"include/rshp/base/port/Port.hpp"
	"include/rshp/base/node/AbstractSink.hpp"
	"include/rshp/base/node/AbstractSource.hpp"
	"include/rshp/base/node/EventNode.hpp"
	"include/rshp/base/port/InputDataPort.hpp"
	"include/rshp/base/port/InputPort.hpp"
	"include/rshp/base/port/OutputDataPort.hpp"
	"include/rshp/base/port/OutputPort.hpp"
	"include/rshp/base/node/Node.hpp"
	"include/rshp/base/node/Sink.hpp"
	"include/rshp/base/node/Source.hpp"
	"include/rshp/base/Timestamp.hpp"
	"include/rshp/base/Utility.hpp"
	"include/rshp/base/ColorRGB.hpp"
	"include/rshp/base/ColorRGBA.hpp"
)

target_include_directories(rendershop-base
	PUBLIC
	"include"
	"../../external/glm/include"
)
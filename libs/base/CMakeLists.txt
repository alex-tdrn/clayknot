add_library(rendershop-base
	"src/stupidHeaderOnlyWorkaroundSoThatTheHeadersGetShownCorrectlyInIDEs.cpp"
	
	"include/rshp/base/port/DataPort.hpp"
	"include/rshp/base/node/AbstractNode.hpp"
	"include/rshp/base/port/Port.hpp"
	"include/rshp/base/node/AbstractSink.hpp"
	"include/rshp/base/node/AbstractSource.hpp"
	"include/rshp/base/node/EventNode.hpp"
	"include/rshp/base/port/InputDataPort.hpp"
	"include/rshp/base/port/InputEventPort.hpp"
	"include/rshp/base/port/InputPort.hpp"
	"include/rshp/base/port/OutputDataPort.hpp"
	"include/rshp/base/port/OutputEventPort.hpp"
	"include/rshp/base/port/OutputPort.hpp"
	"include/rshp/base/node/Node.hpp"
	"include/rshp/base/node/Sink.hpp"
	"include/rshp/base/node/Source.hpp"
	"include/rshp/base/Timestamp.hpp"
	"include/rshp/base/Utility.hpp"
)

target_include_directories(rendershop-base
	PUBLIC
	"include"
)
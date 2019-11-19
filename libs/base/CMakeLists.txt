add_library(rendershop-base
	"src/stupidHeaderOnlyWorkaroundSoThatTheHeadersGetShownCorrectlyInIDEs.cpp"
	"include/rshp/base/AbstractDataPort.hpp"
	"include/rshp/base/AbstractPipe.hpp"
	"include/rshp/base/AbstractPort.hpp"
	"include/rshp/base/AbstractSink.hpp"
	"include/rshp/base/AbstractSource.hpp"
	"include/rshp/base/EventPipe.hpp"
	"include/rshp/base/InputDataPort.hpp"
	"include/rshp/base/InputEventPort.hpp"
	"include/rshp/base/InputPort.hpp"
	"include/rshp/base/OutputDataPort.hpp"
	"include/rshp/base/OutputEventPort.hpp"
	"include/rshp/base/OutputPort.hpp"
	"include/rshp/base/Pipe.hpp"
	"include/rshp/base/Sink.hpp"
	"include/rshp/base/Source.hpp"
	"include/rshp/base/Timestamp.hpp"
	"include/rshp/base/Utility.hpp"
)

target_include_directories(rendershop-base
	PUBLIC
	"include"
)

target_compile_features(rendershop-base PUBLIC cxx_std_17)
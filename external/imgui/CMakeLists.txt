add_library(
	imgui
	"src/imgui.cpp"
	"src/imgui_demo.cpp"
	"src/imgui_draw.cpp"
	"src/imgui_impl_glfw.cpp"
	"src/imgui_impl_opengl3.cpp"
	"src/imgui_widgets.cpp"
	"include/imconfig.h"
	"include/imgui.h"
	"include/imgui_impl_glfw.h"
	"include/imgui_impl_opengl3.h"
	"include/imgui_internal.h"
	"include/imstb_rectpack.h"
	"include/imstb_textedit.h"
	"include/imstb_truetype.h"
)

target_include_directories(imgui SYSTEM PUBLIC "include")

target_link_libraries(imgui PUBLIC glfw::glfw glad::glad)

target_compile_definitions(imgui PUBLIC "IMGUI_IMPL_OPENGL_LOADER_GLAD")

add_library(clayknot::imgui ALIAS imgui)

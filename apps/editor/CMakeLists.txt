add_executable(editor "src/main.cpp")

target_link_libraries(editor PRIVATE clayknot::base clayknot::algorithms clayknot::gui glad::glad glfw)

install(TARGETS editor)
install(FILES $<TARGET_RUNTIME_DLLS:editor> TYPE BIN)

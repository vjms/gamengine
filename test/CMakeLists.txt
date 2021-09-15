add_executable(gl_load_test gl_load.cpp)

find_package(glfw3)
find_package(OpenGL)
find_package(glad)
find_package(glm)
target_link_libraries(gl_load_test glfw::glfw glad::glad opengl32 glm::glm)
# Suppress 4099 warning
set_target_properties(gl_load_test PROPERTIES LINK_FLAGS "/ignore:4099")
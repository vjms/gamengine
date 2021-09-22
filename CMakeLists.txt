cmake_minimum_required(VERSION 3.13)

project(proj CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")

add_definitions(-DGLM_FORCE_SILENT_WARNINGS)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# Set compiler warnings
if (MSVC)
    #SET (CMAKE_CXX_FLAGS_DEBUG "/W4 /WX /EHsc")
    #SET (CMAKE_CXX_FLAGS_RELEASE "/W4 /WX /O2")
    #STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")
    add_compile_options(/W4 /WX)
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /RTCs /O2")

else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()



# Conan setup
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE ${PROJECT_SOURCE_DIR}
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})



# Executable
add_executable(${PROJECT_NAME})
add_subdirectory(src)


# Tests
#add_subdirectory(test)


# Libraries
find_package(fmt)
find_package(glfw3)
find_package(OpenGL)
find_package(glad)
find_package(glm)
find_package(assimp)
target_link_libraries(${PROJECT_NAME}
    fmt::fmt 
    glfw::glfw 
    glad::glad 
    opengl32 
    glm::glm 
    assimp::assimp)

# Suppress 4099 warning
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4099 ")
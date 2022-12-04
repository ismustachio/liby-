cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(liby++ VERSION 0.1)

#set(CMAKE_BUILD_TYPE Debug)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glslang REQUIRED)
#find_package(SPIRV REQUIRED)
set(GLSLC /usr/local/bin/glslc)
#set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/lib -lglfw")
#add_compile_options("-L/usr/local/lib")
#link_libraries("-L/usr/local/lib")


# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true math/src/*.hpp math/src/*.cpp renderer/src/*.cpp renderer/src/*.hpp)
set(SRC main.cpp ${SOURCES})
set(GLSLC /usr/local/bin/glslc)

set(CMAKE_EXE_LINKER_FLAGS_INIT -L/usr/local/lib)

file(GLOB_RECURSE GLSL_SOURCE_FILES
  "shaders/*.frag"
  "shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
    COMMAND ${GLSLC} ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)


#:add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SRC})
add_custom_target(Shaders DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(${PROJECT_NAME} Shaders)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_BINARY_DIR}/shaders"
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)
target_include_directories(${PROJECT_NAME} PRIVATE math/src)
target_include_directories(${PROJECT_NAME} PRIVATE renderer/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${glfw3_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glfw)
#target_link_libraries(${PROJECT_NAME} glslang)
#target_link_libraries(${PROJECT_NAME} SPIRV)
#target_link_libraries(${PROJECT_NAME} shaderc_shared)
#target_include_directories(${PROJECT_NAME} PUBLIC ${glslang_INCLUDE_DIR})





set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

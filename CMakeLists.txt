cmake_minimum_required(VERSION 3.5)

project(quark2)

set(CMAKE_CXX_STANDARD 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables
# `sources` and `data`

file(GLOB_RECURSE sources       src/main/*.cpp src/main/*.h)

file(GLOB_RECURSE sources_test  src/test/*.cpp)
file(GLOB_RECURSE data          src/resources/*.py)
file(GLOB_RECURSE shader        src/resources/*.frag src/resources/*.vert)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources_test})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${data})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${shader})

###############################################################################
## cmake auto moc fix
#list (APPEND sources "engine_automoc.cpp")
#set_property (SOURCE "engine_automoc.cpp" PROPERTY GENERATED TRUE)


###############################################################################
## target definitions #########################################################
###############################################################################

add_executable(engine ${sources} ${data})

# just for example add some compiler flags
# target_compile_options(engine PUBLIC -Wall)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(engine PUBLIC src/main)

# this copies all resource files in the build directory
# we need this, because we want to work with paths relative to the executable
#file(COPY ${data} DESTINATION resources)
# shaders are generated into target dir.. see further down..
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.jpg")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.png")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.obj")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.py")


###############################################################################
## shader target ##############################################################
###############################################################################

add_custom_target(
  shader
  DEPENDS ${shader}
)

add_custom_command(
  TARGET shader
  POST_BUILD
  COMMAND glslangValidator -V ${shader}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/resources/shader/test/
)

add_dependencies(engine shader)

###############################################################################
## dependencies ###############################################################
###############################################################################

if (WIN32)
  #Python
  #find_package(PythonLibs REQUIRED)
  set(PYTHON_INCLUDE_DIRS C:/Users/AJ/AppData/Local/Programs/Python/Python310/include)
  set(PYTHON_LIBRARIES C:/Users/AJ/AppData/Local/Programs/Python/Python310/libs/python3.lib C:/Users/AJ/AppData/Local/Programs/Python/Python310/libs/python310.lib)
  include_directories(${PYTHON_INCLUDE_DIRS})
  target_link_libraries(engine PUBLIC ${PYTHON_LIBRARIES})

  add_subdirectory(thirdparty/pybind11)
  target_link_libraries(engine PRIVATE pybind11::embed)

  #opengl
  #target_link_libraries(engine PUBLIC opengl32)

  #Vulkan
  target_include_directories(engine PUBLIC C:/VulkanSDK/1.3.204.1/Include)
  target_link_libraries(engine PUBLIC debug C:/VulkanSDK/1.3.204.1/Lib/vulkan-1.lib)
  target_link_libraries(engine PUBLIC optimized C:/VulkanSDK/1.3.204.1/Lib/vulkan-1.lib)

  #GLFW
  target_include_directories(engine PUBLIC thirdparty/glfw/include)
  target_link_libraries(engine PUBLIC debug  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/windows/bin/Debug/glfw3.lib)
  target_link_libraries(engine PUBLIC optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/windows/bin/Release/glfw3.lib)

  #GLM
  include_directories(engine PUBLIC thirdparty/glm/include)

  #STB
  include_directories(engine PUBLIC thirdparty/stb/include)

  #TINY OBJ LOADER
  include_directories(engine PUBLIC thirdparty/tinyobjloader/include)

  #PORTAUDIO
  #target_include_directories(engine PUBLIC thirdparty/portaudio/include)
  #target_link_libraries(engine PUBLIC debug  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/portaudio/bin/Debug/portaudio_static_x64.lib)
  #target_link_libraries(engine PUBLIC optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/portaudio/bin/Release/portaudio_static_x64.lib)

  #LIBSNDFILE
  #target_include_directories(engine PUBLIC thirdparty/libsndfile/include)
  #target_link_libraries(engine PUBLIC debug  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libsndfile/bin/Release/libsndfile-1.lib)
  #target_link_libraries(engine PUBLIC optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libsndfile/bin/Release/libsndfile-1.lib)



endif (WIN32)


if (UNIX)
  #Python
  #find_package(PythonLibs REQUIRED)
  set(PYTHON_INCLUDE_DIRS /usr/include/python3.8)
  #set(PYTHON_LIBRARIES /usr/lib/x86_64-linux-gnu/libpython3.8.so)
  include_directories(${PYTHON_INCLUDE_DIRS})
  target_link_libraries(engine PUBLIC ${PYTHON_LIBRARIES})

  add_subdirectory(thirdparty/pybind11)
  target_link_libraries(engine PRIVATE pybind11::embed)

  #Vulkan
  target_include_directories(engine PUBLIC thirdparty/vulkan/include)
  target_link_libraries(engine PUBLIC debug ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vulkan/linux/bin/Release/libvulkan.so)
  target_link_libraries(engine PUBLIC optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vulkan/linux/bin/Release/libvulkan.so)

  #GLFW
  target_include_directories(engine PUBLIC thirdparty/glfw/include)
  target_link_libraries(engine PUBLIC debug  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/linux/bin/Debug/libglfw3.a)
  target_link_libraries(engine PUBLIC optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/linux/bin/Release/libglfw3.a)

  #GLM
  include_directories(engine PUBLIC thirdparty/glm/include)

  #STB
  include_directories(engine PUBLIC thirdparty/stb/include)

  #TINY OBJ LOADER
  include_directories(engine PUBLIC thirdparty/tinyobjloader/include)

  #bla bla could also search for glfw but now i see what libs are linked...
  target_link_libraries(engine PUBLIC -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lpython3.8)

 endif (UNIX)


#remove the console
#set_target_properties(engine PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(engine PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")

###############################################################################
## packaging ##################################################################
###############################################################################

# all install commands get the same destination. this allows us to use paths
# relative to the executable.
#install(TARGETS engine DESTINATION engine_destination)

# this is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package
#install(DIRECTORY resources DESTINATION engine_destination)

# now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here
#set(CPACK_PACKAGE_NAME "Engine")
#set(CPACK_PACKAGE_VERSION "1.0.0")

# we don't want to split our program up into several things
#set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
#include(CPack)

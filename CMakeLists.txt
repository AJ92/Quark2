cmake_minimum_required(VERSION 3.1)

project(engine)

# Basically QT setting

if (UNIX) #msvc compiler does not need this flag ...
  # new cpp standard
  set(CMAKE_CXX_STANDARD 14)
endif (UNIX)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)



###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables
# `sources` and `data`
file(GLOB_RECURSE sources       src/main/*.cpp src/main/*.h
								src/main/base/*.cpp src/main/base/*.h
								src/main/base/datastructures/*.cpp src/main/base/datastructures/*.h
								src/main/base/datastructures/components/*.cpp src/main/base/datastructures/components/*.h	
								src/main/base/systems/*.cpp src/main/base/systems/*.h)

file(GLOB_RECURSE sources_test  src/test/*.cpp)
file(GLOB_RECURSE data          src/resources/*)
# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

###############################################################################
## target definitions #########################################################
###############################################################################


add_executable(engine ${sources})
# add the data to the target, so it becomes visible in some IDE
# add_executable(engine ${data})

# just for example add some compiler flags
target_compile_options(engine PUBLIC -Wall)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(engine PUBLIC src/main)

# this copies all resource files in the build directory
# we need this, because we want to work with paths relative to the executable
#file(COPY ${data} DESTINATION resources)
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.spv")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.jpg")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.png")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.obj")
file(COPY src/resources/ DESTINATION resources FILES_MATCHING PATTERN "*.py")

###############################################################################
## dependencies ###############################################################
###############################################################################


#QT
target_link_libraries(engine PUBLIC Qt5::Widgets)

#Python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(engine PUBLIC ${PYTHON_LIBRARIES})


include_directories(engine PUBLIC thirdparty/pybind11/include)

if (WIN32)
    #opengl
    #target_link_libraries(engine PUBLIC opengl32)

    #Vulkan
    target_include_directories(engine PUBLIC C:/VulkanSDK/1.0.46.0/Include)
    target_link_libraries(engine PUBLIC debug C:/VulkanSDK/1.0.46.0/Lib/vulkan-1.lib)
    target_link_libraries(engine PUBLIC optimized C:/VulkanSDK/1.0.46.0/Lib/vulkan-1.lib)

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
    target_link_libraries(engine PUBLIC -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor)
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

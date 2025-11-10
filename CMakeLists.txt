cmake_minimum_required(VERSION 3.16) 
project(Visual)


# --- Find OpenCV ---
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


#Build Library
add_library(VisualLib 
src/utils.cpp
src/manipulator.cpp
) #For files in src/

target_include_directories(VisualLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(VisualLib PUBLIC cxx_std_20)
target_link_libraries(VisualLib ${OpenCV_LIBS})

#Define config path
add_definitions(-DCONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/config/params.yaml")

#Build main app
add_executable(VisualApp src/main.cpp)
target_compile_features(VisualApp PUBLIC cxx_std_20)
target_link_libraries(VisualApp VisualLib ${OpenCV_LIBS})


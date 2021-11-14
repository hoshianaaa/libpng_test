cmake_minimum_required(VERSION 3.0.2)
project(libpng_test)

find_package(catkin REQUIRED)
find_package(PNG REQUIRED) #追加

message(libpng version: ${PNG_VERSION_STRING}) #追加

catkin_package(
)

include_directories(
 include
 ${PNG_INCLUDE_DIRS} #追加
)

add_executable(read src/read.cpp) #追加
target_link_libraries(read
  ${PNG_LIBRARIES} #追加
)

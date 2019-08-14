project(findBlocks)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_COMPILER /usr/bin/g++)

find_package(OpenCV QUIET)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
  )


set(srcs
  ${PROJECT_SOURCE_DIR}/src/blocks.cpp
  )

add_executable(${PROJECT_NAME} ${srcs} "main.cpp")

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    )

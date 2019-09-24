project(sphere)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_COMPILER /usr/bin/g++)

find_package(Eigen3 REQUIRED)
find_package(Sophus REQUIRED)

include_directories(
  ${EIGEN3_INCLUDE_DIR}
  ${Sophus_INCLUDE_DIRS}
  )

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
  ${EIGEN3_LIBS}
  ${Sophus_LIBRARIES}
  )

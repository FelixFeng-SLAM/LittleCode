cmake_minimum_required(VERSION 2.8)

project(sphericalStars)

include_directories(
  ${PROJECT_SOURCE_DIR}
  )

set(srcs
    ${PROJECT_SOURCE_DIR}/mref.cpp
    ${PROJECT_SOURCE_DIR}/mdata.cpp
    ${PROJECT_SOURCE_DIR}/mio.cpp
    ${PROJECT_SOURCE_DIR}/calculate.cpp
  )

add_executable(${PROJECT_NAME} ${srcs}  "main.cpp")

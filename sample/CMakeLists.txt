project(sample C CXX)
cmake_minimum_required(VERSION 2.8)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

# Possible values of CMAKE_BUILD_TYPE are empty, Debug, Release, RelWithDebInfo and MinSizeRel
set(CMAKE_BUILD_TYPE Debug)
message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
message(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")

# mongodb C driver
find_package(PkgConfig)
pkg_check_modules(LIBMONGOC REQUIRED libmongoc-1.0)
include_directories(${LIBMONGOC_INCLUDE_DIRS})
# rapidjson
include_directories(${PROJECT_SOURCE_DIR}/../thirdparty/rapidjson/include)
# mongoodm
set(MONGOODM_DIR ${PROJECT_SOURCE_DIR}/../../mongo_obj_doc_mapper)
set(MONGOODM_INCLUDE ${MONGOODM_DIR})
include_directories(${MONGOODM_INCLUDE})

set(SRCS color.cpp account.cpp account_name.cpp account_clothes_member.cpp sample.cpp)

add_executable(sample ${SRCS} ${MONGOODM_DIR}/mongoodm/mongoodm_all.cpp)
set_target_properties(sample PROPERTIES OUTPUT_NAME sample)
target_link_libraries(sample ${LIBMONGOC_LIBRARIES})


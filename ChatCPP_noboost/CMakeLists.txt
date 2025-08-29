cmake_minimum_required(VERSION 3.16)

project(Chat LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt6Modules Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS ${Qt6Modules})

file(GLOB_RECURSE SRC_FILES     sources/*.cpp)
file(GLOB_RECURSE HEADERS_FILES sources/*.h)
file(GLOB_RECURSE RC_FILES      sources/*.qrc)
file(GLOB_RECURSE UI_FILES      Forms/*.ui sources/*.ui)

add_executable(Chat
  ${SRC_FILES}
  ${HEADERS_FILES}
  ${RC_FILES}
  ${UI_FILES}
)

target_include_directories(Chat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)

target_link_libraries(Chat PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Chat
  PUBLIC
  $CHAT_LIBRARIES
)

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
find_package(Qt6 REQUIRED COMPONENTS ${Qt6Modules} LinguistTools)

file(GLOB_RECURSE SRC_FILES     sources/*.cpp)
file(GLOB_RECURSE HEADERS_FILES sources/*.h)
file(GLOB_RECURSE RC_FILES      sources/*.qrc)
file(GLOB_RECURSE UI_FILES      Forms/*.ui sources/*.ui)

# ===== переводы (Linux) =====
set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/my_ru.ts)
set(TS_SOURCES ${SRC_FILES} ${HEADERS_FILES} ${UI_FILES})

add_custom_target(translations)

foreach(_ts_file ${TS_FILES})
  get_filename_component(_ts_name ${_ts_file} NAME_WE)
  add_custom_command(
    OUTPUT "${_ts_file}"
    DEPENDS ${TS_SOURCES}
    COMMAND $<TARGET_FILE:Qt6::lupdate> -recursive
            ${CMAKE_CURRENT_SOURCE_DIR} -ts ${_ts_file}
    VERBATIM
  )
  add_custom_target(translate_${_ts_name} DEPENDS ${_ts_file})
  add_dependencies(translations translate_${_ts_name})
endforeach()

qt6_add_translation(QM_FILES ${TS_FILES})
# ============================

add_executable(Chat
  ${SRC_FILES}
  ${HEADERS_FILES}
  ${RC_FILES}
  ${UI_FILES}
  ${QM_FILES}
)

target_include_directories(Chat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)

target_link_libraries(Chat PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Chat PUBLIC $CHAT_LIBRARIES)

cmake_minimum_required(VERSION 3.8)

project(vocabulazer)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
	file/text_extractor.cpp
	text/text_converter
)

set(SOURCE_DIR src)

foreach(SOURCE ${SOURCE_FILES})
    list(APPEND SOURCES ${SOURCE_DIR}/${SOURCE})
endforeach()

add_executable(${PROJECT_NAME} app/main.cpp ${SOURCES})
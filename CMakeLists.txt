cmake_minimum_required(VERSION 3.8)

project(vocabulazer)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost 1.74.0 COMPONENTS locale filesystem REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc io)

# Poppler library location
set(Poppler_LIBRARY /usr/local/lib/libpoppler-cpp.so)

set(SOURCE_FILES
	file/extractor_factory.cpp
	file/text_extractor.cpp
	file/pdf_extractor.cpp
	text/text_converter.cpp
)

set(SOURCE_DIR src)

foreach(SOURCE ${SOURCE_FILES})
    list(APPEND SOURCES ${SOURCE_DIR}/${SOURCE})
endforeach()

add_executable(${PROJECT_NAME} app/main.cpp ${SOURCES})

target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES} ${Poppler_LIBRARY} ${ICU_LIBRARIES})
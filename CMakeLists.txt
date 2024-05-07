cmake_minimum_required(VERSION 3.8)

project(vocabulazer)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost 1.74.0 COMPONENTS locale filesystem REQUIRED)


set(ICU_INCLUDE_DIRS /opt/homebrew/opt/icu4c/include)
include_directories(${ICU_INCLUDE_DIRS})

find_package(ICU COMPONENTS uc io REQUIRED)

# set(Poppler_LIBRARY /usr/local/lib/libpoppler-cpp.so)
set(Poppler_LIBRARY /Users/c/Downloads/poppler-24.05.0/build/cpp/libpoppler-cpp.dylib)


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

target_link_libraries (${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ${Poppler_LIBRARY} ${ICU_LIBRARIES})
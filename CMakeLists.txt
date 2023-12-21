cmake_minimum_required(VERSION 3.18)
project(image_stitching)

find_package(OpenCV REQUIRED)

add_library(${PROJECT_NAME}
    SHARED
        src/image_stitcher.cpp
        inc/image_stitching/image_stitcher.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC inc
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${OpenCV_LIBS}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

install(DIRECTORY inc/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/segue/include)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/segue/lib)

if(BUILD_UNITTEST)
    #TODO: add_subdirectory(test)
    add_subdirectory(demo)
endif()



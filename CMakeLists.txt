cmake_minimum_required (VERSION 2.6)

project (wire)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-g")
# set (CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

option(ENABLE_DEBUG "Set to ON to enable ParseLib debug features" ON)

#create a lib
#--------------------------
file( 
    GLOB Mods 
    ${PROJECT_SOURCE_DIR}/src/*.c
    )

add_library(wire ${Mods})


#create example project
#--------------------------
file( 
    GLOB ExampleMods 
    ${PROJECT_SOURCE_DIR}/Examples/main.c
    )

add_executable(wire_example_target ${ExampleMods})

if(ENABLE_DEBUG)

file( 
    GLOB parse_includes
    ${PROJECT_SOURCE_DIR}/../include/*.h
    ${PROJECT_SOURCE_DIR}/../include/*.hpp
    ${PROJECT_SOURCE_DIR}/../include/concrete/*.hpp
    )
endif()


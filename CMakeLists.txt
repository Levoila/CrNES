cmake_minimum_required( VERSION 3.2.2 )
project( CrNES )

#set( CMAKE_VERBOSE_MAKEFILE ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/nes ${CMAKE_CURRENT_SOURCE_DIR}/include/nes/mapper )

find_package( Qt5Widgets REQUIRED )
find_package( OpenGL REQUIRED )

include_directories( ${HEADER_DIR} ${OPENGL_INCLUDE_DIR} )
file( GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h" )

add_executable( CrNES ${SOURCES} )

target_link_libraries( CrNES Qt5::Widgets Qt5::Gui ${OPENGL_LIBRARIES} )

if ( CMAKE_COMPILER_IS_GNUCC )
    set_property( TARGET CrNES APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ( CMAKE_COMPILER_IS_GNUCC )

if ( MSVC )
    set_property ( TARGET CrNES APPEND_STRING PROPERTY COMPILE_FLAGS /W3 )
endif ( MSVC )

add_custom_command( TARGET CrNES PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/testRoms $<TARGET_FILE_DIR:CrNES>/testRoms )

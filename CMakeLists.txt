cmake_minimum_required( VERSION 3.2.2 )
project( CrNES )

set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include )

find_package( Qt5Widgets REQUIRED )
find_package( OpenGL REQUIRED )

include_directories( ${HEADER_DIR} ${OPENGL_INCLUDE_DIR} )
file( GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h" )

add_executable( CrNES ${SOURCES} )

target_link_libraries( CrNES Qt5::Widgets Qt5::Gui ${OPENGL_LIBRARIES} )

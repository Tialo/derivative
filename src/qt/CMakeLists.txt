set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/src/qt/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/qt/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/qt/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the library
add_library(
	qt
	"${FORMS_DIR}/mainwindow.ui"
	"${INCLUDE_DIR}/mainwindow.h"
	"${SOURCE_DIR}/mainwindow.cpp"
)

# Add the target includes for MY_PROJECT 
target_include_directories(qt PRIVATE ${FORMS_DIR})
target_include_directories(qt PRIVATE ${INCLUDE_DIR})
target_include_directories(qt PRIVATE ${SOURCE_DIR})

target_link_libraries(qt parser)
target_link_libraries(qt functions)
target_link_libraries(qt Qt6::Widgets)
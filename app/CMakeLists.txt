# CMakeLists.txt for the app module

set(APPNAME ${PROJECT_NAME})

message(STATUS "[Application: ${APPNAME}] Module Processing...")

# Collect files without having to explicitly list each header and source file
file(GLOB APP_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

file(GLOB APP_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

  # Collect Module Resource files *.ui *.qrc
file(GLOB LIB_RESOURCES
  "${CMAKE_SOURCE_DIR}/resources/*.ui"
  "${CMAKE_SOURCE_DIR}/resources/*.rc"
  "${CMAKE_SOURCE_DIR}/resources/*.qrc")

file(GLOB LIB_EXTRA
  "${CMAKE_SOURCE_DIR}/resources/background.gif")

qt_add_resources(RESOURCES ${LIB_RESOURCES})

# Create named folders for the sources within the project
source_group("header" FILES ${APP_HEADERS})
source_group("src" FILES ${APP_SOURCES})

# Set Properties->General->Configuration Type to Dynamic Library (.dll/.so/.dylib)
add_executable(${APPNAME} ${APP_HEADERS} ${APP_SOURCES} ${RESOURCES})

target_include_directories(${APPNAME} PUBLIC
						   ${CMAKE_CURRENT_SOURCE_DIR}
						   ${CMAKE_SOURCE_DIR}/src/AppManager
						   )
############# etc....

# Add any dependencies or compile options specific to lock6g
target_link_libraries(${APPNAME} PRIVATE ${QT_LIBRARIES} qt-material-widgets AppManager)
############# etc....

# Set the compile warnings options if enabled
if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET ${APPNAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()


install(TARGETS ${APPNAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin )


set_target_properties(qt-material-widgets PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	POSITION_INDEPENDENT_CODE ON
)

message(STATUS "[Application: ${APPNAME}] Added Executable target: ${APPNAME}")

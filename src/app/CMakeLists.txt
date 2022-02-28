set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/img/icon.rc")

# Add the executable
if (WIN32) 
    add_executable(app WIN32 app.cpp ${APP_ICON_RESOURCE_WINDOWS})
elseif(UNIX)
    add_executable(app app.cpp ${APP_ICON_RESOURCE_WINDOWS})
endif()

target_link_libraries(app qt)

install(TARGETS app DESTINATION ${CMAKE_SOURCE_DIR}/bin)
cmake_minimum_required(VERSION 3.16)

project(WS6 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ModelPart.cpp
        ModelPart.h
        ModelPartList.cpp
        ModelPartList.h
        icons.qrc
        optiondialog.cpp
        optiondialog.h
        VRRenderThread.cpp
        VRRenderThread.h
)

# Define the target executable
qt_add_executable(WS6
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(WS6 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

# Add custom target to copy VRBindings
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/)

# Manual install of Qt DLLs (assuming you're using Qt5 or Qt6)
set(QT_BIN_DIR "C:/Qt/6.8.2/msvc2022_64/bin")
file(GLOB QT_DLLS "${QT_BIN_DIR}/*.dll")
file(GLOB QT_PLUGINS "${QT_BIN_DIR}/../plugins/*")

# Check if Qt bin directory exists and install Qt DLLs and plugins
if(EXISTS "${QT_BIN_DIR}")
    message(STATUS "Qt DLLs found at: ${QT_BIN_DIR}")
    install(FILES ${QT_DLLS} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY ${QT_PLUGINS}/
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt/plugins
        FILES_MATCHING PATTERN "*.dll"
    )
else()
    message(FATAL_ERROR "Qt bin directory not found at: ${QT_BIN_DIR}")
endif()

# Install VTK DLLs
set(VTK_BIN_DIR "C:/Program Files (x86)/VTK/bin")
if(EXISTS "${VTK_BIN_DIR}")
    file(GLOB VTK_DLLS "${VTK_BIN_DIR}/*.dll")
    message(STATUS "VTK Libraries: ${VTK_LIBRARIES}")
    install(FILES ${VTK_DLLS} DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    message(FATAL_ERROR "VTK bin directory not found at: ${VTK_BIN_DIR}")
endif()

# Install OpenVR DLLs
set(OPENVR_DLL "C:/openvr-2.5.1/bin/win64/openvr_api.dll")
if(EXISTS "${OPENVR_DLL}")
    install(FILES ${OPENVR_DLL} DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    message(FATAL_ERROR "OpenVR DLL not found at: ${OPENVR_DLL}")
endif()

# Install the target executable
install(TARGETS WS6
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(VC_REDIST_INSTALLER "C:/C++/VC_redist.x64.exe")
install(FILES ${VC_REDIST_INSTALLER} DESTINATION ${CMAKE_INSTALL_BINDIR})

# Package configuration
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)

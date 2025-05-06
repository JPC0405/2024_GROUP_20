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

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WS6
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        icons.qrc
        optiondialog.h optiondialog.cpp optiondialog.ui
    )

    target_link_libraries(WS6 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

    include(GNUInstallDirs)
    install(TARGETS WS6
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(WS6)
    endif()
endif()

add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/)

# Copy VTK DLLs to the target directory
set(VTK_DLLS
    "C:/Program Files (x86)/VTK/bin/vtkIOGeometry-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkRenderingOpenVR-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkGUISupportQt-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkRenderingOpenGL2-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkIOImage-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkRenderingCore-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkFiltersSources-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkImagingCore-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkFiltersGeneral-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkFiltersCore-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkCommonExecutionModel-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkCommonColor-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkCommonDataModel-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkCommonCore-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtksys-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkFiltersHybrid-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkIOCore-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkFiltersVerdict-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkCommonTransforms-9.4d.dll"
    "C:/Program Files (x86)/VTK/bin/vtkzlib-9.4d.dll"
)

foreach(dll ${VTK_DLLS})
    add_custom_command(TARGET WS6 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dll} $<TARGET_FILE_DIR:WS6>
        COMMENT "Copying VTK DLL: ${dll}"
    )
endforeach()

# Copy OpenVR DLL to the target directory
set(OPENVR_DLLS
    "C:/openvr-2.5.1/bin/win64/openvr_api.dll"
)

foreach(dll ${OPENVR_DLLS})
    add_custom_command(TARGET WS6 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dll} $<TARGET_FILE_DIR:WS6>
        COMMENT "Copying OpenVR DLL: ${dll}"
    )
endforeach()

if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt REQUIRED)

    add_custom_command(TARGET WS6 POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:WS6>" --no-compiler-runtime
        COMMENT "Bundling Qt dependencies with windeployqt"
    )
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)

cmake_minimum_required(VERSION 3.16)

project(image_process VERSION 0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(MAIN_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Core Widgets QML)

# opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
qt_standard_project_setup()
include_directories(${MAIN_INCLUDE})
qt_add_executable(appimage_process
    ${MAIN_SOURCE}/main.cpp
    ${MAIN_INCLUDE}/ImageProvider.h
    ${MAIN_SOURCE}/ImageProvider.cpp
    ${MAIN_INCLUDE}/ImageProcess.h
    ${MAIN_SOURCE}/ImageProcess.cpp
    ${MAIN_INCLUDE}/ImageController.h
    ${MAIN_SOURCE}/ImageController.cpp
)

qt_add_qml_module(appimage_process
    URI image_process
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES WindowFT.qml
    QML_FILES WindowHistogram.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appimage_process PROPERTIES

    # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appimage_process
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appimage_process
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Core
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Qml
)

target_link_libraries(appimage_process
    PRIVATE ${OpenCV_LIBS}
)

include(GNUInstallDirs)
install(TARGETS appimage_process
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

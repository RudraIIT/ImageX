cmake_minimum_required(VERSION 3.5)

project(ImageX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

set(SOURCES
    main.cpp
    MainWindow.cpp 
    MainWindow.h 
    ImageArea.h
    ImageArea.cpp
)

set(CMAKE_AUTOMOC ON)

qt5_wrap_cpp(MOC_SOURCES MainWindow.h)

add_executable(ImageX ${SOURCES} ${MOC_SOURCES})

target_link_libraries(ImageX Qt5::Widgets ${OpenCV_LIBS})
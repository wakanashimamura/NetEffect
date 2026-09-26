cmake_minimum_required(VERSION 3.21)

project(NetEffect VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(NetEffect
  WIN32
  MACOSX_BUNDLE

  src/main.cpp

  src/mainwindow.cpp
  src/mainwindow.h
  src/mainwindow.ui
  src/resource.qrc
  src/app.rc

  src/Random.cpp

  src/wneteffect.cpp
  src/wneteffect.h
)

target_link_libraries(NetEffect PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)

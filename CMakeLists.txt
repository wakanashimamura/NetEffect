cmake_minimum_required(VERSION 3.21)

project(NetEffect VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(NetEffect
  WIN32
  MACOSX_BUNDLE

  main.cpp

  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  resource.qrc
  app.rc

  wneteffect.cpp
  wneteffect.h
)

target_link_libraries(NetEffect PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)

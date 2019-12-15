cmake_minimum_required(VERSION 3.5)

project(oop-lab3-minesweeper LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(oop-lab3-minesweeper
  main.cpp
  gamewindow.cpp
  gamewindow.hpp
  gamewindow.ui
  resources.qrc
  gamestate.cpp
  gamestate.hpp
  rightclickablebutton.cpp
  rightclickablebutton.hpp
  newgamedialog.cpp
  newgamedialog.hpp
  qutesweeperai.cpp
  qutesweeperai.hpp
  gamewindow.fwd.hpp
)

target_link_libraries(oop-lab3-minesweeper PRIVATE Qt5::Widgets)

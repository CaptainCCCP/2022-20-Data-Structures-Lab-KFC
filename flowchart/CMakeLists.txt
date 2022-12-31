cmake_minimum_required(VERSION 3.8.2)

project(flowchart)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add qt5 path, 添加 qt makefile 路径
set(CMAKE_PREFIX_PATH "your qt5 path")

#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets)


SET(SRCS
    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    
    res.qrc

    flowchart.cpp
    flowchart.h

    comdef.h

    class_Charts/chart_base.cpp
    class_Charts/chart_base.h

    class_Charts/chart_diamond.cpp
    class_Charts/chart_diamond.h

    class_Charts/chart_label.cpp
    class_Charts/chart_label.h

    class_Charts/chart_line.cpp
    class_Charts/chart_line.h

    class_Charts/chart_plaintext.cpp
    class_Charts/chart_plaintext.h

    class_Charts/chart_rect.cpp
    class_Charts/chart_rect.h

    class_Charts/chart_roundrect.cpp
    class_Charts/chart_roundrect.h

)

SET(LIBS
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
)
add_executable(flowchart ${SRCS})

target_link_libraries(flowchart ${LIBS})

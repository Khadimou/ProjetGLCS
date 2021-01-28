cmake_minimum_required(VERSION 3.18)

add_library(cliparser
		src/cmdparse.cpp
)
find_package(Qt5 REQUIRED Core Widgets Gui Qml Quick Qml)
target_include_directories(cliparser PUBLIC include/)
target_link_libraries(cliparser PUBLIC GLCS2020_project::baselib
			Qt5::Core
            Qt5::Gui
            Qt5::Widgets
            Qt5::Qml
)
add_library(GLCS2020_project::cliparser ALIAS cliparser)

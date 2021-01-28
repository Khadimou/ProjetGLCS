cmake_minimum_required(VERSION 3.18)

add_library(simpleui
<<<<<<< HEAD
		src/screenprinter.cpp
)
target_include_directories(simpleui PUBLIC include/)
target_link_libraries(simpleui PUBLIC GLCS2020_project::baselib posttreat )
=======
		src/commandlineconfig.cpp
		src/screenprinter.cpp
)
target_include_directories(simpleui PUBLIC include/)
target_link_libraries(simpleui PUBLIC GLCS2020_project::baselib)
>>>>>>> 84f621186237381f864af6857a2b585eb6a4be9e
add_library(GLCS2020_project::simpleui ALIAS simpleui)

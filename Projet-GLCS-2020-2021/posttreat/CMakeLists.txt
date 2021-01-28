cmake_minimum_required(VERSION 3.18)

add_library(posttreat
		src/dataavg.cpp
)
target_include_directories(posttreat PUBLIC include/)
target_link_libraries(posttreat PUBLIC GLCS2020_project::baselib
)
add_library(GLCS2020_project::posttreat ALIAS posttreat)

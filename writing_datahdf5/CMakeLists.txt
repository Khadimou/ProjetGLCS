cmake_minimum_required(VERSION 3.18)

add_library(writing_datahdf5
		src/commandlineconfig.cpp
		src/writing_hdf5.cpp
)
target_include_directories(writing_datahdf5 PUBLIC include/)
target_link_libraries(writing_datahdf5 PUBLIC hdf5::hdf5 GLCS2020_project::baselib)
add_library(GLCS2020_project::writing_datahdf5 ALIAS writing_datahdf5)
cmake_minimum_required(VERSION 3.18)

set(HDF5_PREFER_PARALLEL TRUE)
find_package(HDF5 REQUIRED COMPONENTS C)
find_package(MPI)

add_library(posttreat
		src/dataavg.cpp
		src/resulthdf5.cpp
)
target_include_directories(posttreat PUBLIC include/)
target_link_libraries(posttreat PUBLIC GLCS2020_project::baselib hdf5::hdf5
		MPI::MPI_CXX
)
add_library(GLCS2020_project::posttreat ALIAS posttreat)

cmake_minimum_required(VERSION 3.19)

project(GLCS2020_project C CXX)

set(HDF5_PREFER_PARALLEL TRUE)
find_package(HDF5 REQUIRED COMPONENTS C)
find_package(MPI REQUIRED COMPONENTS CXX)
add_subdirectory("vendor/mdspan/" "mdspan" EXCLUDE_FROM_ALL)

add_subdirectory(baselib)
add_subdirectory(heatlib)
add_subdirectory(simpleui)
add_subdirectory(cliparser)
add_subdirectory(posttreat)
add_subdirectory(writing_datahdf5)

add_executable(simpleheat
		simpleheat.cpp
)

target_link_libraries(simpleheat PRIVATE
		GLCS2020_project::baselib
		GLCS2020_project::heatlib
		GLCS2020_project::simpleui
		GLCS2020_project::cliparser
		GLCS2020_project::posttreat
		MPI::MPI_CXX)
		GLCS2020_project::writing_datahdf5
		hdf5::hdf5
		MPI::MPI_CXX)

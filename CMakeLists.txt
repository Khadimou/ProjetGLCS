cmake_minimum_required(VERSION 3.18)

enable_language(C)
enable_language(Fortran)
project(GLCS2020_project CXX)

find_package(MPI)
find_package(HDF5)
add_subdirectory("vendor/mdspan/" "mdspan" EXCLUDE_FROM_ALL)

add_subdirectory(baselib)
add_subdirectory(heatlib)
add_subdirectory(simpleui)
add_subdirectory(writing_datahdf5)

add_executable(simpleheat
		simpleheat.cpp
)
target_link_libraries(simpleheat PRIVATE
		GLCS2020_project::baselib
		GLCS2020_project::heatlib
		GLCS2020_project::simpleui
        GLCS2020_project::writing_datahdf5
		MPI::MPI_CXX)
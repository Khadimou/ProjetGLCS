#pragma once

#include "resulthdf5.hpp"

void Write::write_avg(const double& avg){

    // file that will contain data
    const char* file_name = "result.h5";

    // create the file access property list
    const hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(fapl, MPI_COMM_WORLD, MPI_INFO_NULL);

    // Create the file
    const hid_t h5file = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, fapl);

    // size of the data distribution over MPI
    int mpi_size; MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    // create the file dataspace
    const std::vector<hsize_t> file_dims { mpi_size, mpi_size };
    const hid_t file_space = H5Screate_simple(file_dims.size(), file_dims.data(), NULL);

    // create the dataset
    const hid_t dataset = H5Dcreate(h5file, "data", H5T_NATIVE_DOUBLE, file_space,H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // select the region we want to write in the file dataspace
    const std::vector<hsize_t> file_start { 0, 0 };
    const std::vector<hsize_t> file_count { mpi_size, mpi_size };
    H5Sselect_hyperslab(file_space, H5S_SELECT_SET, file_start.data(), NULL, file_count.data(), NULL );

    // create the memory dataspace
    const std::vector<hsize_t> mem_dims { mpi_size, mpi_size };
    const hid_t mem_space =  H5Screate_simple(mem_dims.size(), mem_dims.data(), NULL);

    // select the region we want to write in the memory dataspace
    const std::vector<hsize_t> mem_start { 0, 0 };
    const std::vector<hsize_t> mem_count { mpi_size, mpi_size };
    H5Sselect_hyperslab(mem_space, H5S_SELECT_SET, mem_start.data(), NULL, mem_count.data(), NULL );

    // create the data transfer property list
    const hid_t xfer_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(xfer_plist, H5FD_MPIO_COLLECTIVE);

    /* vector to store the average results*/
    std::vector<double> tmp(mpi_size);
    for(int i=0;i<mpi_size;i++){
        tmp[i] = avg;
    }
    int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){
        // write data to HDF5!
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, tmp.data());
    }else
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, tmp.data());

    H5Pclose(fapl);
    H5Pclose(xfer_plist);
    H5Sclose(mem_space);
    H5Dclose(dataset);
    H5Sclose(file_space);
    H5Fclose(h5file);

}
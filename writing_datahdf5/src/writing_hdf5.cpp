// standard C++ library headers
#include <chrono>
#include <iostream>
#include <thread>

// the implemented class (last)
#include "writing_hdf5.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void WritingData::simulation_updated(const Distributed2DField& data){

    // file that will contain data
    const char* file_name = "my_file.h5";

    // create the file access property list
    const hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(fapl, MPI_COMM_WORLD, MPI_INFO_NULL);

    // Create the file
    const hid_t h5file = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, fapl);

    // size of the data distribution over MPI
    int mpi_size = data.distribution().size();
    // create the file dataspace
    const std::vector<hsize_t> file_dims { data.noghost_view().extent(DY), data.noghost_view().extent(DX) };
    const hid_t file_space = H5Screate_simple(file_dims.size(), file_dims.data(), NULL);

    // create the dataset
    const hid_t dataset = H5Dcreate(h5file, "data", H5T_NATIVE_DOUBLE, file_space,H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // select the region we want to write in the file dataspace
    const std::vector<hsize_t> file_start { 0, 0 };
    const std::vector<hsize_t> file_count { data.noghost_view().extent(DY), data.noghost_view().extent(DX) };
    H5Sselect_hyperslab(file_space, H5S_SELECT_SET, file_start.data(), NULL, file_count.data(), NULL );

    // create the memory dataspace
    const std::vector<hsize_t> mem_dims { data.noghost_view().extent(DY), data.noghost_view().extent(DX) };
    const hid_t mem_space =  H5Screate_simple(mem_dims.size(), mem_dims.data(), NULL);

    // select the region we want to write in the memory dataspace
    const std::vector<hsize_t> mem_start { 0, 0 };
    const std::vector<hsize_t> mem_count { data.noghost_view().extent(DY), data.noghost_view().extent(DX) };
    H5Sselect_hyperslab(mem_space, H5S_SELECT_SET, mem_start.data(), NULL, mem_count.data(), NULL );

    // create the data transfer property list
    const hid_t xfer_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(xfer_plist, H5FD_MPIO_COLLECTIVE);

    if(data.distribution().rank() == 0){
        // write data to HDF5!
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, data.noghost_view().data());
    }else
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, data.noghost_view().data());

    H5Pclose(fapl);
    H5Pclose(xfer_plist);
    H5Sclose(mem_space);
    H5Dclose(dataset);
    H5Sclose(file_space);
    H5Fclose(h5file);

}
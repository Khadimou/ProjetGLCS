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

    const char* file_name = "my_file.h5";

    // create the file access property list
    const hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(fapl, MPI_COMM_WORLD, MPI_INFO_NULL);

    // Create the file
    const hid_t h5file = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, fapl);

    // create the file dataspace
    int mpi_size = data.distribution().size(); //MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    const std::vector<hsize_t> file_dims { data.noghost_view().extent(DX), data.noghost_view().extent(DY) };
    const hid_t file_space = H5Screate_simple(file_dims.size(), file_dims.data(), NULL);

    // create the dataset
    const hid_t dataset = H5Dcreate(h5file, "data", H5T_NATIVE_DOUBLE, file_space,H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // select the region we want to write in the file dataspace
    const std::vector<hsize_t> file_start { 0, 0 };
    const std::vector<hsize_t> file_count { data.noghost_view().extent(DX), data.noghost_view().extent(DY) };
    H5Sselect_hyperslab(file_space, H5S_SELECT_SET, file_start.data(), NULL, file_count.data(), NULL );

    // create the memory dataspace
    const std::vector<hsize_t> mem_dims { data.noghost_view().extent(DX), data.noghost_view().extent(DY) };
    const hid_t mem_space =  H5Screate_simple(mem_dims.size(), mem_dims.data(), NULL);

    // select the region we want to write in the memory dataspace
    const std::vector<hsize_t> mem_start { 0, 0 };
    const std::vector<hsize_t> mem_count { data.noghost_view().extent(DX), data.noghost_view().extent(DY) };
    H5Sselect_hyperslab(mem_space, H5S_SELECT_SET, mem_start.data(), NULL, mem_count.data(), NULL );

    // create the data transfer property list
    const hid_t xfer_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(xfer_plist, H5FD_MPIO_COLLECTIVE);

    if(data.distribution().rank() == 0){
        // write data to HDF5!
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, data.noghost_view().data());
    }

    /*for ( int pyy = data.distribution().extent( DY )-1; pyy >=0 ; --pyy ) {
        for (int yy = data.noghost_view().extent(DY) - 1; yy >= 0; --yy) {
            for (int pxx = 0; pxx < data.distribution().extent(DX); ++pxx) {
                if (data.distribution().coord(DX) == pxx && data.distribution().coord(DY) == pyy) {

                    for (int xx = 0; xx < data.noghost_view().extent(DX); ++xx) {
                        if (0 != data.noghost_view(yy, xx)) {
                            std::vector<double> tmp(mem_dims.size(), data.noghost_view(yy, xx));
                            H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, tmp.data());
                        }

                    }

                }

            }
        }
    }*/

    H5Pclose(fapl);
    H5Pclose(xfer_plist);
    H5Sclose(mem_space);
    H5Dclose(dataset);
    H5Sclose(file_space);
    H5Fclose(h5file);

}
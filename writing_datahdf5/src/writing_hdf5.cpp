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

    // Create the file
    const hid_t h5file = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, fapl);

    //if(data)

}
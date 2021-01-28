#pragma once

#include "dataavg.hpp"
#include <hdf5.h>

class Write
{

public:
    /* Writing data average in hdf5 file */
    void write_avg(const double& avg);

};
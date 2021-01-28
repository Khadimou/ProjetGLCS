#pragma once

// library headers
#include <simulationobserver.hpp>
#include <hdf5.h>

class WritingData
: public SimulationObserver
{
public:
    // see overriden function
    void simulation_updated( const Distributed2DField& data ) override;
};
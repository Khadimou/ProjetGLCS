#pragma once

#include "datareduce.hpp"

class DataAvg: 
        public DataReduce
{
    /// average value of the data
	double m_data_avg;

public: 
    /** Calculate the average of the data
	 * @param data the data 
	 */
	DataAvg(const Distributed2DField& data);

	// get the average value
	double average() const override { return m_data_avg; }
	
};
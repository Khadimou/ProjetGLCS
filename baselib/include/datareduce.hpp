#pragma once

// local headers
#include "distributed2dfield.hpp"

class DataReduce
{
public:
	/// The destructor
	virtual ~DataReduce() = default;

	/** Calculate the average of the data
	 * @return 
	 */
	virtual double average() const = 0;
};

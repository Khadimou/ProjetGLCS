#include <cstdlib>
#include <iostream>

#include "dataavg.hpp"
#include "resulthdf5.hpp"

using std::cout;
double somme=0;
DataAvg::DataAvg( const Distributed2DField& data)
{
	for ( int pyy = data.distribution().extent( DY )-1; pyy >=0 ; --pyy ) {
		for ( int yy = data.noghost_view().extent( DY )-1; yy >=0 ; --yy ) {
			for ( int pxx = 0; pxx < data.distribution().extent( DX ); ++pxx ) {
				if ( data.distribution().coord( DX ) == pxx && data.distribution().coord( DY ) == pyy ) {
					for ( int xx = 0; xx < data.noghost_view().extent( DX ); ++xx ) {
						somme += data.noghost_view(yy, xx);
					}
				}
			}
		}
	}
	m_data_avg=somme/(data.noghost_view().extent( DY )*data.noghost_view().extent( DX ));
	cout << "somme"<< somme  <<"the average" << average() << "\n";
	Write write;
	write.write_avg(average());
}
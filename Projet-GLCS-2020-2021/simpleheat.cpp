#include <mpi.h>

#include "cmdparse.hpp"
//#include "commandlineconfig.hpp"
#include "finitediffheatsolver.hpp"
#include "fixedconditions.hpp"
#include "simulation.hpp"
#include "screenprinter.hpp"

//mpirun -n 2 ./simpleheat --nb_iter=10 --height=4 --width=8 --process_height=1 --process_width=2 --delta_t=0.125 --delta_x=1 --delta_y=1
int main( int argc, char* argv[] )
{
	// initialize the MPI library
	MPI_Init( &argc, &argv );
	// Construct the command-line arguments parser
	//CommandLineConfig config( argc, argv );
	CmdParse config( argc, argv );

	// construct the heat equation solver
	FinitediffHeatSolver heat_solver( config );
	// construct the initial condition setter
	FixedConditions init;
	// construct the simulation runner
	Simulation simulation( MPI_COMM_WORLD, config, heat_solver, init );
	// Add a printer to screen to observe the simulation
	ScreenPrinter printer;
	simulation.observe( printer );
	// run the simulation
	simulation.run();

	// finalize MPI
	MPI_Finalize();

	return 0;
}

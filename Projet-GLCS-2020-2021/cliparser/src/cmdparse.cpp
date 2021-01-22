#include <cstdlib>
#include <iostream>
#include <string>

#include "cmdparse.hpp"

using std::cerr;
using std::endl;
using std::exit;
using std::stoi;
using std::stod;
using std::to_string;
using std::sprintf;


static int DEFAULT_ITER = 10;
static int DEFAULT_HEIGHT = 4;
static int DEFAULT_WIDTH = 8;
static int DEFAULT_PROC_HEIGHT = 1;
static int DEFAULT_PROC_WIDTH = 2;
static double DEFAULT_DELTA_T = 0.125;
static int DEFAULT_DELTA_X = 1;
static int DEFAULT_DELTA_Y = 1;

CmdParse::CmdParse( int argc, char** const argv )
{
	QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("heatSolver");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("This is a simple heat solver");
    parser.addHelpOption();
    parser.addVersionOption();

	// Add number of iteration option (--nb_iter)
    QCommandLineOption nbIterOption("nb_iter",
			QCoreApplication::translate("simpleheat", "number of iterations to execute"),
			QCoreApplication::translate("simpleheat", "nb_iter"),
			std::to_string(DEFAULT_ITER).c_str());
	
    parser.addOption(nbIterOption);

	// Add height of the global data field option (--height)
    QCommandLineOption heightOption("height",
			QCoreApplication::translate("simpleheat", "height of the global data field"),
			QCoreApplication::translate("simpleheat", "height"),
			std::to_string(DEFAULT_HEIGHT).c_str());
    parser.addOption(heightOption);

	// Add height of the global data field option (--width)
    QCommandLineOption widthOption("width",
			QCoreApplication::translate("simpleheat", "width of the global data field"),
			QCoreApplication::translate("simpleheat", "width"),
			std::to_string(DEFAULT_WIDTH).c_str());
    parser.addOption(widthOption);

	// Add height of the global data field option (--process_height)
    QCommandLineOption processHeightOption("process_height",
			QCoreApplication::translate("simpleheat", "height of the data distribution"),
			QCoreApplication::translate("simpleheat", "process_height"),
			std::to_string(DEFAULT_PROC_HEIGHT).c_str());
    parser.addOption(processHeightOption);

	// Add height of the global data field option (--process_width)
    QCommandLineOption processWidthOption("process_width",
			QCoreApplication::translate("simpleheat", "width of the data distribution"),
			QCoreApplication::translate("simpleheat", "process_width"),
			std::to_string(DEFAULT_PROC_WIDTH).c_str());
    parser.addOption(processWidthOption);

	// Add time difference between two consecutive points option (--delta_t)
    QCommandLineOption deltaTOption("delta_t",
			QCoreApplication::translate("simpleheat", "time difference between two consecutive points"),
			QCoreApplication::translate("simpleheat", "delta_t"),
			"0.125");
    parser.addOption(deltaTOption);

	// Add width difference between two consecutive points option (--delta_x)
    QCommandLineOption deltaXOption("delta_x",
			QCoreApplication::translate("simpleheat", "width difference between two consecutive points"),
			QCoreApplication::translate("simpleheat", "delta_x"),
			std::to_string(DEFAULT_DELTA_X).c_str());
    parser.addOption(deltaXOption);

	// Add height difference between two consecutive points option (--delta_y)
    QCommandLineOption deltaYOption("delta_y",
			QCoreApplication::translate("simpleheat", "heignt difference between two consecutive points"),
			QCoreApplication::translate("simpleheat", "delta_y"),
			std::to_string(DEFAULT_DELTA_Y).c_str());
    parser.addOption(deltaYOption);

	// Process the actual command line arguments given by the user
    parser.process(app);

	// Get the values 
	
	m_nb_iter = parser.value(nbIterOption).toInt();
	m_global_shape[DY] = parser.value(heightOption).toInt();
	m_global_shape[DX] = parser.value(widthOption).toInt();
	m_dist_extents[DY] = parser.value(processHeightOption).toInt();
	m_dist_extents[DX] = parser.value(processWidthOption).toInt();
	m_delta_t = parser.value(deltaTOption).toDouble();
	m_delta_space[DX] = parser.value(deltaXOption).toDouble();
	m_delta_space[DY] = parser.value(deltaYOption).toDouble();

}
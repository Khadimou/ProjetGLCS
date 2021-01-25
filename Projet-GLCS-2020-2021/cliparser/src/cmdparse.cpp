#include <cstdlib>
#include <iostream>
#include <string>

#include "cmdparse.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::exit;
using std::stoi;
using std::stod;
using std::to_string;
using std::sprintf;


// the default values 
static int DEFAULT_ITER = 10;
static int DEFAULT_HEIGHT = 4;
static int DEFAULT_WIDTH = 8;
static int DEFAULT_PROC_HEIGHT = 1;
static int DEFAULT_PROC_WIDTH = 2;
static int DEFAULT_DELTA_X = 1;
static int DEFAULT_DELTA_Y = 1;
static QString DEFAULT_DELTA_T = "0.125";
static QString DEFAULT_CONFIG_FILE="../default.ini";
static int DEFAULT_WRITE_FREQ=1;


CmdParse::CmdParse( int argc, char** const argv )
{
	QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("heatSolver");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("This is a simple heat solver");
    parser.addHelpOption();
    parser.addVersionOption();

	// Update the default values with those in the default config file if it exists
	if (QFile(DEFAULT_CONFIG_FILE).exists()) 
	{
		QSettings settings(DEFAULT_CONFIG_FILE, QSettings::IniFormat);
		// retrieve the setting
		DEFAULT_DELTA_T = settings.value("params/delta_t",DEFAULT_DELTA_T).toString(); 
		DEFAULT_DELTA_X = settings.value("params/delta_x", DEFAULT_DELTA_X).toInt(); 
		DEFAULT_DELTA_Y = settings.value("params/delta_y", DEFAULT_DELTA_Y).toInt(); 
		DEFAULT_ITER = settings.value("params/nb_iter", DEFAULT_ITER).toInt(); 
		DEFAULT_HEIGHT = settings.value("params/height", DEFAULT_HEIGHT).toInt(); 
		DEFAULT_WIDTH = settings.value("params/width", DEFAULT_WIDTH).toInt(); 
		DEFAULT_PROC_HEIGHT = settings.value("params/process_height", DEFAULT_PROC_HEIGHT).toInt(); 
		DEFAULT_PROC_WIDTH = settings.value("params/process_width", DEFAULT_PROC_WIDTH).toInt(); 
		DEFAULT_WRITE_FREQ = settings.value("params/freq", DEFAULT_WRITE_FREQ).toInt(); 
	}

	//Adding the options with their default values

	// Add config file option (--file)
    QCommandLineOption fileOption("file",
			QCoreApplication::translate("simpleheat", "config file name"),
			QCoreApplication::translate("simpleheat", "file name"),
			std::to_string(DEFAULT_DELTA_Y).c_str());
    parser.addOption(fileOption);

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
			DEFAULT_DELTA_T);
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

	// Add the writting frequency to the hdf5 file option (--delta_y)
    QCommandLineOption freqOption("freq",
			QCoreApplication::translate("simpleheat", "frequence of writting to the hdf5 file"),
			QCoreApplication::translate("simpleheat", "frequence"),
			std::to_string(DEFAULT_WRITE_FREQ).c_str());
    parser.addOption(freqOption);

	// Process the actual command line arguments given by the user
    parser.process(app);

	// if the user introduces a config file with the option --file
	if (parser.isSet(fileOption))
	{
		QString file_name=parser.value(fileOption);
		QSettings settings(file_name, QSettings::IniFormat);
		// get the values from the choosen config file if it exist
	 	// if one value miss in the file, get it from the default values
		if (QFile(file_name).exists()) 
		{ 
			m_nb_iter = (parser.isSet(nbIterOption)) ? parser.value(nbIterOption).toDouble() : settings.value("params/nb_iter", DEFAULT_ITER).toDouble();
			m_global_shape[DY] = (parser.isSet(heightOption)) ? parser.value(heightOption).toInt() : settings.value("params/height", DEFAULT_HEIGHT).toInt();
			m_global_shape[DX] = (parser.isSet(widthOption)) ? parser.value(widthOption).toInt() : settings.value("params/width", DEFAULT_WIDTH).toInt();
			m_dist_extents[DY] = (parser.isSet(processHeightOption)) ? parser.value(processHeightOption).toInt() : settings.value("params/process_height", DEFAULT_PROC_HEIGHT).toInt();
			m_dist_extents[DX] = (parser.isSet(processWidthOption)) ? parser.value(processWidthOption).toInt() : settings.value("params/process_width", DEFAULT_PROC_WIDTH).toInt();
			m_delta_t = (parser.isSet(deltaTOption)) ? parser.value(deltaTOption).toDouble() : settings.value("params/delta_t", DEFAULT_DELTA_T).toDouble();
			m_delta_space[DX] = (parser.isSet(deltaXOption)) ? parser.value(deltaXOption).toInt() : settings.value("params/delta_x", DEFAULT_DELTA_X).toInt();
			m_delta_space[DY] = (parser.isSet(deltaYOption)) ? parser.value(deltaYOption).toInt() : settings.value("params/delta_y", DEFAULT_DELTA_Y).toInt();	
			m_freq=(parser.isSet(freqOption)) ? parser.value(freqOption).toInt() : settings.value("params/freq", DEFAULT_WRITE_FREQ).toInt();		
		}
		else
		{
			cerr << "ERROR: file " << file_name.toStdString().c_str() << " doesn't exist! \n"<< endl;
			exit(1);
		}
	}
	else
	{
		// get the values from their options
	 	// if one option miss, the default value will be returned
		m_nb_iter = parser.value(nbIterOption).toInt();
		m_global_shape[DY] = parser.value(heightOption).toInt();
		m_global_shape[DX] = parser.value(widthOption).toInt();
		m_dist_extents[DY] = parser.value(processHeightOption).toInt();
		m_dist_extents[DX] = parser.value(processWidthOption).toInt();
		m_delta_t = parser.value(deltaTOption).toDouble();
		m_delta_space[DX] = parser.value(deltaXOption).toDouble();
		m_delta_space[DY] = parser.value(deltaYOption).toDouble();
		m_freq = parser.value(freqOption).toInt();
	}
	
	// show the values 
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==0) 
	{
		cout << "Executing application with: \n";
		cout << "nb_iter = " << m_nb_iter <<"\n" ;
		cout << "height = " << m_global_shape[DY] <<"\n";
		cout << "width = " << m_global_shape[DX] <<"\n";
		cout << "process_height = " << m_dist_extents[DY] <<"\n";
		cout << "process_width = " << m_dist_extents[DX] <<"\n";
		cout << "delta_t = " << m_delta_t <<"\n";
		cout << "delta_x = " << m_delta_space[DX] <<"\n";
		cout << "delta_y = " << m_delta_space[DY] <<"\n";
		cout << "freq = " << m_freq <<"\n";
	}

}
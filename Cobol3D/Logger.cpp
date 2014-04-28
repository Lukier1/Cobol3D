#include <Windows.h>
#include <time.h>
#include "Logger.h"


namespace Cobol {
	
	//extern static Logger* Singleton<Logger>::ms_singleton;
	Logger::Logger(std::string name) {
		file.open(name, std::ios::trunc | std::ios::out);
		if(!file.is_open())
		{
			std::string error = "Couldn't create file: ";
			error += name;
			MessageBox(NULL,error.c_str(), "ERROR", MB_ICONERROR | MB_OK);
		}
		else
		{
			time_t czas;
			time( & czas );
			char * data = ctime( & czas );
			file<<"Start logging: "<<data;
		}
	}
	Logger::~Logger() {

		if(file.is_open())
		{
			time_t czas;
			time( & czas );
			char * data = ctime( & czas );
			file<<"End logging: "<<data;
			file.close();
		}
	}
	void Logger::WriteError(std::string desc)
	{
		if(file.is_open())
		{
			file<<"ERROR: " << desc.c_str() << std::endl;
		}
	}
	void Logger::WriteInfo(std::string desc)
	{
		if(file.is_open())
		{
			file<< desc.c_str() << std::endl;
		}
	}
	void Logger::WriteWarning(std::string desc)
	{
		if(file.is_open())
		{
			file<<"WARNING: " << desc.c_str() << std::endl;
		}
	}
}
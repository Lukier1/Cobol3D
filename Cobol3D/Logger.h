// LOGGER.H
// Singleton loggera, potrafi zapisywaæ tylko do jednego pliku podczas trwania obiektu klasy Root
// to zapisu do pliku nale¿y u¿ywaæ funkcji z przedrostkiem Write. 
// Gdy wyst¹pi b³¹d z utworzeniem pliku zostnanie wyrzucony b³¹d w postaci MessageBox'a ale nie zatrzyma on dzia³ania programu
//
//Autor: £ukasz Kowalczyk
//Ostatnie Modyfikacja: 00:31 2013-07-03


#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string.h>
#include <Windows.h>
#include <fstream>
#include "Singleton.h"

#define LOG_MSGERR(str) DXTRACE_MSG(str);  Cobol::Logger::GetSingletonPtr()->WriteError(str);
#define BOTHMSG(str) LOG_MSGERR(str); MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);
#define NODXERR(str)  Cobol::Logger::GetSingletonPtr()->WriteError(str); MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);

namespace Cobol {
	class Logger : public Singleton<Logger> {
	private:
		std::fstream file;
	public:
		Logger(std::string name);
		void WriteError(std::string desc);
		void WriteWarning(std::string desc);
		void WriteInfo(std::string desc);
		~Logger();
	};
}

#endif
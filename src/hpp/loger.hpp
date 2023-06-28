#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>  

class Loger 
{
	public:
	Loger(); 
	Loger(std::string); 
    ~Loger();
    
    std::string logFilePath;
    
	void setLogPath(std::string);
	void writeLog(int);
	void writeLog(double);
	void writeLog(uint8_t);
	void writeLog(std::string);
	void clearLog();
};


#endif

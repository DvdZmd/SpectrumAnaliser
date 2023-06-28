#include "loger.hpp"

Loger::Loger()
{
}
Loger::Loger(std::string _f)
{
    logFilePath = _f;
}
Loger::~Loger()
{
}

void Loger::setLogPath(std::string _f)
{
    logFilePath = _f;
}

void Loger::writeLog(int val)
{
    std::ofstream log(logFilePath,std::ios::app);

    std::string s = std::to_string(val);
    char const *pchar = s.c_str();
    log << pchar << '\n';
    log.close();
}

void Loger::writeLog(double val)
{
    std::ofstream log(logFilePath,std::ios::app);

    std::string s = std::to_string(val);
    char const *pchar = s.c_str();
    log << pchar << '\n';
    log.close();
}

void Loger::writeLog(uint8_t val)
{
    std::ofstream log(logFilePath,std::ios::app);

    std::string s = std::to_string(val);
    char const *pchar = s.c_str();
    log << pchar << '\n';
    log.close();
}
void Loger::writeLog(std::string val)
{
    std::ofstream log(logFilePath,std::ios::app);
    char const *pchar = val.c_str();
    log << pchar << '\n';
    log.close();
}

void Loger::clearLog()
{
    std::ofstream log;
    log.open(logFilePath, std::ofstream::out | std::ofstream::trunc);
    log.close();
}

#include "../hpp/logger.hpp"

Logger* Logger::m_Instance = 0;

Logger* Logger::getInstance()
{
    if (m_Instance == 0) 
    {
        m_Instance = new Logger();
    }
    return m_Instance;
}

void Logger::setLogPath(std::string _f)
{
    logFilePath = _f;
}

void Logger::writeLog(std::string val)
{
    std::ofstream log(logFilePath,std::ios::app);
    char const *pchar = val.c_str();
    log << pchar << '\n';
    log.close();
}

Logger::Logger()
{
}

void Logger::clearLog()
{
    std::ofstream log;
    log.open(logFilePath, std::ofstream::out | std::ofstream::trunc);
    log.close();
}

// Logger::Logger(std::string _f)
// {
//     logFilePath = _f;
// }
// Logger::~Logger()
// {
// }



// void Loger::writeLog(int val)
// {
//     std::ofstream log(logFilePath,std::ios::app);

//     std::string s = std::to_string(val);
//     char const *pchar = s.c_str();
//     log << pchar << '\n';
//     log.close();
// }

// void Loger::writeLog(double val)
// {
//     std::ofstream log(logFilePath,std::ios::app);

//     std::string s = std::to_string(val);
//     char const *pchar = s.c_str();
//     log << pchar << '\n';
//     log.close();
// }

// void Loger::writeLog(uint8_t val)
// {
//     std::ofstream log(logFilePath,std::ios::app);

//     std::string s = std::to_string(val);
//     char const *pchar = s.c_str();
//     log << pchar << '\n';
//     log.close();
// }
// void Loger::writeLog(std::string val)
// {
//     std::ofstream log(logFilePath,std::ios::app);
//     char const *pchar = val.c_str();
//     log << pchar << '\n';
//     log.close();
// }

// void Loger::clearLog()
// {
//     std::ofstream log;
//     log.open(logFilePath, std::ofstream::out | std::ofstream::trunc);
//     log.close();
// }

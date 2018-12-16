/**
 * Logger.cc
 * Purpose: Logging functionality implementation.
 *
 * @author Balazs Varga
 * @version 0.1.0 12/15/18
 */

#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include "Logger.hh"

namespace mlibs
{
namespace logger
{

Logger::Logger(LogLevels level, std::string path, bool consoleLogEnabled):
  minimumLevel(level),
  isConsoleLoggingEnabled(consoleLogEnabled) {
  logFile.open(path);
}

Logger::~Logger() {
  logFile.close();
}

void Logger::createLogger(LogLevels level, std::string path, bool consoleLogEnabled)  {

}

void Logger::setLogFilePath(std::string path)  {

}

void Logger::enableConsoleLog()  {
  isConsoleLoggingEnabled = true;
}

void Logger::disableConsoleLog()  {
  isConsoleLoggingEnabled = false;
}

void Logger::writeLogMessage(LogLevels level, std::string msg) {
  if (level < minimumLevel) {
    return;
  }

  if (isConsoleLoggingEnabled) {
    if (level < ERROR) {
      std::cout << msg << std::endl;
    } else {
      std::cerr << msg << std::endl;
    }
  }

  if (logFile.is_open()) {
    logFile << msg << std::endl;
  }
}

}
}

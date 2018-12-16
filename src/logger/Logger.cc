/**
 * Logger.cc
 * Purpose: Logging functionality implementation.
 *
 * @author Balazs Varga
 * @version 0.1.0 12/15/18
 */

#include <string>
#include <stdarg.h>
#include <iostream>
#include "Logger.hh"

namespace mlibs
{
namespace logger
{

Logger::Logger(LogLevels level, std::string path, bool consoleLogEnabled) {

}

void Logger::createLogger(LogLevels level, std::string path, bool consoleLogEnabled)  {

}

void Logger::setLogFilePath(std::string path)  {

}

void Logger::enableConsoleLog()  {

}

void Logger::disableConsoleLog()  {

}

void Logger::writeLogMessage(LogLevels level, ...) {
  if (level < ERROR) {
//  std::cout << ... << args << std::endl;
  } else {
//  std::cerr << ... << args << std::endl;
  }
}

}
}

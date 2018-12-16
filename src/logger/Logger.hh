/**
 * Logger.hh
 * Purpose: Header for logging functionality implementation.
 *
 * @author Balazs Varga
 * @version 0.1.0 12/15/18
 */

#include <string>
#include <fstream>
#include "LoggerInterface.hh"

#pragma once

namespace mlibs
{
namespace logger
{
/** class Logger
 * brief Logging library implementation.
 */
class Logger : public LoggerInterface {
public:
  /**
   * Parametered constructor
   * Sets instance variables and opens log file.
   *
   * @param level  Minimal log level which is written into the destination
   * @param path Path of the log file
   * @param consoleLogEnabled Sets whether logging to cerr is enabled or not
   * @return none
   */
  Logger(LogLevels level, std::string path, bool consoleLogEnabled);

  /**
   * Destructor
   * Closes the log file
   */
  ~Logger();

  /**
   * Creates a logger singleton.
   *
   * @param level Minimal log level which is written into the destination
   * @param path Path of the log file
   * @param consoleLogEnabled Sets whether logging to cerr is enabled or not
   * @return none
   */
  virtual void createLogger(LogLevels level, std::string path, bool consoleLogEnabled);

  /**
   * Sets the path of destination log file.
   *
   * @param path Path of the log file
   * @return none
   */
  virtual void setLogFilePath(std::string path);

  /**
   * Enables to log into console beside the file based logging.
   *
   * @return none
   */
  virtual void enableConsoleLog();

  /**
   * Disables to log into console beside the file based logging.
   *
   * @return none
   */
  virtual void disableConsoleLog();

  /**
   * Function to write log message according to previous setups.
   *
   * @param level Log level to be used to report this message
   * @param msg Message to log
   * @return none
   */
  virtual void writeLogMessage(LogLevels level, std::string msg);
private:
  /**
   * Signs whether the logging to stderr is enabled or not.
   */
  bool isConsoleLoggingEnabled;
  LogLevels minimumLevel;
  std::ofstream logFile;
};

}
}

/**
 * LoggerInterface.hh
 * Purpose: Pure abstract class for Logger interface.
 *
 * @author Balazs Varga
 * @version 0.1.0 12/15/18
 */

#include <string>
#include <stdarg.h>

#pragma once

namespace mlibs
{
namespace logger
{
/**
 * Enumeration to symbolize logging level can be used in mlibs::logger.
 */
enum LogLevels {
  DEBUG = 0,
  INFO,
  WARNING,
  ERROR,
  CRITICAL
};

/** class LoggerInterface
 * brief Abstract class for implement interface for logger.
 */
struct LoggerInterface {
  /**
   * Creates a logger singleton.
   *
   * @param level Minimal log level which is written into the destination
   * @param path Path of the log file
   * @param consoleLogEnabled Sets whether logging to cerr is enabled or not
   * @return none
   */
  virtual void createLogger(LogLevels level, std::string path, bool consoleLogEnabled) = 0;

  /**
   * Sets the path of destination log file.
   *
   * @param path Path of the log file
   * @return none
   */
  virtual void setLogFilePath(std::string path) = 0;

  /**
   * Enables to log into console beside the file based logging.
   *
   * @return none
   */
  virtual void enableConsoleLog() = 0;

  /**
   * Disables to log into console beside the file based logging.
   *
   * @return none
   */
  virtual void disableConsoleLog() = 0;

  /**
   * Function to write log message according to previous setups.
   *
   * @param level Log level to be used to report this message
   * @param ... Arguments to log
   * @return none
   */
  virtual void writeLogMessage(LogLevels level, ...) = 0;
};

}
}

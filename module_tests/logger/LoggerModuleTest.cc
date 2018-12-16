#include <iostream>
#include <sstream>
#include <fstream>

#include "logger/LoggerModuleTest.hh"

#include "logger/LoggerInterface.hh"
#include "logger/Logger.hh"

LoggerModuleTest::LoggerModuleTest() {
  testCases.push_back({"noOutput", noOutput});
  testCases.push_back({"consoleOutput", consoleOutput});
  testCases.push_back({"fileOutput", fileOutput});
  testCases.push_back({"bothOutput", bothOutput});
}

bool LoggerModuleTest::noOutput(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "", false);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST STR");

  if ((cout_local.str() != "") || (cerr_local.str() != "")) {
    result = false;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  return result;
}

bool LoggerModuleTest::consoleOutput(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "", true);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST OUT STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST ERR STR");

  if ((cout_local.str() != "TEST OUT STR") || (cerr_local.str() != "TEST ERR STR")) {
    result = false;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  return result;
}

bool LoggerModuleTest::fileOutput(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "/tmp/out.log", false);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST 1 STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST 2 STR");

  if ((cout_local.str() != "") || (cerr_local.str() != "")) {
    result = false;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  std::ifstream logFile;
  std::string line;
  logFile.open("/tmp/out.log");

  logFile >> line;
  if (line != "TEST 1 STR") {
    result = false;
  }

  logFile >> line;
  if (line != "TEST 2 STR") {
    result = false;
  }

  logFile.close();

  return result;
}

bool LoggerModuleTest::bothOutput(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "", false);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST 1 STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST 2 STR");

  if ((cout_local.str() != "TEST 1 STR") || (cerr_local.str() != "TEST 2 STR")) {
    result = false;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  std::ifstream logFile;
  std::string line;
  logFile.open("/tmp/out.log");

  logFile >> line;
  if (line != "TEST 1 STR") {
    result = false;
  }

  logFile >> line;
  if (line != "TEST 2 STR") {
    result = false;
  }

  logFile.close();

  return result;
}

#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>

#include "logger/LoggerModuleTest.hh"

#include "logger/LoggerInterface.hh"
#include "logger/Logger.hh"

LoggerModuleTest::LoggerModuleTest() {
  testCases.push_back({"noOutput", noOutput});
  testCases.push_back({"consoleOutput", consoleOutput});
  testCases.push_back({"fileOutput", fileOutput});
  testCases.push_back({"bothOutput", bothOutput});
  testCases.push_back({"filteredConsoleOutput", filteredConsoleOutput});
  testCases.push_back({"filteredFileOutput", filteredFileOutput});
  testCases.push_back({"enablingConsoleLog", enablingConsoleLog});
  testCases.push_back({"disablingConsoleLog", disablingConsoleLog});
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
  bool result = false;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "", true);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST OUT STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST ERR STR");

  if (
    (cout_local.str().compare(std::string("TEST OUT STR\n")) == 0) &&
    (cerr_local.str().compare(std::string("TEST ERR STR\n")) == 0)) {
    result = true;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

//std::cout << (cout_local.str().compare("TEST OUT STR") == 0) << " " << (cerr_local.str().compare("TEST ERR STR") == 0) << std::endl;
//std::cout << "STDOUT: " << std::endl << cout_local.str();
//std::cerr << "STDERR: " << std::endl << cerr_local.str();

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

  std::string str((std::istreambuf_iterator<char>(logFile)),
                   std::istreambuf_iterator<char>());

  if (str.compare(std::string("TEST 1 STR\nTEST 2 STR\n")) != 0) {
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

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "", true);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST 1 STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST 2 STR");

  if (
    (cout_local.str().compare(std::string("TEST 1 STR\n")) == 0) &&
    (cerr_local.str().compare(std::string("TEST 2 STR\n")) == 0)) {
    result = true;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  std::ifstream logFile;
  std::string line;
  logFile.open("/tmp/out.log");

  std::string str((std::istreambuf_iterator<char>(logFile)),
                   std::istreambuf_iterator<char>());

  if (str.compare(std::string("TEST 1 STR\nTEST 2 STR\n")) != 0) {
    result = false;
  }

  logFile.close();

  return result;
}

bool LoggerModuleTest::filteredConsoleOutput(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = false;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::ERROR, "", true);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST OUT STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST ERR STR");

  if (
    (cout_local.str().compare(std::string("TEST OUT STR\n")) != 0) &&
    (cerr_local.str().compare(std::string("TEST ERR STR\n")) == 0)) {
    result = true;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

//std::cout << (cout_local.str().compare("TEST OUT STR") == 0) << " " << (cerr_local.str().compare("TEST ERR STR") == 0) << std::endl;
//std::cout << "STDOUT: " << std::endl << cout_local.str();
//std::cerr << "STDERR: " << std::endl << cerr_local.str();

  delete logger;

  return result;
}

bool LoggerModuleTest::filteredFileOutput(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::ERROR, "/tmp/out.log", false);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST 1 STR");
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST 2 STR");

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  std::ifstream logFile;
  std::string line;
  logFile.open("/tmp/out.log");

  std::string str((std::istreambuf_iterator<char>(logFile)),
                   std::istreambuf_iterator<char>());

  if (str.compare(std::string("TEST 2 STR\n")) != 0) {
    result = false;
  }

  logFile.close();

  return result;
}

bool LoggerModuleTest::enablingConsoleLog(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "/tmp/out.log", false);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST 1 STR");
  logger->enableConsoleLog();
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST 2 STR");

  if ((cout_local.str() != "") || (cerr_local.str().compare(std::string("TEST 2 STR\n")) != 0 )) {
    result = false;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  std::ifstream logFile;
  std::string line;
  logFile.open("/tmp/out.log");

  std::string str((std::istreambuf_iterator<char>(logFile)),
                   std::istreambuf_iterator<char>());

  if (str.compare(std::string("TEST 1 STR\nTEST 2 STR\n")) != 0) {
    result = false;
  }

//std::cout << (cout_local.str().compare("TEST OUT STR") == 0) << " " << (cerr_local.str().compare("TEST ERR STR") == 0) << std::endl;
//std::cout << "STDOUT: " << std::endl << cout_local.str();
//std::cerr << "STDERR: " << std::endl << cerr_local.str();

  logFile.close();

  return result;
}

bool LoggerModuleTest::disablingConsoleLog(void) {
  auto cout_buff = std::cout.rdbuf();
  auto cerr_buff = std::cerr.rdbuf();
  std::ostringstream cout_local;
  std::ostringstream cerr_local;
  bool result = true;

  std::cout.rdbuf(cout_local.rdbuf());
  std::cerr.rdbuf(cerr_local.rdbuf());

  mlibs::logger::LoggerInterface * logger = new mlibs::logger::Logger(mlibs::logger::DEBUG, "", true);

  logger->writeLogMessage(mlibs::logger::DEBUG, "TEST 1 STR");
  logger->disableConsoleLog();
  logger->writeLogMessage(mlibs::logger::CRITICAL, "TEST 2 STR");

  if (
    (cout_local.str().compare(std::string("TEST 1 STR\n")) == 0) &&
    (cerr_local.str().compare(std::string("TEST 2 STR\n")) == 0)) {
    result = true;
  }

  std::cout.rdbuf(cout_buff);
  std::cerr.rdbuf(cerr_buff);

  delete logger;

  std::ifstream logFile;
  std::string line;
  logFile.open("/tmp/out.log");

  std::string str((std::istreambuf_iterator<char>(logFile)),
                   std::istreambuf_iterator<char>());

  if (str.compare(std::string("TEST 1 STR\nTEST 2 STR\n")) != 0) {
    result = false;
  }

  logFile.close();

  return result;
}


#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include "common/TestSuite.hh"

class LoggerModuleTest : public TestSuite {
  static bool noOutput(void);
  static bool consoleOutput(void);
  static bool fileOutput(void);
  static bool bothOutput(void);
public:
  LoggerModuleTest();
};

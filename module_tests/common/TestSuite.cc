#include "common/TestSuite.hh"

#include <exception>
#include <iostream>
#include <sstream>

void TestSuite::preTesting(void) {
  return;
}

bool TestSuite::runTest(TestCase &tc) {
  bool result = false;

  try {
    result = tc.tc();
  }
  catch (std::exception& e) {
    std::cerr << "TestSuite " << typeid(this).name() <<
                " was failed at " << tc.name << " test case." <<
                " Exception was caught: " << e.what() << std::endl;
  }

  return result;
}

bool TestSuite::runAll(void) {
  preTesting();

  unsigned int successCnt = 0u, failCnt = 0u, progressCnt = 0u, testCaseCnt = testCases.size();
  for (auto &tc: testCases) {
    progressCnt++;
    std::cout << "\x1b[0;33;40mRunning " <<
                 "[" << progressCnt << "/" << testCaseCnt << "] (" <<
                 progressCnt / (float)testCaseCnt * 100.0f << " %): " <<
                 tc.name << "\x1b[0m" << std::endl;
    if (runTest(tc)) {
      successCnt++;
      std::cout << "\x1b[0;32;40mPASSED\x1b[0m" << std::endl;
    } else {
      failCnt++;
      std::cout << "\x1b[0;31;40mFAILED\x1b[0m" << std::endl;
    }
  }

  postTesting();
}

void TestSuite::postTesting(void) {
  return;
}

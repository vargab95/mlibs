#include <vector>

#include "common/TestSuite.hh"
#include "logger/LoggerModuleTest.hh"

int main(int, const char *[])
{
  std::vector<TestSuite*> testSuites;

  testSuites.push_back(new LoggerModuleTest());

  for (auto ts: testSuites) {
    ts->runAll();
    delete ts;
  }
}

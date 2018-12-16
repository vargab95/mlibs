#pragma once

#include <vector>
#include <string>

struct TestCase {
  std::string name;
  bool (*tc)(void);
};

class TestSuite {
protected:
  std::vector<TestCase> testCases;

public:
  virtual void preTesting(void);
  virtual bool runTest(TestCase &tc);
  virtual bool runAll(void);
  virtual void postTesting(void);
};

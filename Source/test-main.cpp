//
//  test-main.cpp
//  JUCE Tests
//
//  Created by Benjamin Tilley on 08/04/2020.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

int main(int argc, const char *argv[]) {

  std::cout << "Running all JUCE tests!\n";

  UnitTestRunner testRunner;
  testRunner.runAllTests();

  return 0;
}

//
//  main.cpp
//  JUCE Tests
//
//  Created by Benjamin Tilley on 08/04/2020.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

int main(int argc, const char *argv[]) {
  // insert code here...
  std::cout << "Hello, World! My gully and my pride...\n";

  UnitTestRunner testRunner;
  testRunner.runAllTests();

  return 0;
}

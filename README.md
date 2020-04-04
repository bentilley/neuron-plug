# Wells Audio Plugin

## Directory Structure

- Builds - this is where JUCE keeps the build options for the project;
- JuceLibraryCode - this is some of the JUCE library code;
- Source - these are the main code files for the plugin;
- scripts - these are some useful build scripts etc.
- tests - tests written in catch (used to test JUCE agnostic code)

## Files

There are some useful files in the root directory of the project:

- Wells.jucer - this is the Projucer file for the project;
- playground.filtergraph - this is the saved config for the AudioPluginHost;
- tags - this is a tags file for the project (recompile with `ctags -R . ../JUCE/modules`);
- xcodebuild.log - this is the build log from running `make`;
- Makefile - the make file.

### The Makefile

The make file is set up to compile the plugin using `xcodebuild` and then
restart the AudioPluginHost program that comes with JUCE. This program is used
to host the plug in without having to load up a DAW.

When the plugin is recompiled, it pipes the output of `xcodebuild` through
`xcpretty` for pretty output. The raw output ends up in the "xcodebuild.log"
file.

## Testing

There are two sets of tests for this project:

1. JUCE tests - written using the JUCE `UnitTestRunner` to test the code
   written using JUCE [TODO];
   - see
     https://forum.juce.com/t/any-better-way-to-write-unit-tests-in-juce/25288/4
   - and https://docs.juce.com/master/classUnitTestRunner.html
2. Catch2 tests - written using the catch2 library to test algorithmic code
   that does not depend on the JUCE library (i.e. the well models).

To run the JUCE test... TODO.

To run the Catch tests run `make test`.

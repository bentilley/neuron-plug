# Usage:
# make		# compile plugin and restart audio plugin host

.PHONY: all compile clean test jucetest xcode

all: compile

xcode:
	@open Builds/MacOSX/Wells.xcodeproj	

compile:
	@xcodebuild \
	  -project Builds/MacOSX/Wells.xcodeproj \
	  -target "Wells - VST3" \
	  | tee xcodebuild.log \
	  | xcpretty
	@./scripts/refresh_pluginhost


# Testing

GCC = gcc-9
COMPILER_OPTIONS = -lstdc++ -Ilibs
FILES_TO_TEST = Source/WellNeurons/*.cpp

ODIR = obj
BRAIN_HEADERS = $(wildcard Source/WellNeurons/*.hpp)
BRAIN_SRC = $(wildcard Source/WellNeurons/*.cpp)
BRAIN_TESTS_SRC = $(wildcard tests/*.test.cpp)
BRAIN_OBJ = $(patsubst Source/WellNeurons/%.cpp,obj/%.o,$(BRAIN_SRC))
BRAIN_TESTS_OBJ = $(patsubst tests/%.cpp,tests/obj/%.o,$(BRAIN_TESTS_SRC))

test: tests/run_tests
	@./tests/run_tests

tests/run_tests: $(BRAIN_TESTS_OBJ) $(BRAIN_OBJ)
	@$(GCC) $(COMPILER_OPTIONS) \
	  $(BRAIN_TESTS_OBJ) $(BRAIN_OBJ)\
	  -o tests/run_tests

tests/obj/%.test.o: tests/%.test.cpp $(BRAIN_HEADERS)
	@$(GCC) $(COMPILER_OPTIONS) -c $< -o $@

obj/%.o: Source/WellNeurons/%.cpp $(BRAIN_HEADERS)
	@$(GCC) $(COMPILER_OPTIONS) -c $< -o $@

clean:
	@rm obj/*
	@rm tests/obj/*

MIDI_GENERATOR_DIR = Source/MidiGenerator/

jucetest: tests/run_juce_tests
	@./tests/run_juce_tests

tests/run_juce_tests: Source/test-main.cpp $(shell find $(MIDI_GENERATOR_DIR) -name "*.cpp")
	@xcodebuild \
	  -project Builds/MacOSX/Wells.xcodeproj \
	  -target "testwells" \
	  | xcpretty

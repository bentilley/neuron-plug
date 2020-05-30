# Usage:
# make		# compile plugin and restart audio plugin host

.PHONY: all compile clean test jucetest xcode clean_xcode

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

ODIR = obj
MIDI_GENERATOR_DIR = Source/MidiGenerator/

BRAIN_HEADERS = $(wildcard $(MIDI_GENERATOR_DIR)/WellNeurons/*.hpp)
BRAIN_SRC = $(wildcard $(MIDI_GENERATOR_DIR)/WellNeurons/*.cpp)
BRAIN_TESTS_SRC = $(wildcard tests/*.test.cpp)
BRAIN_OBJ = $(patsubst $(MIDI_GENERATOR_DIR)/WellNeurons/%.cpp,obj/%.o,$(BRAIN_SRC))
BRAIN_TESTS_OBJ = $(patsubst tests/%.cpp,tests/obj/%.o,$(BRAIN_TESTS_SRC))

test: tests/run_tests
	@./tests/run_tests

tests/run_tests: $(BRAIN_TESTS_OBJ) $(BRAIN_OBJ)
	@$(GCC) $(COMPILER_OPTIONS) \
	  $(BRAIN_TESTS_OBJ) $(BRAIN_OBJ)\
	  -o tests/run_tests

tests/obj/%.test.o: tests/%.test.cpp $(BRAIN_HEADERS)
	@$(GCC) $(COMPILER_OPTIONS) -c $< -o $@

obj/%.o: $(MIDI_GENERATOR_DIR)/WellNeurons/%.cpp $(BRAIN_HEADERS)
	@$(GCC) $(COMPILER_OPTIONS) -c $< -o $@

clean:
	@rm obj/*
	@rm tests/obj/*

jucetest: tests/run_juce_tests
	@./tests/run_juce_tests

tests/run_juce_tests: Source/test-main.cpp $(shell find $(MIDI_GENERATOR_DIR) -name "*.cpp")
	@xcodebuild \
	  -project Builds/MacOSX/Wells.xcodeproj \
	  -target "testwells" \
	  | xcpretty

clean_xcode:
	@xcodebuild \
	  clean \
	  -project Builds/MacOSX/Wells.xcodeproj

compile_commands.json: clean_xcode
	@xcodebuild \
	  -project Builds/MacOSX/Wells.xcodeproj \
	  | xcpretty \
	  -r json-compilation-database \
	  -o compile_commands.json

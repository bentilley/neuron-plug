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

# If you set up new targets you might need to rebuild this. Only if you're using clangd though...
compile_commands.json: clean_xcode
	@xcodebuild \
	  -project Builds/MacOSX/Wells.xcodeproj \
	  -alltargets \
	  | xcpretty \
	  -r json-compilation-database \
	  -o compile_commands.json

clean_xcode:
	@xcodebuild \
	  clean \
	  -project Builds/MacOSX/Wells.xcodeproj


# Testing

test: Builds/MacOSX/build/Debug/test-wells-plugin
	@./Builds/MacOSX/build/Debug/test-wells-plugin

Builds/MacOSX/build/Debug/test-wells-plugin: $(shell find Source -name "*.cpp") 
	@xcodebuild \
	  -project Builds/MacOSX/Wells.xcodeproj \
	  -target "Test Wells" \
	  | xcpretty

# TODO Remove old separate JUCE test target

# jucetest: tests/run_juce_tests
# 	@./tests/run_juce_tests

# tests/run_juce_tests: Source/test-main.cpp $(shell find $(MIDI_GENERATOR_DIR) -name "*.cpp")
# 	@xcodebuild \
# 	  -project Builds/MacOSX/Wells.xcodeproj \
# 	  -target "testwells" \
# 	  | xcpretty

# ENDTODO Remove old separate JUCE test target

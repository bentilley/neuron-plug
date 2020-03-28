# Usage:
# make		# compile plugin and restart audio plugin host

.PHONY: all compile

all: compile

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

test: tests/run_tests
	@./tests/run_tests

tests/run_tests: tests/run_tests.cpp tests/*.test.cpp
	@$(GCC) $(COMPILER_OPTIONS) \
	  tests/run_tests.cpp \
	  -o tests/run_tests

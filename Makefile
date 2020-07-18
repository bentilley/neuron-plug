# Usage:
# make		# compile plugin and restart audio plugin host

.PHONY: all compile test xcode clean_xcode docs

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


# Documentation

docs: docs/generated
	@python -m http.server 8000 --directory docs/generated/html

docs/generated: $(shell find Source -name "*.?pp")
	@rm -r docs/generated
	@doxygen .doxygenrc

docs/%.png: docs/%.plantuml
	@plantuml $<

#
# Helper script to build the project using CMake and CI/CD pipelines
#

# Variables
BUILD_DIR := build

.PHONY: all install dependency build run clean doc test release

all: install dependency build run doc test

# Install dependencies
dependency:
	@echo "Installing dependencies..."
ifeq ($(OS), Windows_NT)
	@choco install gcc g++ cmake llvm ninja doxygen graphviz clang-format lcov
else
	@sudo apt-get install -y gcc g++ cmake llvm ninja-build doxygen graphviz clang-format texlive
endif

# Dependency graph generation
dep_graph:
	@echo "Generating dependency graph..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. --graphviz=graph.dot
	@dot -Tpng $(BUILD_DIR)/graph.dot -o $(BUILD_DIR)/graphImage.png

# Prepare the build directory
clean:
	@echo "Preparing build directory..."
	@rm -rf $(BUILD_DIR)
	@mkdir $(BUILD_DIR)

# Build debug
build-debug:
	@echo "Cleaning cmake cache..."
	@rm -rf $(BUILD_DIR)/CMakeCache.txt $(BUILD_DIR)/CMakeFiles $(BUILD_DIR)/Makefile $(BUILD_DIR)/cmake_install.cmake

	@echo "Configuring Debug build..."
	@cmake -DCMAKE_BUILD_TYPE=Debug \
	       -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
	       --no-warn-unused-cli \
	       -S . \
	       -B $(BUILD_DIR) \
	       -G "Ninja"

	@echo "Building Debug..."
	@cd $(BUILD_DIR) && cmake .. && cmake --build . --config Debug -j 4
	@echo "========= Debug build done ========="

# Build release
build-release:
	@echo "Cleaning cmake cache..."
	@rm -rf $(BUILD_DIR)/CMakeCache.txt $(BUILD_DIR)/CMakeFiles $(BUILD_DIR)/Makefile $(BUILD_DIR)/cmake_install.cmake

	@echo "Configuring release build..."
	@cmake -DCMAKE_BUILD_TYPE=Release \
	       -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
	       --no-warn-unused-cli \
	       -S . \
	       -B $(BUILD_DIR) \
	       -G "Ninja" \
		   -DENABLE_TESTS=OFF

	@echo "Building release..."
	@cmake --build $(BUILD_DIR) --config Release --target all -- -j 4
	@echo "========= Release build done ========="

# Install the project
install:
	@echo "Installing the application..."
# Create the necessary directories in the INSTALL_ROOT
	@mkdir -p $(DESTDIR)/$(INSTALL_ROOT)/bin

# Copy all the files from release build to the bin directory
	@cp $(BUILD_DIR)/Release/* $(DESTDIR)/$(INSTALL_ROOT)/bin
	@echo "List of files in the bin directory:"
	@ls $(BUILD_DIR)/Release/
	@echo "List of files in the install directory:"
	@ls -l $(DESTDIR)/$(INSTALL_ROOT)/bin
# Copy the desktop file to the applications directory
	@cp ./resources/*.desktop $(DESTDIR)/$(INSTALL_ROOT)/bin
# Copy the icon to the icons directory
	@cp ./resources/icons/*.png $(DESTDIR)/$(INSTALL_ROOT)/bin

# Run the application
run:
	@echo "Running the project..."
ifeq ($(OS), Windows_NT)
	@./$(BUILD_DIR)/build/Debug/*.exe
else
	@./$(BUILD_DIR)/build/Debug/*
endif

# Generate documentation (custom target)
html:
	@echo "Generating html doxygen documentation..."
	@cd $(BUILD_DIR) && cmake --build . --target html

pdf:
	@echo "Generating pdf-latex doxygen documentation..."
	@cd $(BUILD_DIR) && cmake --build . --target pdf

doc: html pdf
	@echo "All Documentation generated successfully!"

test:
	@echo "Running tests..."
	@cd $(BUILD_DIR) && cmake .. -DENABLE_TESTS=ON && cmake --build . && ctest --output-on-failure

diagrams: build

ifeq ($(OS), Windows_NT)
	@echo "Generating diagrams..."
	mkdir -p docs/diagrams/plantuml
	mkdir -p docs/diagrams/mermaid
	clang-uml -g plantuml -g json -g mermaid -p
	@echo "Converting .puml files to SVG and PNG..."
	plantuml -tsvg -nometadata -o plantuml docs/diagrams/*.puml
	plantuml -tpng -nometadata -o plantuml docs/diagrams/*.puml
	@echo "Convert .mmd files to svg images"
	py utils/generate_mermaid.py "docs/diagrams/*.mmd"
	@echo "Format generated SVG files..."
	py utils/format_svg.py docs/diagrams/plantuml/*.svg
	py utils/format_svg.py docs/diagrams/mermaid/*.svg
	@echo "Done!"
else
	@echo "installing dependencies..."
	sudo apt-get install -y plantuml npm python3 python3-pip python3-yaml
	npm install -g mermaid.cli
	pip install pyyaml
	pip install cairosvg
	@echo "installing clang-uml..."

	sudo add-apt-repository ppa:bkryza/clang-uml
	sudo apt update
	sudo apt install clang-uml
	@echo "Generating diagrams..."
	mkdir -p docs/diagrams/plantuml
	mkdir -p docs/diagrams/mermaid
	clang-uml -g plantuml -g json -g mermaid -p
	@echo "Convert .puml files to svg images"
	plantuml -tsvg -nometadata -o plantuml docs/diagrams/*.puml
	@echo "Convert .mmd files to svg images"
	py utils/generate_mermaid.py "docs/diagrams/*.mmd"
	@echo "Format generated SVG files..."
	py utils/format_svg.py docs/diagrams/plantuml/*.svg
	py utils/format_svg.py docs/diagrams/mermaid/*.svg
	@echo "Done!"
endif



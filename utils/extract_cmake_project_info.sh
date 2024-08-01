#!/bin/bash

# Script to extract project name and version from CMakeLists.txt

CMAKE_FILE=$1

if [ ! -f "$CMAKE_FILE" ]; then
  echo "Error: $CMAKE_FILE not found."
  exit 1
fi

PROJECT_NAME=$(grep -E 'set\s*\(\s*PROJECT_NAME\s*".*"\s*\)' "$CMAKE_FILE" | sed -E 's/set\s*\(\s*PROJECT_NAME\s*"([^"]*)"\s*\)/\1/')
PROJECT_VERSION=$(grep -E 'set\s*\(\s*PROJECT_VERSION\s*[0-9]+\.[0-9]+\.[0-9]+\s*\)' "$CMAKE_FILE" | sed -E 's/set\s*\(\s*PROJECT_VERSION\s*([0-9]+\.[0-9]+\.[0-9]+)\s*\)/\1/')

echo "PROJECT_NAME=$PROJECT_NAME"
echo "PROJECT_VERSION=$PROJECT_VERSION"

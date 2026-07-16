#!/bin/bash

echo "Building Nova Language v0.2.0"

# Create directories
mkdir -p src/lexer src/parser src/ast src/interpreter src/compiler src/vm src/runtime src/stdlib src/errors
mkdir -p examples tests

# Make bin executable
chmod +x bin/nova
chmod +x build.sh
chmod +x setup.sh

echo "Build complete!"
echo ""
echo "Usage:"
echo "  ./bin/nova examples/hello.nv"
echo "  ./bin/nova examples/complex.nv --vm"
echo "  ./bin/nova examples/complex.nv --ast --tokens"
echo ""
echo "Testing:"
echo "  node tests/test.js"
echo "  node tests/vm-test.js"

#!/bin/bash
# Compilación para el proyecto

mkdir -p build
cd build
cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ..
make

echo ""
echo "Para ejecutar: "
echo "cd build && ./homework_graph"

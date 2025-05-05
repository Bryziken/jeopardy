#!/bin/bash

# Compiler and flags
CXX=g++
CXXFLAGS="-Wall -std=c++17"

# Include path for cgicc (adjust if different)
INCLUDES="-I/usr/include"

# Source files and corresponding output executables
declare -A sources=(
    ["main.cpp"]="main"
    ["getQuestion.cpp"]="getQuestion"
    ["getScores.cpp"]="getScores"
    ["validate.cpp"]="validate"
    ["scores.cpp"]="scores"
    ["timerSystem.cpp"]="timerSystem"
)

echo "Building C++ CGI programs in public_html/cgi-bin/..."

# Loop over sources and compile
for src in "${!sources[@]}"; do
    exe="${sources[$src]}"
    echo "Compiling $src -> $exe"
    
    # Add -lcgicc only for files that include cgicc (e.g., main.cpp, validate.cpp)
    if grep -q "cgicc" "$src"; then
        $CXX $CXXFLAGS $INCLUDES -o "$exe" "$src" -lcgicc
    else
        $CXX $CXXFLAGS -o "$exe" "$src"
    fi

    if [ $? -ne 0 ]; then
        echo "❌ Failed to compile $src"
        exit 1
    fi
done

# Make them executable
chmod +x main getQuestion getScores validate scores timerSystem

echo "Build completed successfully!"

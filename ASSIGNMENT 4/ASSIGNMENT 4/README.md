# Assignment 4 – CO4: Designing Reusable Class Hierarchies
**Name:** G Yuvaraj  
**Register Number:** 192421065  
**Repository:** `DSA01-192421065-Assignment4`

## Contents
- `Report.md`: answers, class diagrams, access table, predictions, explanations and complexity analysis for Tasks 1–6.
- `Task01` … `Task06`: C++17 programs and sample outputs.
- `compile_all.sh`: compiles and runs all six programs.

## Compiler and commands
Prepared for GCC/g++ and C++17. Check the compiler installed on your own system:
```bash
g++ --version
g++ -std=c++17 -Wall -Wextra -pedantic Task01/main.cpp -o Task01/task01
./Task01/task01
```
Use the corresponding folder for Tasks 02–06. For AddressSanitizer:
```bash
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer Task04/main.cpp -o task04_asan
./task04_asan
```

## GitHub submission
Create a repository named exactly `DSA01-192421065-Assignment4`. Commit each task separately and add the faculty as a collaborator if private. Add the actual repository URL to this README and the report cover page after creating it. No GitHub URL is invented here.

## Required work before submission
The assignment requires individual execution, screenshots, progressive commits, sanitizer evidence and a viva. The programs were compiled and run in the preparation environment, and sample outputs are included. You must still run them on your own machine, record your actual compiler version, capture genuine screenshots, perform the requested experiments (including virtual/non-virtual size comparisons and sanitizer runs), and understand the code. Do not claim screenshots, measurements or GitHub commits that you have not personally produced.

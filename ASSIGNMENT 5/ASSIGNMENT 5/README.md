# Assignment 5 – CO5
**Name:** G Yuvaraj  
**Register Number:** 192421065  
**Course Outcome:** CO5 – Runtime polymorphism and exception handling

## Build environment
Tested with GCC/G++ available in the preparation environment. Check your own compiler with `g++ --version`.

## Compile
From the assignment root:
```bash
g++ --version
g++ -std=c++17 -Wall -Wextra -pedantic Task01/main.cpp -o task01
g++ -std=c++17 -Wall -Wextra -pedantic Task02/main.cpp -o task02
g++ -std=c++17 -Wall -Wextra -pedantic Task03/main.cpp -O2 -o task03
g++ -std=c++17 -Wall -Wextra -pedantic Task04/main.cpp -o task04
```
Run each executable with `./task01`, etc. On Windows use the corresponding `.exe`.

## Contents
- `PartA_Design_Report.md`: Tasks A1–A5, diagrams, pseudocode and alternative analyses.
- `Task01`: B1 polymorphic bill hierarchy and completed TODOs.
- `Task02`: B2 exception validation, logging and rethrowing; completed TODOs.
- `Task03`: B3 virtual-vs-switch benchmark harness and exception-cost harness.
- `Task04`: B4 new bill/payer extension.
- Each task folder includes source and expected sample output.

## Important submission checklist
This package is a worked solution and reproducible starting point. Before submitting:
1. Run every program on your own computer and save genuine terminal screenshots.
2. Run the sanitizer command shown in each task and include the actual report.
3. Run benchmark tests on your machine; replace illustrative/expected results with your measured results.
4. Create `DSA01-192421065-Assignment5` on GitHub, commit each task separately, and put the real repository URL on the report cover page. No GitHub URL or screenshots are fabricated here.

# ASSIGNMENT 2 – CO2
**Student:** G Yuvaraj  
**Register Number:** 192421065

## Repository contents
- `Task01/` – call by value, reference, pointer, return-by-reference and default arguments
- `Task02/` – safe and unsafe reference-return examples
- `Task03/` – default arguments and function overloading
- `Task04/` – class with a fixed transaction array
- `Task05/` – static members and arrays of objects
- `Task06/` – friend functions and inline timing experiment
- `Task07/` – diagnose and redesign a transaction-history buffer
- `REPORT.md` – written answers, predictions, explanations and design evaluation
- Each task contains source code and captured console output.

## Build environment
The programs are standard C++17. The source files were compiled in the preparation environment with:
```bash
g++ --version
g++ -std=c++17 -Wall -Wextra -pedantic source.cpp -o program
```
Run a task, for example:
```bash
cd Task01
g++ -std=c++17 -Wall -Wextra -pedantic q1.cpp -o q1
./q1
```
For the inline timing experiment, use both:
```bash
g++ -std=c++17 -O0 -Wall -Wextra q6_timing.cpp -o timing_O0
g++ -std=c++17 -O2 -Wall -Wextra q6_timing.cpp -o timing_O2
./timing_O0
./timing_O2
```

## Task result notes
- Q1: Value parameters do not modify the caller; reference and pointer parameters do.
- Q2: A reference to an array element can modify it; returning a local variable by reference is invalid.
- Q3: Overload sets are redesigned so calls have unambiguous signatures.
- Q4: A private fixed-size array retains the latest ten transactions.
- Q5: Static data tracks the shared issued-seat count across all ticket objects.
- Q6: Friend functions compare private time fields; a timing program compares ordinary and inline calls.
- Q7: A circular buffer avoids shifting all stored entries when the history is full.

## Submission checklist
1. Replace/confirm the compiler version and rerun every program on your own machine.
2. Add your actual GitHub repository URL below and on the report cover page.
3. Create `DSA01-192421065-Assignment2` on GitHub, upload this folder, and commit each task progressively (one or more commits per task).
4. Add the faculty as a collaborator if the repository is private.
5. Include your own screenshots of compilation and execution if your faculty requires screenshots.

**GitHub Repository Link:** TO BE ADDED BY STUDENT

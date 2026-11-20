# Experimental commands and evidence log

## Build and run
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic Task03/main.cpp -o bench
./bench 1000
./bench 1000000
./bench 10000000
g++ -std=c++17 -O2 -Wall -Wextra -pedantic Task03/exception_cost.cpp -o exception_cost
./exception_cost
```
Repeat each test at least five times; report median and machine/compiler details. Do not copy timings from another machine.

## AddressSanitizer
```bash
g++ -std=c++17 -g -O1 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer Task01/main.cpp -o task01_asan
./task01_asan
g++ -std=c++17 -g -O1 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer Task02/main.cpp -o task02_asan
./task02_asan
```
A clean run should finish without sanitizer diagnostics. Save your actual terminal output.

## Required student evidence
| Experiment | Record |
|---|---|
| Virtual vs switch, 1,000 | Add measured milliseconds |
| Virtual vs switch, 1,000,000 | Add measured milliseconds |
| Virtual vs switch, 10,000,000 | Add measured milliseconds |
| Return code vs exception, 0% invalid | Add measured milliseconds |
| Return code vs exception, 1% invalid | Add measured milliseconds |
| Return code vs exception, 50% invalid | Add measured milliseconds |
| AddressSanitizer | Attach genuine report |
| GitHub progressive commits | Add actual repository URL and commit evidence |

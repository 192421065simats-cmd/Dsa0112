# ASSIGNMENT 2 – CO2: C++ Functions and Object-Oriented Concepts

**Name:** G Yuvaraj  
**Register Number:** 192421065  
**GitHub repository:** TO BE ADDED AFTER CREATING THE REQUIRED REPOSITORY

> This report is a worked reference solution based on the supplied assignment brief. The programs were compiled and run in the preparation environment. Re-run them on your own system, retain your own screenshots, and replace the GitHub placeholder before submission. Timing values are environment-dependent and must not be presented as universal results.

---

## Q1 – Call by Value, Reference and Pointer

### 1. Prediction before execution

For `change(a, b, &c)`, initially `a=1`, `b=2`, `c=3`. The value parameter `a` is a copy and remains 1 in the caller. The reference `b` and pointer `c` update the original variables.

Predicted output of the supplied program:

```text
1 12 13
1 100
16 13 6
```

### 2. Explanation of the supplied program

- `change(int a, int& b, int* c)`: `a` is a separate local copy. `b` is an alias for the caller's `b`; `c` stores the address of the caller's `c`. Therefore only `b` and `c` change in the caller.
- `bigger(a,c)` returns a reference to the larger argument. After `change`, `a=1` and `c=13`; the expression `bigger(a,c)=100` assigns to `c`, producing `a=1, c=100`.
- `show(1)` uses `y=5` and `z=10`, giving 16.
- `show(1,2)` uses the default `z=10`, giving 13.
- `show(1,2,3)` supplies every argument, giving 6.

### 3. Complete the Code – TODO 1

```cpp
void applyConcession(long long& fee, int percent) {
    fee -= fee * percent / 100;
}
```

Observed output:

```text
after call by value   : 45000
after call by reference: 40500
after call by pointer  : 36450
returned new fee       : 32805
```

The call-by-value function receives a separate `long long`. It reduces that local copy, not `hostelFee`; therefore the caller's value remains 45000. A reference parameter aliases the original object, so the assignment updates the caller. A pointer achieves the same by dereferencing the supplied address.

### 4. Where parameters are stored

A value parameter is a distinct parameter object (typically stored in a register or stack frame, depending on compiler/ABI and optimisation). A reference behaves as an alias to the original object; an implementation may represent it using an address. A pointer is an ordinary pointer value containing an address. The language specifies behaviour, not a mandatory physical stack/register location.

Use `const T&` when a function must read a potentially large object without copying it and must not modify it. It also accepts temporaries in many common cases.

### 5. Design decision

For a sequence of concessions that must update the student's one authoritative fee, an in-place reference interface is suitable:

```cpp
void applyConcession(long long& fee, int percent);
```

It clearly communicates mutation and avoids accidentally ignoring a returned value. A return-new-value design is also valid and can be preferable for pure calculations:

```cpp
long long applyConcession(long long fee, int percent) {
    return fee - fee * percent / 100;
}
fee = applyConcession(fee, 10);
```

The return-value version is easier to compose and test as a pure function. The reference version is convenient when the caller's state is intentionally updated. For this hostel updater, the reference version is selected because the requirement explicitly asks to update the stored fee.

### 6. Complexity

All three arithmetic implementations take O(1) time and O(1) parameter storage. A `long long` value copy is a fixed-size scalar copy. Passing a large `Student` object by value copies its members and can require O(size of Student) copy work/storage; passing by reference or const reference passes an alias and avoids that object copy, using O(1) reference-sized parameter overhead. A reference does not make the object's own storage disappear.

---

## Q2 – Return by Reference and Its Dangers

### 1. Working functions and observed behaviour

`highest(int marks[], int n)` scans the array and returns a reference to the element with the maximum mark. The caller can write:

```cpp
highest(marks, n) += 2;
```

This modifies the selected array element directly. The included program reports:

```text
Highest before moderation: 91
Highest after +2: 93
Read-only highest: 93
Copied highest: 93
Index identifies element 0
Pointer value: 95
```

The final pointer value is 95 because the example applies a second +2 using the index after the first moderation.

### 2. Returning a local reference

The unsafe example is:

```cpp
int& bad() {
    int local = 7;
    return local; // invalid: local lifetime ends when function returns
}
```

The compiler diagnostic captured in `Task02/compiler_warning.txt` warns that a reference to local variable `local` is returned (`-Wreturn-local-addr`). The local object's lifetime ends at function exit. The returned reference therefore dangles; using it is undefined behaviour. The program deliberately does not call this function.

### 3. Reference, const reference and copy

- `int&`: caller can modify the referred-to array element.
- `const int&`: caller can read the element but cannot modify it through this reference.
- `int`: returns an independent copy; modifying the result does not modify the array.

### 4. Vector growth condition

A `std::vector` may reallocate its storage when its capacity is exceeded. Reallocation moves/copies elements to a new allocation and invalidates references, pointers and iterators to its old elements. A reference to an element obtained before growth must not be used after an operation that may reallocate. `reserve()` can reduce reallocation frequency but does not make references universally safe if capacity is later exceeded.

### 5. Alternative: index or pointer

- Index: `int i = highestIndex(marks,n); marks[i] += 2;`
- Pointer: `int* p = highestPointer(marks,n); *p += 2;`
- Reference: `highest(marks,n) += 2;`

Finding the maximum is O(n) time and O(1) auxiliary space in all three designs. An index is generally the most robust handle across vector reallocations: after growth, use the index again to access the new storage (provided the logical element still exists). A pointer/reference to a vector element can be invalidated by reallocation. A pointer is useful for APIs that need nullable/explicit address semantics, but it has the same invalidation concern.

---

## Q3 – Default Arguments and Function Overloading

### 1. Why the original set is ambiguous

The proposed overloads `charge(double)`, `charge(double,bool)` and `charge(double,bool=false,bool=false)` overlap. A call such as `charge(2.0)` can match more than one overload because the third function can be called with its default Boolean arguments. The compiler cannot choose a unique best overload, so the call is ambiguous.

### 2. Redesigned overload set

The implementation in `Task03/q3.cpp` uses:
- `charge(double, bool=false, bool=false)` for kilograms and optional express/insurance;
- `charge(int)` for grams;
- `charge(float)` for a float kilogram input;
- `charge(const Parcel&)` for a parcel object.

The overloads are intentionally distinct. The `int` overload interprets its argument as grams, while `float` and `double` represent kilograms. This unit distinction must be documented because the numeric type affects meaning.

### 3. Six calls and predictions

| Call | Selected function | Reason |
|---|---|---|
| `charge(2)` | `charge(int)` | exact match; grams |
| `charge(2.0)` | `charge(double,bool,bool)` | exact match; omitted flags use defaults |
| `charge(2.0f)` | `charge(float)` | exact match; float kilograms |
| `charge(2000)` | `charge(int)` | exact match; grams |
| `charge(2.0,true)` | `charge(double,bool,bool)` | exact match for supplied types |
| `charge(p)` | `charge(const Parcel&)` | exact object-type match |

Observed output with the sample rate of 40 currency units per kg, express surcharge 50 and insurance surcharge 20:

```text
charge(2) [int grams]: 0.08
charge(2.0) [double kg]: 80
charge(2.0f) [float kg]: 80
charge(2000) [int grams]: 80
charge(2.0, true) [double + express]: 130
charge(Parcel) [object]: 150
```

### 4. Overload resolution and default arguments

The compiler ranks viable candidates using conversion sequences. In general, an exact match is preferred to a promotion, and a promotion is preferred to a standard conversion. Default arguments make a function callable with fewer explicit arguments; they do not create a separate overload. Default arguments must be supplied from the right because an omitted argument can only be filled when all following parameters also have defaults.

### 5. Alternative concept and complexity

A single `Parcel` structure with fields for weight, express and insurance could be passed to one function. A function template could generalise accepted numeric types, but units still need explicit semantics. Overload resolution happens at compile time and has no per-call runtime cost. Passing a large parcel by value copies it; `const Parcel&` avoids that copy. Adding a new option to a struct is often straightforward but changes the data type/API; overloads can make distinct input forms explicit but may increase the number of functions.

---

## Q4 – Specifying a Class with an Array Inside It

### 1. Class design

`Task04/q4.cpp` defines `SavingsAccount` with private `accountNumber`, `balance`, `transactions[10]` and `count`. `deposit()` and `withdraw()` are declared in the class and defined outside with `SavingsAccount::`. `miniStatement()` is defined inside the class. The private `record()` helper ensures that transaction storage is modified only through the class's rules.

### 2. Why the array is private

The array and count form one invariant: only valid transactions should be recorded, and the count must never exceed ten. Making them private prevents external code from changing the count, overwriting entries or breaking the mini-statement order. Public member functions provide controlled access.

### 3. Eleventh transaction: shift versus circular index

With shifting, the oldest entry is removed by moving entries 1–9 to positions 0–8 and storing the new entry at position 9. Insertion when full costs O(k), where k is the number of retained entries (here at most 10). A circular buffer overwrites the oldest slot and advances a start index in O(1) time. Both use O(10) storage, which is O(1) for a fixed capacity.

The program performs 12 successful operations after account creation; the statement contains the most recent ten entries. The balance and retained transactions are printed by the program output in `Task04/output.txt`.

### 4. Decision for 50 transactions

A circular buffer is selected for 50 entries because it avoids shifting 49 elements on every new transaction after the buffer fills. Increasing the fixed capacity from 10 to 50 changes the array size and capacity constant, but not the circular-buffer algorithm. If dynamic capacity is required, a dynamically allocated circular buffer or a deque can be considered. A deque supports efficient insertion at the back and removal at the front, with O(1) amortised endpoint operations, but uses container management overhead.

---

## Q5 – Static Members and Arrays of Objects

### 1. Static design

In `BusTicket`, `issued` and `CAPACITY` are class-level members. `issued` is shared by all objects, while `seat` and `name` belong to each ticket. The static data member is defined once outside the class:

```cpp
int BusTicket::issued = 0;
```

A static member function has no object on which to operate, so it has no `this` pointer. It cannot directly access a non-static member because such a member requires a particular object. `count()` can access `issued` because it is static.

### 2. Prediction for the supplied Token program

The static `next` starts at 100. Three array objects are constructed, receiving IDs 101, 102 and 103. The `extra` object is constructed next and receives 104. The constructor runs four times.

Predicted output:

```text
101 103 104 4
```

### 3. Completed BusTicket TODOs

```cpp
if (issued < CAPACITY) {
    seat = ++issued;
}
```

and:

```cpp
return issued;
```

Observed output:

```text
Asha: Seat 1
Bala: Seat 2
Chitra: Seat 3
Dev: Waiting
Issued: 3
```

The array `trip[4]` contains four objects. Each constructor uses the same static count, so the first three objects receive seats and the fourth remains on the waiting list.

### 4. Objects created inside a function called twice

The static counter persists for the lifetime of the program; it is not reset when a function returns or when local ticket objects are destroyed. If the function is called twice, the second call continues from the previous `issued` value. In this particular fixed-capacity design, once three seats are issued, later tickets are waiting. If each function call represents a separate bus trip, a per-trip counter/object is required instead of one global class-wide count.

### 5. Global variable and TicketOffice alternatives

A global counter also uses one shared integer and O(1) operations, but it weakens encapsulation and can collide with unrelated code. A `TicketOffice` object passed by reference to each ticket centralises seat allocation and supports multiple independent buses. It adds an office object but allows each bus to have its own capacity and count. The static design is simple for one shared bus service; the office-object design is preferable when two buses/trips must be managed independently.

---

## Q6 – Friend Functions and Inline Functions

### 1. Completed friend functions

```cpp
int minutesBetween(const Time& a, const Time& b) {
    int first = a.h * 60 + a.m;
    int second = b.h * 60 + b.m;
    return std::abs(first - second);
}

bool earlier(const Time& a, const Time& b) {
    return a.h * 60 + a.m < b.h * 60 + b.m;
}
```

Observed output:

```text
205 false
2 true false
```

The first line confirms that 09:45 to 13:10 is 205 minutes and that 13:10 is not earlier than 09:45. The second line tests 11:59 versus 12:01 (two minutes, and the first is earlier) and equal times (not earlier).

### 2. Why friend functions

A friend function is not a member, but the class grants it access to private and protected members. It is called as an ordinary function, for example `minutesBetween(a,b)`, rather than `a.minutesBetween(b)`. A symmetric operation involving two `Time` objects can be expressed naturally with both operands at the same level. A member function would have an implicit left-hand object (`this`).

### 3. Inline meaning and timing

`inline` is a request/permission relevant to linkage and the one-definition rule; it does not force the compiler to substitute the function body at every call. Modern compilers decide whether to inline based on optimisation and other factors.

The included timing program performs 10,000,000 calls for a non-inlined member and an inline member. In this run:

| Build | Non-inlined calls | Inline calls |
|---|---:|---:|
| `-O0` | 14,411 µs | 14,735 µs |
| `-O2` | 16,808 µs | 2,938 µs |

These are one-run measurements from the preparation environment, not guaranteed results. At `-O0`, the inline keyword did not produce a measurable advantage in this run. At `-O2`, the inline accessor was much faster because the compiler could eliminate call overhead and optimise the loop. CPU scheduling, compiler version, machine load and measurement noise affect the result. The source uses a volatile data member and a no-inline attribute for the comparison; the latter is a GCC/Clang extension used only for the experiment.

### 4. Encapsulation and deployment decision

Making `h` and `m` public would let any caller assign invalid values such as hour 27 or minute 90, weakening the class invariant. Friend functions preserve private representation but grant selected external functions privileged access. A member function plus getters avoids friendship but may expose more interface; getters can preserve validation while allowing comparisons. For two-operand symmetric comparison, the friend functions are retained because they keep the interface concise and expose no general public setters. If the class later requires stricter control or a larger API, member functions/getters may be preferable.

---

## Q7 – Diagnose, Redesign and Validate

### Selected program: Q4 transaction history

#### A. Diagnose the limitation

The original shift-array design must move every retained entry when the array is full. For a capacity of k, each new transaction after the first k requires k−1 string assignments plus one new assignment. The work per full-buffer insertion is O(k). This is acceptable for ten records, but the work increases linearly with the configured history capacity.

#### B. Design objective

Retain the latest ten transactions in chronological order while making insertion into a full history O(1), without increasing the fixed storage capacity.

#### C. Revised solution

Use a fixed array, a `start` index identifying the oldest entry, and a `count`. While the buffer is not full, append at `(start + count) % capacity`. Once full, overwrite `data[start]` and advance `start = (start + 1) % capacity`.

Pseudocode:

```text
add(item):
    if count < capacity:
        data[(start + count) % capacity] = item
        count = count + 1
    else:
        data[start] = item
        start = (start + 1) % capacity
```

No additional transaction slots are needed; only two integer indices are maintained. The trade-off is that logical chronological order is no longer identical to physical array order. Printing must traverse from `start` using modular indexing.

#### D. Prediction before implementation

For a full buffer, the shift design performs O(k) element assignments per insertion. The circular design performs one overwrite and one index update, O(1). Both retain ten entries and use O(k) storage.

#### E. Implementation and validation

`Task07/q7.cpp` implements both designs and inserts the same 200,000 generated transaction strings into each. It prints retained counts and elapsed microseconds. Both retain exactly ten entries. The timing output is saved in `Task07/output.txt`. The exact times vary by system; the asymptotic improvement is established by the number of operations rather than one noisy timing run.

#### F. Final evaluation

The redesign meets the objective: it avoids shifting the remaining entries when full and retains the same fixed capacity. Its additional state is two integer fields, O(1), and its insertion is O(1). The original shift design remains simpler and may be preferable for very small histories where implementation simplicity matters more than a bounded number of assignments. For a larger or frequently updated history, the circular buffer is the better design.

---

## Overall conclusion

The assignment demonstrates how parameter-passing choices affect caller state, why returned references must refer to objects that remain alive, how overloads and defaults interact, how class invariants are protected through private members, how static data is shared, and how friend/inline features affect interface and implementation. The redesign task applies complexity analysis to improve a fixed-size transaction history without hiding the underlying data-structure logic.

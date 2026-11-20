# Assignment 4 – CO4: Designing Reusable Class Hierarchies
**Name:** G Yuvaraj  
**Register Number:** 192421065  
**Repository:** `DSA01-192421065-Assignment4`

This worked report follows the six tasks and the terminology used in the assignment. Run each program yourself, capture genuine screenshots, record your compiler version and measurements, and add your actual GitHub link before submission.

## TASK 1 – CLASS DESIGN REVIEW (6 marks)

### Task 1.1 – Diagnose the design
The original payroll classes have these deficiencies:
1. `name`, `id` and `basic` are repeated in multiple classes.
2. Data members are public, so callers can put objects into invalid states.
3. The same concepts use inconsistent names (`name`/`nm`, `id`/`code`).
4. `da` is declared but never used in either Professor or LabAssistant.
5. `0.42` is a magic number with no named meaning or central policy.
6. Adding a staff category requires another unrelated class and duplicated processing logic.
7. The classes have no common base, so all staff cannot be handled uniformly through one base pointer/reference loop.
8. Pay rules are duplicated instead of being expressed through a shared interface.

### Task 1.2 – Repaired hierarchy
`Task01/main.cpp` uses an abstract `Staff` base class. Common identity and basic-pay data are private. Derived classes implement `pay()` with `override`. A virtual destructor makes deletion through a base pointer safe. `std::vector<std::unique_ptr<Staff>>` stores heterogeneous staff without slicing.

UML:
```text
                 <<abstract>>
                    Staff
          -------------------------
          - name : string
          - id : int
          - basic : double
          + pay() : double = 0
          + print() : void
          + virtual ~Staff()
              /       |       \\
             /        |        \\
       Professor  LabAssistant  GuestFaculty
       ---------  ------------  ------------
       - research  (no extra)   - hours
                                - rate
       + pay()     + pay()      + pay()
```
Professor pay = basic + 42% of basic + research. LabAssistant pay = basic + 42% of basic. GuestFaculty pay = hours × rate.

Sample output:
```text
Anita (101) pay=83000
Bala (102) pay=42600
Charan (103) pay=32000
```

### Task 1.3 – Design defence (under 100 words)
The hierarchy puts shared identity and the common `pay()` interface in `Staff`. Each category supplies its own overridden pay rule. The payroll loop works with `Staff` pointers, so it does not need to know the concrete type. To add a new category, create a derived class and implement `pay()`; existing payroll processing remains unchanged. Encapsulation also protects common data from arbitrary external modification.

### Alternative: one class plus type and switch
A single `Staff` class would store a category code and fields for every possible category. `pay()` would switch on the code. Both a switch and virtual dispatch are O(1) per call. The single class wastes per-object space on fields irrelevant to that category. Adding a new type requires changing the type definition, the switch in `pay()`, and often constructors/validation/output code. With a virtual hierarchy, normally only the new derived class and its creation/registration code need to be added.

## TASK 2 – FROM REQUIREMENT TO HIERARCHY (8 marks)

### Task 2.1 – Relationships
- `Student is-a Person`: public single inheritance.
- `Staff is-a Person`: public single inheritance.
- `TeachingAssistant is-a Student` and `is-a Staff`: public multiple inheritance; combined structure is hybrid inheritance.
- `ResearchScholar is-a Student`: public multilevel inheritance (Person → Student → ResearchScholar).
- `Person has-a Address` and `Student has-a Course` are composition/member-object relationships, not inheritance.

### Task 2.2 – Diamond problem
Without virtual inheritance, `TeachingAssistant : Student, Staff` contains two separate `Person` subobjects, because both Student and Staff inherit Person. A reference such as `ta.name_` becomes ambiguous, and the object duplicates the common Person state. Declaring both intermediate bases as `virtual public Person` makes the most-derived TeachingAssistant contain one shared Person subobject. The most-derived constructor must initialize that virtual base.

### Task 2.3 – Class declarations
The complete declarations and executable demonstration are in `Task02/main.cpp`. `Student` and `Staff` use `virtual public Person`; `TeachingAssistant` inherits publicly from both. `ResearchScholar` inherits publicly from Student. Public inheritance models the stated is-a relationships and preserves the public Person interface. Person data is protected in this demonstration; a production design could keep it private and expose protected/public accessors.

### Task 2.4 – Access table
Visibility of a `Person` member inside `Student`:

| Person member | public inheritance | protected inheritance | private inheritance |
|---|---|---|---|
| public | public | protected | private |
| protected | protected | protected | private |
| private | not directly accessible | not directly accessible | not directly accessible |

Private base members remain inaccessible directly to derived classes regardless of inheritance mode; public/protected accessors can expose controlled operations.

### Predict the output
For the supplied virtual-diamond program, predicted output:
```text
Person Student Staff TA
```
The virtual base `Person` is constructed once, before the non-virtual direct bases. Then `Student`, `Staff`, and `TeachingAssistant` constructors run in their declaration order. Removing `virtual` gives two Person subobjects and the normal output becomes:
```text
Person Student Person Staff TA
```
There is no required compile error for that exact program because it does not access an ambiguous Person member; ambiguity appears if code refers to an inherited Person member such as `ta.name`.

### Alternative: composition
A composition version would contain `Student student; Staff staff;` members in TeachingAssistant. This models a TA as having two role records, rather than being substitutable for both Student and Staff. If each role separately contains a Person, there are two Person subobjects; if both roles share a single Person by storing references/pointers, lifetime and ownership must be designed carefully. Access becomes explicit (`ta.student.name()` or `ta.staff.department()`), avoiding inherited-name ambiguity. Composition usually uses extra member storage for both contained objects; compare actual `sizeof` values on the target compiler because padding and virtual-base implementation are ABI-dependent. Access time remains O(1).

## TASK 3 – CONSTRUCTORS IN DERIVED CLASSES AND MEMBER CLASSES (9 marks)

### Task 3.1 – Passing values to base constructors
A derived constructor invokes its base constructor in its member-initializer list, e.g. `Student(...) : Person(name, id, phone), ...`. With a virtual base, only the most-derived class's initializer for that virtual base is effective. Therefore `TeachingAssistant` must directly initialize `Person`; the `Person(...)` initializers written in Student and Staff are ignored when those classes are constructed as base subobjects of TeachingAssistant.

### Task 3.2 – Member classes
`Person` contains an `Address` object and `Student` contains a `Course` object. Their values are passed in the initializer list, e.g. `address_(city, pin)` and `course_(courseName)`. Composition ensures the member's lifetime is tied to its containing object.

### Task 3.3 – Construction/destruction trace
The executable `Task03/main.cpp` creates one Student and prints constructor/destructor messages. For a Student, the order is:
1. Base class `Person` begins construction.
2. Person's member `Address` is constructed before the Person constructor body.
3. Person constructor body runs.
4. Student member `Course` is constructed.
5. Student constructor body runs.
Destruction reverses the order: Student destructor body, Course destructor, Person destructor body, Address destructor. For a TeachingAssistant with virtual Person, construction order is virtual base Person, direct bases in declaration order, members, then most-derived body; destruction is the reverse. For a standalone Staff object, Person is constructed before Staff and destroyed after Staff.

Sample output from Task03:
```text
Student lifetime: Address+ Person+ Course+ Student+ 192311001 Kavya (19) Salem-636001
Student- Course- Person- Address-
```

### Task 3.4 – Removing virtual
Removing `virtual` from the two intermediate inheritance declarations makes a TeachingAssistant contain two Person subobjects. The virtual-base construction occurs once only in the virtual design. `sizeof(TeachingAssistant)` generally increases when the duplicate Person state is present, but exact byte counts depend on the compiler ABI, padding and implementation. Record `sizeof` from your own g++ run; do not use a guessed number.

### Predict the output
For the supplied Engine/Vehicle/Car code:
```text
Vehicle Engine Car | ~Car ~Engine ~Vehicle
```
The base class `Vehicle` is constructed first, then the member object `Engine`, then the `Car` constructor body. At scope exit, destruction runs in reverse: derived destructor body, member destructor, then base destructor.

### Complete the code – TODO 1
The correct initializer-list constructor is:
```cpp
Student(const std::string& n, int a, long long reg,
        const std::string& city, int pin)
    : Person(n, a), regNo(reg), home(city, pin) {}
```
For the supplied complete program, expected output:
```text
192311001 Kavya (19) Salem-636001
```
If initializer-list order is deliberately changed so it differs from member declaration order, `-Wall -Wextra` can produce `-Wreorder`. Actual initialization still follows declaration order, not the textual order of the initializer list.

### Alternative: dynamically allocated Address
With `Address* home = new Address(...)`, the constructor must allocate the Address and the destructor must `delete home`. A deep-copy constructor and copy-assignment operator are needed to avoid two Person objects owning and deleting the same pointer; alternatively use `std::unique_ptr<Address>` and define/disable copying appropriately. The pointer adds `sizeof(Address*)` storage per Person, and dynamic allocation adds allocation/deallocation overhead. Access uses `home->str()` instead of `home.str()`. A member object avoids a separate heap allocation and follows Rule of Zero.

## TASK 4 – POINTERS TO DERIVED CLASSES: FORENSICS (7 marks)

### Task 4.1 – Defects
1. `rate()` in the original Account is not virtual, so `a->rate()` is statically bound to `Account::rate()` when called through `Account*`; SeniorAccount's function does not override polymorphically.
2. The base destructor is not virtual. Deleting a SeniorAccount through `Account*` has undefined behaviour, so the derived destructor may not run and its `history` allocation may leak.
3. `Account copy = SeniorAccount();` performs object slicing: only the Account base subobject is copied into `copy`; the SeniorAccount-specific part is discarded.
4. The raw-pointer array requires manual deletion and is vulnerable to missed deletes and exception paths.

### Task 4.2 and 4.3 – evidence and repair
`Task04/main.cpp` repairs the design by making `rate()` virtual, declaring a virtual base destructor, overriding with `override`, and owning objects using `std::unique_ptr<Account>`. The derived destructor releases `history_`. Clearing the vector demonstrates derived then base destruction.

Expected output:
```text
3.5
4
Account destroyed
SeniorAccount destroyed
Account destroyed
```
The destructor ordering shown here is the derived destructor body first, then the base destructor. For the base-only object, only the base destructor runs. Run the original unsafe code under AddressSanitizer to investigate; exact sanitizer output is platform/runtime dependent, and undefined behaviour is not guaranteed to produce a diagnostic on every run.

### Task 4.4 – Prevention rules
- If a base class is intended to be deleted through a base pointer or used polymorphically, give it a virtual destructor.
- Mark intended overrides with `override` so the compiler diagnoses signature mismatches.
- Prefer RAII ownership such as `std::unique_ptr` over owning raw pointers.

### Predict the output
For the supplied code:
```text
3.5 4 3.5
```
`p->rate()` calls the non-virtual `Account::rate()` because binding is static from the pointer's declared type. `p->vrate()` calls the virtual override at run time and returns 4.0. `copy.vrate()` returns 3.5 because `Account copy = s` slices away the SeniorAccount part; the copied object is an Account.

### Alternative: vector of unique_ptr
Store accounts as `std::vector<std::unique_ptr<Account>>`; iterate through the vector and call virtual functions. Processing n accounts remains O(n), and each dispatch is O(1). The pointer/ownership metadata has small constant space overhead; each dynamic object still occupies its own storage. Smart pointers prevent forgotten deletion and correctly invoke derived destructors when the base destructor is virtual. They do not by themselves make a non-virtual function polymorphic, and they do not prevent slicing if objects are separately copied by value.

## TASK 5 – POINTERS TO OBJECTS AND THE this POINTER (7 marks)

### Task 5.1 – Four ways to reach a Cart
```cpp
Cart c;                 // (a) direct object access: c.add(...)
Cart* p = &c;           // (b) pointer: p->add(...)
Cart& ref = c;          // (c) reference: ref.add(...)
Cart* heap = new Cart;  // (d) dynamic object: heap->add(...)
delete heap;
```
Direct access is simplest for a local object. A pointer can be reseated and can be null, so it must be checked. A reference is an alias that must be initialized and cannot be reseated. A dynamically allocated object lives until deleted; RAII smart pointers are generally safer than raw `new`/`delete`.

`this` is the implicit pointer to the object on which a non-static member function is called. `*this` is the object itself as an lvalue. Returning `*this` by `Cart&` supports chaining. In `this->discount_ = discount`, the left side explicitly names the data member while the right side is the parameter.

### Task 5.2 – Array of pointers and maximum
The program in `Task05/main.cpp` stores `Cart*` values in an array and walks it to find the greatest total. The loop is O(n) for n carts, with O(1) extra search space.

### Task 5.3 – Safety and lifetime
A direct object and a reference do not require manual allocation. A pointer is useful for optional/reseatable access but can dangle. A `new` object has dynamic lifetime and must be released exactly once; prefer `std::unique_ptr<Cart>`. `this` is not an independent object; it identifies the current receiver.

### Complete the code
`add()` stores an item/price if there is capacity and returns `*this`. `setDiscount()` uses `this->discount_` to distinguish the member from the parameter and returns `*this`. The program prints:
```text
324.00
Highest cart total: 324
```
Returning `Cart` by value instead of `Cart&` would return a copy. Chained calls would operate on temporary copies, so changes would not necessarily affect the original `Cart`; it also adds copying overhead. Returning `Cart&` preserves the original object and avoids that copy.

### Alternative: vector of Cart objects
A `std::vector<Cart>` stores objects contiguously and manages their lifetimes. Finding the highest total remains O(n). The vector stores each full Cart object rather than one pointer per element; pointer arrays store only pointer values but require separately managed objects. When a vector grows and reallocates, pointers/references/iterators to its elements may be invalidated. Reserve capacity or avoid retaining such pointers across growth.

## TASK 6 – DESIGN CHALLENGE: VEHICLE RENTAL FLEET (13 marks)

### Stage 1 – Interpret the requirement
Entities and data:
- Vehicle: registration number, daily rate, rental state.
- Bicycle: a non-motor vehicle with its rental charge rule.
- MotorVehicle: fuel/charge level and service due date.
- TwoWheeler: motor vehicle with fuel and service information.
- Car: motor vehicle with seating capacity.
- ElectricCar: car with battery capacity and electricity price per kWh.
- ElectricBus: an example new electric vehicle type with seating capacity and charge level.

Common behaviour: rent, return, list, calculate charge. Differing behaviour: each concrete type computes its rental charge and displays its specialized data.

Testable requirements:
1. Every vehicle has a unique registration string and non-negative daily rate.
2. The fleet can list all stored vehicles.
3. A vehicle can be rented only if it is not already rented.
4. A vehicle can be returned only if currently rented.
5. Return calculation includes daily rental charge and a late fee when returned late.
6. Each concrete class implements `charge()` and `list()`.
7. A new derived vehicle type can be registered without editing existing concrete classes or the fleet's polymorphic loop.

### Stage 2 – Proposed hierarchy
```text
                 <<abstract>>
                    Vehicle
       --------------------------------
       - registration : string
       - dailyRate : double
       - rented : bool
       + charge(days) : double = 0
       + list() : void = 0
       + virtual ~Vehicle()
                    |
          -----------------------
          |                     |
       Bicycle              MotorVehicle
                         - fuel/charge level
                         - service due date
                                |
                       -----------------
                       |               |
                  TwoWheeler          Car
                                  - seats
                                      |
                                  ElectricCar
                              - battery capacity
                              - price per kWh

New extension: ElectricBus derives from MotorVehicle.
```
The hierarchy uses public inheritance for is-a relationships. `Vehicle` is abstract through pure virtual functions. `MotorVehicle` is an intermediate reusable class. `Car` adds seating capacity and `ElectricCar` extends it with battery-related data.

### Stage 3 – Critical decisions
- Put common registration, daily rate and rented state in `Vehicle`.
- Make `charge(int days)` and `list()` pure virtual; give `Vehicle` a virtual destructor.
- Put fuel/charge level and service due date in `MotorVehicle`.
- Keep car seating capacity in `Car`; battery capacity and per-kWh price in `ElectricCar`.
- Store the fleet as `std::vector<std::unique_ptr<Vehicle>>`, an owning collection of base-class pointers. This avoids slicing and automates destruction.
- Use `override` in every concrete derived class.

### Stage 4 – Class declarations and pseudocode
The complete C++ implementation is in `Task06/main.cpp`. Main declarations include:
```cpp
class Vehicle {
    std::string registration;
    double dailyRate;
    bool rented;
public:
    virtual double charge(int days) const = 0;
    virtual void list() const = 0;
    virtual ~Vehicle() = default;
};
```
SIMATS-style algorithmic pseudocode:

**Algorithm RENT(regNo)**
1. BEGIN
2. FOR each vehicle in fleet
   - IF vehicle.registration equals regNo AND vehicle.rented is false
     - SET vehicle.rented to true
     - RETURN true
3. RETURN false
4. END

**Algorithm RETURN_VEHICLE(regNo, lateDays)**
1. BEGIN
2. FOR each vehicle in fleet
   - IF registration matches AND rented is true
     - SET rented to false
     - SET days to MAX(lateDays, 0)
     - SET amount to vehicle.charge(days)
     - IF lateDays is greater than 0, ADD lateDays × lateFeePerDay
     - RETURN amount
3. RETURN -1 to indicate not found or not rented
4. END

### Stage 5 – Implementation and demonstration
`Task06/main.cpp` creates a Bicycle, TwoWheeler, Car, ElectricCar and ElectricBus, lists them through base-class ownership, rents a car, and returns it with a two-day late fee. ElectricBus is added by creating a new derived class and registering it with `fleet.add<ElectricBus>(...)`; existing fleet iteration and rent/return functions do not change.

Sample output:
```text
Bicycle BI-101 rate=100
TwoWheeler TW-202 fuel=5 service=2027-01-10 rate=250
Car CA-303 seats=5 fuel=20 rate=1200
  Electric battery=60kWh price/kWh=8
ElectricBus EB-505 seats=30 charge-level=90 rate=3000
Rent CA-303: true
Return CA-303 after 2 late days: Rs 2600
```
The example's late fee is Rs 100 per late day, and the return charge uses two charge days plus the late fee. The ElectricCar class stores the battery and per-kWh rate; its current example's `charge()` keeps the daily rental amount and adds zero-valued energy usage because the task does not specify a kWh-consumption quantity. A real billing policy must define energy consumed before calculating an energy charge.

### Alternative: one Vehicle class with a type code
A single class would contain fields for every type and a `switch` in `charge()`. Both switch dispatch and virtual dispatch are O(1) per call; processing the whole fleet is O(n). The single class reserves space for fields unused by many objects, while a polymorphic implementation commonly adds a vtable pointer per object (implementation detail) and stores specialized data only in the appropriate derived class. Adding a new vehicle requires changing the switch and often shared field/constructor/list logic. With the hierarchy, add a new derived class and register it; the existing polymorphic processing loop remains unchanged.

## Final submission checklist
- [ ] Compile and run Tasks 1–6 on your own g++ installation.
- [ ] Record `g++ --version` and exact compile commands.
- [ ] Capture genuine output screenshots in each task folder.
- [ ] Run the original Task 4 defect example with AddressSanitizer and save actual diagnostics.
- [ ] Compare `sizeof(TeachingAssistant)` with and without virtual inheritance on your compiler.
- [ ] Add the actual GitHub repository URL to README and cover page.
- [ ] Commit each task progressively and prepare to explain the hierarchy and constructor traces in the viva.

# Assignment 5 – CO5: Reliable Software with Runtime Polymorphism and Exceptions
**Name:** G Yuvaraj  
**Register Number:** 192421065  
**GitHub Repository:** [Add your actual repository URL]

## Case study
A hospital bills patients using outpatient, inpatient, day-care and emergency bill types. Payers may be self-pay, insurance or a government scheme. The design must calculate bills consistently, reject invalid claims clearly and preserve resources when errors occur.

# TASK A1 – Design the polymorphic bill hierarchy

### A1.1 Common interface
Every bill has a patient identifier and must provide:
- `total() const`: calculate the complete bill amount.
- `type() const`: return a stable bill category label.
- `print() const`: print the category, patient and calculated amount.

Common data: patient identifier. Derived classes own type-specific charge data. Monetary values are represented as `double` in this teaching example; production billing should use integer minor currency units or a decimal-money type to avoid binary floating-point rounding.

### A1.2 Abstract hierarchy
`Bill` is abstract because `total()` is pure virtual. The destructor is virtual so deleting a derived bill through `Bill*` destroys the complete object.

```text
                    <<abstract>> Bill
       patient: string
       + total() const: double {pure virtual}
       + type() const: string {virtual}
       + print() const: void
       + ~Bill() {virtual}
              /       |        |          \
             /        |        |           \
 OutPatientBill  InPatientBill  DayCareBill  EmergencyBill
 total() override total() override total() override total() override
 type() override  type() override  type() override  type() override
```

All four derived `total()` and `type()` functions override the base interface. `print()` can be a non-virtual base function because it calls the virtual `type()` and `total()` operations.

### A1.3 Process a mixed list
```text
ALGORITHM DailyTotal(bills, n)
    total ← 0
    FOR i ← 0 TO n - 1
        total ← total + bills[i].total()
    END FOR
    RETURN total
END ALGORITHM
```
Here `bills[i]` denotes a base-class pointer. Each call dispatches to the concrete bill's override.

### Alternative: tagged structure and switch
Use one record containing `kind` and a union/variant-like set of charge fields. `total(record)` switches on `kind` and calculates the matching formula. Both approaches take O(n) time to total n bills. The tagged record may avoid a vptr but may reserve space for the largest field set; a polymorphic object commonly stores a vptr plus its own fields and may require separate allocations. Adding a new type requires editing the switch and central record in the tagged design. With the virtual design, add a derived class without modifying the loop or existing bill classes. The switch can be faster in some workloads due to locality, so benchmark rather than assume.

# TASK A2 – Runtime binding

### A2.1 Early vs late binding
A non-virtual call is resolved from the static type at compile time (early binding). A virtual call such as `b->total()` is dispatched at run time using the dynamic type of the object (late binding). When `b` points to `OPBill`, `OPBill::total()` executes.

### A2.2 Conceptual vtables
```text
Bill vtable (abstract)
  destructor -> Bill::~Bill
  total      -> pure virtual slot (no Bill implementation)

OutPatientBill vtable
  destructor -> OutPatientBill::~OutPatientBill
  total      -> OutPatientBill::total
  type       -> OutPatientBill::type

InPatientBill vtable
  destructor -> InPatientBill::~InPatientBill
  total      -> InPatientBill::total
  type       -> InPatientBill::type

b->total(): object vptr -> OutPatientBill vtable -> OutPatientBill::total()
```
This is a conceptual model; exact vtable layout and symbol details are compiler/ABI implementation-specific.

### A2.3 Virtual destructor
Deleting a derived object through a base pointer without a virtual base destructor is undefined behavior. A virtual destructor ensures derived destruction is selected and resources owned by the derived object are released.

### Predict output
Given `Bill* b = new OPBill; b->show(); delete b;`, expected output:
```text
500 ~OPBill ~Bill
```
`show()` is non-virtual, but its body calls virtual `total()`. The virtual call is dispatched using the dynamic object type, so `OPBill::total()` returns 500. Deletion invokes the derived destructor first, then the base destructor.

If `~Bill` is made non-virtual, deleting `OPBill` through `Bill*` has undefined behavior. Do not treat any observed output as guaranteed. AddressSanitizer may or may not diagnose it; compiler warnings and sanitizer output are evidence, not a replacement for the language rule.

### Alternative: dynamic_cast dispatch
A non-virtual design can inspect each concrete type using `dynamic_cast` and call a corresponding method. For k bill types, a chain may require up to O(k) casts per bill, versus one virtual dispatch (O(1)) per bill. A polymorphic object needs runtime type information for casts; virtual objects also generally contain a vptr. The cast chain becomes harder to extend and centralizes knowledge of every type.

# TASK A3 – Abstract payer classes

### A3.1 Interface
```cpp
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};
```
- `SelfPay`: covers the full bill amount.
- `Insurance`: stores a cover limit and co-payment percentage. For a bill amount B, cover is `min(B, limit) * (1 - copay_percent/100)`. Patient responsibility is the remaining amount.
- `GovernmentScheme`: stores package rates keyed by bill category and returns the configured package amount, bounded by the bill amount if the scheme must not overpay.

The exact policy rule should be confirmed with the hospital; this example states the assumption explicitly.

### A3.2 Extension
Add `CorporatePayer : public Payer` and implement `amountCovered`. Existing `Bill`, existing payer classes and processing code remain unchanged. This follows the open/closed principle.

### Alternative: function pointer / std::function
Store a callable policy such as `std::function<double(const Bill&)>` with any captured configuration. Each invocation is O(1) in the usual model, as is a virtual call. A callable can capture a cover limit and co-pay value; the abstract class stores those values as derived-class data. `std::function` may require type-erasure storage and can allocate for large captures, while a function pointer alone cannot carry arbitrary per-payer state without an additional context pointer.

# TASK A4 – Exception strategy

### A4.1 Error cases and hierarchy
```text
std::exception
  std::runtime_error
    BillingError
      InvalidChargeError
      MissingPatientError
      UnknownPackageError
      ClaimError
        ClaimExceedsCover
  std::bad_alloc (standard allocation failure; separate standard branch)
  std::invalid_argument (e.g. malformed numeric input)
```
- Negative consultation, tests, room, procedure or emergency charges: `InvalidChargeError`.
- Claim above policy cover: `ClaimExceedsCover` or `ClaimError`.
- Unknown government package: `UnknownPackageError`.
- Missing patient record: `MissingPatientError`.
- Allocation failure: `std::bad_alloc`.
- Text parsed as a number: `std::invalid_argument` from `std::stod`.

### A4.2 Throw specifications
Validation, parsing, bill construction and payer calculations may throw documented exceptions. Logging may throw if the logging implementation allocates or performs I/O; production logging should define its failure policy. Destructors and cleanup functions should not throw. Mark genuinely non-throwing operations `noexcept`, such as simple accessors that cannot allocate. A destructor that throws during stack unwinding can cause `std::terminate`.

### Alternative: return status
Return a status enum and write the result through an output parameter, or return a result structure containing status, amount and message. The normal path avoids exception propagation and can be efficient; every caller must inspect status. The error path is explicit but requires status/message storage and repetitive propagation checks. Runtime is O(1) per validation in either design; exception construction/stack unwinding makes exceptional paths comparatively expensive.

# TASK A5 – Recovery and rethrowing

### A5.1 Catch locations
Catch a specific exception at the layer able to recover. A service boundary may log context and use bare `throw;` to preserve the same exception. The application boundary catches expected domain errors and reports a clear message to the user. A final `catch (...)` may prevent an exception escaping a thread or process boundary, but should not silently hide the failure.

### A5.2 Stack unwinding and RAII
When an exception leaves a scope, automatic objects in that scope are destroyed in reverse construction order. `std::vector`, `std::string`, `std::unique_ptr`, file streams and lock guards release their resources through destructors. RAII therefore protects resources even when control leaves a function early.

### Predict output
For the `Guard` example, expected output:
```text
~f ~main caught 42
```
`f()` throws, so local `g` is destroyed first (`~f`). The exception propagates to `main`; `m` is destroyed while unwinding (`~main`). The statement `not printed` is skipped because normal control flow is interrupted. The `catch(int)` handler prints `caught 42`.

### Alternative: manual delete in catch blocks
Manual cleanup has similar asymptotic time for releasing resources, but every error path must remember to release every resource. With r independent resources and e exit/error paths, the number of cleanup obligations can grow roughly with r×e. RAII centralizes cleanup in destructors and reduces leak risk.

# Part B – implementation and experimental investigation

See Task01–Task04 for executable examples. The source assignment requires actual local measurements, screenshots and sanitizer reports. The package provides reproducible harnesses and commands, but does not invent measurements from the student's computer.

## Benchmark interpretation guidance
- Both virtual and switch totals are O(n); per-element dispatch overhead is constant.
- `std::variant` plus `std::visit` is another closed-set alternative; element storage is at least the size of its largest alternative plus a discriminator, subject to alignment.
- Exceptions are appropriate for genuinely exceptional failures; return codes/`std::optional` are often preferable for frequent expected rejection.
- Use optimized (`-O2`) builds, warm-up runs, multiple repetitions and report the median. Avoid printing inside timed loops.
- AddressSanitizer detects many memory errors but does not prove correctness or guarantee diagnosis of undefined behavior.

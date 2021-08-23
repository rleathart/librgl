Requirements
============

Tested with Clang 12.0.0

Building the library
====================

```bash
cmake -B build
cmake --build buid
```

Installing the library
====================

```bash
cmake --install build
```

Running tests
=============

```bash
cmake -B build -DRGL_BUILD_TESTS=ON
cmake --build build --target test
```

Directory Structure
===================

Directory | Contains
--------- | --------
include   | Public headers.
src       | All library source code and private headers.
test      | Source code for unit/functional tests.

Code style
==========

Variables and functions should be snake\_case. Global variables should be
prefixed with `g_`, thread local variables should be prefixed with `t_`.

Type names are CamelCase.

Macro names are UPPER\_SNAKE\_CASE.

Data structures
===============

For datastructures that have constructors with the signature
```c
void datastructure_new(DataStructure* self, ...)
```
their corresponding `free` functions shall be responsible for freeing only the
data contained in the datastructure, and not the variable `self`. This is
because the constructor signature gives the caller the option to allocate on the
stack or the heap. As such, the caller shall be responsible for freeing `self`.

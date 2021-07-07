Requirements
============

Tested with Clang 12.0.0

On Windows, requires pthreads-win32 to be installed as a CMake package. See
[here](https://github.com/rleathart/pthreads-win32-CMake) for details.

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

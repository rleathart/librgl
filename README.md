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

Directory Structure
===================

Directory | Contains
--------- | --------
include   | Public headers.
src       | All library source code and private headers.

Code style
==========

Variables and functions should be snake\_case. Global variables should be
prefixed with `g_`, thread local variables should be prefixed with `t_`.

Type names are CamelCase.

Macro names are UPPER\_SNAKE\_CASE.

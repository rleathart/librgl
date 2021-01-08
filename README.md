Building the library
====================

```bash
cmake -S . -B lib && cmake --build lib
```

Directory Structure
===================

Directory | Contains
--------- | --------
include   | Public headers.
lib       | Generated shared object binaries.
src       | All library source code and private headers.

Public vs Private headers
=========================

Public
------
Define types etc. that need to exist in the global (user) namespace.

Private
-------
Define internal types, functions etc. that need only be visible to the library
namespace.

Example module
==============

`include/rgl/example.h:`
```c
#pragma once

typedef struct
{
  int example;
} example_t;
```

`src/example/example.h`
```c
#pragma once

#include <rgl/rgl.h>

void example_ExampleMethod(example_t* self);
```

`src/example/example.c`
```c
#include "example.h"

void example_ExampleMethod(example_t* self)
{
  self->example = 42;
}
```

`include/rgl/rgl.h`
```c
#pragma once

#include <rgl/example.h>

typedef struct
{
  struct
  {
    void (*ExampleMethod)(example_t*);
  } example;
} _rglNamespace;

extern const _rglNamespace rgl;
```

`src/rgl.c`
```c
#include <rgl/rgl.h>

#include "example/example.h"

const _rglNamespace rgl = {
  .example = {
    .ExampleMethod = example_ExampleMethod
  }
};
```

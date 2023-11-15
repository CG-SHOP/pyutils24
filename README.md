# CG:SHOP - Official pyutils24 for the 2024 Challenge on Packing Polygons

Utilities for verifying solutions of the CG:SHOP 2024 Competition. Feel free to
use the code, but it is optimized for _exact_ verification not for sampling or
other purposes.

## Installation

You can install this package via PyPI:

```bash
pip install -U -v cgshop2024-pyutils
```

During the installation, CGAL and other dependencies will be downloaded and
compiled. This can take a while but should happen mostly automatic. You need to
have a C++ compiler installed.

> Please check for updates of the utils frequently as we are still working on
> them.

## Usage

### Reading instances

```python
from cgshop_pyutils24 import InstanceDatabase

db = InstanceDatabase("../cgshop2024_benchmark.zip")

instance = db["atris1240"]
assert isinstance(instance, dict)

for instance in db:
    print("Found", instance["instance_name"])
```

The instances are dicts of the following format:

```json
{
  "type": "cgshop2024_instance",
  "instance_name": "test",
  "num_items": 9,
  "container": {
    "x": [26, 2, 0, 12, 91, 158, 180, 194, 225, 232, 225, 210, 200],
    "y": [250, 202, 149, 107, 0, 30, 42, 56, 131, 151, 177, 225, 243]
  },
  "items": [
    {
      "value": 601,
      "quantity": 1,
      "x": [3, 40, 113, 134, 85, 143, 152, 157, 157, 92, 18, 12, 2, 0],
      "y": [31, 59, 59, 27, 6, 0, 51, 95, 139, 141, 77, 145, 152, 65]
    },
    {
      "value": 90,
      "quantity": 1,
      "x": [1, 6, 25, 54, 49, 39, 25, 31, 14, 0, 13],
      "y": [11, 10, 8, 0, 23, 28, 37, 56, 34, 39, 26]
    },
    { "value": 1, "quantity": 1, "x": [12, 0, 28], "y": [16, 26, 0] },
    {
      "value": 9,
      "quantity": 1,
      "x": [8, 5, 0, 12, 13],
      "y": [14, 19, 12, 0, 5]
    },
    { "value": 133, "quantity": 1, "x": [101, 61, 0], "y": [0, 104, 95] },
    { "value": 51, "quantity": 2, "x": [68, 48, 0, 71], "y": [45, 38, 17, 0] },
    { "value": 25, "quantity": 1, "x": [21, 28, 0, 1], "y": [0, 47, 17, 0] },
    {
      "value": 37,
      "quantity": 1,
      "x": [11, 1, 17, 26, 19, 0, 9],
      "y": [16, 0, 16, 12, 34, 34, 26]
    },
    { "value": 32, "quantity": 1, "x": [58, 73, 79, 0], "y": [5, 10, 39, 0] }
  ]
}
```

### Verifying solutions

```python
from cgshop2024_pyutils import (
    verify,
    InstanceDatabase,
    InvalidSolution,
    BadSolutionFile,
)

db = InstanceDatabase("../cgshop2024_benchmark.zip")
solution = parse_solution(SOLUTION_DATA)
instance = db[solution["instance_name"]]

try:
    value = verify(instance, solution)
except InvalidSolution as inv_sol:
    print("Solution is invalid:", inv_sol)
except BadSolutionFile as bad_sol:
    print("Solution file is invalid:", bad_sol)
else:
    print("Solution is valid and has value", value)
```

### Verifying a batch of solutions

```python
from cgshop2024_pyutils import (
    verify_batch,
    InstanceDatabase,
    InvalidSolution,
    BadSolutionFile,
    ZipSolutionIterator,
    ZipReaderError,
)

db = InstanceDatabase("../cgshop2024_benchmark.zip")
sol_it = ZipSolutionIterator()
try:
    for solution in sol_it(ZIP_FILE):
        instance = db[solution["instance_name"]]
        try:
            value = verify(instance, solution)
        except InvalidSolution as inv_sol:
            print("Solution is invalid:", inv_sol)
            break
        except BadSolutionFile as bad_sol:
            print("Solution file is invalid:", bad_sol)
            break
        print("Solution is valid and has value", value)
except ZipReaderError as err:
    print("Error while reading zip file:", err)
```

## Trouble Shooting

The native parts are known to cause trouble on some systems. Check out
[this page](https://github.com/d-krupke/skbuild-conan#common-problems) for
solutions of various problems.

## License

The code in this repository can be used under the terms of the MIT license.
However, the package binds against CGAL which is licensed under the
[GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html). This should still allow
you to modify and use the code for the competition.

## Changelog

- **1.2.0:** Fixing catching of exception of bad files at the wrong place. Improved error messages.
- **1.1.1:** A long on Windows is only 32bit, so changed to using the explicit `int64_t` type.
- **1.1.0:** The previous workaround could lead to excessive memory usage.
  Changed quadtree implementation to keep large elements on higher levels even
  if the node is full. Gives a warning in this case.
- **1.0.1:** Only giving a warning if a node cannot be split instead of throwing
  an exception. [Issue #1](https://github.com/CG-SHOP/pyutils24/issues/1)
- **1.0.0:** Initial release

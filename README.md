Using Allman Style

## Namespace and folder layout

The root namespace is `cpu`. One folder equals one namespace, and `src/` mirrors `include/cpu/`.

| Folder (`include/cpu/...`) | Namespace | Contents |
|---|---|---|
| `Types.hpp`, `Config.hpp`, `CPU.hpp` | `cpu` | Fundamental types, configuration, top-level `CPU` |
| `registers/` | `cpu::registers` | `Register`, `RegisterFile`, `RegisterPrinter`, `RegisterId` |
| `isa/` | `cpu::isa` | `Opcode`, `Instructions`, `Decoder` |
| `memory/` | `cpu::memory` | `Memory` |
| `execution/` | `cpu::execution` | `ExecutionUnit` |

Rules:
- Include paths show the namespace, e.g. `#include "cpu/registers/RegisterFile.hpp"`.
- Maximum depth is `cpu::<area>::detail`; `detail` lives in the header that needs it.
- No `using namespace` in headers.
- Dependencies flow one way: `cpu` <- `registers`, `memory` <- `isa` <- `execution` <- `CPU`.

Using Allman Style

## Namespace and folder layout

The root namespace is `cpu`. One folder equals one namespace, and `src/` mirrors `include/cpu/`.

| Folder (`include/cpu/...`) | Namespace | Contents |
|---|---|---|
| `Types.hpp` | `cpu::types` | Fundamental type aliases |
| `errors/` | `cpu::errors` | One error enum per header (`RegisterError`, `StackError`, `MemoryError`, `ALUError`) |
| `storage/` | `cpu::storage` | `Register`, `RegisterFile`, `Stack`, `Memory` |
| `execution/` | `cpu::execution` | `ALU`, `ExecutionUnit` |
| `isa/` | `cpu::isa` | `Opcode`, `Instruction`, `InstructionDecoder` |
| `parser/` | `cpu::parser` | `Token`, `Lexer` |
| `io/` | `cpu::io` | `Print` |
| `utils/` | `cpu::utils` | General helpers |
| `CPU.hpp`, `Config.hpp` | `cpu` | Top-level CPU only |

Rules:
- Include paths show the namespace, e.g. `#include "cpu/storage/RegisterFile.hpp"`.
- Maximum depth is `cpu::<area>::detail`; `detail` stays in the header that needs it and is never used for public types.
- No `using namespace` in headers.
- Dependencies flow one way: `types` <- `errors` <- `storage` <- `execution`; `parser` and `io` are independent.

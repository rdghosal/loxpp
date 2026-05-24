---
name: cpp-style-review
description: Reviews uncommitted C++ changes (git diff HEAD) for formatting violations, static analysis warnings, and idiomatic issues. Includes Rust-to-C++ comparisons for developers learning C++. Use when the user wants C++ style feedback, code review, or idiomatic guidance on their current changes.
---

# C++ Style Review

Three-pass review of changed C++ files: **clang-format → clang-tidy → idiom review**. Reports only; never applies fixes.

Conventions template: [references/conventions-reference.md](references/conventions-reference.md)

## Workflow

### 1. Find changed files

```bash
git diff HEAD --name-only -- '*.cpp' '*.hpp' '*.h' '*.cc'
```

Stop with "No changed C++ files to review." if empty.

### 2. Load project conventions

```bash
ls .agents/skills/cpp-style-review/references/conventions-cpp.md 2>/dev/null
```

If missing:
- Create `.agents/references/` if needed
- Write the content of the conventions template to `.agents/skills/cpp-style-review/references/conventions-cpp.md`
- Inform the user: "`.agents/skills/cpp-style-review/references/conventions-cpp.md` created — edit it as your project conventions evolve."

Load `.agents/skills/cpp-style-review/references/conventions-cpp.md` for use in passes 3–5.

### 3. clang-format pass

```bash
clang-format --dry-run --Werror <file>
```

Run on every changed file. Capture violations.

### 4. clang-tidy pass

```bash
find build -name compile_commands.json 2>/dev/null | head -1
```

If found: `clang-tidy -p <build-dir> <file>` on each changed `.cpp`/`.cc`.

If not found: skip and note — "Run `cmake --preset debug` to enable clang-tidy."

### 5. Idiom review

Read each changed file. Check against `.agents/skills/cpp-style-review/references/conventions-cpp.md` for issues tools miss: RAII violations, raw owning pointers, missing `const`, C-style casts, manual loops replaceable by algorithms, missing `[[nodiscard]]`, etc.

Include Rust comparisons where they clarify the C++ idiom.

### 6. Report

```
## <filepath>

### Errors
[tool errors, undefined behavior, null-deref]

### Warnings
[clang-tidy warnings, clang-format drift]

### Style
[violations from .agents/skills/cpp-style-review/references/conventions-cpp.md]

### Learning notes
[idiomatic guidance; Rust comparisons where relevant]
```

Omit empty sections. Files with no findings: `✓ <filepath> — no issues found.`

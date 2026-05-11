# VS Code C/C++ Build & Debug Workflow

Generic VS Code setup for building and debugging C/C++ projects.

---

## Generate VS Code configuration

### Open any `.c` or `.cpp` file first.

### Create configs:

- Shift + Cmd + P → C/C++: Edit Configurations (JSON)  
  → creates `.vscode/c_cpp_properties.json`

- Shift + Cmd + P → C/C++: Add Debug Configuration → clang  
  → creates `.vscode/launch.json` and `.vscode/tasks.json`

## Build

- Shift + Cmd + B → build active file

- Shift + Cmd + P → → Tasks: Run Task → Build ARM

## Debug

- Fn + F5 → start debugging  
  OR
- Run and Debug → Start Debugging

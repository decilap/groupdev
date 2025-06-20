# 🐚 Minishell

> 42 School Project — A simplified Unix shell written in C

---

## 🚀 Introduction

**Minishell** is a UNIX command line interpreter, inspired by Bash and Zsh, fully written in C as part of the 42 school curriculum.  
The goal is to recreate a simplified shell environment handling:

- Command parsing
- Process creation & management
- File descriptors & redirections
- Signals
- Environment variables
- Expansions & quotes

---

## ✅ Features

- 🔧 Built-in commands:
  - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- 🔄 Pipelines (`|`)
- 🔗 Logical operators (`&&`, `||`, `;`)
- 📂 File redirections (`>`, `>>`, `<`, `<<`)
- 🌎 Environment variable expansion (`$VAR`, `$?`)
- 📝 Quote handling (`'`, `"`)
- 🐚 Subshells (`(command)`)
- ✨ Wildcards expansion (`*`)
- 📉 Signal handling (`SIGINT`, `SIGQUIT`)
- 🧹 Memory management without leaks

---

## 🔧 Installation

```bash
git clone https://github.com/your_username/minishell.git
cd minishell
make

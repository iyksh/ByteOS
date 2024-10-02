**ByteOS** is an abandoned 32-bit x86 architecture operating system project that was designed for educational purposes.

---

## Installation

Make sure you have the following tools installed:

- i686-elf toolchain.
- qemu-system-i386 for virtual machine.
- GCC (GNU Compiler Collection).
- LD (GNU Linker).

For detailed instructions on building and using a cross-compiler, refer to the [OSdev Wiki](https://osdev.wiki/wiki/GCC_Cross-Compiler). You can also check the [i686-elf.sh](./res/i686-elf.sh) to do this for you.

---

1. Clone the repository:
   ```bash
   git clone https://github.com/iyksh/ByteOS && cd ByteOS
   ```

2. Build and run the project:
   ```bash
   ./run.sh         # This will automatically clean, build and run the project with QEMU
   ```

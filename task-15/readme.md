# Task 15: x64 syscall implementation

## How to add a new syscall ?

`syscalls` are dependent of the architecture, so we
chose `x86_64`

### 1: Add func signature in `linux/syscalls.h`

This header contains linux syscalls interfaces for
*ALL* architecture.

So add our function prototype with suffixed by the
`asmlinkage` decorator, a #define for some gcc magic that
tells the compiler that the function should not expect to find any
of its arguments in registers (a common optimization),
but only on the CPU's stack.

### 2: Implement your logic code

Since we are targetting this syscall on the x86_64 platform,
create a new file in the corresponding directory: `arch/x86/kernel/`.

Your file impl should correctly describe your newly added proto,
so if the name is `sys_eudyptula` then the first argument in `SYSCALL_DEFINEX`
would be eudyptula.

Your implementation should use the `SYSCALL_DEFINEX` macro, where X define the
number of arguments in your function.

Note that the variadic arg list should be properly formated, with for each arguments
the type referenced before:

```
               | NAME    |     1    |    2    |_
SYSCALL_DEFINE2(eudyptula, int, high, int, low)
```

Then don't forget to add your file in x86 Makefile: `arch/x86/kernel/Makefile`

Goal definitions are the main part (heart) of the kbuild Makefile.
These lines define the files to be built, any special compilation options,
and any subdirectories to be entered recursively.

> Note: `m` means module, `y` means built-in (stands for yes in the kernel config process)

```
obj-y                          += eudyptula.o
```

### 3: Reference your syscall symbole in syscall table


Edit the arch specific syscall table: `arch/x86/entry/syscalls/syscall_64.tbl`

And append new line by incrementing the last identifer (this is convention, but
can technicaly take any unatributed number).

For instance:

```
# ID  | ARCH  | NAME                  | TABLE SYMBOL
439    64      eudyptula               __x64_sys_eudyptula # Where sys_eudyptula is on you syscall.h
```
### 4: Profit !

Make, run & test with assembly OR syscall() func in <sys/syscall.h>

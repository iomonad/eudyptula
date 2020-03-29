# Sparse: kernel static code analysis

> Sparse is a tool for static code analysis that helps kernel developers to detect coding errors. 

Kernel code that is prone to mistakes is annotated by kernel developers using the 'attribute' specifiers. 
Sparse tool uses these specifiers to pinpoint coding mistakes.

## Usage

Do a kernel make with “make C=1” to run sparse on all the C files that get recompiled, 
or use “make C=2” to run sparse on the files whether they need to be recompiled or not. 
The latter is a fast way to check the whole tree if you have already built it.

```
make C=2 drivers/staging/my_stagging_driver
```

You can use the variable CF to pass more checkflags to sparse.
For example, you can enable endian checks doing:

```
make C=2 CF="-D__CHECK_ENDIAN__" drivers/staging/my_stagging_driver
```

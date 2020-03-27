# Hacking `task_struct`

The goal of thi assignement was to show us how pid are managed / scheduled.

### Changes mades

Added new field in `task_struct` structure located in `include/linux/sched.h`.
This structure describe each PID procs. Once added, I allocated new buffer in PID
instanciation in `copy_process` function in fork source that can be seen like our 
PID constructor. And finally I add fs mapping in proc, using `REG()` macro it `tgid_base_stuff[]` 
struct array (meta object mapping) with associated newly created file ops.

> Note: No memory free is added in this patch

### Testing & Proof

On your branch, build your kernel using `make -j$(nproc)`.

Then launch it with Qemu:

```bash
export LINUX_TRUNK=linux-master
KERNEL_OPT="console=ttyS0 quiet"

# Lauch kernel in Qemu
qemu-system-x86_64 \
  -kernel $LINUX_TRUNK/arch/x86/boot/bzImage \
  -initrd ./initramfs.cpio.gz -nographic \
  -append "$KERNEL_OPT" -enable-kvm
```

That give us:

```
[    0.046299] Spectre V2 : Spectre mitigation: kernel not compiled with retpoline; no mitigation available!


Boot took 0.29 seconds

    ▄▄▄
   ▀█▀██  ▄  < Shrink up you kernel
 ▀▄██████▀
    ▀█████
       ▀▀▀▀▄
/bin/sh: can't access tty; job control turned off
/ # ls /proc/2
arch_status    exe            mem            oom_score_adj  stat
auxv           fd             mountinfo      pagemap        statm
clear_refs     fdinfo         mounts         personality    status
cmdline        id             mountstats     root           syscall
comm           limits         ns             sched          task
cwd            map_files      oom_adj        smaps          timerslack_ns
environ        maps           oom_score      smaps_rollup
/ # cat /proc/2/id
iomonad
/ # cat /proc/2/id
omonad
/ # cat /proc/4/id
iomonad
/ # cat /proc/4/id
omonad
/ # cat /proc/4/id
monad
/ # cat /proc/4/id
onad
iom/ # 
```

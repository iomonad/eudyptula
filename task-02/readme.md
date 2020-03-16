# Building kernel from sources

## Fetching

```bash
git clone --depth=1 git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
```

## Building

```
cd linux
make kvmconfig
make -j8

# To tweak specific features
make nconfig
```

## Running

```
qemu-img create qemu-image.img 1g
qemu-system-x86_64 -kernel bzImage
                     -hda qemu-image.img
                     -append "root=/dev/sda"
                     --enable-kvm
```

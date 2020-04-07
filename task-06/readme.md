## Building

```bash
make && sudo insmod *.ko
```

## Create device node

We need to create a device node associated with our driver.
We should retrieve the major number that our driver
got assigned by the kernel.

```bash
grep eudyptula </proc/devices
219 eudyptula
```

Our major number is `219`, let's create our node with `mknod`:

```bash
mknod /dev/eudyptula c 219 0
```

## Usage

```bash
cat /dev/eudyptula
DEAD BEEF
```

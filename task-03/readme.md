# Customize EXTRAVERSION field

## Go to last cloned trunk

```bash
cd linux
```

## Set custom VERSION

```bash
make nconfig

# Go to `General Config` > Local Version
# and set the configuration field as: `-eudyptula`
```

> Rebuild & Run

## Check changes

```bash
# In qemu logs, check first lines that
# contains full release string
```

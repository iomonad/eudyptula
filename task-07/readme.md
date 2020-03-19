# Installing `linux-next`

The `linux-next` tree is the holding area for patches aimed at the next kernel
merge window.
If you're doing bleeding edge kernel development, you may want to work from
that tree rather than Linus Torvalds' mainline tree.

## Setup

```bash
# Get mainline tree
git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
```

```bash
# Add remote tree
cd linux
git remote add linux-next https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git
```

```bash
# Fetch latests tags
git fetch linux-next
git fetch --tags linux-next
```

## Tracking

```bash
git remote update
git tag -l "next-*" | tail

git checkout -b my_local_branch next-<version>
```

## Building

Once tracked, same building procedure

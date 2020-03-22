# Write & Submit a patch of running kernel module

## Find sources of modules running on your machine

```bash
cat /proc/modules | sort

grep "^thinkpad_acpi" < /proc/modules
thinkpad_acpi 110592 0 - Live 0x0000000000000000
```

Then go to $KERNEL_SRC root folder and
locate your driver.
In our case we choose `thinkpad_acpi`

```bash
find drivers/ -name "thinkpad_acpi.c"
drivers/platform/x86/thinkpad_acpi.c
```

## Creating patch

Once your modification done in another branch
than the main one, use `git format-patch` to prepare
the patch to send.

```
git format-patch master
```

## Testing patch

Checkout clean branch and copy your
patch in the directory.

Then apply your patch with `git am`

```bash
cat *.patch | git am
```

## Checking patch

```bash
$SRCROOT/scripts/checkpatch.pl 001-my-formated-patch.patch
total: 0 errors, 0 warnings, 0 checks, 942 lines checked

001-my-formated-patch.patch has no obvious style problems and is ready for submission.
```

## Sending the patch to the maintainer

First, find the proper name and mailing lists to
send it to by running the tool:

```bash
$SRCROOT/scripts/get_maintainer.pl 001-my-formated-patch.patch
Foo bar Quux <foobarquu@linuxfoundation.org> (supporter:STAGING SUBSYSTEM,commit_signer:6/8=75%)
[...]
devel@driverdev.osuosl.org (open list:STAGING SUBSYSTEM)
linux-kernel@vger.kernel.org (open list)
```

Then upload your patch online in order to get a web
link to share to other developers.

Once done, send your patch by e-mail with the following
link and patch description.

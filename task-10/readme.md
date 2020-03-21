# Submiting patch to community

## Creating our patch

In a first time, create a new branch
from main branch.

Once your changes commited, create
your patch file using:

```bash
git diff master > my-lovely-patch.patch
```

## Checking the patch

```bash
$SRCROOT/scripts/checkpatch.pl my-lovely-patch.pl
total: 0 errors, 0 warnings, 0 checks, 942 lines checked

my-lovely-patch.patch has no obvious style problems and is ready for submission.
```

## Sending the patch to the maintainer

First, find the proper name and mailing lists to
send it to by running the tool:

```bash
$SRCROOT/scripts/get_maintainer.pl my-lovely-patch.pl
Foo bar Quux <foobarquu@linuxfoundation.org> (supporter:STAGING SUBSYSTEM,commit_signer:6/8=75%)
[...]
devel@driverdev.osuosl.org (open list:STAGING SUBSYSTEM)
linux-kernel@vger.kernel.org (open list)
```

Then upload your patch online in order to get a web
link to share to other developers.

Once done, send your patch by e-mail with the following
link and patch description.

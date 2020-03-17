# How to check patches

## Generate your patches

Write your trunk modification to a diff file
using `git diff`

git diff $BRANCH > modifications.patch

## Check your patch

Linux repo ship a script named:

`$SRCROOT/scripts/checkpatch.pl`

Which let you to check & optionaly fix your patch
before submiting to the community.

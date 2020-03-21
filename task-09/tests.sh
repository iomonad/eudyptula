#!/bin/bash

VFS="/sys/kernel/eudyptula"

ls -l $VFS
cat $VFS/id
cat $VFS/jiffies
cat $VFS/foo
echo "FOO" > $VFS/foo
cat $VFS/foo


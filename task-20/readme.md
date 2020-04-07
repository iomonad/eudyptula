# Implementing new IOCTL for FAT Devices

Input-output control (ioctl, in short) are a kind of "syscalls" implemented
for devices drivers.

## Defining IOCTL

The Linux header file /usr/include/asm/ioctl.h defines macros that must be used to create the ioctl command number. These macros take various combinations of three arguments:

- `type`, an 8-bit integer selected to be specific to the device driver. type should be chosen so as not to conflict with other drivers that might be ''listening'' to the same file descriptor.
- `number`, an 8-bit integer command number. Within a driver, distinct numbers should be chosen for each different kind of ioctl command that the driver services
- `data_type`, the name of a type used to compute how many bytes are exchanged between the client and the driver. This argument is, for example, the name of a structure.

The macros used to generate command numbers are:

- `_IO(int type, int number)`, used for a simple ioctl that sends nothing but the type and number, and receives back nothing but an (integer) retval
- `_IOR(int type, int number, data_type)`, used for an ioctl that reads data from the device driver. The driver will be allowed to return sizeof(data_type) bytes to the user
- `_IOW(int type, int number, data_type)`, similar to _IOR, but used to write data to the driver
- `_IORW(int type, int number, data_type)`, a combination of _IOR and _IOW. That is, data is both written to the driver and then read back from the driver by the client

In our case, we will add a new IOCTL command for FAT Systems, with 32bit compat:

```c
#define FAT_IOCTL_SET_LABEL             _IOW('r', 0x14, const __user char*)
#define FAT_IOCTL_SET_LABEL32           _IOW('r', 0x14, const __user char*)
```

## Implementing new IOCTL

### Boilerplate

FAT ioctl contralize calls in two functions:

```c
static long fat_dir_ioctl();
```

```c
// FOR 32bit Compat
static long fat_compat_dir_ioctl();
```

Add your newly created call in cmd match:

```c
       // For 64bit
       case FAT_IOCTL_SET_LABEL:
               if ((dir = inode->i_sb->s_root->d_inode))
                       return fat_ioctl_set_label(dir, arg);
               break;
```

Then implement your logic of your *return* function.

### `fat_ioctl_set_label` logic


- [1] https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
- [2] http://www.circlemud.org/jelson/software/fusd/docs/node31.html

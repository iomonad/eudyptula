# Kernel Threads & Wait Queues

## Create the misc device

Basic device setup process

```c
static struct file_operations mfops = {
	.owner = THIS_MODULE,
	.write = driver_write,
};

static struct miscdevice miscdev = { .minor = MISC_DYNAMIC_MINOR,
				     .name = MISCDEV_NAME,
				     .fops = &mfops,
				     .mode = 0222 }; /* Implement perms */
```

And in your `__init` handler:

```c
if ((status = misc_register(&miscdev)))
		return status;
```

## Implement Wait queues

While writing modules there might be situations where one
might have to wait for input some condition to occur before proceeding further.
Tasks that need such behavior can make use of the sleep
functionality available in the kernel.
In Linux sleeping is handled by a data structure called `wait queue`,
which is nothing but *a list of processes* waiting for an input or event.

### Initialize

```c
DECLARE_WAIT_QUEUE_HEAD(wee_wait);
```

### Use waiters

`wait_event("queue","condition")`: The task will keep waiting on the queue as long as the condition does not become true.If put to sleep using this call, the task can not be interrupted.

`wait_event_interruptible("queue","condition")`: similar to wait_event, but it can be interrupted by other signals too. It is always preferable to use this interruptible way of sleeping so that the task can be stopped in case the condition never becomes true. 

## Using Kernel Threads

### Initialize your thread

```c
static struct task_struct *mythread = NULL;
```

And a callback:

```c
int thread_runner(void *data)
{
	printk(KERN_INFO "In thread1");
}
```

### Create your thread with runnable callback

The `kthread_run` is a convenient wrapper for `kthread_create()` followed by
`wake_up_process()`. Returns the kthread or ERR_PTR(-ENOMEM).

```c
mythread = kthread_run(thread_runner, NULL, MAINTHREAD_N);
```

### Killing thread

```c
kthread_stop(mythread);
```

## Testing all the things

Load and check permission:

```bash
$ sudo insmod waitqueues.ko
$ ls -l /dev/eudyptula
c-w--w--w- 1 root root 10, 56 Mar 29 16:59 /dev/eudyptula
```

Check kernel thread (in `ps` they are in brackets):

```bash
$ ps -ef | grep "\[eudyptula\]"
root        8960       2  0 16:59 ?        00:00:00 [eudyptula]
```

Remove & check:

```bash
sudo rmmod waitqueues.ko
$ ps -ef | grep "\[eudyptula\]"
```

(no matchs ...)

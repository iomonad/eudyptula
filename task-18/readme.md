# Blocking Pattern

## Delegated the work to kthread

Implement your logic in your kthread runnable:

#### Block for new event

By setting the condition to always true in the event waiter.

The process is put to sleep until the condition evaluates to true
or a signal is received. The condition is checked each time the
waitqueue is woken up.

```c
	for (;;) {
		if (wait_event_interruptible(wee_wait, 1)) /* 1 */
			return -ERESTARTSYS;
```

Then add your runnable snippet, dont' forget to sleep
(in an interruptable state, don't go increasing the system load in a bad way):

```c
		if ((it = identity_get()) != NULL) {
			/* Make service poll relaxed */
			msleep_interruptible(5000);
```

#### Wake Up on events

On new write call, use `wake_up()`

wake_up() has to be called after changing any variable that could
change the result of the wait condition.

At the end of your `.write` handler, in the success case, add:

```c
	wake_up(&wee_wait);
```

> Every 5 seconds each one element of the list should pop. No event should be broadcasted to the kernel once the list is empty


#### Unit Tests

See `unit_test.sh` file.

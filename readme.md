<b>backpool</b> is a data structure for storing data used in real-time event processing. It uses pre-allocated memory and provides zero-based historical or 'backward' access to its pool items.

<b>backpool</b> is similar to a circular buffer. Memory is reused or written over when an incremented index exceeds the size of the pool.

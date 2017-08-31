<h3>What is a backpool?</h3>

<b>backpool</b> is a data structure for storing data that can be effectively used in real-time event processing. It uses pre-allocated memory and provides zero-based historical or 'backward' access to its pool items.

<h3>Reading from backpool</h3>

<b>backpool</b> is similar to a circular buffer. The fixed-size memory is reused or 'written over' when an incremented internal write index exceeds the pool size. However there is no buffer 'tail' and reading from the pool always starts from the last written item.

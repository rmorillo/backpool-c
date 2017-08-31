<h3>What is a backpool?</h3>

<b>backpool</b> is a data structure that can be effectively used in real-time event processing. It uses pre-allocated memory and provides zero-based historical or 'backward' access to its pool items.

<h3>Writing</h3>

<b>backpool</b> is similar to a circular buffer. The fixed-size memory is reused or 'written over' when an incremented internal write index exceeds the pool size.

<h3>Reading</h3>

Unlike circular buffer, there is no buffer 'tail' in backpool and reading from the pool always starts from the last written item.

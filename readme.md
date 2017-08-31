<h3>What is a backpool?</h3>

<b>backpool</b> is a data structure designed for real-time event processing. It uses pre-allocated memory and provides zero-based historical or 'backward' access to its pool items.

<h3>Writing</h3>

<b>backpool</b> is similar to a circular buffer. The fixed-size memory is reused or 'written over' when an incremented internal write index exceeds the pool size.

<h3>Reading</h3>

Unlike circular buffer, there is no buffer 'tail' in backpool and reading always starts from the last written item.  backpool offers 3 methods of reading from backpool depending on usage, concurrency and latency requirements.

<h4>Direct access method (single-threaded)</h4>
Using current, previous and indexer values.

<h4>Subscription method (thread-safe)</h4>

<h4>Trailing method (thread-safe)</h4>

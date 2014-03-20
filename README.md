# A zero-copy FreeRTOS demonstration


## Description

This demo implements a single-producer, multi-consumer FreeRTOS-based 
(www.freertos.org) system with a shared buffer for data exchange.  The producer
uses a counter to generate data which is copied into a shared circular buffer 
for a number of consumers to process.  When a consumer is done with the data, 
it decrements a shared barrier.  The last consumer to process the data frees
the relevant space in the circular buffer.



## Notes

- The included Eclipse project is currently ported to use a Windows/MingW 
  real-time "simulator", but can utilize other architectures using the portable
  files in the official FreeRTOS distribution.
  
## Copying

_[to be populated]_

## To-do

- Add a wrapper around the memcpy calls within circular buffer functions, to 
  later implement thread-safe DMA transfers for embedded systems.
- Add producer/consumer counters in the shared cbuff to determine if locks are 
  actually needed for read/writes.
- Clean up code formatting (as always).

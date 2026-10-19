Hello,

Welcome to my Multi-Sensor Controller. This project uses the STM32 Nucleo board and the low-level drivers provided by STM. These drivers offer a combination of flexibility and convenience for this project. Functions given by the LL drivers are often just to send signals through the pins and set different modes on the STM32 board. 

The overall structure of this project can be divided into main, basic drivers, and sensor drivers. Main is where the overall idea and execution of the code happens. Basic drivers include functions that take in a certain input and either read or write from the sensors/pins. Sensor drivers are functions specifically for that sensor so that they can be configured. Send out a special combination of commands and return values. This is a very basic overview, but you are welcome to take a look at the code (there are a lot of comments). The path to look at is /Core/Src/ ... 

The scheduler works in a way that saves the registers of the CPU and puts them onto a stack. The round-robin scheduler switches from one task to another with a priority logic of switching every few milliseconds, or depending on how fast you make the interrupt. So when when inturrpted, all tasks are paused and pushed onto the stack. Then the stack pushes a set of saved values onto the registers to continue the next task, then so on and so on. To make sure this scheduler works as intended, I included a tracker that increments by one when a task starts. That is the general idea of this scheduler.

Again, you are welcome to look through all the code.

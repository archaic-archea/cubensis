# Control Block
The Control Block (CB) of the GPU is used to run a Real-Time Operatin System (RTOS) that controls the scheduling, and memory protection of different GPU thread blocks, and threads. This can be used to prevent some memory accesses, start a new program, move data between the host system and the VRAM, or access the framebuffer in the VRAM. The control block also includes a custom micro-controller focused architecture, this serves as the host for the RTOS in control of the processor.

# Micro-controller
The Milk Cap micro-controller architecture is a 64 bit SIMD-VLIW design. To help further speed up the processor it also relies on a Harvard architecture, it seperates data and instructions into different memory controllers. In order to let the operating system boot properly it is provided instruction and data ROMs, these ROMs can provide up to 32 MiB, and 16 MiB of storage respectively. To further assist the speed of the Milk Cap micro-controller it is provided with it's own 128 MiB RAM region, this can be used by the operating system for general purpose allocation, and is exclusively accessible by the Milk Cap processor.

## Boot
The Milk Cap should boot with the lowest 16 MiB mapped to nothing, the following 16 MiB should be mapped to the data ROM, any attached IO subsystem, the 128 MiB Milk Cap RAM, and then the VRAM in the data region.
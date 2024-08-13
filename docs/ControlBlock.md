# Control Block
The Control Block (CB) of the GPU is used to run a Real-Time Operatin System (RTOS) that controls the scheduling, and memory protection of different GPU thread blocks, and threads. This can be used to prevent some memory accesses, start a new program, move data between the host system and the VRAM, or access the framebuffer in the VRAM. The control block also includes a custom micro-controller focused architecture, this serves as the host for the RTOS in control of the processor.

# Micro-controller
The Milk Cap micro-controller architecture is a 64 bit SIMD-VLIW design. To help further speed up the processor it also relies on a Harvard architecture, it seperates data and instructions into different memory controllers. In order to let the operating system boot properly it is provided instruction and data ROMs, these ROMs can provide up to 32 MiB, and 16 MiB of storage respectively. To further assist the speed of the Milk Cap micro-controller it is provided with it's own 128 MiB RAM region, this can be used by the operating system for general purpose allocation, and is exclusively accessible by the Milk Cap processor.

## Instruction Memory Map
- 0x0000000 to 0x1FFFFFF: Mapped to instruction ROM
- 0x2000000 to 0xXXXXXXX: Unused

## Data Memory Map
- 0x0000000 to 0x07FFFFF: Mapped to nothing, processor should raise an addressing fault if accessed
- 0x0800000 to 0x0FFFFFF: Mapped to the Graphics Specification Tree (GST)
- 0x1000000 to 0x1FFFFFF: Mapped to data ROM
- 0x2000000 to 0x9FFFFFF: Mapped to Milk Cap RAM
- 0xA000000 to 0xXXXXXXX: Mapped to VRAM, size specified in GST
- 0xXXXXXXX to 0xYYYYYYY: Mapped to IO Subsystem, size and location specified in GST
- 0xYYYYYYY to 0xZZZZZZZ: Unused

## Boot Conditions
The processor should be fully functional before passing off execution to the RTOS, the RTOS must be able to initialize devices itself without worry of starting up processor features. The RTOS should be able to read off the GST to gather information on what devices are available, the size of VRAM, the beginning and size of the IO subsystem, as well as any additional sections of data or instruction memory that are not specified here.

## VRAM Access
VRAM can be accessed directly by the Milk Cap processor, this includes extremely large memory accesses. It is recommended the Milk Cap processor only access VRAM when performing large memory access to write instructions or data for a program.

## Register File
The Milk Cap includes a register file supporting 8 'static' registers, these are guaranteed to always be visible in the register file. It also supports 24 more 'rotating' registers, these may be swapped around through the RTOS requesting a register rotation. This remaps the rotating registers to a new set of actual registers, this new window of registers may overlap with the previous window. Milk Cap processor implementations are required to implement 8 static registers, and at least 48 rotating registers. Upon a rotation request that may take up any space remaining in the register file the processor must write it out to the location specified in the "Register File Stack" register, this region does not need to be viewed by the operating system or other software, so the processor may do whatever it pleases to write out currently out of frame registers to the region, as long as data is not lost or corrupted.

## Execution Units
Milk Cap includes three execution types, and can support up to 32 execution units. The exact bundle format expected by a processor will vary by implementation, but every instruction bundle should be 512 bits. The 512 bit instruction bundles are split up into 32 seperate 16 bit instructions, each instruction directly interacts with it's given instruction unit. If multiple instructions within a given bundle read/write from overlapping registers, then the processor must guarantee that the write actions will not be present until at least the next cycle. If multiple instructions in a bundle write to overlapping registers then the behavior is specific to each processor, and possibly undefined. Milk Caps can have any combination of Integer, Memory, and Special execution units.
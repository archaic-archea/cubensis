# Choices
This document serves as a list of reasons for different architectural decisions about the Cubensis, Milk Cap, and other aspects of the GPU.

## Milk Cap VLIW
Milk Cap was chosen to be VLIW as it allows the system to more simply be implemented, and instead pushes hardware complexity that would be unneeded in a micro-controller onto the compiler. This allows more of a focus to go into the design of the Mycelium interconnect, and the Cubensis GPU cores. This may later on change in favor of a RISC design when more time can be allocated for the Milk Cap's design.
# Cubensis
Cubensis is a Graphical Processing Unit (GPU) architecture inspired by the design of VLIW/EPIC systems such as Itanium.
It relies on a cache hierarchy, read/write protection, a SIMT design, and an EPIC design.  
The processor is split into an L3 cache, a control block, and several 'thread blocks', each of these are specified in their own files.
Internal dynamic link library for Assault Cube x86 (32-bit).

This has been an ongoing project for my development in reverse engineering and developing tools to leverage the information gathered from reverse engineering, whilst continuing my practice with C++

Current features access player entity objects and write to their relative static addresses for modification to:
  - Health
  - Current weapon ammo
  - Rapidfire
  - Recoil
  - Grenades
  - Hooks in-game message function - prints text to it (more for testing - can add cool stuff to it though :) )

Most of the modifications happen at the start of the injection. However, there are some ASyncKeyStates (Hotkeys). These include:
  - No Recoil Toggle (Insert)
  - Safe Eject .dll (END)

# Snake (Win32, C)

A fully custom Snake game written in C using the Win32 API and GDI for rendering.  
This project focuses on low-level systems programming, manual memory management, and clean modular structure.

---

## Features

- Real-time Snake gameplay with smooth movement
- Win32 window creation and message loop
- Custom GDI rendering (double-buffered)
- Linked-list snake body implementation
- Dynamic fruit spawning and collision detection
- Energy / boost mechanic
- Pause / reset functionality
- File-based debug and error logging
- Modular architecture (Game / Platform / Win32 separation)

---

## Architecture Overview

The project is divided into three main modules:

### `game.c`
Owns **all game state and logic**:
- Game initialization and reset
- Snake movement and growth
- Collision detection
- Score and energy management

This module contains no rendering or Win32 logic.

---

### `snakeWin32.c`
Owns **all windowing and rendering**:
- Win32 window creation and message handling
- Input processing (keyboard)
- Frame rendering using GDI
- Double-buffered drawing

This module does not implement game rules.

---

### `platform.c`
Acts as a **bridge between game logic and Win32**:
- Initializes game and window system
- Routes timers and input to the game
- Exposes game state required by the renderer
- Handles application lifecycle and shutdown

---

## Build Instructions (MSYS2 / MinGW)


This project must be built using **MSYS2 MinGW (UCRT64 or MINGW64)**.

### Steps

1. Open **MSYS2 MinGW UCRT64** (or MINGW64) terminal  
2. Navigate to the project root directory:

```bash
cd /c/path/to/snake-game
```

3. Build and run:

```bash
mingw32-make clean
mingw32-make
./snake.exe
```

Alternatively, you can run `msys2.bat` from the project root, which launches the MSYS2 shell and builds automatically.


Requires:
- Windows OS
- GCC (MinGW)
- MSYS2 (UCRT64 / MinGW64)

---

## Controls

| Key | Action |
|-----|--------|
| W / ↑ | Move Up |
| A / ← | Move Left |
| S / ↓ | Move Down |
| D / → | Move Right |
| Enter | Pause / Resume |
| Shift | Boost |

---

## Technical Highlights

- Manual memory management (linked list snake)
- Event-driven architecture (Win32 message loop)
- Separation of concerns (Game vs Rendering vs Platform)
- Real-time update loop using Win32 timers
- Custom double-buffered rendering system
- Struct-based state organization

---

## Why This Project

This project was built to deepen my understanding of:

- Low-level C programming
- Win32 systems programming
- Game loop programming
- Memory and pointer management
- Modular software design

---

## Future Improvements (Optional)

- Sound effects
- Colour selection for snake body
- Improved snake sprites
- Additional fruit types
- Energy refill pick-ups
- Improved rendering abstraction
- Cross-platform rendering layer (SDL / OpenGL)
- Multiplayer mode (player versus player style)
- randomized levels (with varying map size, randomized wall placement, and different color palettes)
- High score display

---

## Author

Brandon Mokrytzki  
C / Systems Programming / Software Engineering

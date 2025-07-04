# Song Playlist Web App

## Architecture
- **Backend:** C program (`dscbp.c`) that manages the playlist and communicates via stdin/stdout using a simple command protocol (ADD, DELETE, LIST, PLAY, etc.).
- **HTTP Bridge:** Python Flask server that launches the C backend as a subprocess and exposes HTTP endpoints for the frontend.
- **Frontend:** Web page (HTML/JS) that interacts with the Python server via HTTP.

## How to Run
1. Compile the C backend:
   ```sh
   gcc dscbp.c -o playlist
   ```
2. Start the Python HTTP server:
   ```sh
   cd web
   python3 server.py
   ```
3. Open `web/index.html` in your browser.

## Commands
- Add, delete, list, play, shuffle, and sort songs from the web UI.
- Audio playback is handled by the backend using system audio tools (afplay/aplay/start).

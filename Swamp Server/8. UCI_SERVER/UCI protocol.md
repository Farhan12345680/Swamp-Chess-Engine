# Universal Chess Interface (UCI) — Condensed Specification

## 1. General

* UCI is independent of the operating system.
* Communication between GUI and engine uses **stdin/stdout** with text commands.
* Commands end with `\n`.
* The engine starts and waits for commands.
* The engine must be able to process input while searching.
* The engine must not start calculating until it receives `go`.
* Before `go`, the GUI sends a `position` command.
* Unknown commands or tokens should be ignored.
* Commands that are invalid in the current state should also be ignored.
* Arbitrary whitespace between tokens is allowed.

---

# 2. Move Format

Moves use long algebraic notation:

```text
e2e4
e7e5
e1g1
e7e8q
```

Promotion uses the promoted piece:

```text
e7e8q
e7e8r
e7e8b
e7e8n
```

A null move is:

```text
0000
```

---

# 3. GUI → Engine Commands

## `uci`

Tells the engine to enter UCI mode.

The engine must respond with:

```text
id name <name>
id author <author>
```

It may then send supported `option` commands.

Finally:

```text
uciok
```

The engine must send `uciok` after completing the UCI initialization response.

---

## `debug [on|off]`

Enables or disables debug output.

Debug mode is off by default.

---

## `isready`

Checks whether the engine has finished initialization or other lengthy operations.

The engine must always respond:

```text
readyok
```

It may receive `isready` while the engine is searching. The engine should respond without stopping the search.

---

## `setoption`

Changes an engine option.

Format:

```text
setoption name <id> [value <x>]
```

Examples of option types include:

* `check` — true/false
* `spin` — integer range
* `combo` — predefined values
* `button` — command/action
* `string` — text value

Option names and values are case-insensitive.

---

## `ucinewgame`

Tells the engine that a new game is starting.

The engine may clear its game-specific state, such as:

* Transposition table
* Search information
* Game history

The GUI should normally send:

```text
ucinewgame
isready
```

---

## `position`

Sets the current chess position.

Starting position:

```text
position startpos
```

FEN position:

```text
position fen <fen>
```

Moves may follow the position:

```text
position startpos moves e2e4 e7e5 g1f3
```

The engine must set up the position and apply all supplied moves.

---

## `go`

Starts searching the current position.

Possible search limits include:

```text
go depth <x>
go nodes <x>
go movetime <x>
go mate <x>
go infinite
```

Time controls:

```text
go wtime <x> btime <x>
go winc <x> binc <x>
go movestogo <x>
```

Where times are in milliseconds.

`go infinite` means search until `stop` is received.

---

## `stop`

Stops the current search as soon as possible.

The engine must send:

```text
bestmove <move>
```

when the search finishes.

---

## `ponderhit`

Tells the engine that the move it was pondering on has actually been played.

The engine switches from pondering to normal search.

---

## `quit`

Tells the engine to terminate.

---

# 4. Engine → GUI Commands

## `id`

Identifies the engine:

```text
id name <name>
id author <author>
```

---

## `uciok`

Indicates that UCI initialization is complete.

```text
uciok
```

---

## `readyok`

Response to:

```text
isready
```

---

## `bestmove`

Reports the best move found after a search.

Format:

```text
bestmove <move>
```

Optionally:

```text
bestmove <move> ponder <move>
```

Every `go` command must eventually result in a `bestmove`.

---

# 5. `info`

Used by the engine to send search information to the GUI.

Common form:

```text
info depth <x> score cp <x> nodes <x> time <x> nps <x> pv <moves>
```

Important fields:

### `depth`

Search depth in plies.

```text
info depth 15
```

### `seldepth`

Selective search depth.

### `time`

Search time in milliseconds.

### `nodes`

Number of searched positions.

### `nps`

Nodes searched per second.

### `pv`

Principal variation — the best line currently found.

```text
info pv e2e4 e7e5 g1f3
```

### `score cp`

Evaluation in centipawns.

```text
info score cp 35
```

Positive values mean an advantage from the engine's point of view.

### `score mate`

Mate score.

```text
info score mate 3
```

### `currmove`

Move currently being searched.

### `currmovenumber`

Number of the move currently being searched.

### `hashfull`

Percentage of the hash table used, expressed in permill.

### `string`

Human-readable information:

```text
info string <text>
```

---

# 6. `option`

Used during UCI initialization to tell the GUI which engine settings are available.

Format:

```text
option name <id> type <type> [default <x>] [min <x>] [max <x>] [var <x>]
```

Supported types:

```text
check
spin
combo
button
string
```

Example:

```text
option name Hash type spin default 16 min 1 max 1024
```

The GUI can later change it using:

```text
setoption name Hash value 128
```

---

# 7. Typical UCI Communication

### Initialization

```text
GUI → Engine:
uci

Engine → GUI:
id name Swamp
id author YourName
uciok
```

### Ready check

```text
GUI → Engine:
isready

Engine → GUI:
readyok
```

### Set position

```text
GUI → Engine:
position startpos moves e2e4 e7e5
```

### Start search

```text
GUI → Engine:
go
```

### Search information

```text
Engine → GUI:
info depth 10 score cp 25 nodes 100000 nps 500000 pv g1f3 e7e5
```

### Stop/search completion

```text
GUI → Engine:
stop

Engine → GUI:
bestmove g1f3
```

---

# 8. Minimum UCI Implementation

A basic chess engine should implement at least:

### GUI → Engine

```text
uci
isready
position
go
stop
quit
```

### Engine → GUI

```text
id name
id author
uciok
readyok
info
bestmove
```

This is sufficient for a basic engine to communicate with a UCI-compatible chess GUI.

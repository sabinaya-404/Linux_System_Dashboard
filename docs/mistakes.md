# the oopsies log (how i broke C and how i fixed it) (ノ_＜)

a graveyard of my silly syntax blunders, compiler rage moments, and what i learned so i don't do them again (•̀ᴗ•́)و ̑̑

---

## bug 01: the rogue curly brace that nuked the whole file (╯°□°)╯︵ ┻━┻
- **the symptom:** gcc threw a 50-line wall of terror shouting `expected identifier or '(' before 'while'` and claiming `stats` wasn't declared.
- **the mistake:** right after checking `if (!stats) { return -1; }`, i accidentally added an extra `}` right below it on line 12. this closed the entire `memory_get_stats` function immediately, leaving `memset`, `fopen`, and the `while` loop floating in global space outside of any function.
- **the fix:** deleted that sneaky `}` on line 12 so the function body actually stays open.
- **lesson learned:** in C, always double-check your brace pairs when gcc starts crying that variables don't exist inside what you thought was a function (・_・;)

---

## bug 02: missing comma in `sscanf` (*/ω＼*)
- **the symptom:** compiler threw a syntax error inside the `while` loop parsing `/proc/meminfo`.
- **the mistake:** wrote `sscanf(line, "MemTotal: %lu kB" &value)` with no comma between the format string and the variable address.
- **the fix:** added the comma -> `sscanf(line, "MemTotal: %lu kB", &value)`.
- **lesson learned:** C functions need commas between arguments, my brain was moving faster than my fingers (¬_¬ )

---

## bug 03: the nonexistent `swap` variable (⊙_⊙)
- **the symptom:** `error: 'swap' undeclared`
- **the mistake:** in the fallback branch of the swap calculation, wrote `swap->swap_usage_percent = 0;` instead of `stats->swap_usage_percent = 0.0;`.
- **the fix:** changed `swap->` to `stats->`.
- **lesson learned:** check your struct pointer names, don't invent new variables mid-code (*ﾉωﾉ)

---

## bug 04: `menu` vs `mem` & the 36000-second typo in `main.c` (´･ω･`)
- **the symptom:** `error: 'menu' undeclared` and uptime seconds showing weird numbers.
- **the mistake:** 
  1. typed `menu.uptime_seconds` when my struct variable in `main()` was named `mem`.
  2. wrote `mem.uptime_seconds % 36000` (extra zero) for minutes instead of `3600`.
  3. wrote `% 86400` for seconds instead of `% 60`.
- **the fix:** 
  - renamed `menu` -> `mem`
  - minutes: `(mem.uptime_seconds % 3600) / 60`
  - seconds: `mem.uptime_seconds % 60`
- **lesson learned:** 60 seconds in a minute, 3600 seconds in an hour, 86400 in a day. don't add random zeroes (b_d)

---

## bug 05: "sysmon: command not found" on linux ( ﾟдﾟ)
- **the symptom:** `make` compiled `sysmon` cleanly, but typing `sysmon` in the terminal gave "command not found".
- **the mistake:** linux doesn't look in the current folder for executables unless you tell it to.
- **the fix:** ran `./sysmon` with the dot-slash prefix.
- **lesson learned:** always use `./` when running binaries in the local directory on unix systems (｀・ω・´)

---

## bug 06: the perpetual 0.0% CPU ghost (゜-゜)
- **the symptom:** CPU usage was stuck at `0.0%` on every single run of `./sysmon`.
- **the mistake:** `cpu_get_stats` relied on an in-memory `static` snapshot to calculate deltas. But `main()` was only calling `cpu_get_stats` once and then exiting immediately. Since process memory gets wiped by Linux when an app exits, every run was a "first run" where `have_prev == 0`, returning `0.0%` forever!
- **the fix:** turned `main.c` into a live continuous loop with `while (1)` and `sleep(1)` so the process stays alive and computes the real delta on every second tick.
- **lesson learned:** `static` variables only live as long as the process is alive! Once the process terminates, all state vanishes (ﾟoﾟ〃)

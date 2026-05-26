
To get the serial output working for the classic Game Boy (DMG) — especially for tests like Blargg's Game Boy Test ROMs — you only need a *very small subset* of hardware behavior stubbed.

The important part is: many test ROMs print text through the serial registers at `FF01` and `FF02`.

You do **not** need a full PPU, timers, interrupts, or actual serial hardware to start seeing output.

You mainly need these stubs:

---

# 1. Serial Data Register (`SB`) — `0xFF01`

This holds the byte being "sent".

Example:

```cpp
case 0xFF01:
    sb = value;
    break;
```

and reads:

```cpp
case 0xFF01:
    return sb;
```

---

# 2. Serial Control Register (`SC`) — `0xFF02`

This is the important one.

The test ROM usually does:

```asm
LD A, character
LD (FF01), A

LD A, $81
LD (FF02), A
```

Meaning:

* bit 7 = start transfer
* bit 0 = internal clock

When your emulator sees a write of `0x81` to `FF02`, you should:

1. Read the byte from `SB`
2. Print it to console
3. Clear the transfer flag

Minimal stub:

```cpp
case 0xFF02:
    sc = value;

    if (value == 0x81) {
        char c = (char)sb;
        std::cout << c << std::flush;

        sc = 0x01; // transfer complete
    }
    break;
```

That alone is enough for many ROMs.

---

# 3. LY Register Stub (`0xFF44`)

Some tests hang forever unless this exists.

You do NOT need a real PPU yet.

Just return a constant.

Simplest:

```cpp
case 0xFF44:
    return 0x90;
```

or:

```cpp
return 0;
```

Many ROMs only poll LY waiting for it to change from impossible states.

A fake value is enough initially.

---

# 4. DIV Register (`0xFF04`) Stub

Some ROMs touch timers early.

You can fake this too.

Simplest:

```cpp
case 0xFF04:
    return div;
```

and increment `div` every CPU step or every few cycles.

Even a bad implementation is often enough for early testing.

---

# 5. Interrupt Enable / IF Registers

At minimum:

* `0xFFFF` → IE
* `0xFF0F` → IF

Just store/read values.

No real interrupt logic needed immediately.

---

# 6. Unimplemented IO Reads

Very important.

If your emulator crashes or aborts on unknown IO reads/writes, the tests may never reach serial output.

Instead of:

```cpp
abort();
```

do:

```cpp
std::cout << "UNHANDLED IO READ "
          << std::hex << addr << std::endl;

return 0xFF;
```

Returning `0xFF` is common behavior for unmapped hardware.

---

# Minimal Set Needed

If your goal is ONLY:

> "see serial text from Blargg"

then usually this is enough:

* `FF01` SB
* `FF02` SC
* `FF44` LY stub
* unknown IO returns `0xFF`

That is often sufficient to get:

```text
Passed
```

from CPU instruction tests.

---

# Important Detail

Do NOT print when writing to `FF01`.

Only print when:

```cpp
FF02 == 0x81
```

because that signals the transfer start.

The ROM loads bytes into `SB` first, then triggers transfer through `SC`.

---

# Typical Emulator Stub Phase

Early emulator development usually goes:

1. CPU executes correctly
2. Serial stub works
3. Blargg tests print text
4. Then timers
5. Then interrupts
6. Then PPU
7. Then boot real games

So what you're doing is the standard path.



# Best Systems For Beginners

* gets how many cpp files have i written
```bash
find . -name "*.cpp" -o -name "*.hpp" | xargs wc -l


│           ;[x] CODE XREF from main @ 0x216(x)
│       ┌─> 0x00000200      47             ld b, a
│       ╎   0x00000201      1100c0         ld de, 0xc000
│       ╎   0x00000204      0e10           ld c, 0x10
│       ╎   ; CODE XREFS from main @ 0x209(x), 0x20d(x)
│     ┌┌──> 0x00000206      2a             ldi a, [hl]
│     ╎╎╎   0x00000207      12             ld [de], a
│     ╎╎╎   0x00000208      1c             inc e
│     └───< 0x00000209      20fb           jr nZ, 0xfb
│      ╎╎   0x0000020b      14             inc d
│      ╎╎   0x0000020c      0d             dec c
│      └──< 0x0000020d      20f7           jr nZ, 0xf7
│       ╎   0x0000020f      78             ld a, b
│       ╎   0x00000210      c300c0         jp 0xc000
        ╎   ; CODE XREF from fcn.00000101 @ 0x101(x)
┌ 25: int main (int argc, char **argv, char **envp);
│       ╎   0x00000213      210040         ld hl, section.rombank01    ; 0x4000
│       └─< 0x00000216      c30002         jp 0x0200



JP u16: 0xC3 -- addr: 213 PC: 101 --
LD HL, d16 0x21 -- (HL == 4000) --
JP u16: 0xC3 -- addr: 200 PC: 216 --
LD B, A -- A: 0 --
LD (DE), d16 -- C000 --

LD C, d8 -- 0 --
LD A, (HL+)
Memory access out of bounds: 0

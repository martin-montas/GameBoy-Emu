
# Best Systems For Beginners

* gets how many cpp files have i written
```bash
find . -name "*.cpp" -o -name "*.hpp" | xargs wc -l
```


* These are the easiest starting points:
* System	Difficulty	Why
* NES	Medium	Very documented
* SNES	Medium	Huge community
* GBA	Easy-Medium	Excellent tooling
* Game Boy	Easy	Simple hardware
* Nintendo DS	Medium	Modern enough to be comfortable


# next

opcode 0x28
opcode 0x38


- finish this for the tet01  
     0x00000100      00             nop

    // NOP  0
    // JP u16: 0xC3
    // LD HL, d16 0x21 -- (HL == C000) --
    // LD A, d8: 0x3E A: -- 16 --
    // LD (HL), A: 0x10 0x77
    // INC (HL) 0x34
    // -- Debug: 0x11 to 0xC000 --
    // LD A, (HL) 0x11 0x7E
    // CP A,n8 opcode:0xFE, n8:11
    // JR NZ, r8 0x20 bool: 1
    // DEC (HL) 0x35 0x10
    // LD A, (HL) 0x10 0x7E
    // CP A,n8 opcode:0xFE, n8:10
    // JR NZ, r8 0x20 bool: 1
    // LD A, d8: 0x3E A: -- 5 --
    //  SUB nn. result of a -- 2 --
    // CP A,n8 opcode:0xFE, n8:2
    // JR NZ, r8 0x20 bool: 1
    // JR Z, r8 0x28
    // **LD A, d8: 0x3E A: -- 0 --**
    //  SUB nn. result of a -- FF --
    // JR NC, r8 should be checked for pc augmentation

┌ 32432: int main (int argc, char **argv, char **envp);
│           0x00000150      2100c0         ld hl, 0xc000
│           0x00000153      3e10           ld a, 0x10
│           0x00000155      77             ld [hl], a
│           0x00000156      34             inc [hl]
│           0x00000157      7e             ld a, [hl]
│           0x00000158      fe11           cp 0x11
│       ┌─< 0x0000015a      2023           jr nZ, 0x23
│       │   0x0000015c      35             dec [hl]
│       │   0x0000015d      7e             ld a, [hl]
│       │   0x0000015e      fe10           cp 0x10
│      ┌──< 0x00000160      201d           jr nZ, 0x1d
│      ││   0x00000162      3e05           ld a, 0x05
│      ││   80x00000164      d603           sub 0x03
│      ││   0x00000166      fe02           cp 0x02
│     ┌───< 0x00000168      2015           jr nZ, 0x15
│    ┌────< 0x0000016a      2802           jr Z, 0x02
│   ┌─────< 0x0000016c      1811           jr 0x11
│   │││││   ; CODE XREF from main @ 0x16a(x)
│   │└────> 0x0000016e      3e00           ld a, 0x00
│   │ │││   0x00000170      d601           sub 0x01
│   │┌────< 0x00000172      300b           jr nC, 0x0b
│   │││││   0x00000174      3e50           ld a, 0x50
│   │││││   0x00000176      ea01ff         ld [0xff01], a
│   │││││   0x00000179      3e81           ld a, 0x81
│   │││││   0x0000017b      ea02ff         ld [0xff02], a
│   │││││   0x0000017e      76             halt
│   │││││   ; CODE XREFS from main @ 0x15a(x), 0x160(x), 0x168(x), 0x16c(x), 0x172(x)
│   └└└└└─> 0x0000017f      3e46           ld a, 0x46                  ; 'F'**
│           0x00000181      ea01ff         ld [0xff01], a
│           0x00000184      3e81           ld a, 0x81
│           0x00000186      ea02ff         ld [0xff02], a
│           0x00000189      76             halt

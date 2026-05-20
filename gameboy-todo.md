
# Best Systems For Beginners

* gets how many cpp files have i written
```bash
find . -name "*.cpp" -o -name "*.hpp" | xargs wc -l
```
// NOP  0
    // JP u16: 0xC3
    // LD HL, d16 0x21 -- (HL == C000) --
    // LD A, d8: 0x3E A: -- F --
    // LD (HL), A: 0xF 0x77
    // INC (HL) 0x34
    // -- Debug: 0x10 to 0xC000 --
    // LD A, (HL) 0x10 0x7E
    // CP A,n8 opcode:0xFE, n8:10
    **JR NZ, r8 0x20 bool: 1**
    // LD A, d8: 0x3E A: -- F --
    // ADD A, d8
    // CP A,n8 opcode:0xFE, n8:10
    // JR NZ, r8 0x20 bool: 0
    // LD A, d8: 0x3E A: -- 46 --
    // LD(nn), A 0xEA nn: -- FF01 --
    // LD A, d8: 0x3E A: -- 81 --
    // LD(nn), A 0xEA nn: -- FF02 --
    // HALT
    // NOP  0

┌ 32432: int main (int argc, char **argv, char **envp);
│           0x00000150      2100c0         ld hl, 0xc000
│           0x00000153      3e0f           ld a, 0x0f
│           0x00000155      77             ld [hl], a
│           0x00000156      34             inc [hl]
│           0x00000157      7e             ld a, [hl]
│           0x00000158      fe10           cp 0x10
│       ┌─< 0x0000015a      203e           jr nZ, 0x3e
│       │   0x0000015c      3e0f           ld a, 0x0f
│       │   0x0000015e      c601           add 0x01
│       │   0x00000160      fe10           cp 0x10
│      ┌──< 0x00000162      2036           jr nZ, 0x36
│      ││   0x00000164      3eff           ld a, 0xff
│      ││   0x00000166      37             scf
│      ││   0x00000167      ce00           adc 0x00
│     ┌───< **0x00000169      202f           jr nZ, 0x2f**
│    ┌────< 0x0000016b      302d           jr nC, 0x2d
│    ││││   0x0000016d      3e00           ld a, 0x00
│    ││││   0x0000016f      de01           sbc 0x01
│    ││││   0x00000171      fefe           cp 0xfe
│   ┌─────< 0x00000173      2025           jr nZ, 0x25
│  ┌──────< 0x00000175      3023           jr nC, 0x23
│  ││││││   0x00000177      3e81           ld a, 0x81
│  ││││││   0x00000179      cb07           rlc a
│  ││││││   0x0000017b      fe03           cp 0x03
│ ┌───────< 0x0000017d      201b           jr nZ, 0x1b
│ ────────< 0x0000017f      3019           jr nC, 0x19
│ │││││││   0x00000181      2100c0         ld hl, 0xc000
│ │││││││   0x00000184      3eaa           ld a, 0xaa
│ │││││││   0x00000186      77             ld [hl], a
│ │││││││   0x00000187      c655           add 0x55
│ │││││││   0x00000189      77             ld [hl], a
│ │││││││   0x0000018a      7e             ld a, [hl]
│ │││││││   0x0000018b      feff           cp 0xff
│ ────────< 0x0000018d      200b           jr nZ, 0x0b
│ │││││││   0x0000018f      3e50           ld a, 0x50
│ │││││││   0x00000191      ea01ff         ld [0xff01], a
│ │││││││   0x00000194      3e81           ld a, 0x81
│ │││││││   0x00000196      ea02ff         ld [0xff02], a
│ │││││││   0x00000199      76             halt
│ │││││││   ; XREFS: CODE 0x0000015a  CODE 0x00000162  CODE 0x00000169  CODE 0x0000016b  CODE 0x00000173  CODE 0x00000175
│ │││││││   ; XREFS: CODE 0x0000017d  CODE 0x0000017f  CODE 0x0000018d
│ └└└└└└└─> 0x0000019a      3e46           ld a, 0x46                  ; 'F'
│           0x0000019c      ea01ff         ld [0xff01], a
│           0x0000019f      3e81           ld a, 0x81
│           0x000001a1      ea02ff         ld [0xff02], a


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

[0x00000150 [xAdvc]0 0% 270 add_test.gb]> afsQ;pd $r.. @ main
            0x0000014f  ~   d63e           sub 0x3e
            ; CODE XREF from fcn.00000101 @ 0x101(x)
┌ 32432: int main (int argc, char **argv, char **envp);
│           0x00000150      3e01           ld a, 0x01
│           0x00000152      0602           ld b, 0x02
│           0x00000154      80             add b
│           0x00000155      fe03           cp 0x03
│       ┌─< 0x00000157      200f           jr nZ, 0x0f
│      ┌──< 0x00000159      280d           jr Z, 0x0d
│     ┌───< 0x0000015b      380b           jr C, 0x0b
│     │││   0x0000015d      3e50           ld a, 0x50
│     │││   0x0000015f      ea01ff         ld [0xff01], a
│     │││   0x00000162      3e81           ld a, 0x81
│     │││   0x00000164      ea02ff         ld [0xff02], a
│     │││   0x00000167      76             halt
│     │││   ; CODE XREFS from main @ 0x157(x), 0x159(x), 0x15b(x)
│     └└└─> 0x00000168      3e46           ld a, 0x46                  ; 'F'
│           0x0000016a      ea01ff         ld [0xff01], a
│           0x0000016d      3e81           ld a, 0x81
│           0x0000016f      ea02ff         ld [0xff02], a
│           0x00000172      76             halt
│           0x00000173      00             nop
│           0x00000174      00             nop
│           0x00000175      00             nop
│           0x00000176      00             nop
│           0x00000177      00             nop
│           0x00000178      00             nop
│           0x00000179      00             nop
│           0x0000017a      00             nop

┌ 32432: int main (int argc, char **argv, char **envp);
│           0x00000150      3e01           ld a, 0x01
│           0x00000152      0602           ld b, 0x02
│           0x00000154      80             add b
│           0x00000155      fe03           cp 0x03
│       ┌─< 0x00000157      200f           jr nZ, 0x0f
│      ┌──< 0x00000159      280d           jr Z, 0x0d
│     ┌───< 0x0000015b      380b           jr C, 0x0b
│     │││   0x0000015d      3e50           ld a, 0x50
│     │││   0x0000015f      ea01ff         ld [0xff01], a
│     │││   0x00000162      3e81           ld a, 0x81
│     │││   0x00000164      ea02ff         ld [0xff02], a
│     │││   0x00000167      76             halt
│     │││   ; CODE XREFS from main @ 0x157(x), 0x159(x), 0x15b(x)
│     └└└─> 0x00000168      3e46           ld a, 0x46                  ; 'F'
│           0x0000016a      ea01ff         ld [0xff01], a
│           0x0000016d      3e81           ld a, 0x81
│           0x0000016f      ea02ff         ld [0xff02], a
│           0x00000172      76             halt
│           0x00000173      00             nop
│           0x00000174      00             nop
│           0x00000175      00             nop

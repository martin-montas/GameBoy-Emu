
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


- finish this for the tet01  
     0x00000100      00             nop

 ┌─< 0x00000101      c35001         jp main
 │   0x00000104      ceed           adc 0xed
 │   0x00000106      66             ld h, [hl]
 │   0x00000107      66             ld h, [hl]
 │   0x00000108      cc0d00         call Z, 0x000d              ;[1]
 │   0x0000010b      0b             dec bc
 │   0x0000010c      03             inc bc
 │   0x0000010d      73             ld [hl], e
 │   0x0000010e      00             nop
 │   0x0000010f      83             add e
 │   0x00000110      00             nop
 │   0x00000111      0c             inc c
 │   0x00000112      00             nop
 │   0x00000113      0d             dec c
 │   0x00000114      00             nop
 │   0x00000115      08111f         ld [0x1f11], sp
 │   0x00000118      88             adc b
 │   0x00000119      89             adc c
 │   0x0000011a      00             nop
 │   0x0000011b      0edc           ld c, 0xdc
 │   0x0000011d      cc6ee6         call Z, 0xe66e
 │   0x00000120      dd             invalid
 │   0x00000121      dd             invalid
 │   0x00000122      d9             reti
 │   0x00000123      99             sbc c
 │   0x00000124      bb             cp e
 │   0x00000125      bb             cp e
 │   0x00000126      67             ld h, a
 │   0x00000127      63             ld h, e
 │   0x00000128      6e             ld l, [hl]
 │   0x00000129      0eec           ld c, 0xec
 │   0x0000012b      ccdddc         call Z, 0xdcdd
 │   0x0000012e      99             sbc c
 │   0x0000012f      9f             sbc a
 │   0x00000130      bb             cp e
 │   0x00000131      b9             cp c
 │   0x00000132      33             inc sp
 │   0x00000133      3e00           ld a, 0x00


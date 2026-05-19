SECTION "Header", ROM0[$100]

nop
jp Start

ds $150 - @, 0

Start:

        // JP u16: 0xC3




    ld hl, $C000      ; WRAM test address
    ld a, $10
    ld [hl], a        ; [C000] = 0x10

    inc [hl]          ; 0x10 -> 0x11
    ld a, [hl]

    cp $11
    jr nz, .fail

    dec [hl]          ; 0x11 -> 0x10
    ld a, [hl]

    cp $10
    jr nz, .fail

    ld a, $05
    sub $03           ; A = 0x02

    cp $02
    jr nz, .fail

    jr z, .check_carry
    jr .fail

.check_carry:
    ld a, $00
    sub $01           ; should set C flag

    jr nc, .fail      ; must NOT skip (C must be 1)

.pass:
    ld a, 'P'
    ld [$FF01], a

    ld a, $81
    ld [$FF02], a
    halt

.fail:
    ld a, 'F'
    ld [$FF01], a

    ld a, $81
    ld [$FF02], a
    halt

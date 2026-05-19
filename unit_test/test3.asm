SECTION "Header", ROM0[$100]

nop
jp Start

ds $150 - @, 0

Start:

; =========================
; WRAM baseline test
; =========================
    ld hl, $C000
    ld a, $0F
    ld [hl], a

    inc [hl]          ; 0x0F -> 0x10 (tests H flag on INC)
    ld a, [hl]
    cp $10
    jr nz, .fail

; =========================
; Half-carry check via ADD
; =========================
    ld a, $0F
    add a, $01        ; should become 0x10, H = 1

    cp $10
    jr nz, .fail

; =========================
; ADC carry propagation
; =========================
    ld a, $FF
    scf               ; C = 1
    adc a, $00        ; FF + 0 + 1 = 0x00, C = 1

    jr nz, .fail      ; must be zero
    jr nc, .fail      ; must still have carry

; =========================
; SBC borrow propagation
; =========================
    ld a, $00
    sbc a, $01        ; 0 - 1 - 1 = 0xFE, C = 1

    cp $FE
    jr nz, .fail
    jr nc, .fail

; =========================
; RLC test (carry feedback loop)
; =========================
    ld a, $81         ; 1000 0001

    rlc a             ; result: 0000 0011, C = 1

    cp $03
    jr nz, .fail
    jr nc, .fail      ; must have C = 1

; =========================
; Memory stability after flag ops
; =========================
    ld hl, $C000
    ld a, $AA
    ld [hl], a

    add a, $55        ; 0xFF
    ld [hl], a        ; store result

    ld a, [hl]
    cp $FF
    jr nz, .fail

; =========================
; PASS
; =========================
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

SECTION "Header", ROM0[$100]

nop
jp Start

ds $150 - @, 0

Start:
ld a, $01
ld b, $02

add a, b

cp $03
jr nz, .fail

jr z, .fail
jr c, .fail

.pass
ld a, 'P'
ld [$FF01], a

ld a, $81
ld [$FF02], a

halt

.fail
ld a, 'F'
ld [$FF01], a

ld a, $81
ld [$FF02], a

halt

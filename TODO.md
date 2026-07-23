## Project TODOs

## PPU / rendering
- finish sprite rendering `you are here.`
- finish the window layer rendering  **DONE**
- finish stat   rendering
- flag in the STAT register is set, and (if enabled) a STAT interrupt is requested.
- FF45 — LYC: LY compare The Game Boy constantly compares the value of the LYC and LY registers. When both values are identical, the “LYC=LY” **DONE**


---

## fix immediately
- Try to understand this

```
//     fn render_window_scanline(&mut self) {
//         let line = self.ly as usize;
// 
//         let tilemap_base: u16 = if self.lcdc & LCDC_WIN_TILEMAP != 0 {
//             0x9C00
//         } else {
//             0x9800
//         };
// 
//         let unsigned_addressing = self.lcdc & LCDC_BG_WIN_TILEDATA != 0;
// 
//         // The window X on screen starts at (WX - 7). Values 0-6 are
//         // effectively negative and clip the left side of the window.
//         let wx_screen = self.wx as i16 - 7;
// 
//         let pixel_y = self.window_line % 8;
//         let tile_row = (self.window_line as u16 / 8) & 31;
// 
//         let mut any_drawn = false;
// 
//         for px in 0..SCREEN_WIDTH {
//             let screen_x = px as i16;
//             if screen_x < wx_screen {
//                 continue;
//             }
// 
//             any_drawn = true;
//             let win_x = (screen_x - wx_screen) as u8;
//             let tile_col = (win_x as u16 / 8) & 31;
//             let pixel_x = win_x % 8;
// 
//             let map_addr = tilemap_base + tile_row * 32 + tile_col;
//             let tile_index = self.vram_read_internal(map_addr);
// 
//             let tile_data_addr = tile_data_address(tile_index, unsigned_addressing, pixel_y);
// 
//             let lo = self.vram_read_internal(tile_data_addr);
//             let hi = self.vram_read_internal(tile_data_addr + 1);
// 
//             let colour_id = pixel_colour_id(lo, hi, pixel_x);
//             self.bg_color_ids[px] = colour_id;
//             self.frame_buf[line][px] = apply_palette(self.bgp, colour_id);
//         }
// 
//         // The window line counter only increments on scanlines where the
//         // window was actually rendered.
//         if any_drawn {
//             self.window_line += 1;
//             self.window_triggered = true;
//         }
//     }


- **ALSO**:
try to read from this repo:
`https://github.com/Dhi13man/gb-emulator/blob/main/gb-core/src/ppu/mod.rs`

- create another loop that separate the other one from the background

---

## audio
- finish audio

---

## requires immediate action:
- [X] Better interrupt handling
- [X] Finish Halt instruction
- [X] Test halt instruction
- [X] Test IME Interrupt with blarrgs
- [X] Finish Timer 
- [X] Test interrupt
- [ ] Finish the STAT part

- [ ] Blargg's  test

## Milestones:
- [X] CPU.
- [X] Finish bus.
- [X] Boot rom rendering.
- [X] Finish timer
- [X] Finish Interrupt
- [ ] Dmg Acid2 test rom.
- [ ] Finish PPU.
- [ ] CPU-PPU-TIMER sync.

---

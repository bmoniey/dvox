# dvox
Ham Radio Sound Card Interface
Arduino Vox Control
128x64 SSD1306 display operated by rotary encoder and switch

Based on DuinoVOX by Kevin Loughin KB9RLW
Modified by: Brian Moran N6MNE

## 1. Features:

* USB Sound Card Interface (USB - Mic, Headphone Left, Headphone Right)

* VOX Detection Circuit (listens on headphone right sound card channel, Threshold detect on A1 of Arduino)

* Push to Talk Relay (D13 of Arduino Nano)

* Left Channel Connected to Radio Mic via Audio Transformer

* Microphone Connected to Radio Headphone Jack via Audio Transformer

* Mic Gain Adjust pot

* Headphone Gain Adjust pot

* SSD1306 OLED Display

* KY-40 Encoder to drive menu

## 2.Useful Links:

* https://vk6ysf.com/scits430s.htm

* http://solorb.com/elect/hamcirc/ts430interface/

* http://kb9rlw.blogspot.com/2016/08/cheap-and-easy-to-build-digital-modes.html

## 3. Testing

* Radio: TS-430S Mic,PTT, and Head phone interface

* Tested with WSJT-X with FT8

* Tested with FLDigi with BPSK-31

* Sound card: Sabrent USB Sound Card from Amazon (see BOM)

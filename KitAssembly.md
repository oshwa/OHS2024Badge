# OHS 2024 Badge Kit Assembly
Some of the badges distributed at OHS 2024 came without electronic components, unfortunately. However, we've put together some parts kits for you to try assembling it yourself!
## Parts List
You should have received the following:
- 1 Battery Holder
- 1 Pushbutton
- 1 Slide Switch
- 1 SAO header
- 1 USB Type C connector
- 1-2 Diodes (you only need one, we tried to give you a spare)
- 8 LEDs (we have a few spares, but please be careful - you need 8)
- 2 ATMEGA328PB Microcontrollers (you only need one, we gave you a spare)
- 2 0.1uF capacitors (short paper tape, parts are brown with silver ends. You only need one, we gave you a spare)
- 3 270 ohm resistors (blue stripe on the paper tape. You only need one, we gave you a spare)
- 15 100 ohm resistors (long paper tape. You only need 10, we gave you spares)
- 4 5.1K ohm resistors (short paper tape. You only need two, we gave you spares)

Here's a photo of the parts you should have:
<img alt="Top view of the parts provided in the kit, with text labels for each part." src="images/kit_parts.JPG" width="800px">

## Where the parts go
The diagram below shows where the parts go on the board:

<img alt="Rear view of Open Hardware Summit 2024 Badge PCB, with different components circled to indicate their positions." src="images/badge_parts.jpg" width="600px">

The biggest thing to be aware of is the mounting of the LEDs:
- We're using slightly different LEDs than were on the original boards, because they're much, much cheaper.
- The pins of the LEDs stick out straight from the body of the LED. You'll need to bend them down - this is towards the side of the LED with the clear lens
- When you mount the LED to the board, note that one of the pins of the LED is chamfered (has a corner cut off). This pin should not be placed on the pad with the white stripe - it should be on the pin diagonally across from that.

Some other notes on placing the parts:
- The capacitors and resistors don't care which way they go on the board - there are two ways that they fit, and both work fine
- The diode is polarized. If you look at it from the right angle, you'll see a stripe on one side. This lines up with a white stripe on the board.
- The battery holder is polarized. One corner is chamfered (cut at an angle) - this is on the positive side. This lines up with the + side indicated on the PCB
- The pushbutton has 2 orientations that it will fit the pads on the board. Either of those orientations will work.
- The microcontroller has 4 ways that it will fit, but only one will work. There is an indented dot on the microcontroller - this lines up with the white stripe beside one of the pins on the PCB. Also, with the lanyard loop pointed away from you, the microcontroller should be mounted such that you can read the text.
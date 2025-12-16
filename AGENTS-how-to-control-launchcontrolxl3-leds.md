# Novation Launch Control XL 3 - LED Control Reference

This document describes how to programmatically control the RGB LEDs on the Novation Launch Control XL 3 via MIDI.

## Overview

The Launch Control XL 3 has RGB LEDs on all 24 rotary encoders and the 16 track buttons. These can be set to any RGB color via SysEx messages, but **only when the device is in DAW mode**.

## MIDI Port Selection

The Launch Control XL 3 exposes **two USB MIDI ports**:

| Port | macOS Name | Windows Name | Purpose |
|------|------------|--------------|---------|
| 1 | `Launch Control XL 3` | `MIDIIN/OUT (LCXL3)` | Main MIDI port for Custom Modes |
| 2 | `Launch Control XL 3 DAW Out` | `MIDIIN2/OUT2 (LCXL3)` | DAW port - **required for LED control** |

**You must send LED commands to the DAW port (port 2).**

## Enabling DAW Mode

LED control only works when the device is in DAW mode. You must enable DAW mode before sending LED commands.

### Method 1: Note Message (Recommended)

Send a Note On message on MIDI channel 16:

| Action | Status | Note | Velocity | Hex |
|--------|--------|------|----------|-----|
| Enable DAW Mode | Note On Ch16 | 12 | 127 | `9F 0C 7F` |
| Disable DAW Mode | Note On Ch16 | 12 | 0 | `9F 0C 00` |

### Method 2: SysEx Message

```
Enable:  F0 00 20 29 02 15 02 7F F7
Disable: F0 00 20 29 02 15 02 00 F7
```

### openFrameworks Example (ofxMidi)

```cpp
// Enable DAW mode
midiOut.sendNoteOn(16, 12, 127);

// Disable DAW mode
midiOut.sendNoteOn(16, 12, 0);
```

## Setting LED Colors

### SysEx Message Format

To set an LED to a specific RGB color:

```
F0 00 20 29 02 15 01 53 <index> <R> <G> <B> F7
```

| Byte | Value | Description |
|------|-------|-------------|
| F0 | 240 | SysEx start |
| 00 20 29 | 0 32 41 | Novation manufacturer ID |
| 02 | 2 | Device family |
| 15 | 21 | Launch Control XL 3 device ID |
| 01 | 1 | LED command |
| 53 | 83 | RGB color subcommand |
| `<index>` | 5-52 | Control index (see table below) |
| `<R>` | 0-127 | Red intensity |
| `<G>` | 0-127 | Green intensity |
| `<B>` | 0-127 | Blue intensity |
| F7 | 247 | SysEx end |

### Control Index Map

#### Faders (indices 5-12)
| Fader | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|-------|---|---|---|---|---|---|---|---|
| Index | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
| Hex | 05 | 06 | 07 | 08 | 09 | 0A | 0B | 0C |

#### Encoders Row 1 - Top (indices 13-20)
| Encoder | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---------|---|---|---|---|---|---|---|---|
| Index | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| Hex | 0D | 0E | 0F | 10 | 11 | 12 | 13 | 14 |

#### Encoders Row 2 - Middle (indices 21-28)
| Encoder | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|---------|---|---|---|---|---|---|---|---|
| Index | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 |
| Hex | 15 | 16 | 17 | 18 | 19 | 1A | 1B | 1C |

#### Encoders Row 3 - Bottom (indices 29-36)
| Encoder | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 |
|---------|---|---|---|---|---|---|---|---|
| Index | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 |
| Hex | 1D | 1E | 1F | 20 | 21 | 22 | 23 | 24 |

#### Track Focus Buttons - Top Row (indices 37-44)
| Button | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|--------|---|---|---|---|---|---|---|---|
| Index | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 |
| Hex | 25 | 26 | 27 | 28 | 29 | 2A | 2B | 2C |

#### Track Control Buttons - Bottom Row (indices 45-52)
| Button | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|--------|---|---|---|---|---|---|---|---|
| Index | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 |
| Hex | 2D | 2E | 2F | 30 | 31 | 32 | 33 | 34 |

### openFrameworks Example (ofxMidi)

```cpp
void setLED(ofxMidiOut& midiOut, int controlIndex, int r, int g, int b) {
    // Clamp to valid MIDI data range
    r = ofClamp(r, 0, 127);
    g = ofClamp(g, 0, 127);
    b = ofClamp(b, 0, 127);

    vector<unsigned char> sysex;
    sysex.push_back(0xF0);  // SysEx start
    sysex.push_back(0x00);  // Novation manufacturer ID
    sysex.push_back(0x20);
    sysex.push_back(0x29);
    sysex.push_back(0x02);  // Device family
    sysex.push_back(0x15);  // Launch Control XL 3
    sysex.push_back(0x01);  // LED command
    sysex.push_back(0x53);  // RGB subcommand
    sysex.push_back(controlIndex);
    sysex.push_back(r);
    sysex.push_back(g);
    sysex.push_back(b);
    sysex.push_back(0xF7);  // SysEx end

    midiOut.sendMidiBytes(sysex);
}

// Set encoder 8 (top row, rightmost) to red
setLED(midiOut, 20, 127, 0, 0);

// Set button 1 to blue
setLED(midiOut, 37, 0, 0, 127);
```

### Helper Functions

```cpp
// Set encoder LED (encoderNum 1-24)
void setEncoderLED(ofxMidiOut& midiOut, int encoderNum, int r, int g, int b) {
    if (encoderNum < 1 || encoderNum > 24) return;
    int controlIndex = 12 + encoderNum;  // encoder 1 = index 13
    setLED(midiOut, controlIndex, r, g, b);
}

// Set button LED (buttonNum 1-16)
void setButtonLED(ofxMidiOut& midiOut, int buttonNum, int r, int g, int b) {
    if (buttonNum < 1 || buttonNum > 16) return;
    int controlIndex = 36 + buttonNum;  // button 1 = index 37
    setLED(midiOut, controlIndex, r, g, b);
}
```

## Complete Initialization Sequence

```cpp
#include "ofxMidi.h"

ofxMidiOut midiOut;

void setup() {
    // 1. List ports and find DAW port
    midiOut.listOutPorts();
    vector<string> ports = midiOut.getOutPortList();

    for (int i = 0; i < ports.size(); i++) {
        string portLower = ofToLower(ports[i]);
        if (portLower.find("launch control xl") != string::npos &&
            portLower.find("daw") != string::npos) {
            midiOut.openPort(i);
            break;
        }
    }

    // 2. Enable DAW mode
    ofSleepMillis(100);  // Brief delay after opening port
    midiOut.sendNoteOn(16, 12, 127);

    // 3. Now you can set LEDs
    // Set all encoders to a rainbow
    for (int i = 1; i <= 24; i++) {
        float hue = ofMap(i, 1, 24, 0, 1);
        ofFloatColor c = ofFloatColor::fromHsb(hue, 1.0, 1.0);
        setEncoderLED(midiOut, i, c.r * 127, c.g * 127, c.b * 127);
    }
}

void exit() {
    // Disable DAW mode before closing
    midiOut.sendNoteOn(16, 12, 0);
    midiOut.closePort();
}
```

## Color Examples

| Color | R | G | B |
|-------|---|---|---|
| Red | 127 | 0 | 0 |
| Green | 0 | 127 | 0 |
| Blue | 0 | 0 | 127 |
| Yellow | 127 | 127 | 0 |
| Cyan | 0 | 127 | 127 |
| Magenta | 127 | 0 | 127 |
| White | 127 | 127 | 127 |
| Orange | 127 | 64 | 0 |
| Purple | 64 | 0 | 127 |
| Off | 0 | 0 | 0 |

## Transport & Navigation Buttons (DAW Mode)

In DAW mode, the additional buttons on the Launch Control XL 3 send CC messages. Most are on channel 1, except Shift which is on channel 7:

| Button | Channel | CC | Description |
|--------|---------|-----|-------------|
| Track Left | 1 | 103 | Navigate tracks left |
| Track Right | 1 | 102 | Navigate tracks right |
| Page Left | 1 | 106 | Navigate pages/banks left |
| Page Right | 1 | 107 | Navigate pages/banks right |
| Play | 1 | 116 | Transport play |
| Record | 1 | 118 | Transport record |
| Shift | 7 | 63 | Modifier key (note: different channel!) |
| DAW Control | 1 | 65 | Switch to DAW Control mode |
| DAW Mixer | 1 | 66 | Switch to DAW Mixer mode |
| Novation | 1 | 104 | Novation button |

All buttons send value 127 on press and value 0 on release.

### openFrameworks Example

```cpp
void handleTransportButton(int cc, int value) {
    bool pressed = value > 64;
    
    switch (cc) {
        case 116:  // Play
            if (pressed) togglePlayback();
            break;
        case 118:  // Record
            if (pressed) toggleRecording();
            break;
        case 102:  // Track Right
            if (pressed) nextTrack();
            break;
        case 103:  // Track Left
            if (pressed) prevTrack();
            break;
        case 106:  // Page Left
            if (pressed) prevPage();
            break;
        case 107:  // Page Right
            if (pressed) nextPage();
            break;
    }
}
```

## Important Notes

1. **DAW mode is required** - LEDs will not respond in standalone mode (Custom Modes)
2. **Use the DAW port** - The main MIDI port does not accept LED commands
3. **Color range is 0-127** - Standard MIDI data byte range, not 0-255
4. **Disable DAW mode on exit** - Return the controller to standalone mode when your app closes
5. **Brief delay after port open** - Wait ~100ms after opening the port before sending commands

## Troubleshooting

| Problem | Solution |
|---------|----------|
| LEDs don't respond | Verify you're using the DAW port, not the main MIDI port |
| LEDs don't respond | Ensure DAW mode is enabled (send `9F 0C 7F`) |
| Wrong LED lights up | Check the control index - encoders start at 13, not 1 |
| Colors look wrong | Remember range is 0-127, not 0-255 |
| App crashes on exit | Close MIDI port after disabling DAW mode |

## OLED Display Control

The Launch Control XL 3 has a 128×64 monochrome OLED display that can be controlled via SysEx messages in DAW mode.

### Display Types

| Target | Hex | Dec | Description |
|--------|-----|-----|-------------|
| Faders | 05-0C | 5-12 | Temporary display for fader controls |
| Encoders | 0D-24 | 13-36 | Temporary display for encoder controls |
| Stationary | 35 | 53 | Permanent display (always shown unless overridden) |
| Temporary | 36 | 54 | Overlay display (shown temporarily) |

### Display Arrangements

| ID | Description | Num Fields | Field 0 | Field 1 | Field 2 |
|----|-------------|------------|---------|---------|---------|
| 0 | Cancel display | - | - | - | - |
| 1 | 2 lines: Parameter Name + Text Value | 2 | Name | Value | - |
| 2 | 3 lines: Title + Name + Text Value | 3 | Title | Name | Value |
| 3 | 1 line + 8 names (encoder designations) | 9 | Title | Name×8 | - |
| 4 | 2 lines: Parameter Name + Numeric Value (default) | 1 | Name | - | - |
| 31 | Trigger display with current contents | - | - | - | - |

### Configure Display

Configure the display arrangement before setting text.

```
F0 00 20 29 02 15 04 <target> <config> F7
```

| Byte | Value | Description |
|------|-------|-------------|
| F0 | 240 | SysEx start |
| 00 20 29 | 0 32 41 | Novation manufacturer ID |
| 02 | 2 | Device family |
| 15 | 21 | Launch Control XL 3 device ID |
| 04 | 4 | Configure display command |
| `<target>` | 5-54 | Display target (see table above) |
| `<config>` | 0-31 | Arrangement ID + flags (see below) |
| F7 | 247 | SysEx end |

**Config byte flags:**
- Bit 6 (0x40): Allow auto temp display on value change (default: set)
- Bit 5 (0x20): Allow auto temp display on touch/Shift+rotate (default: set)
- Bits 0-4: Arrangement ID (0-31)

**Special config values:**
- `00` (0): Cancel/hide the display
- `7F` (127): Trigger/show the display with current contents

### Set Text

Set the text content for a configured display.

```
F0 00 20 29 02 15 06 <target> <field> <text...> F7
```

| Byte | Value | Description |
|------|-------|-------------|
| F0 | 240 | SysEx start |
| 00 20 29 | 0 32 41 | Novation manufacturer ID |
| 02 | 2 | Device family |
| 15 | 21 | Launch Control XL 3 device ID |
| 06 | 6 | Set text command |
| `<target>` | 5-54 | Display target |
| `<field>` | 0-2 | Which text field to set |
| `<text...>` | ASCII | Text content (see character map) |
| F7 | 247 | SysEx end |

**Character Map:**
- Standard ASCII: 0x20 (32) to 0x7E (126)
- Special characters:
  - `1B` (27): Empty box □
  - `1C` (28): Filled box ■
  - `1D` (29): Flat symbol ♭
  - `1E` (30): Heart ♥

### Bitmap Display

Display custom 128×64 pixel graphics.

```
F0 00 20 29 02 15 09 <target> <bitmap_data> 7F
```

| Byte | Value | Description |
|------|-------|-------------|
| F0 | 240 | SysEx start |
| 00 20 29 | 0 32 41 | Novation manufacturer ID |
| 02 | 2 | Device family |
| 15 | 21 | Launch Control XL 3 device ID |
| 09 | 9 | Bitmap command |
| `<target>` | 20/21 | 0x20=Stationary, 0x21=Temporary |
| `<bitmap_data>` | 1216 bytes | Pixel data (see format below) |
| 7F | 127 | SysEx end (note: 7F not F7) |

**Bitmap Format:**
- 19 bytes per row × 64 rows = 1216 bytes total
- Each byte encodes 7 pixels (MSB = leftmost)
- 19 bytes × 7 bits = 133 bits per row (128 used, 5 unused in last byte)

**Acknowledgment:**
The device sends this response when ready for the next frame:
```
F0 00 20 29 02 15 09 7F
```

### openFrameworks Examples

```cpp
// Helper to send display SysEx
void sendDisplaySysEx(ofxMidiOut& midiOut, const std::vector<unsigned char>& data) {
    std::vector<unsigned char> sysex;
    sysex.push_back(0xF0);  // SysEx start
    sysex.push_back(0x00);  // Novation manufacturer ID
    sysex.push_back(0x20);
    sysex.push_back(0x29);
    sysex.push_back(0x02);  // Device family
    sysex.push_back(0x15);  // Launch Control XL 3
    sysex.insert(sysex.end(), data.begin(), data.end());
    sysex.push_back(0xF7);  // SysEx end
    midiOut.sendMidiBytes(sysex);
}

// Configure stationary display with 2-line arrangement (Name + Value)
void configureStationaryDisplay(ofxMidiOut& midiOut) {
    sendDisplaySysEx(midiOut, {0x04, 0x35, 0x01});  // cmd=04, target=35, arrangement=1
}

// Set text on stationary display
void setStationaryText(ofxMidiOut& midiOut, const std::string& name, const std::string& value) {
    // Set field 0 (Name)
    std::vector<unsigned char> nameMsg = {0x06, 0x35, 0x00};  // cmd=06, target=35, field=0
    for (char c : name) nameMsg.push_back(static_cast<unsigned char>(c));
    sendDisplaySysEx(midiOut, nameMsg);
    
    // Set field 1 (Value)
    std::vector<unsigned char> valueMsg = {0x06, 0x35, 0x01};  // cmd=06, target=35, field=1
    for (char c : value) valueMsg.push_back(static_cast<unsigned char>(c));
    sendDisplaySysEx(midiOut, valueMsg);
}

// Trigger/show the display
void showDisplay(ofxMidiOut& midiOut, unsigned char target) {
    sendDisplaySysEx(midiOut, {0x04, target, 0x7F});  // config=7F triggers display
}

// Cancel/hide display
void hideDisplay(ofxMidiOut& midiOut, unsigned char target) {
    sendDisplaySysEx(midiOut, {0x04, target, 0x00});  // config=00 cancels display
}

// Example: Show "Agency Mode" on stationary display
void showAgencyMode(ofxMidiOut& midiOut) {
    configureStationaryDisplay(midiOut);
    setStationaryText(midiOut, "Mode", "Agency");
    showDisplay(midiOut, 0x35);
}

// Set encoder parameter name (shown when adjusting encoder)
void setEncoderName(ofxMidiOut& midiOut, int encoderNum, const std::string& name) {
    if (encoderNum < 1 || encoderNum > 24) return;
    unsigned char target = 0x0C + encoderNum;  // encoder 1 = target 0x0D (13)
    
    // Configure with arrangement 4 (Name + numeric value)
    sendDisplaySysEx(midiOut, {0x04, target, 0x04});
    
    // Set the name
    std::vector<unsigned char> msg = {0x06, target, 0x00};
    for (char c : name) msg.push_back(static_cast<unsigned char>(c));
    sendDisplaySysEx(midiOut, msg);
}
```

### Display Control Notes

1. **Stationary vs Temporary**: The stationary display is always visible. Temporary displays overlay it briefly (duration set by user preference on device).

2. **Auto-display on control change**: By default, adjusting a control shows its temp display. Use config flags to disable this.

3. **Text length**: Keep text reasonably short for the small display. Test to find practical limits.

4. **Bitmap memory**: Only one bitmap can be held in memory at a time. Wait for acknowledgment before sending the next frame.

5. **Cancel before reconfigure**: When changing display arrangements, cancel the display first (config=0x00), then reconfigure.

## References

- [Novation Launch Control XL 3 Programmer's Reference](https://userguides.novationmusic.com/hc/en-gb/sections/27840433446546-Launch-Control-XL-3-programmer-s-reference-guide)
- [ofxMidi GitHub Repository](https://github.com/danomatika/ofxMidi)

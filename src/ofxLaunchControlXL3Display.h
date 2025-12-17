#pragma once

#include "ofxMidi.h"

#include <string>
#include <vector>

/**
 * OLED display control for the Novation Launch Control XL 3.
 *
 * The XL3 has a 128x64 monochrome OLED display that can be controlled
 * via SysEx messages in DAW mode.
 *
 * Reference: AGENTS-how-to-control-launchcontrolxl3-leds.md
 */
class ofxLaunchControlXL3Display {
public:
  // Display targets
  static constexpr uint8_t kTargetStationary = 0x35;  // Permanent display
  static constexpr uint8_t kTargetTemporary = 0x36;   // Overlay display

  // Display arrangements
  static constexpr uint8_t kArrangementCancel = 0x00;
  static constexpr uint8_t kArrangement2Line = 0x01;   // Name + Value
  static constexpr uint8_t kArrangement3Line = 0x02;   // Title + Name + Value
  static constexpr uint8_t kArrangementTrigger = 0x7F;

  /**
   * Setup the display controller with a shared MIDI output.
   * @param midiOutPtr Pointer to an already-open MIDI output (shared with LED controller)
   */
  void setup(ofxMidiOut* midiOutPtr);

  /**
   * Set the stationary (permanent) display content.
   * Uses 2-line arrangement: line1 on top, line2 on bottom.
   */
  void setStationary(const std::string& line1, const std::string& line2);

  /**
   * Show a temporary overlay display.
   * Uses 2-line arrangement: name on top, value on bottom.
   * Auto-dismisses after device timeout setting.
   */
  void showTemporary(const std::string& name, const std::string& value);

  /**
   * Clear/cancel a display.
   */
  void clearStationary();
  void clearTemporary();

private:
  void configure(uint8_t target, uint8_t config);
  void setText(uint8_t target, uint8_t field, const std::string& text);
  void sendSysEx(const std::vector<unsigned char>& data);

  ofxMidiOut* midiOut = nullptr;
};

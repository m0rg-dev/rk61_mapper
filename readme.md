# rk61_mapper

Changes the keymap of my RK61 (or any USB keyboard, actually) using a Teensy 4.0 microcontroller set up for USB host mode.

```
$ arduino-cli --additional-urls https://www.pjrc.com/teensy/td_156/package_teensy_index.json core update-index
$ arduino-cli --additional-urls https://www.pjrc.com/teensy/td_156/package_teensy_index.json core install teensy:avr
$ arduino-cli compile -b teensy:avr:teensy40:usb=keyboard
```


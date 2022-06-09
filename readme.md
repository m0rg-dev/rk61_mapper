```
$ arduino-cli --additional-urls https://www.pjrc.com/teensy/td_156/package_teensy_index.json core update-index
$ arduino-cli --additional-urls https://www.pjrc.com/teensy/td_156/package_teensy_index.json core install teensy:avr
$ arduino-cli compile -b teensy:avr:teensy40:usb=keyboard
```


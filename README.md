# NMEA Parser
## Description
In the current configuration, ~~only NAVSTAR/GPS NMEA~~ NAVSTAR/GPS, Gallileo, GLONASS, and BeiDou NMEA sentences are accepted.

Supported NMEA sentences:
  - `GPGGA`
  - `GPGSV`
  - `GPGSA`
  - `GPRMC`

Tested functions:
  - `GPGGA`[^1]
## Todo:
- [x] Add `GPRMC` sentence parser function
- [x] Add interal DD to DMS converter on directive/macro
- [ ] Add directive/macro(s) for accepted NMEA sentences
- [ ] Add option to disable `GPGSV` sentence
- [x] Add date option
- [ ] In `parse()` decide whether to `std::cout` the error or to remove it
- [x] Fix issue with not being able to run functions or access variables
- [ ] Analyze NMEA data, from start up to functioning
- [x] Test `verify_checksum()` function
- [x] Continue `verify_checksum()` tests, and add to `parse()` function as an `&&` condition
- [x] Change vectors to char array arrays[^2]
- [ ] Add `__attribute((always_inline))` to `NMEA::init_enums`?
- [x] If vectors are kept, change vector inputs in parser functions to `const std::vector<std::string>`
- [ ] Add more documentation/comments

[^1]: Static test
[^2]: In SNMEA.h

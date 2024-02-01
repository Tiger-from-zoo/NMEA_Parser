# NMEA Parser
## Description
In the current configuration, only NAVSTAR/GPS NMEA sentences are accepted.

Supported NMEA sentences:
  - `GPGGA`
  - `GPGSV`
  - `GPGSA`
  - `GPRMC`

Tested functions:
  - `GPGGA`[^1]
## Todo:
- [x]  Add `GPRMC` sentence parser function
- [x]  Add interal DD to DMS converter on directive
- [ ]  Add directives for accepted NMEA sentences
- [ ]  Add option to disable `GPGSV` sentence
- [x]  Add date option
- [ ]  In `parse()` decide whether to `std::cout` the error or to remove it
- [x]  Fix issue with not being able to run functions or access variables
- [ ]  Analyze NMEA data, from start up to functioning
- [x]  Test `verify_checksum()` function
- [x]  Continue `verify_checksum()` tests, and add to `parse()` function as an `&&` condition

[^1]: Static test

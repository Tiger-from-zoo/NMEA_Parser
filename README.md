# NMEA Parser
## Description
In the current configuration, only NAVSTAR/GPS NMEA sentences are accepted.

Supported NMEA sentences:
  - `GPGGA`
  - `GPGSV`
  - `GPGSA`
  - `GPRMC`
## Todo:
- [x]  Add `GPRMC` sentence parser function
- [x]  Add interal DD to DMS converter on directive
- [ ]  Add directives for accepted NMEA sentences
- [ ]  Add option to disable `GPGSV` sentence
- [ ]  Add date option
- [ ]  In `parse()` decide whether to `std::cout` the error or to remove it
- [ ]  Fix issue with not being able to run functions or access variables
- [ ]  Analyze NMEA data, from start up to functioning
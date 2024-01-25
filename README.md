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
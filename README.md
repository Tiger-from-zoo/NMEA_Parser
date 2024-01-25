# NMEA Parser
## Description
In the current configuration, only NAVSTAR/GPS NMEA sentences are accepted.

Supported NMEA sentences:
  - `GPGGA`
  - `GPGSV`
  - `GPGSA`
  - `GPRMC`[^1]
## Todo:
- [ ]  Add `GPRMC` sentence parser function
- [ ]  Add interal DD to DMS converter on directive
- [ ]  Add directives for accepted NMEA sentences
- [ ]  Add option to disable `GPGSV` sentence
- [ ]  Add date option

[^1]: Not implemented as of 01/24/2024

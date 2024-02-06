#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>

// Check if GNSS system ID is enabled with NMEAVERSION 4.11
// Table of GNSS System and Signal ID'S: (https://docs.novatel.com/OEM7/Content/Logs/GPGRS.htm#System)
// NMEAVERSION command: (https://docs.novatel.com/OEM7/Content/Commands/NMEAVERSION.htm)

// https://cplusplus.com/reference/string/string/append/
// https://en.wikipedia.org/wiki/List_of_GPS_satellites

class NMEA_Parser {
    public:
    NMEA_Parser () {
        init_enums();
    }

    // Latitude
    // DD, NOT DMS OR DMM
    double latitude = 0;

    // Longitude
    // DD, NOT DMS OR DMM
    double longitude = 0;

    /// All precision dilutions

    float hdop = 0;
    float vdop = 0;
    float pdop = 0;

    // Altitude
    // METERS
    float msl = 0;

    // Geoid Seperation / Undulation
    // METERS
    float gsep = 0;

    // Time since latest correction data update
    // SECONDS
    float adgd = 0;

    //Track made good / True heading
    // DEGREES
    float deg_t = 0;

    // Magnetic Variation
    // DEGREES
    float mag_var = 0;

    // Magnetic Variation Direction
    // STING (E OR W)
    char mag_var_dir = 'E';

    //Position Status
    // A = Valid
    // V = invalid
    char pos_stat = 'V';

    // Spped
    // KNOTS
    float speed = 0;

    /// Time

    // Hours
    unsigned short hh = 0;

    // Minutes
    unsigned short mm = 0;

    // Seconds
    unsigned short ss = 0;

    // Milliseconds
    unsigned short ms = 0;

    /// Date
    
    // Year 
    unsigned short year = 2024;

    // Month
    unsigned short month = 1;

    // Day
    unsigned short day = 1;

    /// Misc.

    // Fix
    // 0 = Fix not availible or valid
    // 1 = Single point
    // 2 = Pseudorange differential
    // 4 = RTK fixed ambiguity solution
    // 5 = RTK floating ambiguity solution
    // 6 = Dead reckoning mode (ESTIMATED POSITION)
    // 7 = Manually inputed position
    // 8 = Simulation
    // 9 = SBAS
    unsigned short fix = 0;

    // Satellites
    // COUNT
    unsigned short sat = 0;

    // Fix mode
    // 1 = Fix not availible 
    // 2 = 2D
    // 3 = 3D
    unsigned short mode = 0;

    // Satellite information vector
    // Each element is an array of 4 doubles and, in order, they mean: {PRN, ELEVATION, AZIMUTH, SNR}
    std::array<std::array<float, 4>, 32> sat_status;
    std::array<float, 4> sv_buffer {};

    /// (parse, split) Deliminators

    const char start_delim = '$';
    const char end_delim = '*';
    const char c_delim = ',';
    const char s_delim = ' ';

    /// (GSV) Indicies of satellite information in sv_buffer
    
    const unsigned short prn = 4;
    const unsigned short elev = 5;
    const unsigned short az = 6;
    const unsigned short snr = 7;

    // Takes in a pointer to a buffer string to parse NMEA sentences
    void parse(std::string* sentence_group) {
        std::size_t idx = 0;
        // std::string sentence_group_cpy = sentence_group;
        
        // Remove any spaces
        while ((idx = (*sentence_group).find(s_delim)) != std::string::npos) {
            (*sentence_group).erase((*sentence_group).begin()+idx);
        }
        
        // Get individual sentences
        std::size_t start_idx = 0;
        std::size_t end_idx;
        std::string NMEA_sentence;

        /// Loops through every valid NMEA sentence, parses it, hands off sentence in a vector to appropriate sub-parser function, and stores relevant information into variables
        while ((start_idx = (*sentence_group).find(start_delim)) != std::string::npos && (end_idx = (*sentence_group).find(end_delim)) != std::string::npos && verify_checksum(NMEA_sentence)) {
            NMEA_sentence = (*sentence_group).substr(start_idx, (end_idx-start_idx+3));
            switch (NMEA_sentence_map[NMEA_sentence.substr(0, 6)]) {
                case ev_GGA:
                    parse_GGA(split(NMEA_sentence));
                    break;
                case ev_GSV:
                    parse_GSV(split(NMEA_sentence));
                    break;
                case ev_RMC:
                    parse_GSV(split(NMEA_sentence));
                    break;
                case ev_GSA:
                    parse_GSV(split(NMEA_sentence));
                    break;
                default:
                    //Remove message?
                    // std::cout << "\"" << NMEA_sentence.substr(0, 6) << "\" " << "is not a verified GPS NMEA sentence or unsupported NMEA sentence type" << std::endl;
                    break;
            }
            std::cout << "DEBUG-BEFORE: " << (*sentence_group) << std::endl;
            (*sentence_group).erase(start_idx, (end_idx-start_idx+3));
            std::cout << "DEBUG-AFTER: " << (*sentence_group) << std::endl;
        }

        return;
    }

    private:
    enum StringValue {ev_GGA, ev_GSV, ev_RMC, ev_GSA};
    std::map<std::string, StringValue> NMEA_sentence_map;

    void init_enums() {
        NMEA_sentence_map["$GPGGA"] = ev_GGA;

        NMEA_sentence_map["$GPGSV"] = ev_GSV;
        NMEA_sentence_map["$GAGSV"] = ev_GSV;
        NMEA_sentence_map["$GLGSV"] = ev_GSV;
        NMEA_sentence_map["$BDGSV"] = ev_GSV;

        NMEA_sentence_map["$GPRMC"] = ev_RMC;
        NMEA_sentence_map["$GARMC"] = ev_RMC;
        NMEA_sentence_map["$GLRMC"] = ev_RMC;
        NMEA_sentence_map["$BDRMC"] = ev_RMC;

        NMEA_sentence_map["$GPGSA"] = ev_GSA;
        NMEA_sentence_map["$GAGSA"] = ev_GSA;
        NMEA_sentence_map["$GLGSA"] = ev_GSA;
        NMEA_sentence_map["$BDGSA"] = ev_GSA;

        return; 
    }

    // Sub-parser functions
    void parse_GGA(const std::vector<std::string> GGA) {
        // https://docs.novatel.com/OEM7/Content/Logs/GPGGA.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____59

        // Time
        hh = std::stoi(GGA[1].substr(0, 2));
        mm = std::stoi(GGA[1].substr(2, 2));
        ss = std::stoi(GGA[1].substr(4, 2));
        ms = std::stoi(GGA[1].substr(7, 3));

        // Latitude & Longitude
        if(GGA[2] != "") {
            double inter_lat, inter_lon;
            inter_lat = std::trunc((std::stod(GGA[2])/100)) + (std::stod(GGA[2].substr(2, 7))/60);
            if (GGA[3] == "S") {
                inter_lat *= -1;
            }
            inter_lon = std::trunc((std::stod(GGA[4])/100)) + (std::stod(GGA[4].substr(3, 7))/60);
            if (GGA[5] == "W") {
                inter_lon *= -1;
            }
            latitude = inter_lat;
            longitude = inter_lon;
        } else { return; }


        // Fix and satellites
        fix = std::stoi(GGA[6]);
        sat = std::stoi(GGA[7]);

        // Altitude, Geoid Seperation, and Age of Differential GPS Data
        if (GGA[9] != "") { msl = std::stof(GGA[9]); }
        if (GGA[11] != "") { gsep = std::stof(GGA[11]); }
        if (GGA[13] != "") { adgd = std::stof(GGA[13]); }

        return;
    }

    void parse_GSV(const std::vector<std::string> GSV) {
        // ^1 = https://docs.novatel.com/OEM7/Content/Logs/GPGSV.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____65

        // If a new set of GSV sentences arrives, the sentence index(Message number^1) will be "1," so it clears the old satellite information
        if (GSV[2] == "1") {
            memset(&sat_status, 0, 32);
        }
        //             {            }{           }{           }{           }
        //   0   1 2 3   4  5  6   7  8  9  10 11 12 13  14 15 16 17 18  19
        //$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
        
        // Amount of SV's
        // ({sentence parts}-{First four irrelevant sentence parts (sentence tpye, amount of GSV sentence, current GSV sentence, satellite count)})/{4; amount of SV information in each part}
        std::size_t svs = (GSV.size()-4)/4;

        for (int i = 0; i < svs; i++) {
            if (GSV[prn+(i*4)] != "") {
                if (GSV[snr+(i*4)] != "") {
                    sv_buffer = {std::stof(GSV[prn+(i*4)]), std::stof(GSV[elev+(i*4)]), std::stof(GSV[az+(i*4)]), std::stof(GSV[snr+(i*4)])};
                } else if (GSV[snr+(i*4)] == "") {
                    sv_buffer = {std::stof(GSV[prn+(i*4)]), std::stof(GSV[elev+(i*4)]), std::stof(GSV[az+(i*4)]), 0};
                } else {
                    sv_buffer = {std::stof(GSV[prn+(i*4)]), 0, 0, 0};
                }
                sat_status.push_back(sv_buffer);
            }
        }

        return;
    }

    void parse_RMC(std::vector<std::string> RMC) {
        // https://docs.novatel.com/OEM7/Content/Logs/GPRMC.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____69
        if (RMC[2] != "") { pos_stat = (RMC[2])[0]; }

        if(RMC[7] != "") { speed = std::stof(RMC[7]); }

        if(RMC[9] != "") { 
            day = std::stoi(RMC[9].substr(0, 2));
            month = std::stoi(RMC[9].substr(2, 2));
            year = 2000 + std::stoi(RMC[9].substr(4, 2));
        }

        if(RMC[8] != "") { deg_t = std::stof(RMC[8]); }
        if(RMC[10] != "") { mag_var = std::stof(RMC[10]); }
        if(RMC[11] != "") { mag_var_dir = (RMC[11])[0]; }
        
        return; 
    }

    void parse_GSA(std::vector<std::string> GSA) {
        // https://aprs.gids.nl/nmea/#gsa
        // https://docs.novatel.com/OEM7/Content/Logs/GPGSA.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____63
        if (GSA[2] == "") {
            mode = std::stoi(GSA[2]);
            if (GSA[15] != "") { pdop = std::stof(GSA[15]); }
            if (GSA[16] != "") { hdop = std::stof(GSA[16]); }
            if (GSA[17] != "") { vdop = std::stof(GSA[17]); }
            } else { mode = 0; pdop = 0; hdop = 0; vdop = 0; }

        return;
    }

    // Individual sentence part splitter
    std::vector<std::string> split(std::string sentence) {
        std::size_t idx = 0;
        // std::string delim = ",";
        std::string sentence_cpy = sentence;
        std::vector<std::string> rtn;

        // Seperate sentence parts
        while ((idx = sentence_cpy.find(c_delim)) != std::string::npos) {
            rtn.push_back(sentence_cpy.substr(0, idx));
            sentence_cpy.erase(0, idx + 1); // 1 is the length of the "," deliminator
        }
        rtn.push_back(sentence_cpy);

        return rtn;
    }

    bool verify_checksum(std::string snt) {
        std::string pure_nmea = snt.substr(1, snt.length()-4);
        unsigned short int_hex, i, checksum, given_checksum;

        checksum = 0;
        given_checksum = std::stoi(snt.substr(snt.length()-2, 2), nullptr, 16);

        std::stringstream nmea_bytes;

        for(i=0; i<pure_nmea.length(); i++) {
            int_hex = int(pure_nmea[i]);
            nmea_bytes.str("");

            nmea_bytes << std::hex << int_hex;

            checksum = checksum ^ std::stoi(nmea_bytes.str(), nullptr, 16);
        }

        if(checksum == given_checksum) {
            return true;
        }

        return false;
    }
    
};
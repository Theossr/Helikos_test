#pragma once

#include <vector>
#include <chrono>
#include <ctime>

// sgp4 external library
#include "SGP4.h"
#include "Tle.h"
#include "DateTime.h"
#include "CoordGeodetic.h"
// sgp4 external library

// Struct for describing satellite position
class Geo {
public:

    Geo(double lat_deg, double lon_deg, double alt_km);

    double getLat() const;

    double getLon() const;

    double getAlt() const;

private:

    double lat_deg;
    double lon_deg;
    double alt_km;
};


// counting the time of decay
class DecayCounting {
public:

    DecayCounting();

    DecayCounting(std::string time_utc, double time_from_start_s);

    DecayCounting& operator=(const DecayCounting& other);

    bool getCrossed() const;

    std::string getTimeUtc() const;

    double getTimeFromStart() const;

private:

    bool crossed;
    std::string time_utc;
    double time_from_start_s;
};


// time of decay. Physical drag counting doesn't exist, right now it's just a template
class Decay {
public:

    Decay();

    Decay(double threshold_km, DecayCounting tle_forward, DecayCounting physics_drag);

    Decay& operator=(const Decay& other);

    double getThreshold() const;

    DecayCounting getTleForward() const;

    DecayCounting getPhysicsDrag() const;

private:

    double threshold_km;
    DecayCounting tle_forward;
    DecayCounting physics_drag;
};


// struct for providing with simulation time point and satellite position
class OrbitNodes {
public:

    OrbitNodes(std::string& line1, std::string& line2, uint64_t start_epoch_ms, double duration_s, double step_s, double h_fail_km);

    std::pair<double, Geo> formSatPair(libsgp4::SGP4& sgp4, const libsgp4::DateTime& time_date, const libsgp4::DateTime& epoch, 
                                        double h_fail_km, double current, DecayCounting& tle_forward);

    std::vector<std::pair<double, Geo>> getNodes() const;

    Decay getDecay() const;

    std::string getTleEpoch() const;

    bool getOutOfEpoch() const;

private:

    std::vector<std::pair<double, Geo>> nodes;
    Decay dc;
    std::string tle_epoch_utc;
    bool out_of_tle_epoch_window;
};

// getting the actual date from UNIX epoch time format
libsgp4::DateTime fromUnixMs(uint64_t time_ms);


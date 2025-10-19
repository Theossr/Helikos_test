#pragma once

#include <vector>
#include <chrono>
#include <ctime>

#include "SGP4.h"
#include "Tle.h"
#include "DateTime.h"
#include "CoordGeodetic.h"

class Geo {
public:

    Geo(double lat_deg, double lon_deg, double alt_km);


private:
    double lat_deg;
    double lon_deg;
    double alt_km;
};

class DecayCounting {
public:

    DecayCounting();

    DecayCounting(std::string time_utc, double time_from_start_s);

    DecayCounting& operator=(const DecayCounting& other);

private:

    bool crossed;
    std::string time_utc;
    double time_from_start_s;
};


class Decay {
public:

    Decay();
    Decay(double threshold_km, DecayCounting tle_forward, DecayCounting physics_drag);

private:
    double threshold_km;
    DecayCounting tle_forward;
    DecayCounting physics_drag;
};


class OrbitNodes {
public:

    OrbitNodes(std::string& line1, std::string& line2, uint64_t start_epoch_ms, double duration_s, double step_s, double h_fail_km);

    std::pair<double, Geo> formSatPair(libsgp4::SGP4& sgp4, const libsgp4::DateTime& time_date, double h_fail_km, double current, 
                                        DecayCounting& tle_forward, DecayCounting& physics_drag);

    libsgp4::DateTime fromUnixMs(uint64_t time_ms);

private:
    std::vector<std::pair<double, Geo>> nodes;
    Decay dc;

};

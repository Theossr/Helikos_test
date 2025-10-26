#include <cmath>

#include "OrbitNodes.hpp"


Geo::Geo(double lat_deg, double lon_deg, double alt_km) {

        this->lat_deg = lat_deg;
        this->lon_deg = lon_deg;
        this->alt_km = alt_km;
}

double Geo::getLat() const{

    return this->lat_deg;

}

double Geo::getLon() const{
    
    return this->lon_deg;

}
    
double Geo::getAlt() const{
    
    return this->alt_km;

}

DecayCounting::DecayCounting() {

    this->crossed = false;
    this->time_utc = "";
    this->time_from_start_s = 0.0;
}

DecayCounting::DecayCounting(std::string time_utc, double time_from_start_s) {

    this->crossed = true;
    this->time_utc = time_utc;
    this->time_from_start_s = time_from_start_s;

}

DecayCounting& DecayCounting::operator=(const DecayCounting& other) {

    if (this != &other) {

        this->crossed = other.crossed;
        this->time_utc = other.time_utc;
        this->time_from_start_s = other.time_from_start_s;
    }
    return *this;
}

bool DecayCounting::getCrossed() const {

    return this->crossed;

}

std::string DecayCounting::getTimeUtc() const {

    return this->time_utc;

}

double DecayCounting::getTimeFromStart() const {

    return this->time_from_start_s;

}

Decay::Decay() {

    this->threshold_km = 0.0;
    this->tle_forward = DecayCounting();
    this->physics_drag = DecayCounting();
}

Decay::Decay(double threshold_km, DecayCounting tle_forward, DecayCounting physics_drag) {

    this->threshold_km = threshold_km;
    this->tle_forward = tle_forward;
    this->physics_drag = physics_drag;
}

Decay& Decay::operator=(const Decay& other) {

    if (this != &other) {

        this->threshold_km = other.threshold_km;
        this->tle_forward = other.tle_forward;
        this->physics_drag = other.physics_drag;
    }
    return *this;
}

double Decay::getThreshold() const {

    return this->threshold_km;

}

DecayCounting Decay::getTleForward() const {

    return this->tle_forward;

}

DecayCounting Decay::getPhysicsDrag() const {

    return this->physics_drag;

}

OrbitNodes::OrbitNodes(std::string& line1, std::string& line2, uint64_t start_epoch_ms, double duration_s, double step_s, double h_fail_km)  {

    libsgp4::Tle tle(line1, line2);
    libsgp4::SGP4 sgp4(tle);
    double current = 0.0;
    DecayCounting tle_forward;
    DecayCounting physics_drag;
    libsgp4::DateTime epoch = tle.Epoch();
    this->tle_epoch_utc = epoch.ToString();

    for (int i = 0; i <= static_cast<int>(duration_s / step_s); ++i) {
        auto satPair = formSatPair(sgp4, fromUnixMs(start_epoch_ms + i * 1000),
                                   epoch, h_fail_km, current + i * step_s,
                                   tle_forward);

        this->nodes.push_back(satPair);
    }
    this->dc = Decay(h_fail_km, tle_forward, physics_drag);
}

std::pair<double, Geo> OrbitNodes::formSatPair(libsgp4::SGP4& sgp4, const libsgp4::DateTime& time_date, const libsgp4::DateTime& epoch, 
                                                double h_fail_km, double current, DecayCounting& tle_forward) {

    libsgp4::Eci eci = sgp4.FindPosition(time_date);
    libsgp4::CoordGeodetic coGeo = eci.ToGeodetic();
    double alt_km = coGeo.altitude;
    if(alt_km < h_fail_km) {
        tle_forward = DecayCounting(time_date.ToString(), current);
    }
    if(epoch < time_date) {

        this->out_of_tle_epoch_window = false;
    }
    Geo geoCurrent(coGeo.latitude * 180.0 / M_PI, coGeo.longitude * 180 / M_PI, alt_km);
    return std::pair<double, Geo>(current, geoCurrent);
}

libsgp4::DateTime fromUnixMs(uint64_t time_ms) {

    std::time_t seconds = static_cast<time_t>(time_ms / 1000);
    double fractSec = (time_ms % 1000) / 1000.0;
    std::tm* utcTm = std::gmtime(&seconds);

    return libsgp4::DateTime (
        utcTm->tm_year + 1900,
        utcTm->tm_mon + 1,
        utcTm->tm_mday,
        utcTm->tm_hour,
        utcTm->tm_min,
        utcTm->tm_sec + fractSec
    );
}

std::vector<std::pair<double, Geo>> OrbitNodes::getNodes() const {

    return this->nodes;

}

Decay OrbitNodes::getDecay() const {

    return this->dc;

}

std::string OrbitNodes::getTleEpoch() const {

    return this->tle_epoch_utc;

}

bool OrbitNodes::getOutOfEpoch() const {

    return this->out_of_tle_epoch_window;

}
#include "EarthCore.hpp"
#include <cmath>
#include <iostream>

namespace earthcore {

double getDecimalYear() {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t now_c = system_clock::to_time_t(now);
    std::tm tm_now = *std::gmtime(&now_c);

    int year = tm_now.tm_year + 1900;
    int day_of_year = tm_now.tm_yday + 1;
    bool leap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    int days_in_year = leap ? 366 : 365;

    return year + static_cast<double>(day_of_year) / days_in_year;
}

MagneticField computeMagneticField(double lat_deg, double lon_deg, double alt_km) {
    double year = getDecimalYear();

    geomag::Vector posECEF = geomag::geodetic2ecef(lat_deg, lon_deg, alt_km * 1000.0);
    geomag::Vector B = GeoMag(year, posECEF, geomag::WMM2025);

    MagneticField field;
    field.x_nT = B.x * 1e9;
    field.y_nT = B.y * 1e9;
    field.z_nT = B.z * 1e9;
    field.f_nT = std::sqrt(B.x * B.x + B.y * B.y + B.z * B.z) * 1e9;

    return field;
}

} // namespace earthcore
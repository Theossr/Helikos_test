#pragma once
#include "XYZgeomag.hpp"
#include <chrono>
#include <string>

// namespace earthcore
namespace earthcore {

    // Getting current year
    double getDecimalYear();

    // Struct for frame making
    struct MagneticField {

        double x_nT;
        double y_nT;
        double z_nT;
        double f_nT;
    };

    // Counting method
    MagneticField computeMagneticField(double lat_deg, double lon_deg, double alt_km);
} // namespace earthcore
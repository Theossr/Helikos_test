#pragma once
#include "XYZgeomag.hpp"
#include <chrono>
#include <string>

namespace earthcore {

    double getDecimalYear();

    struct MagneticField {
        double x_nT;
        double y_nT;
        double z_nT;
        double f_nT;
    };

    MagneticField computeMagneticField(double lat_deg, double lon_deg, double alt_km);
}
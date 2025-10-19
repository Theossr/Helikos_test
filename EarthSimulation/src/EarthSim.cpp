#include "EarthSim.hpp"

double getDecimalYear() {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t now_c = system_clock::to_time_t(now);
    std::tm tm_now = *std::gmtime(&now_c);

    int year = tm_now.tm_year + 1900;
    int day_of_year = tm_now.tm_yday + 1;
    bool leap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    int days_in_year = leap ? 366 : 365;

    double decimal_year = year + (static_cast<double>(day_of_year) / days_in_year);
    return decimal_year;
}

Status EarthSimulationServiceImpl::SendEarthData(ServerContext* context, const EarthRequest* request, 
                                                    EarthResponse* response) {

    std::cout << "Received " << request->nodes_size() << " nodes" << std::endl;

    double year = getDecimalYear();

    for (const auto& node : request->nodes()) {
        double lat = node.geo().lat_deg();
        double lon = node.geo().lon_deg();
        double alt = node.geo().alt_km();

        geomag::Vector posECEF = geomag::geodetic2ecef(lat, lon, alt * 1000.0);

        geomag::Vector B = GeoMag(year, posECEF, geomag::WMM2025);

        double F = std::sqrt(B.x * B.x + B.y * B.y + B.z * B.z);

        auto* frame = response->add_frames();
        frame->set_t_rel_s(node.t_rel_s());

        auto* geo = frame->mutable_geo();
        geo->set_lat_deg(lat);
        geo->set_lon_deg(lon);
        geo->set_alt_km(alt);

        auto* mag = frame->mutable_mag();
        mag->set_x_nt(B.x);
        mag->set_y_nt(B.y);
        mag->set_z_nt(B.z);
        mag->set_f_nt(F);
    }

    std::cout << "Computed " << response->frames_size() << " magnetic frames" << std::endl;

    return Status::OK;
}

void RunServer() {

    std::string server_address("0.0.0.0:50052");  // Порт, на который подключается первый C++ сервис
    EarthSimulationServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "EarthSimulation C++ server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {

    RunServer();
    return 0;

}
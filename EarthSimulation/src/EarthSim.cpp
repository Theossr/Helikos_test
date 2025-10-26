#include "EarthSim.hpp"
#include "EarthCore.hpp"

using namespace earthcore;

Status EarthSimulationServiceImpl::SendEarthData(ServerContext* context, const EarthRequest* request, 
                                                    EarthResponse* response) {

    std::cout << "Received " << request->nodes_size() << " nodes" << std::endl;

    for (const auto& node : request->nodes()) {
        double lat = node.geo().lat_deg();
        double lon = node.geo().lon_deg();
        double alt = node.geo().alt_km();

        auto field = computeMagneticField(lat, lon, alt);

        auto* frame = response->add_frames();
        frame->set_t_rel_s(node.t_rel_s());

        auto* geo = frame->mutable_geo();
        geo->set_lat_deg(lat);
        geo->set_lon_deg(lon);
        geo->set_alt_km(alt);

        auto* mag = frame->mutable_mag();
        mag->set_x_nt(field.x_nT);
        mag->set_y_nt(field.y_nT);
        mag->set_z_nt(field.z_nT);
        mag->set_f_nt(field.f_nT);
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
#include "SatSim.hpp"
#include "OrbitNodes.hpp"


Status SatelliteServiceImpl::RunSimulation(ServerContext* context, const SimRequest* request, SimResponse* response) {

    auto tle = request->tle();
    std::string line1 = tle.line1();
    std::string line2 = tle.line2();

    uint64_t start_epoch_ms = request->start_epoch_ms();
    double duration_s = request->duration_s();
    double step_s = request->step_s();
    double h_fail_km = request->h_fail_km();

    OrbitNodes currentTleNodes(line1, line2, start_epoch_ms, duration_s, step_s, h_fail_km);


    response->set_message("Simulation started successfully");
    response->set_status_code(200);

    return Status::OK;   
}

void RunServer() {

    std::string server_address("0.0.0.0:50051");
    SatelliteServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "C++ gRPC server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {

    RunServer();
    return 0;
}
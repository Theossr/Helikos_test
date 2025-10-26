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

    const auto& nodes = currentTleNodes.getNodes();
    
    EarthSimulationClient earthClient(grpc::CreateChannel("earth_sim:50052", grpc::InsecureChannelCredentials()));

    EarthResponse earthResp =  earthClient.SendSimulationData(nodes);

    for (int i = 0; i < earthResp.frames_size(); ++i) {

        const auto& frame_in = earthResp.frames(i);
        auto* frame_out = response->add_frames();

        frame_out->set_t_rel_s(frame_in.t_rel_s());

        auto* geo_out = frame_out->mutable_geo();
        geo_out->set_lat_deg(frame_in.geo().lat_deg());
        geo_out->set_lon_deg(frame_in.geo().lon_deg());
        geo_out->set_alt_km(frame_in.geo().alt_km());

        auto* mag_out = frame_out->mutable_mag();
        mag_out->set_x_nt(frame_in.mag().x_nt());
        mag_out->set_y_nt(frame_in.mag().y_nt());
        mag_out->set_z_nt(frame_in.mag().z_nt());
        mag_out->set_f_nt(frame_in.mag().f_nt());
    }

    auto* decay = response->mutable_decay();
    decay->set_threshold_km(currentTleNodes.getDecay().getThreshold());

    auto* tle_forward = decay->mutable_tle_forward();
    tle_forward->set_crossed(currentTleNodes.getDecay().getTleForward().getCrossed());
    tle_forward->set_time_utc(currentTleNodes.getDecay().getTleForward().getTimeUtc());
    tle_forward->set_time_from_start_s(currentTleNodes.getDecay().getTleForward().getTimeFromStart());

    auto* physics_drag = decay->mutable_physics_drag();
    physics_drag->set_crossed(currentTleNodes.getDecay().getPhysicsDrag().getCrossed());
    physics_drag->set_time_utc(currentTleNodes.getDecay().getPhysicsDrag().getTimeUtc());
    physics_drag->set_time_from_start_s(currentTleNodes.getDecay().getPhysicsDrag().getTimeFromStart());

    auto* meta = response->mutable_meta();
    meta->set_points_generated(nodes.size());
    meta->set_tle_epoch_utc(currentTleNodes.getTleEpoch());
    meta->set_out_of_tle_epoch_window(currentTleNodes.getOutOfEpoch());

    std::cout << "Computed " << response->frames_size() << " points" << std::endl;

    return Status::OK;   
}

EarthSimulationClient::EarthSimulationClient(std::shared_ptr<Channel> channel)
        : stub_(EarthSimulationService::NewStub(channel)) {}

EarthResponse EarthSimulationClient::SendSimulationData(const std::vector<std::pair<double, Geo>>& nodes) {
    EarthRequest request;

    for (const auto& [t_rel_s, geo] : nodes) {
        auto* node = request.add_nodes();
        node->set_t_rel_s(t_rel_s);

        auto* geo_pb = node->mutable_geo();
        geo_pb->set_lat_deg(geo.getLat());
        geo_pb->set_lon_deg(geo.getLon());
        geo_pb->set_alt_km(geo.getAlt());
    }

    EarthResponse response;
    ClientContext context;

    Status status = stub_->SendEarthData(&context, request, &response);

    if (!status.ok()) {

        std::cerr << "EarthSimulation RPC failed: " << status.error_message() << std::endl;
    } 
    return response;
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
#pragma once

#include "OrbitNodes.hpp"

#include <grpcpp/grpcpp.h>
#include "satellite_Earth_simulation.grpc.pb.h"
#include "satellite_Earth_simulation.pb.h"
#include "Earth_simulation.grpc.pb.h"
#include "Earth_simulation.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using SatelliteEarthSimulation::SatelliteEarthSimulationService;
using SatelliteEarthSimulation::SimRequest;
using SatelliteEarthSimulation::SimResponse;

using EarthSimulation::EarthSimulationService;
using EarthSimulation::EarthRequest;
using EarthSimulation::EarthResponse;

// Getting and parsing the protobuf data from REST API service 
class SatelliteServiceImpl final : public SatelliteEarthSimulationService::Service {
public:

    Status RunSimulation(ServerContext* context, const SimRequest* request, SimResponse* response) override;

};


// Sending the data to earth sim with request
class EarthSimulationClient {
public:

    explicit EarthSimulationClient(std::shared_ptr<Channel> channel);

    EarthResponse SendSimulationData(const std::vector<std::pair<double, Geo>>& nodes);

private:
    std::unique_ptr<EarthSimulationService::Stub> stub_;
};

// Running the satellite simulation server
void RunServer();





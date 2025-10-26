#pragma once

#include <iostream>
#include <cmath>
#include <memory>
#include <chrono>
#include <ctime>
#include <grpcpp/grpcpp.h>

#include "Earth_simulation.pb.h"
#include "Earth_simulation.grpc.pb.h"
#include "XYZgeomag.hpp"
#include "EarthCore.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using EarthSimulation::EarthSimulationService;
using EarthSimulation::EarthRequest;
using EarthSimulation::EarthResponse;

class EarthSimulationServiceImpl final : public EarthSimulationService::Service {
public:

    Status SendEarthData(ServerContext* context, const EarthRequest* request, EarthResponse* response) override;

};

void RunServer();
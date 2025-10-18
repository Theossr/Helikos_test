#include <quicktle/dataset.h>
#include <quicktle/func.h>
#include <quicktle/node.h>
#include <quicktle/stream.h>

#include <grpcpp/grpcpp.h>
#include "satellite_Earth_simulation.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using SatelliteEarthSimulation::SatelliteEarthSimulationService;
using SatelliteEarthSimulation::SimRequest;
using SatelliteEarthSimulation::SimResponse;

class SatelliteServiceImpl final : public SatelliteEarthSimulationService::Service {
public:

    Status RunSimulation(ServerContext* context, const SimRequest* request, SimResponse* response) override;

};





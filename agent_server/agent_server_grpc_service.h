#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_GRPC_INTERFAFE_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_GRPC_INTERFAFE_H_

#include "agent_server.h"
#include "proto/agent_server.grpc.pb.h"

namespace agent_server {

namespace service {

class AgentServerGrpcService final : public AgentServer::Service {
 public:
  ::grpc::Status CreateEnvironment(
      ::grpc::ServerContext* context, const CreateEnvironmentRequest* request,
      CreateEnvironmentResponse* response) override;
  ::grpc::Status DeleteEnvironment(
      ::grpc::ServerContext* context, const DeleteEnvironmentRequest* request,
      DeleteEnvironmentResponse* response) override;

  ::grpc::Status CreateAgent(::grpc::ServerContext* context,
                             const CreateAgentRequest* request,
                             CreateAgentResponse* response) override;
  ::grpc::Status DeleteAgent(::grpc::ServerContext* context,
                             const DeleteAgentRequest* request,
                             DeleteAgentResponse* response) override;

  ::grpc::Status GetEnvironment(::grpc::ServerContext* context,
                                const GetEnvironmentRequest* request,
                                GetEnvironmentResponse* response) override;
  ::grpc::Status SimulateToTime(::grpc::ServerContext* context,
                                const SimulateToTimeRequest* request,
                                SimulateToTimeResponse* response) override;
  ::grpc::Status AgentAddCrop(::grpc::ServerContext* context,
                              const AgentAddCropRequest* request,
                              AgentAddCropResponse* response) override;
  ::grpc::Status AgentRemoveCrop(::grpc::ServerContext* context,
                                 const AgentRemoveCropRequest* request,
                                 AgentRemoveCropResponse* response) override;

 private:
  agent_server::AgentServer agent_server_;

  // TODO: may need a lock
  // assume no need for multi-client for now
};

}  // namespace service

}  // namespace agent_server

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_GRPC_INTERFAFE_H_
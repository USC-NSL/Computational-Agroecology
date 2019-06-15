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
  ::grpc::Status CreateAgent(::grpc::ServerContext* context,
                             const CreateAgentRequest* request,
                             CreateAgentResponse* response) override;
  ::grpc::Status GetEnvironment(::grpc::ServerContext* context,
                                const GetEnvironmentRequest* request,
                                GetEnvironmentResponse* response) override;

 private:
  agent_server::AgentServer agent_server_;

  // TODO: may need a lock
  // assume no need for multi-client for now
};

}  // namespace service

}  // namespace agent_server

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_SERVER_AGENT_SERVER_GRPC_INTERFAFE_H_
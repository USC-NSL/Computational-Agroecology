#include "agent_server_grpc_service.h"

#include <chrono>
#include <iostream>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "environment/config.h"
#include "environment/terrain.h"
#include "message_convertor.h"

namespace {

std::string kDefaultHostAndPort = "0.0.0.0:50000";

}

namespace agent_server {

namespace service {

::grpc::Status AgentServerGrpcService::CreateEnvironment(
    ::grpc::ServerContext* context, const CreateEnvironmentRequest* request,
    CreateEnvironmentResponse* response) {
  if (context == nullptr || request == nullptr) {
    return ::grpc::Status(
        ::grpc::FAILED_PRECONDITION,
        "`ServerContext` or `CreateEnvironmentRequest` is nullptr.");
  }

  environment::Config config = FromProtobuf(request->config());
  std::chrono::system_clock::time_point tp =
      FromProtobuf(request->timestamp_epoch_count());
  environment::Terrain terrain = FromProtobuf(request->terrain());

  auto ret =
      agent_server_.CreateEnvironment(request->name(), config, tp, terrain);

  if (ret == ::agent_server::AgentServer::ALREADY_EXISTS) {
    return grpc::Status(grpc::ALREADY_EXISTS,
                        request->name() + " has been used.");
  }

  return ::grpc::Status::OK;
}

::grpc::Status AgentServerGrpcService::CreateAgent(
    ::grpc::ServerContext* context, const CreateAgentRequest* request,
    CreateAgentResponse* response) {
  if (context == nullptr || request == nullptr) {
    return ::grpc::Status(
        ::grpc::FAILED_PRECONDITION,
        "`ServerContext` or `CreateAgentRequest` is nullptr.");
  }

  auto ret = agent_server_.CreateAgent(request->agent_name(),
                                       request->environment_name());

  if (ret == ::agent_server::AgentServer::NOT_FOUND) {
    return ::grpc::Status(::grpc::NOT_FOUND,
                          request->environment_name() + " is not found.");
  } else if (ret == ::agent_server::AgentServer::ALREADY_EXISTS) {
    return ::grpc::Status(::grpc::ALREADY_EXISTS,
                          request->environment_name() + " has been used.");
  }

  return ::grpc::Status::OK;
}

::grpc::Status AgentServerGrpcService::GetEnvironment(
    ::grpc::ServerContext* context, const GetEnvironmentRequest* request,
    GetEnvironmentResponse* response) {
  if (context == nullptr || request == nullptr || response == nullptr) {
    return ::grpc::Status(::grpc::FAILED_PRECONDITION,
                          "`ServerContext`, `GetEnvironmentRequest`, or "
                          "`GetEnvironmentResponse` is nullptr.");
  }

  auto result = agent_server_.GetEnvironment(request->name());

  if (result.first == ::agent_server::AgentServer::NOT_FOUND) {
    return ::grpc::Status(::grpc::NOT_FOUND,
                          request->name() + " is not found.");
  }

  *(response->mutable_environment()) = ToProtobuf(*(result.second));
  return ::grpc::Status::OK;
}

}  // namespace service

}  // namespace agent_server

int main(int argc, char** argv) {
  using namespace agent_server::service;
  AgentServerGrpcService service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(kDefaultHostAndPort,
                           grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server is listening on " << kDefaultHostAndPort << std::endl;
  server->Wait();

  return 0;
}
/*!==========================================================================
 * \file
 *   Program:       auth-grpc
 *   File:          main.cpp
 *   Created:       02/11/2022
 *   Author:        Vitaly Bulganin
 *   Description:
 *   Comments:
 *
-----------------------------------------------------------------------------
 *
 * History:
 *
===========================================================================*/
#include <iostream>
//-------------------------------------------------------------------------//
#include <grpcpp/grpcpp.h>
//-------------------------------------------------------------------------//
#include "common/arg-list.h"
//-------------------------------------------------------------------------//
#include "stubs/agentcontrol.grpc.pb.h"
//-------------------------------------------------------------------------//
namespace {
    using request_t = Avanpost::Sso::Agent::Grpc::AuthenticationRequest;
    using response_t = Avanpost::Sso::Agent::Grpc::AuthenticationResponse;
    // Prints help command line.
    auto usage() -> void {
        std::cout << "Usage: auth-grpc -uri <URI> -login <username> -password <password>" << std::endl
                  << "\t-uri <URI> - uri address of server." << std::endl
                  << "\t-login <username> - user name or login name to connect to server." << std::endl
                  << "\t-password <password> - user password to connect to server." << std::endl;
    }
    // Validates parameters.
    auto validate(const auth::common::arglist & args) -> void {
        if (not args.contains("-uri")) {
            std::cerr << "No parameter [-uri] found" << std::endl;
        }
        if (not args.contains("-login")) {
            std::cerr << "No parameter [-login] found" << std::endl;
        }
        if (not args.contains("-password")) {
            std::cerr << "No parameter [-password] found" << std::endl;
        }
    }
    template<typename Object>
    auto obj2str(const Object & object) -> std::string {
        static std::mutex s_lock;
        std::string output;
        google::protobuf::util::JsonOptions options;
        {
            options.add_whitespace = false;
        }
        {
            std::unique_lock<std::mutex> sync(s_lock);
            // Converting a message to json.
            google::protobuf::util::MessageToJsonString(object, &output, options);
        }
        return output;
    }
    // Builds a client request.
    auto buildRequest(auth::common::arglist & args) -> request_t {
        request_t request;
        request.set_applicationid("CredentialProvider");
        request.set_factor(Avanpost::Sso::Agent::Grpc::PASSWORD);
        request.set_username(args["-login"]);
        request.set_passcode(args["-password"]);
        return std::move(request);
    }
}; // namespace
//-------------------------------------------------------------------------//
auto main(int argc, char *argv[]) -> int {
    auth::common::arglist args(argc, argv);
    if (args.contains("-h") || args.contains("--help")) {
        return usage(), 0;
    }
    try {
        response_t response;
        grpc::ClientContext context;
        // Validating input parameters.
        validate(args);
        // Building a channel (insecure).
        auto channel = grpc::CreateChannel(args["-uri"], grpc::InsecureChannelCredentials());
        // Creating a client stub.
        auto stub = Avanpost::Sso::Agent::Grpc::AgentControl::NewStub(channel);
        // Sending a client request to server (sync mode).
        auto status = stub->Authenticate(&context, buildRequest(args), &response);
        if (not status.ok()) {
            throw (std::invalid_argument("Authenticate request failed: " + status.error_message()));
        }
        std::cout << "Authenticate request successful: " << obj2str(response) << std::endl;
    } catch (const std::exception & exc) {
        std::cerr << "[ERROR] " << exc.what() << std::endl;
        return 1;
    }
    return 0;
}

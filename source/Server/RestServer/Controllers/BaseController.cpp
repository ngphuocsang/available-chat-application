#include "BaseController.h"
#include "MyLibrary.h"
#include "UserInterface.h"

using namespace MyLibrary;

namespace Server::RestServer::Controllers
{
    BaseController::BaseController() {}
    BaseController::~BaseController() {}
    void BaseController::setEndpoint(const string &value)
    {
        uri endpointURI(value);
        uri_builder endpointBuilder;

        endpointBuilder.set_scheme(endpointURI.scheme());
        endpointBuilder.set_host(endpointURI.host());
        endpointBuilder.set_port(endpointURI.port());
        endpointBuilder.set_path(endpointURI.path());

        _listener = http_listener(endpointBuilder.to_uri());
    }
    pplx::task<void> BaseController::accept()
    {
        initRestOpHandlers();
        return _listener.open();
    }
    pplx::task<void> BaseController::shutdown()
    {
        return _listener.close();
    }
    vector<utility::string_t> BaseController::requestPath(const http_request &message)
    {
        auto relativePath = uri::decode(message.relative_uri().path());
        return uri::split_path(relativePath);
    }

    int BaseController::initializeController(string endpoint)
    {
        UserInterface::printHorizontalSeparatedLine();
        INFO_LOG("Modern C++ Rest API server now initializing for requests...\n");

        this->setEndpoint(endpoint);

        try
        {
            this->accept().wait();
            SUCCESS_LOG("Modern C++ Rest API server now listening for requests at: ");
            cout << this->_listener.uri().to_string() << '\n';
            UserInterface::printHorizontalSeparatedLine();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG(e.what());
            cout << endl;
            return RESULT_CODE::REST_SERV_INIT_ERR;
        }

        return RESULT_CODE::RESULT_SUCCESS;
    }
} // namespace Server::RestServer::Controllers

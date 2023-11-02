// handler.h
#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include "controller.h"
#include "infer.h"
#include "oatpp/web/server/HttpRequestHandler.hpp"
#include <string>
#define O_UNUSED(x) (void)x;

// 自定义请求处理程序
class Handler : public oatpp::web::server::HttpRequestHandler
{
public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        O_UNUSED(request);
        std::cout<<"12121aaaaaaaaaa";
        auto source = request->getPathVariable("param");
        // // int myInt = std::stoi(myString->c_str());
        // // std::cout<<myInt;
        std::string stdSource = source; // 显式转换为 std::string
        std::cout << "canshu: " << stdSource << std::endl;
        Start(stdSource);
        return ResponseFactory::createResponse(Status::CODE_200, "Hello, Worldppp!");
    }
};

// class Handler1 : public oatpp::web::server::HttpRequestHandler
// {
// public:
//     // 处理传入的请求，并返回响应
//     std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
//         O_UNUSED(request);
//         std::string str = ControHello();
//         return ResponseFactory::createResponse(Status::CODE_200, "Hello, World1111111111111111!"+str);
//     }
// };

class Handler2 : public oatpp::web::server::HttpRequestHandler
{
public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        O_UNUSED(request);
        ControHello();
        return ResponseFactory::createResponse(Status::CODE_200, "222");
    }
};

class Handler3 : public oatpp::web::server::HttpRequestHandler
{
public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        O_UNUSED(request);
        FreeHello();
        return ResponseFactory::createResponse(Status::CODE_200, "222");
    }
};


#endif // HANDLER_H

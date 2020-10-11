#pragma once
#include <string>
#include <exception>
namespace idos{
    class IDOSException : public std::exception{
    private:
        std::string msg;
    public:
        IDOSException(const std::string &what)
        :msg(what){

        }
        const char* what() const noexcept
        {
            return msg.c_str();
        }
    };
}
#pragma once

#include <string>
#include <memory>
#include <cxxabi.h>

std::string demangle(
        const char* name)
{
    int status = -4;
    std::unique_ptr<char, void (*)(
        void*)> res{
        abi::__cxa_demangle(name, NULL, NULL, &status), std::free};
    return (status == 0) ? res.get() : name;
}

template <class T> std::string type(
        const T& t)
{

    return demangle(typeid(t).name());
}
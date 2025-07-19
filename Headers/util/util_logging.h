#pragma once
#include <iostream>


// contains macros for logging functions.
// standardised logging to quickly pinpoint places to debug.


#if defined(__clang__) || defined(__GNUC__)
#define FUNC_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC_SIG __FUNCSIG__
#else
#define FUNC_SIG __func__  // fallback
#endif


#define LOG_INFO(message)  std::cout  << "[INFO] -    <" << FUNC_SIG << ">: " << message << '\n'
#define LOG_WARN(message)  std::cout  << "[WARNING] - <" << FUNC_SIG << ">: " << message << '\n'
#define LOG_ERROR(message) std::cout  << "[ERROR] -   <" << FUNC_SIG << ">: " << message << '\n'








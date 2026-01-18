#pragma once
#include <iostream>
#include <glm/glm.hpp>

// contains macros for logging functions.
// standardised logging to quickly pinpoint places to debug.


#if defined(__clang__) || defined(__GNUC__)
#define FUNC_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC_SIG __FUNCSIG__
#else
#define FUNC_SIG __func__  // fallback
#endif

#if defined(__clang__) || defined(__GNUC__)
#define FUNC_NAME __func__   // just prints the bare function name
#elif defined(_MSC_VER)
#define FUNC_NAME __FUNCTION__  // MSVC equivalent
#else
#define FUNC_NAME __func__
#endif


#ifdef _DEBUG
	#define DEBUG_MODE 1
#else
	#define DEBUG_MODE 0
#endif

#define LOG_TEXT_RESET   "\033[0m"
#define LOG_TEXT_RED     "\033[31m"
#define LOG_TEXT_GREEN   "\033[32m"
#define LOG_TEXT_YELLOW  "\033[33m"
#define LOG_TEXT_CYAN    "\033[36m"
#define LOG_TEXT_WHITE   "\033[37m"

#define LOG_INFO(message) \
    if (DEBUG_MODE) std::cout << LOG_TEXT_WHITE "[INFO] -    <" << FUNC_NAME << ">: " << message << LOG_TEXT_RESET << '\n'

#define LOG_WARN(message) \
    if (DEBUG_MODE) std::cout << "[WARNING] - " << LOG_TEXT_YELLOW "<" << FUNC_NAME << ">: " << message << LOG_TEXT_RESET << '\n'

#define LOG_ERROR(message) \
    if (DEBUG_MODE) std::cout << LOG_TEXT_RED "[ERROR] -   <" << FUNC_NAME << ">: " << message << LOG_TEXT_RESET << '\n'

#define LOG_SUCCESS(message) \
    if (DEBUG_MODE) std::cout << LOG_TEXT_GREEN "[SUCCESS] - <" << FUNC_NAME << ">: " << message << LOG_TEXT_RESET << '\n'

#define LOG_DEBUG(message) \
    if (DEBUG_MODE) std::cout << "[DEBUG] -   " << LOG_TEXT_CYAN "<" << FUNC_NAME << ">: " << message << LOG_TEXT_RESET << '\n'



#define LOG_SPLITTER() std::cout << "// --------------------------------------------------------- //\n"


template <unsigned N, typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream & os, const glm::vec<N,T,Q> & v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}



template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream & os, const glm::qua<T, Q> & q) {
    os << "[" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << "]";
    return os;
}



template <unsigned R, unsigned C, typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream & os, const glm::mat<R, C, T, Q> & m) {
    for (int i = 0; i < R; ++i) {
        os << "| ";
        for (int j = 0; j < C; ++j) {
            os << m[j][i] << " ";
        }
        os << "|\n";
    }
    return os;
}




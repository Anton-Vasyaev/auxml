#pragma once

// std
#include <memory>
#include <sstream>
// 3rd party
#include <boost/exception/all.hpp>
// project
#include <auxml/data/return_status.h>

#define API_TRY     \
    try             \
    {               \
        try         \
        {           \


#define API_CATCH                                                                                                       \
        }                                                                                                               \
        catch(std::exception& exc)                                                                                      \
        {                                                                                                               \
            return_status status;                                                                                       \
            status.correct_status = 0;                                                                                  \
                                                                                                                        \
            auto exc_message = exc.what();                                                                              \
            auto msg_len = std::strlen(exc_message);                                                                    \
                                                                                                                        \
            auto message_ptr = std::make_unique<char[]>(msg_len + 1);                                                   \
            status.error_message = message_ptr.release();                                                               \
            status.error_message[msg_len] = '\0';                                                                       \
            std::strcpy(status.error_message, exc_message);                                                             \
            status.is_const_message = 0;                                                                                \
            return status;                                                                                              \
        }                                                                                                               \
        catch(boost::exception& exc)                                                                                    \
        {                                                                                                               \
            return_status status;                                                                                       \
            status.correct_status = 0;                                                                                  \
                                                                                                                        \
            std::stringstream ss;                                                                                       \
            ss << boost::diagnostic_information(exc);                                                                   \
            auto err_str= ss.str();                                                                                     \
                                                                                                                        \
            auto message_ptr = std::make_unique<char[]>(err_str.length() + 1);                                          \
            status.error_message = message_ptr.release();                                                               \
            std::copy_n(err_str.c_str(), err_str.length(), status.error_message);                                       \
            status.error_message[err_str.length()] = '\0';                                                              \
            status.is_const_message = 0;                                                                                \
            return status;                                                                                              \
        }                                                                                                               \
        catch(const char* exc_message)                                                                                  \
        {                                                                                                               \
            return_status status;                                                                                       \
            status.correct_status = 0;                                                                                  \
            status.error_message = (char*)exc_message;                                                                  \
            status.is_const_message = 1;                                                                                \
        }                                                                                                               \
        catch(...)                                                                                                      \
        {                                                                                                               \
            return_status status;                                                                                       \
            status.correct_status = 0;                                                                                  \
            status.error_message = (char*)"Unknown error message from API.";                                            \
            status.is_const_message = 1;                                                                                \
        }                                                                                                               \
                                                                                                                        \
        return_status status;                                                                                           \
        status.correct_status = 1;                                                                                      \
        status.error_message = nullptr;                                                                                 \
        status.is_const_message = 0;                                                                                    \
                                                                                                                        \
        return status;                                                                                                  \
    }                                                                                                                   \
    catch(...)                                                                                                          \
    {                                                                                                                   \
        return_status status;                                                                                           \
        status.correct_status = 0;                                                                                      \
        status.error_message = (char*)"Total fatal error. Cannot construct message of return_status for API.";          \
        status.is_const_message = 1;                                                                                    \
        return status;                                                                                                  \
    }                                                                                                                   \
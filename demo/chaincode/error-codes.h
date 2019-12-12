/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

typedef enum {
    EC_UNDEFINED,              // 0
    EC_SUCCESS,                // 1
    EC_ERROR,                  // 2
    EC_HIDDEN,                 // 3
    EC_BAD_FUNCTION_NAME,      // 4
    EC_INVALID_INPUT,          // 5
    EC_MEMORY_ERROR,           // 6
    EC_SHORT_RESPONSE_BUFFER,  // 7
    EC_BAD_PARAMETERS          // 8
} error_codes_e;

namespace ClockAuction
{
    class ErrorReport
    {
        private:
            error_codes_e ec_;
            std::string errorString_;
        public:
            ErrorReport();

            void set(error_codes_e ec, const std::string& errorString);
            void toStatusJsonString(std::string& jsonString);
            bool isSuccess();
    };
}

#define FAST_FAIL_CHECK(errorReport, code, b) {\
    if(b)\
    {\
        errorReport.set(code, std::string(__FILE__) + ":" + std::to_string(__LINE__));\
        return false;\
    }\
}

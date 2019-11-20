/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include "error_codes.h"

ClockAuction::Dispatcher::Dispatcher(const std::string& functionName, const std::vector<std::string>& functionParameters, const uint8_t* response, const uint32_t& max_response_len, const uint32_t* actual_response_len): functionName_(functionName), functionParameters_(functionParameters), response_(response), max_response_len_(max_response_len) 
{
    fMap_ = 
    {
        //{"createAuction", &spectrumAuction_.createAuction},
        {"getAuctionDetails", &spectrumAuction_.getAuctionDetails}
    };


    // Call auction function
    auto fIter = fMap.find(functionName_);
    if (fIter == m.end())
    {
        // No such function
        errorReport.set(EC_BAD_FUNCTION_NAME, "function not available");
    }
    else
    {
        (*fIter->second)(functionParameters[0], responseString_, errorReport_);
    }

    // Write response string
    if(responseString.length() > max_response_len_)
    {
        *actual_response_len_ = 0;
    }
    else 
    {
        *actual_response_len_ = responseString.length();
        memcpy(response_, responseString.c_str(), *actual_response_len_);
    }
}

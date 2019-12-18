/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"
#include <string>
#include "dispatcher.h"
#include "error-codes.h"
#include "spectrum-auction.h"

ClockAuction::Dispatcher::Dispatcher(const std::string& functionName,
        const std::vector<std::string>& functionParameters,
        uint8_t* response,
        const uint32_t max_response_len,
        uint32_t* actual_response_len,
        shim_ctx_ptr_t ctx)
    :
        functionName_(functionName),
        functionParameters_(functionParameters),
        response_(response),
        max_response_len_(max_response_len),
        actual_response_len_(actual_response_len),
        spectrumAuction_(ctx)
{
    std::map<std::string, spectrumAuctionFunctionP> fMap_;
    fMap_.emplace(std::make_pair("createAuction",       &ClockAuction::SpectrumAuction::createAuction));
    fMap_.emplace(std::make_pair("getAuctionDetails",   &ClockAuction::SpectrumAuction::getAuctionDetails));
    fMap_.emplace(std::make_pair("getAuctionStatus",   &ClockAuction::SpectrumAuction::getAuctionStatus));
    fMap_.emplace(std::make_pair("startNextRound",   &ClockAuction::SpectrumAuction::startNextRound));
    fMap_.emplace(std::make_pair("endRound",   &ClockAuction::SpectrumAuction::endRound));

    LOG_DEBUG("Try dispatch function %s with parameters %s", functionName_.c_str(), functionParameters[0].c_str());

    // Call auction function
    auto fIter = fMap_.find(functionName_);
    if (fIter == fMap_.end())
    {
        // No such function
        errorReport_.set(EC_BAD_FUNCTION_NAME, "Auction API not found");
    }
    else
    {
        LOG_DEBUG("Auction API found, call it");
        (spectrumAuction_.*(fIter->second))(functionParameters[0], responseString_, errorReport_);
        LOG_DEBUG("API response string: %s", responseString_.c_str());
    }

    // prepare response string
    if(responseString_.length() == 0 || !errorReport_.isSuccess())
    {
        // an error occurred: fill the response with the error/status message
        errorReport_.toStatusJsonString(responseString_);
        LOG_DEBUG("Error response string set: %s", responseString_.c_str());
    }

    if(responseString_.length() > max_response_len_)
    {
        LOG_ERROR("Response string too long to be output");
        errorReport_.set(EC_SHORT_RESPONSE_BUFFER, "Response string too long to be output");
        errorReport_.toStatusJsonString(responseString_);
    }

    // write response string (if possible)
    *actual_response_len_ = (responseString_.length() > max_response_len_ ? 0 : responseString_.length());
    memcpy(response_, responseString_.c_str(), *actual_response_len_);
    LOG_DEBUG("Response written (length %u)", *actual_response_len_);
}


/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "shim.h"
#include "logging.h"
#include <string>
#include "base64/base64.h"
#include "json/parson.h"
#include "dispatcher.h"
int init(
    uint8_t* response,
    uint32_t max_response_len,
    uint32_t* actual_response_len,
    shim_ctx_ptr_t ctx)
{
    return 0;
}

int invoke(
    uint8_t* response,
    uint32_t max_response_len,
    uint32_t* actual_response_len,
    shim_ctx_ptr_t ctx)
{
    LOG_INFO("Clock Auction: executing...");

    int ret = -1;
    std::string functionName;
    std::vector<std::string> functionParameters;

    get_func_and_params(functionName, functionParameters, ctx);

    ClockAuction::Dispatcher(functionName, functionParameters, response, max_response_len, actual_response_len);

    if(*actual_response_len == 0)
    {
        LOG_ERROR("Response length is zero");
        ret = -1;
    }
    else 
    {
        ret = 0;
    }

    LOG_INFO("Clock Auction: exiting.");
    return 0;
}


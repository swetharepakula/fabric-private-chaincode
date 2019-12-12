/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include "error-codes.h"
#include "spectrum-auction-message.h"

ClockAuction::ErrorReport::ErrorReport()
{}

void ClockAuction::ErrorReport::set(error_codes_e e, const std::string& s)
{
    ec_ = e;
    errorString_ = s;
}

void ClockAuction::ErrorReport::toStatusJsonString(std::string& jsonString)
{
    ClockAuction::SpectrumAuctionMessage msg;
    msg.toStatusJsonString(ec_, errorString_, jsonString);
}

bool ClockAuction::ErrorReport::isSuccess()
{
    if(ec_ == EC_SUCCESS)
    {
        return true;
    }
    return false;
}

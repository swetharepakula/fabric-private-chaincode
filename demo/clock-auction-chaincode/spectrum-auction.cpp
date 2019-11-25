/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include "spectrum-auction.h"
#include "spectrum-auction-message.h"
#include "error-codes.h"

ClockAuction::SpectrumAuction::SpectrumAuction(shim_ctx_ptr_t ctx) : auctionStorage(ctx)
{}

int ClockAuction::SpectrumAuction::createAuction(const std::string& inputString,
        std::string& outputString, ClockAuction::ErrorReport& er)
{
    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;


    std::string statusMessage("Auction created");
    unsigned int auctionId = 1;
    msg.toCreateAuctionJson(rc, statusMessage, auctionId);
    outputString = msg.getJsonString();
}

int ClockAuction::SpectrumAuction::getAuctionDetails(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er)
{
}

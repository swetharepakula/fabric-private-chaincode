/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include "spectrum-auction.h"
#include "spectrum-auction-message.h"
#include "error-codes.h"
#include "storage.h"

#define AUCTION_ID_COUNTER_STRING "AuctionIdCounter"
#define AUCTION_ID_FIRST_COUNTER_VALUE 401

ClockAuction::SpectrumAuction::SpectrumAuction(shim_ctx_ptr_t ctx) : auctionStorage_(ctx)
{}

void ClockAuction::SpectrumAuction::InitializeAuctionIdCounter()
{
    uint32_t bytesRead = 0;
    const uint8_t* key = (const uint8_t*)AUCTION_ID_COUNTER_STRING;
    uint32_t keyLength = strlen(AUCTION_ID_COUNTER_STRING);
    uint8_t* value = (uint8_t*)&auctionIdCounter_;
    uint32_t valueLength = sizeof(auctionIdCounter_);
    auctionStorage_.ledgerPrivateGetBinary(key, keyLength, value, valueLength, &bytesRead);
    if(bytesRead == 0)
        auctionIdCounter_ = AUCTION_ID_FIRST_COUNTER_VALUE;
}

void ClockAuction::SpectrumAuction::StoreAuctionIdCounter()
{
    const uint8_t* key = (const uint8_t*)AUCTION_ID_COUNTER_STRING;
    uint32_t keyLength = strlen(AUCTION_ID_COUNTER_STRING);
    uint8_t* value = (uint8_t*)&auctionIdCounter_;
    uint32_t valueLength = sizeof(auctionIdCounter_);
    auctionStorage_.ledgerPrivatePutBinary(key, keyLength, value, valueLength);
}

bool ClockAuction::SpectrumAuction::createAuction(const std::string& inputString,
        std::string& outputString, ClockAuction::ErrorReport& er)
{
    {   // parse and validate input string
        ClockAuction::SpectrumAuctionMessage inMsg;
        bool b = inMsg.fromCreateAuctionJson(staticAuctionState_);
        if(!b)
        {
            er = staticAuctionState_.getErrorReport();
            return false;
        }
    }

    //all check passed, install auction

    //get auction id
    InitializeAuctionIdCounter();
    unsigned int auctionId = auctionIdCounter_++;
    StoreAuctionIdCounter();

    // initialize dynamic state
    dynamicAuctionState_ = DynamicAuctionState(CLOCK_PHASE, INITIAL_CLOCK_ROUND_NUMBER, false);

    //store static state

    //store dynamic state

    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;
    std::string statusMessage("Auction created");
    msg.toCreateAuctionJson(rc, statusMessage, auctionId);
    outputString = msg.getJsonString();
    return true;
}

bool ClockAuction::SpectrumAuction::getAuctionDetails(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er)
{
}

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
#include "utils.h"

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

void ClockAuction::SpectrumAuction::IncrementAndStoreAuctionIdCounter()
{
    uint32_t c = auctionIdCounter_ + 1;
    const uint8_t* key = (const uint8_t*)AUCTION_ID_COUNTER_STRING;
    uint32_t keyLength = strlen(AUCTION_ID_COUNTER_STRING);
    uint8_t* value = (uint8_t*)&c;
    uint32_t valueLength = sizeof(auctionIdCounter_);
    auctionStorage_.ledgerPrivatePutBinary(key, keyLength, value, valueLength);
}

void ClockAuction::SpectrumAuction::storeAuctionState()
{
    {   // store static state
        ClockAuction::SpectrumAuctionMessage outStateMsg;
        outStateMsg.toStaticAuctionStateJson(staticAuctionState_);
        std::string stateKey("Auction." + std::to_string(auctionIdCounter_) + ".staticAuctionState");
        auctionStorage_.ledgerPrivatePutString(stateKey, outStateMsg.getJsonString());
        LOG_DEBUG("Stored static state: %s", (outStateMsg.getJsonString()).c_str());
    }
    {   // store dynamic state
        ClockAuction::SpectrumAuctionMessage outStateMsg;
        outStateMsg.toDynamicAuctionStateJson(dynamicAuctionState_);
        std::string stateKey("Auction." + std::to_string(auctionIdCounter_) + ".dynamicAuctionState");
        auctionStorage_.ledgerPrivatePutString(stateKey, outStateMsg.getJsonString());
        LOG_DEBUG("Stored dynamic state: %s", (outStateMsg.getJsonString()).c_str());
    }
}

bool ClockAuction::SpectrumAuction::loadAuctionState()
{
    {   // load static state
        std::string stateKey("Auction." + std::to_string(auctionIdCounter_) + ".staticAuctionState");
        std::string stateValue;
        auctionStorage_.ledgerPrivateGetString(stateKey, stateValue);
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, stateValue.length() == 0);
        // next is same as for createAuction
        JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(stateValue.c_str());
        FAST_FAIL_CHECK_EX(er_, &staticAuctionState_.er_, EC_INVALID_INPUT, !staticAuctionState_.fromExtendedJsonObject(root_object));
        ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    }
    {   // load dynamic state
        std::string stateKey("Auction." + std::to_string(auctionIdCounter_) + ".dynamicAuctionState");
        std::string stateValue;
        auctionStorage_.ledgerPrivateGetString(stateKey, stateValue);
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, stateValue.length() == 0);
        JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(stateValue.c_str());
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, !dynamicAuctionState_.fromJsonObject(root_object));
        ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    }
    return true;
}

bool ClockAuction::SpectrumAuction::createAuction(const std::string& inputString,
        std::string& outputString, ClockAuction::ErrorReport& er)
{
    // parse and validate input string
    ClockAuction::SpectrumAuctionMessage inMsg(inputString);
    if(!inMsg.fromCreateAuctionJson(staticAuctionState_) || !staticAuctionState_.checkValidity())
    {
        LOG_ERROR("creation of static state failed");
        er = staticAuctionState_.getErrorReport();
        return false;
    }
    
    //all check passed, install auction

    // FAKE owner
    staticAuctionState_.fakeOwner();    

    //get auction id
    InitializeAuctionIdCounter();
    IncrementAndStoreAuctionIdCounter();

    // initialize dynamic state
    dynamicAuctionState_ = DynamicAuctionState(CLOCK_PHASE, INITIAL_CLOCK_ROUND_NUMBER, false);

    //store static and dynamic state
    storeAuctionState();

    er.set(EC_SUCCESS, "");

    //prepare response message
    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;
    std::string statusMessage("Auction created");
    msg.toCreateAuctionJson(rc, statusMessage, auctionIdCounter_);
    outputString = msg.getJsonString();
    return true;
}

bool ClockAuction::SpectrumAuction::getAuctionDetails(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er)
{
    // parse and validate input string
    ClockAuction::SpectrumAuctionMessage inMsg(inputString);
    FAST_FAIL_CHECK(er, EC_INVALID_INPUT, !inMsg.fromGetAuctionDetailsJson(auctionIdCounter_));
    FAST_FAIL_CHECK_EX(er, &er_, EC_INVALID_INPUT, !loadAuctionState());

    //all check passed, return details
    er.set(EC_SUCCESS, "");
    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;
    std::string statusMessage("Auction details");
    msg.toGetAuctionDetailsJson(rc, statusMessage, staticAuctionState_);
    outputString = msg.getJsonString();
    return true;
}

bool ClockAuction::SpectrumAuction::getAuctionStatus(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er)
{
    // parse and validate input string
    ClockAuction::SpectrumAuctionMessage inMsg(inputString);
    FAST_FAIL_CHECK(er, EC_INVALID_INPUT, !inMsg.fromGetAuctionStatusJson(auctionIdCounter_));
    FAST_FAIL_CHECK_EX(er, &er_, EC_INVALID_INPUT, !loadAuctionState());

    //all check passed, return status
    er.set(EC_SUCCESS, "");
    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;
    std::string statusMessage("Auction status");
    msg.toGetAuctionStatusJson(rc, statusMessage, dynamicAuctionState_);
    outputString = msg.getJsonString();
    return true;
}

bool ClockAuction::SpectrumAuction::startNextRound(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er)
{
    // parse and validate input string
    ClockAuction::SpectrumAuctionMessage inMsg(inputString);
    FAST_FAIL_CHECK(er, EC_INVALID_INPUT, !inMsg.fromStartNextRoundJson(auctionIdCounter_));
    FAST_FAIL_CHECK_EX(er, &er_, EC_INVALID_INPUT, !loadAuctionState());
    FAST_FAIL_CHECK(er, EC_ROUND_ALREADY_ACTIVE, dynamicAuctionState_.isRoundActive());
    FAST_FAIL_CHECK(er, EC_RESTRICTED_AUCTION_STATE, !dynamicAuctionState_.isStateClockPhase() && !dynamicAuctionState_.isStateAssignmentPhase());

    //all check passed

    dynamicAuctionState_.startRound();

    storeAuctionState();

    er.set(EC_SUCCESS, "");
    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;
    std::string statusMessage("Start next round");
    msg.toStartNextRoundJson(rc, statusMessage);
    outputString = msg.getJsonString();
    return true;
}

bool ClockAuction::SpectrumAuction::endRound(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er)
{
    // parse and validate input string
    ClockAuction::SpectrumAuctionMessage inMsg(inputString);
    FAST_FAIL_CHECK(er, EC_INVALID_INPUT, !inMsg.fromEndRoundJson(auctionIdCounter_));
    FAST_FAIL_CHECK_EX(er, &er_, EC_INVALID_INPUT, !loadAuctionState());
    FAST_FAIL_CHECK(er, EC_ROUND_ALREADY_ACTIVE, !dynamicAuctionState_.isRoundActive());
    FAST_FAIL_CHECK(er, EC_RESTRICTED_AUCTION_STATE, !dynamicAuctionState_.isStateClockPhase() && !dynamicAuctionState_.isStateAssignmentPhase());

    //all check passed

    dynamicAuctionState_.endRound();

    // TODO evaluate round

    er.set(EC_SUCCESS, "");
    ClockAuction::SpectrumAuctionMessage msg;
    int rc = 0;
    std::string statusMessage("End round");
    msg.toEndRoundJson(rc, statusMessage);
    outputString = msg.getJsonString();
    return true;
}

/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"
#include "utils.h"
#include "error-codes.h"
#include "spectrum-auction-message.h"

ClockAuction::SpectrumAuctionMessage::SpectrumAuctionMessage()
{}

ClockAuction::SpectrumAuctionMessage::SpectrumAuctionMessage(const std::string& message) : inputJsonString_(message)
{}

ClockAuction::ErrorReport ClockAuction::SpectrumAuctionMessage::getErrorReport()
{
    return er_;
}

void ClockAuction::SpectrumAuctionMessage::toStatusJsonObject(JSON_Object* root_object, int rc, std::string& message)
{
    json_object_set_number(root_object, "rc", rc);
    json_object_set_string(root_object, "message", message.c_str());
}

void ClockAuction::SpectrumAuctionMessage::toWrappedStatusJsonObject(JSON_Object* root_object, int rc, std::string& message)
{
    JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
    toStatusJsonObject(r, rc, message);
    json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
}

void ClockAuction::SpectrumAuctionMessage::toStatusJsonString(int rc, std::string& message, std::string& jsonString)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    toStatusJsonObject(root_object, rc, message);
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString);
}

std::string ClockAuction::SpectrumAuctionMessage::getJsonString()
{
    return jsonString_;
}

void ClockAuction::SpectrumAuctionMessage::toCreateAuctionJson(int rc, std::string& message, unsigned int auctionId)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        toStatusJsonObject(r, rc, message);
        json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
    }
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        json_object_set_number(r, "auctionId", auctionId);
        json_object_set_value(root_object, "response", json_object_get_wrapping_value(r));
    }
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromCreateAuctionJson(StaticAuctionState& staticAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    bool b = staticAuctionState.fromJsonObject(root_object);
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return b;
}

bool ClockAuction::SpectrumAuctionMessage::fromGetAuctionDetailsJson(uint32_t& auctionId)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    auctionId = json_object_get_number(root_object, "auctionId");
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return auctionId != 0;
}

void ClockAuction::SpectrumAuctionMessage::toGetAuctionDetailsJson(int rc, std::string& message, const StaticAuctionState& staticAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        toStatusJsonObject(r, rc, message);
        json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
    }
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        staticAuctionState.toJsonObject(r);
        json_object_set_value(root_object, "response", json_object_get_wrapping_value(r));
    }
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromGetAuctionStatusJson(uint32_t& auctionId)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    auctionId = json_object_get_number(root_object, "auctionId");
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return auctionId != 0;
}

void ClockAuction::SpectrumAuctionMessage::toGetAuctionStatusJson(int rc, std::string& message, const DynamicAuctionState& dynamicAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        toStatusJsonObject(r, rc, message);
        json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
    }
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        dynamicAuctionState.toJsonObject(r);
        json_object_set_value(root_object, "response", json_object_get_wrapping_value(r));
    }
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromStartNextRoundJson(uint32_t& auctionId)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    auctionId = json_object_get_number(root_object, "auctionId");
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return auctionId != 0;
}

void ClockAuction::SpectrumAuctionMessage::toStartNextRoundJson(int rc, std::string& message)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        toStatusJsonObject(r, rc, message);
        json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
    }
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromEndRoundJson(uint32_t& auctionId)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    auctionId = json_object_get_number(root_object, "auctionId");
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return auctionId != 0;
}

void ClockAuction::SpectrumAuctionMessage::toEndRoundJson(int rc, std::string& message)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        toStatusJsonObject(r, rc, message);
        json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
    }
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromSubmitClockBidJson(Bid& bid)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    FAST_FAIL_CHECK_EX(er_, &bid.er_, EC_INVALID_INPUT, !bid.fromJsonObject(root_object));
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return true;
}

void ClockAuction::SpectrumAuctionMessage::toSubmitClockBidJson(int rc, std::string& message)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    {
        JSON_Object* r = ClockAuction::JsonUtils::openJsonObject(NULL);
        toStatusJsonObject(r, rc, message);
        json_object_set_value(root_object, "status", json_object_get_wrapping_value(r));
    }
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

void ClockAuction::SpectrumAuctionMessage::toStaticAuctionStateJson(const StaticAuctionState& staticAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    staticAuctionState.toJsonObject(root_object);
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromStaticAuctionStateJson(StaticAuctionState& staticAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(jsonString_.c_str());
    bool b = staticAuctionState.fromJsonObject(root_object);
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return b;
}

void ClockAuction::SpectrumAuctionMessage::toDynamicAuctionStateJson(const DynamicAuctionState& dynamicAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    dynamicAuctionState.toJsonObject(root_object);
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString_);
}

bool ClockAuction::SpectrumAuctionMessage::fromDynamicAuctionStateJson(DynamicAuctionState& dynamicAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(jsonString_.c_str());
    bool b = dynamicAuctionState.fromJsonObject(root_object);
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return b;
}


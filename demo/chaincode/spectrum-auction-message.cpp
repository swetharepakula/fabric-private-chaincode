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

void ClockAuction::SpectrumAuctionMessage::toStatusJsonString(int rc, std::string& message, std::string& jsonString)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(NULL);
    json_object_set_number(root_object, "rc", rc);
    json_object_set_string(root_object, "message", message.c_str());
    ClockAuction::JsonUtils::closeJsonObject(root_object, &jsonString);
}

std::string ClockAuction::SpectrumAuctionMessage::getJsonString()
{
    return jsonString_;
}

void ClockAuction::SpectrumAuctionMessage::toCreateAuctionJson(int rc, std::string& message, unsigned int auctionId)
{
    std::string jsonStatusString;
    toStatusJsonString(rc, message, jsonStatusString);

    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "status", jsonStatusString.c_str());
    json_object_set_number(root_object, "id", auctionId);
    char* serialized_string = json_serialize_to_string(root_value);
    jsonString_.assign(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
}

bool ClockAuction::SpectrumAuctionMessage::fromCreateAuctionJson(StaticAuctionState& staticAuctionState)
{
    JSON_Object* root_object = ClockAuction::JsonUtils::openJsonObject(inputJsonString_.c_str());
    bool b = staticAuctionState.fromJsonObject(root_object);
    ClockAuction::JsonUtils::closeJsonObject(root_object, NULL);
    return b;
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


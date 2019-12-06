/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "spectrum-auction-message.h"
#include "error-codes.h"
#include <string>

#include "json/parson.h"

ClockAuction::SpectrumAuctionMessage::SpectrumAuctionMessage()
{}

void ClockAuction::SpectrumAuctionMessage::toStatusJsonString(int rc, std::string& message, std::string& jsonString)
{
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_number(root_object, "rc", rc);
    json_object_set_string(root_object, "message", message.c_str());
    char* serialized_string = json_serialize_to_string(root_value);
    jsonString.assign(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value); 
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



/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "principal.h"

ClockAuction::Principal::Principal(uint32_t id, std::string& mspId, std::string& dn, std::string& name) : id_(id), mspId_(mspId), dn_(dn), name_(name)
{}

bool ClockAuction::Principal::toJson(std::string& jsonString)
{
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_number(root_object, "id", id_);
    json_object_set_string(root_object, "mspid", mspId_.c_str());
    json_object_set_string(root_object, "dn", dn_.c_str());
    json_object_set_string(root_object, "name", name_.c_str());

    char* serialized_string = json_serialize_to_string(root_value);
    jsonString.assign(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return true;
}



bool ClockAuction::Principal::fromJson(std::string& jsonString)
{
    JSON_Value* root_value = json_parse_string(jsonString.c_str());
    JSON_Object* root_object = json_value_get_object(root_value);
    {
        id_ = json_object_get_number(root_object, "id");
        if(id_ == 0)
        {
            er_.set(EC_INVALID_INPUT, std::string("line: ") + std::to_string(__LINE__));
            return false;
        }
    }
    {
        const char* str = json_object_get_string(root_object, "mspId");
        if(str == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        mspId_ = std::string(str);
    }
    {
        const char* str = json_object_get_string(root_object, "dn");
        if(str == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        dn_ = std::string(str);
    }
    {
        const char* str = json_object_get_string(root_object, "name");
        if(str == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        name_ = std::string(str);
    }
    json_value_free(root_value);
    return true;
}

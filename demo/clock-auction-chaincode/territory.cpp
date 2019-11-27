/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "territory.h"

ClockAuction::Channel::Channel()
{}

ClockAuction::Channel::Channel(uint32_t id, std::string& name, uint32_t impairment) : id_(id), name_(name), impairment_(impairment)
{}

bool ClockAuction::Channel::toJson(std::string& jsonString)
{
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_number(root_object, "id", id_);
    json_object_set_string(root_object, "name", name_.c_str());
    json_object_set_number(root_object, "impairment", impairment_);

    char* serialized_string = json_serialize_to_string(root_value);
    jsonString.assign(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return true;
}

bool ClockAuction::Channel::fromJson(const std::string& jsonString)
{
    JSON_Value* root_value = json_parse_string(jsonString.c_str());
    JSON_Object* root_object = json_value_get_object(root_value);
    {
        id_ = json_object_get_number(root_object, "id");
        if(id_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
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
    {
        impairment_ = json_object_get_number(root_object, "impairment");
        if(impairment_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
    }
    json_value_free(root_value);
    return true;
}

ClockAuction::Territory::Territory()
{}

ClockAuction::Territory::Territory(uint32_t id, std::string& name, bool isHighDemand, double minPrice, std::vector<Channel>& channels) : id_(id), name_(name), isHighDemand_(isHighDemand), minPrice_(minPrice), channels_(channels)
{}

bool ClockAuction::Territory::toJson(std::string& jsonString)
{
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_number(root_object, "id", id_);
    json_object_set_string(root_object, "name", name_.c_str());
    json_object_set_boolean(root_object, "isHighDemand", isHighDemand_);
    json_object_set_value(root_object, "channels", json_value_init_array());
    JSON_Array* channel_array = json_object_get_array(root_object, "channels");
    for(unsigned int i = 0; i < channels_.size(); i++)
    {
        std::string channelJsonString;
        channels_[i].toJson(channelJsonString);
        json_array_append_string(channel_array, channelJsonString.c_str());
        channelJsonString.clear();
    }

    char* serialized_string = json_serialize_to_string(root_value);
    jsonString.assign(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
}

bool ClockAuction::Territory::fromJson(const std::string& jsonString)
{
    JSON_Value* root_value = json_parse_string(jsonString.c_str());
    JSON_Object* root_object = json_value_get_object(root_value);
    {
        id_ = json_object_get_number(root_object, "id");
        if(id_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
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
    {
        int b = json_object_get_boolean(root_object, "isHighDemand");
        if(b == -1)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        isHighDemand_ = b;
    }
    {
        JSON_Array* channel_array = json_object_get_array(root_object, "channels");
        if(channel_array == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        unsigned int channelN = json_array_get_count(channel_array);
        for(unsigned int i = 0; i < channelN; i++)
        {
            const char *s = json_array_get_string(channel_array, i);
            if(s == 0)
            {
                er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
                return false;
            }
            Channel ch;
            bool b = ch.fromJson(s);
            if(!b)
            {
                er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
                return false;
            }
            channels_.push_back(ch);
        }
    }
    json_value_free(root_value);
    return true;
}

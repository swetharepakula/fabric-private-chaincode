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

void ClockAuction::Channel::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_number(root_object, "id", id_);
    json_object_set_string(root_object, "name", name_.c_str());
    json_object_set_number(root_object, "impairment", impairment_);
}

bool ClockAuction::Channel::fromJsonObject(const JSON_Object* root_object)
{
    {
        id_ = json_object_get_number(root_object, "id");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, id_ == 0);
    }
    {
        const char* str = json_object_get_string(root_object, "name");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, str == 0);
        name_ = std::string(str);
    }
    {
        impairment_ = json_object_get_number(root_object, "impairment");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, impairment_ == 0);
    }
    return true;
}

ClockAuction::Territory::Territory()
{}

ClockAuction::Territory::Territory(uint32_t id, std::string& name, bool isHighDemand, double minPrice, std::vector<Channel>& channels) : id_(id), name_(name), isHighDemand_(isHighDemand), minPrice_(minPrice), channels_(channels)
{}

void ClockAuction::Territory::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_number(root_object, "id", id_);
    json_object_set_string(root_object, "name", name_.c_str());
    json_object_set_boolean(root_object, "isHighDemand", isHighDemand_);
    json_object_set_number(root_object, "minPrice", minPrice_);
    json_object_set_value(root_object, "channels", json_value_init_array());
    JSON_Array* channel_array = json_object_get_array(root_object, "channels");
    for(unsigned int i = 0; i < channels_.size(); i++)
    {
        JSON_Value* v = json_value_init_object();
        JSON_Object* o = json_value_get_object(v);
        channels_[i].toJsonObject(o);
        json_array_append_value(channel_array, v);
    }
}

bool ClockAuction::Territory::fromJsonObject(const JSON_Object* root_object)
{
    {
        id_ = json_object_get_number(root_object, "id");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, id_ == 0);
    }
    {
        const char* str = json_object_get_string(root_object, "name");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, str == 0);
        name_ = std::string(str);
    }
    {
        int b = json_object_get_boolean(root_object, "isHighDemand");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, b == -1);
        isHighDemand_ = b;
    }
    {
        minPrice_ = json_object_get_number(root_object, "minPrice");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, minPrice_ == 0);
    }
    {
        JSON_Array* channel_array = json_object_get_array(root_object, "channels");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, channel_array == 0);
        unsigned int channelN = json_array_get_count(channel_array);
        for(unsigned int i = 0; i < channelN; i++)
        {
            JSON_Object* o = json_array_get_object(channel_array, i);
            Channel ch;
            FAST_FAIL_CHECK_EX(er_, &ch.er_, EC_INVALID_INPUT, !ch.fromJsonObject(o));
            channels_.push_back(ch);
        }
    }
    return true;
}

uint32_t ClockAuction::Territory::getTerritoryId() const
{
    return id_;
}

uint32_t ClockAuction::Territory::numberOfChannels() const
{
    return channels_.size();
}

double ClockAuction::Territory::getMinPrice() const
{
    return minPrice_;
}

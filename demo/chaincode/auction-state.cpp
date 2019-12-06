/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"
#include "auction-state.h"

bool ClockAuction::StaticAuctionState::toJsonObject(JSON_Object* root_object)
{
    json_object_set_string(root_object, "name", name_.c_str());
    json_object_set_value(root_object, "territories", json_value_init_array());
    JSON_Array* territory_array = json_object_get_array(root_object, "territories");
    for(unsigned int i = 0; i < territories_.size(); i++)
    {
        JSON_Value* v = json_value_init_object();
        JSON_Object* o = json_value_get_object(v);
        territories_[i].toJsonObject(o);
        json_array_append_value(territory_array, v);
    }
    JSON_Array* bidder_array = json_object_get_array(root_object, "bidders");
    for(unsigned int i = 0; i < bidders_.size(); i++)
    {
        JSON_Value* v = json_value_init_object();
        JSON_Object* o = json_value_get_object(v);
        bidders_[i].toJsonObject(o);
        json_array_append_value(bidder_array, v);
    }
    JSON_Array* eligibility_array = json_object_get_array(root_object, "initialEligibilities");
    for(unsigned int i = 0; i < initialEligibilities_.size(); i++)
    {
        JSON_Value* v = json_value_init_object();
        JSON_Object* o = json_value_get_object(v);
        initialEligibilities_[i].toJsonObject(o);
        json_array_append_value(eligibility_array, v);
    }
    json_object_set_number(root_object, "activityRequirementPercentage", activityRequirementPercentage_);
    json_object_set_number(root_object, "clockPriceIncrementPercentage", clockPriceIncrementPercentage_);
    return true;
}

bool ClockAuction::StaticAuctionState::fromJsonObject(const JSON_Object* root_object)
{
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
        JSON_Array* territory_array = json_object_get_array(root_object, "territories");
        if(territory_array == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        unsigned int territoryN = json_array_get_count(territory_array);
        for(unsigned int i = 0; i < territoryN; i++)
        {
            JSON_Object* o = json_array_get_object(territory_array, i);
            Territory territory;
            bool b = territory.fromJsonObject(o);
            if(!b)
            {
                er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
                return false;
            }
            territories_.push_back(territory);
        }
    }
    {
        JSON_Array* bidder_array = json_object_get_array(root_object, "bidders");
        if(bidder_array == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        unsigned int bidderN = json_array_get_count(bidder_array);
        for(unsigned int i = 0; i < bidderN; i++)
        {
            JSON_Object* o = json_array_get_object(bidder_array, i);
            Bidder bidder;
            bool b = bidder.fromJsonObject(o);
            if(!b)
            {
                er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
                return false;
            }
            bidders_.push_back(bidder);
        }
    }
    {
        JSON_Array* eligibility_array = json_object_get_array(root_object, "initialEligibilities");
        if(eligibility_array == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
        unsigned int eligibilityN = json_array_get_count(eligibility_array);
        for(unsigned int i = 0; i < eligibilityN; i++)
        {
            JSON_Object* o = json_array_get_object(eligibility_array, i);
            Eligibility eligibility;
            bool b = eligibility.fromJsonObject(o);
            if(!b)
            {
                er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
                return false;
            }
            initialEligibilities_.push_back(eligibility);
        }
    }
    {
        activityRequirementPercentage_ = json_object_get_number(root_object, "activityRequirementPercentage");
        if(activityRequirementPercentage_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
    }
    {
        clockPriceIncrementPercentage_ = json_object_get_number(root_object, "clockPriceIncrementPercentage");
        if(clockPriceIncrementPercentage_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
    }
    return true;
}

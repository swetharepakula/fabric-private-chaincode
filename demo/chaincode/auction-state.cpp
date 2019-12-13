/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"
#include "utils.h"
#include "auction-state.h"

bool ClockAuction::StaticAuctionState::toJsonObject(JSON_Object* root_object) const
{
    {
        //JSON_Value* v = json_value_init_object();
        //JSON_Object* o = json_value_get_object(v);
        JSON_Object* o = ClockAuction::JsonUtils::openJsonObject(NULL);
        owner_.toJsonObject(o);
        json_object_set_value(root_object, "owner", json_object_get_wrapping_value(o));
        // do not close json object here -- it will be freed when parent object is closed
    }
    {
        json_object_set_string(root_object, "name", name_.c_str());
    }
    {
        json_object_set_value(root_object, "territories", json_value_init_array());
        JSON_Array* territory_array = json_object_get_array(root_object, "territories");
        for(unsigned int i = 0; i < territories_.size(); i++)
        {
            JSON_Value* v = json_value_init_object();
            JSON_Object* o = json_value_get_object(v);
            territories_[i].toJsonObject(o);
            json_array_append_value(territory_array, v);
        }
    }
    {
        json_object_set_value(root_object, "bidders", json_value_init_array());
        JSON_Array* bidder_array = json_object_get_array(root_object, "bidders");
        for(unsigned int i = 0; i < bidders_.size(); i++)
        {
            JSON_Value* v = json_value_init_object();
            JSON_Object* o = json_value_get_object(v);
            bidders_[i].toJsonObject(o);
            json_array_append_value(bidder_array, v);
        }
    }
    {
        json_object_set_value(root_object, "initialEligibilities", json_value_init_array());
        JSON_Array* eligibility_array = json_object_get_array(root_object, "initialEligibilities");
        for(unsigned int i = 0; i < initialEligibilities_.size(); i++)
        {
            JSON_Value* v = json_value_init_object();
            JSON_Object* o = json_value_get_object(v);
            initialEligibilities_[i].toJsonObject(o);
            json_array_append_value(eligibility_array, v);
        }
    }
    {
        json_object_set_number(root_object, "activityRequirementPercentage", activityRequirementPercentage_);
    }
    {
        json_object_set_number(root_object, "clockPriceIncrementPercentage", clockPriceIncrementPercentage_);
    }
    return true;
}

bool ClockAuction::StaticAuctionState::fromJsonObject(const JSON_Object* root_object)
{
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, root_object == NULL);

    {
        const char* str = json_object_get_string(root_object, "name");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, str == 0);
        name_ = std::string(str);
    }
    {
        JSON_Array* territory_array = json_object_get_array(root_object, "territories");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, territory_array == 0);
        unsigned int territoryN = json_array_get_count(territory_array);
        for(unsigned int i = 0; i < territoryN; i++)
        {
            JSON_Object* o = json_array_get_object(territory_array, i);
            Territory territory;
            FAST_FAIL_CHECK_EX(er_, &territory.er_, EC_INVALID_INPUT, !territory.fromJsonObject(o));
            territories_.push_back(territory);
        }
    }
    {
        JSON_Array* bidder_array = json_object_get_array(root_object, "bidders");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, bidder_array == 0);
        unsigned int bidderN = json_array_get_count(bidder_array);
        for(unsigned int i = 0; i < bidderN; i++)
        {
            JSON_Object* o = json_array_get_object(bidder_array, i);
            Bidder bidder;
            FAST_FAIL_CHECK_EX(er_, &bidder.er_, EC_INVALID_INPUT, !bidder.fromJsonObject(o));
            bidders_.push_back(bidder);
        }
    }
    {
        JSON_Array* eligibility_array = json_object_get_array(root_object, "initialEligibilities");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, eligibility_array == 0);
        unsigned int eligibilityN = json_array_get_count(eligibility_array);
        for(unsigned int i = 0; i < eligibilityN; i++)
        {
            JSON_Object* o = json_array_get_object(eligibility_array, i);
            Eligibility eligibility;
            FAST_FAIL_CHECK_EX(er_, &eligibility.er_, EC_INVALID_INPUT, !eligibility.fromJsonObject(o));
            initialEligibilities_.push_back(eligibility);
        }
    }
    {
        activityRequirementPercentage_ = json_object_get_number(root_object, "activityRequirementPercentage");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, activityRequirementPercentage_ == 0);
    }
    {
        clockPriceIncrementPercentage_ = json_object_get_number(root_object, "clockPriceIncrementPercentage");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, clockPriceIncrementPercentage_ == 0);
    }
    return true;
}

bool ClockAuction::StaticAuctionState::fromExtendedJsonObject(const JSON_Object* root_object)
{
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, root_object == NULL);
    {
        JSON_Value* v = json_object_get_value(root_object, "owner");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, v == 0);
        JSON_Object* o = json_value_get_object(v);
        FAST_FAIL_CHECK_EX(er_, &owner_.er_, EC_INVALID_INPUT, !owner_.fromJsonObject(o));
    }
    return fromJsonObject(root_object);
}

bool ClockAuction::StaticAuctionState::checkValidity()
{
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, name_.length() == 0);
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, territories_.size() == 0);
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, bidders_.size() == 0);
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, initialEligibilities_.size() == 0);
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, bidders_.size() != initialEligibilities_.size());
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, activityRequirementPercentage_ > 100);
    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, clockPriceIncrementPercentage_ > 100);
    return true;
}

void ClockAuction::StaticAuctionState::fakeOwner()
{
    owner_ = Principal("duffy", "duck");
}

ClockAuction::ErrorReport ClockAuction::StaticAuctionState::getErrorReport()
{
    return er_;
}

ClockAuction::DynamicAuctionState::DynamicAuctionState() {}

ClockAuction::DynamicAuctionState::DynamicAuctionState(auction_state_e auctionState, uint32_t clockRound, bool roundActive)
: auctionState_(auctionState), clockRound_(clockRound), roundActive_(roundActive) {}

bool ClockAuction::DynamicAuctionState::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_number(root_object, "state", auctionState_);
    json_object_set_number(root_object, "clockRound", clockRound_);
    json_object_set_boolean(root_object, "RoundActive", (int)roundActive_);
    return true;
}

bool ClockAuction::DynamicAuctionState::fromJsonObject(const JSON_Object* root_object)
{
    {
        double d = json_object_get_number(root_object, "state");
        auctionState_ = (d >= 0 && d <= MAX_STATE_INDEX ? (auction_state_e) d : STATE_UNDEFINED);
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, auctionState_ == 0);
    }
    {
        clockRound_ = json_object_get_number(root_object, "clockRound");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, clockRound_ == 0);
    }
    {
        roundActive_ = json_object_get_boolean(root_object, "roundActive");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, roundActive_ == 0);
    }
    return true;
}


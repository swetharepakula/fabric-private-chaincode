/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bid.h"
#include "auction-state.h"

ClockAuction::Demand::Demand()
{}

ClockAuction::Demand::Demand(uint32_t territoryId, uint32_t quantity, double price) : territoryId_(territoryId), quantity_(quantity), price_(price)
{}

bool ClockAuction::Demand::fromJsonObject(const JSON_Object* root_object)
{
    {
        territoryId_ = json_object_get_number(root_object, "terId");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, territoryId_ == 0);
    }
    {
        quantity_ = json_object_get_number(root_object, "qty");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, territoryId_ == 0);
    }
    {
        price_ = json_object_get_number(root_object, "price");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, territoryId_ == 0);
    }
    return true;
}

void ClockAuction::Demand::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_number(root_object, "terId", territoryId_);
    json_object_set_number(root_object, "qty", quantity_);
    json_object_set_number(root_object, "price", price_);
}

bool ClockAuction::Bid::fromJsonObject(const JSON_Object* root_object)
{
    {
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, !json_object_has_value_of_type(root_object, "auctionId", JSONNumber));
        auctionId_ = json_object_get_number(root_object, "auctionId");
    }
    {
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, !json_object_has_value_of_type(root_object, "round", JSONNumber));

        round_ = json_object_get_number(root_object, "round");
    }
    {
        JSON_Array* demand_array = json_object_get_array(root_object, "bids");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, demand_array == 0);
        unsigned int bidsN = json_array_get_count(demand_array);
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, bidsN == 0 && auctionId_ != 0);
        for(unsigned int i = 0; i < bidsN; i++)
        {
            JSON_Object* o = json_array_get_object(demand_array, i);
            Demand d;
            FAST_FAIL_CHECK_EX(er_, &d.er_, EC_INVALID_INPUT, !d.fromJsonObject(o));
            demands_.push_back(d);
        }
    }
    return true;
}

void ClockAuction::Bid::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_number(root_object, "auctionId", auctionId_);
    json_object_set_number(root_object, "round", round_);
    json_object_set_value(root_object, "bids", json_value_init_array());
    JSON_Array* demand_array = json_object_get_array(root_object, "bids");
    for(unsigned int i = 0; i < demands_.size(); i++)
    {
        JSON_Value* v = json_value_init_object();
        JSON_Object* o = json_value_get_object(v);
        demands_[i].toJsonObject(o);
        json_array_append_value(demand_array, v);
    }
}
/*
bool ClockAuction::Bid::isValid(const ClockAuction::StaticAuctionState& sState, const ClockAuction::DynamicAuctionState& dState)
{
    // TODO check bidder

    std::set<uint32_t> duplicateIdTracker;
    FAST_FAIL_CHECK(er_, EC_ROUND_NOT_CURRENT, dState.getRound() != round_);
    FAST_FAIL_CHECK(er_, EC_ROUND_NOT_ACTIVE, !dState.isRoundActive());

    for(unsigned int i = 0; i < demands_.size(); i++)
    {
        const Territory* pTerritory = sState.getTerritory(demands_[i].territoryId_);

        // check territory existence
        FAST_FAIL_CHECK(er_, EC_UNRECOGNIZED_TERRITORY, pTerritory == NULL);

        //check if entered territory is duplicate
        auto it = duplicateIdTracker.find(demands_[i].territoryId_);
        FAST_FAIL_CHECK(er_, EC_DUPLICATE_TERRITORIES, it != duplicateIdTracker.end());
        duplicateIdTracker.insert(demands_[i].territoryId_);

        // check demand does not exceed supply
        FAST_FAIL_CHECK(er_, EC_TOO_MUCH_DEMAND, demands_[i].quantity_ > pTerritory->numberOfChannels());

        if(round_ == 1)
        {
            // check demand does not exceed eligibility
            uint32_t elig = sState.getEligibilityNumber(sState.fromPrincipalToBidderId(dState.getSubmitter()));
            FAST_FAIL_CHECK(er_, EC_NOT_ENOUGH_ELIGIBILITY, sumQuantityDemands() > elig);
            // no price check in first round
        }
        else
        {
            // check demand does not exceed supply
            FAST_FAIL_CHECK(er_, EC_TOO_MUCH_DEMAND, demands_[i].quantity_ > pTerritory->numberOfChannels());
        }

        
    }

    return true;
}
*/
uint32_t ClockAuction::Bid::sumQuantityDemands() const
{
    uint32_t total = 0;
    for(unsigned int i = 0; i < demands_.size(); i++)
    {
        total += demands_[i].quantity_;
    }
    return total;
}

std::vector<uint32_t> ClockAuction::Bid::getDemandedTerritoryIds() const
{
    std::vector<uint32_t> demandedTerritoryIds;
    for(unsigned int i = 0; i < demands_.size(); i++)
    {
        demandedTerritoryIds.push_back(demands_[i].territoryId_);
    }
    return demandedTerritoryIds;
}

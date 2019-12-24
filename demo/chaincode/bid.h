/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common.h"
#include "error-codes.h"
#include "auction-state.h"

namespace ClockAuction
{
    class Demand 
    {
        public:
            uint32_t territoryId_;
            uint32_t quantity_;
            double price_;

            ErrorReport er_;

            bool fromJsonObject(const JSON_Object* root_object);
            void toJsonObject(JSON_Object* root_object) const;
    };

    class Bid
    {
        public:
            uint32_t auctionId_;
            uint32_t round_;
            std::vector<Demand> demands_;
            ErrorReport er_;

            bool fromJsonObject(const JSON_Object* root_object);
            void toJsonObject(JSON_Object* root_object) const;

            bool isValid(const StaticAuctionState& sState, const DynamicAuctionState& dState);
            uint32_t sumQuantityDemands();
    };
}

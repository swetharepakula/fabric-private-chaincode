/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common.h"
#include "principal.h"
#include "bidder.h"
#include "eligibility.h"
#include "territory.h"
#include "error-codes.h"

typedef enum
{
    STATE_UNDEFINED,
    CLOCK_PHASE,
    ASSIGNMENT_PHASE,
    FSR_FAILED
} auction_state_e;

namespace ClockAuction
{
    class StaticAuctionState
    {
        private:
            Principal owner_;
            std::string name_;
            std::vector<Territory> territories_;
            std::vector<Bidder> bidders_;
            std::vector<Eligibility> initialEligibilities_;
            uint32_t activityRequirementPercentage_;
            uint32_t clockPriceIncrementPercentage_;
            ErrorReport er_;
            
        public:
            bool toJsonObject(JSON_Object* root_object);
            bool fromJsonObject(const JSON_Object* root_object);
    };

    class DynamicAuctionState
    {
        auction_state_e auctionState_;
        uint32_t clockRound_;
        bool roundActive_;
    };
}

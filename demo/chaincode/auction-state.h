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
    FSR_FAILED,
    CLOSED,
    MAX_STATE_INDEX
} auction_state_e;

#define INITIAL_CLOCK_ROUND_NUMBER 1

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

        public:
            bool toJsonObject(JSON_Object* root_object) const;
            bool fromJsonObject(const JSON_Object* root_object);
            bool fromExtendedJsonObject(const JSON_Object* root_object);
            bool checkValidity();
            void fakeOwner();
            ErrorReport getErrorReport();
            ErrorReport er_;

            bool isTerritoryIdValid(uint32_t checkId);
            const Territory* getTerritory(uint32_t territoryId) const;

            bool isPrincipalOwner(const Principal& p) const;
            bool isPrincipalBidder(const Principal& p);
            uint32_t fromPrincipalToBidderId(const Principal& p) const;
            const Principal fromBidderIdToPrincipal(uint32_t bidderId) const;
            uint32_t getEligibilityNumber(uint32_t bidderId) const;

            std::vector<double> getInitialPrices();
            std::vector<uint32_t> getInitialEligibilities();
            uint32_t getClockPriceIncrementPercentage();
    };

    class DynamicAuctionState
    {
        private:
            auction_state_e auctionState_;
            uint32_t clockRound_;
            bool roundActive_;

            Principal submitterPrincipal_;

            std::vector< std::vector<double> > postedPrice_; //vector: [round][territory] = price
            std::vector< std::vector<double> > clockPrice_;  //vector: [round][territory] = price
            std::vector< std::vector<uint32_t> > eligibility_; //vector: [round][bidder]    = number

            ErrorReport er_;
            
        public:
            DynamicAuctionState();
            DynamicAuctionState(auction_state_e auctionState, uint32_t clockRound, bool roundActive, StaticAuctionState& staticAuctionState);
            bool toJsonObject(JSON_Object* root_object) const;
            bool fromJsonObject(const JSON_Object* root_object);

            bool isRoundActive() const;
            void startRound();
            void endRound();
            void endRoundAndAdvance();
            bool isStateClockPhase();
            bool isStateAssignmentPhase();
            uint32_t getRound() const;

            void fakeSubmitter(Principal p);
            const Principal getSubmitter() const;
    };
}

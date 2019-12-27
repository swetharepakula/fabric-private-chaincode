/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "spectrum-auction.h"
#include "auction-state.h"

void ClockAuction::SpectrumAuction::evaluateClockRound()
{
    if(dynamicAuctionState_.getRound() == 1)
    {
        LOG_DEBUG("Processing initial bids");
        dynamicAuctionState_.processInitialRoundBids(staticAuctionState_, auctionIdCounter_);
    }
    else
    {
        LOG_DEBUG("Processing regular bids");
        dynamicAuctionState_.processRegularRoundBids(staticAuctionState_, auctionIdCounter_);
    }
}

void ClockAuction::SpectrumAuction::evaluateAssignmentRound()
{}

void ClockAuction::DynamicAuctionState::processInitialRoundBids(StaticAuctionState& sState, uint32_t auctionId)
{
    // missing bids
    fillMissingBids(sState, auctionId);

    // NOTE: missing bids are already set in startStart.
    // Such bids can be recognized by the auctionId = 0

    // set processed licenses
    processedLicenses_.resize(2); // add the processedLicenses_[round=1] vector
    processedLicenses_[1].resize(sState.getBiddersN());
    for(unsigned int i = 0; i < sState.getBiddersN(); i++)
    {
        processedLicenses_[1][i].resize(sState.getTerritoryN());

        for(unsigned int j = 0; j < clockBids_[1][i].demands_.size(); j++)
        {
            uint32_t demandedTerritoryId = clockBids_[1][i].demands_[j].territoryId_;
            uint32_t demandedQuantity = clockBids_[1][i].demands_[j].quantity_;
            processedLicenses_[1][i][sState.getTerritoryIndex(demandedTerritoryId)] = demandedQuantity;
        }
    }

    // set postedPrice to min/initial price
    postedPrice_.resize(2); // ensures postedPrice_[1] is defined
    postedPrice_[1] = sState.getInitialPrices();

    // aggregated demand (per territory)
    std::vector<uint32_t> aggregatedDemand(sState.getTerritoryN());
    for(unsigned int i = 0; i < aggregatedDemand.size(); i++)
    {
        aggregatedDemand[i] = 0;
        for(unsigned int j = 0; j < sState.getBiddersN(); j++)
        {
            aggregatedDemand[i] += processedLicenses_[1][j][i];
        }
    }

    // excess demand
    std::vector<uint32_t> supply = sState.getSupply();
    std::vector<int32_t> roundExcessDemand(sState.getTerritoryN());
    for(unsigned int i = 0; i < sState.getTerritoryN(); i++)
    {
        roundExcessDemand[i] = aggregatedDemand[i] - supply[i];
    }
    excessDemand_.push_back(roundExcessDemand);

    // final stage rule check
    std::vector<bool> highDemand = sState.getHighDemandVector();
    for(unsigned int i = 0; i < roundExcessDemand.size(); i++)
    {
        if(roundExcessDemand[i] < 0 && highDemand[i])
        {
            LOG_INFO("Final Stage Rule: FAILED - demand below supply for  %u-th high-demand territory");
            auctionState_ = FSR_FAILED;
        }
    }

    //check clock phase termination
    if(auctionState_ == CLOCK_PHASE)
    {
        if(*max_element(roundExcessDemand.begin(), roundExcessDemand.end()) <= 0)
        {
            LOG_INFO("Clock Phase: COMPLETED!");
            auctionState_ = ASSIGNMENT_PHASE;
        }
        else
        {
            LOG_INFO("Clock Phase: to be continued");

            // processed activity, required activity and new eligibility
            std::vector<uint32_t> processedActivity(sState.getBiddersN());
            std::vector<uint32_t> requiredActivity(sState.getBiddersN());
            std::vector<uint32_t> newRoundEligibility(sState.getBiddersN());
            for(unsigned int i = 0; i < sState.getBiddersN(); i++)
            {
                processedActivity[i] = 0;
                for(unsigned int j = 0; j < sState.getTerritoryN(); j++)
                {
                    processedActivity[i] += processedLicenses_[clockRound_][i][j];
                }
                requiredActivity[i] = eligibility_[clockRound_][i] * sState.getActivityRequirementPercentage() / 100;
                if(processedActivity[i] >= requiredActivity[i])
                {
                    newRoundEligibility[i] = eligibility_[clockRound_][i];
                }
                else
                {
                    newRoundEligibility[i] = processedActivity[i] * 100 / sState.getActivityRequirementPercentage();
                }
            }
            eligibility_.push_back(newRoundEligibility);

            // clock price
            std::vector<double> newRoundClockPrice = postedPrice_[clockRound_];
            for(unsigned int j = 0; j < sState.getTerritoryN(); j++)
            {
                newRoundClockPrice[j] *= (1 + ((double)sState.getClockPriceIncrementPercentage()/100));
            }
            clockPrice_.push_back(newRoundClockPrice);

            // new round
            clockRound_ ++;
        }
    }
}

void ClockAuction::DynamicAuctionState::processRegularRoundBids(StaticAuctionState& sState, uint32_t auctionIdCounter)
{
    // pre-set posted price
    std::vector<double> roundPostedPrice(sState.getTerritoryN());
    postedPrice_.push_back(roundPostedPrice);
    for(unsigned int i = 0; i < sState.getTerritoryN(); i++)
    {
        if(excessDemand_[clockRound_ -1][i] > 0)
        {
            postedPrice_[clockRound_][i] = postedPrice_[clockRound_ -1][i];
        }
        else
        {
            postedPrice_[clockRound_][i] = clockPrice_[clockRound_][i];
        }
    }
}

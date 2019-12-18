/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "error-codes.h"
#include "common.h"
#include "auction-state.h"
#include "utils.h"

typedef struct {
    std::string s;
} create_auction_t;

namespace ClockAuction
{
    class SpectrumAuctionMessage
    {
        private:
            const std::string inputJsonString_;
            std::string jsonString_;

            ErrorReport er_;


        public:
            SpectrumAuctionMessage();
            SpectrumAuctionMessage(const std::string& message);
            ErrorReport getErrorReport();

            std::string getJsonString();

            void toStatusJsonObject(JSON_Object* root_object, int rc, std::string& message);
            void toStatusJsonString(int rc, std::string& message, std::string& jsonString);

            void toCreateAuctionJson(int rc, std::string& message, unsigned int auctionId);
            bool fromCreateAuctionJson(StaticAuctionState& staticAuctionState);

            void toStaticAuctionStateJson(const StaticAuctionState& staticAuctionState);
            bool fromStaticAuctionStateJson(StaticAuctionState& staticAuctionState);

            void toDynamicAuctionStateJson(const DynamicAuctionState& dynamicAuctionState);
            bool fromDynamicAuctionStateJson(DynamicAuctionState& dynamicAuctionState);

            bool fromGetAuctionDetailsJson(uint32_t& auctionId);
            void toGetAuctionDetailsJson(int rc, std::string& message, const StaticAuctionState& staticAuctionState);

            bool fromStartNextRoundJson(uint32_t& auctionId);
            void toStartNextRoundJson(int rc, std::string& message);

            bool fromEndRoundJson(uint32_t& auctionId);
            void toEndRoundJson(int rc, std::string& message);

            bool fromGetAuctionStatusJson(uint32_t& auctionId);
            void toGetAuctionStatusJson(int rc, std::string& message, const DynamicAuctionState& dynamicAuctionState);

    };
}

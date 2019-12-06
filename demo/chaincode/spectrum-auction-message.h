/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "error-codes.h"
#include "common.h"
#include "auction-state.h"

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

            void toStatusJsonString(int rc, std::string& message, std::string& jsonString);

        public:
            SpectrumAuctionMessage();
            SpectrumAuctionMessage(const std::string& message);

            std::string getJsonString();

            void toCreateAuctionJson(int rc, std::string& message, unsigned int auctionId);
            bool fromCreateAuctionJson(StaticAuctionState& staticAuctionState);
    };
}

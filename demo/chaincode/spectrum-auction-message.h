/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "error-codes.h"
#include <string>

typedef struct {
    std::string s;
} create_auction_t;

namespace ClockAuction
{
    class SpectrumAuctionMessage
    {
        private:
            std::string jsonString_;

            void toStatusJsonString(int rc, std::string& message, std::string& jsonString);

        public:
            SpectrumAuctionMessage();
            std::string getJsonString();

            void toCreateAuctionJson(int rc, std::string& message, unsigned int auctionId);
            void toCreateAuctionStruct();
    };
}

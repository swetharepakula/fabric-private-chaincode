/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common.h"
#include "error-codes.h"
#include "storage.h"
#include "auction-state.h"

namespace ClockAuction
{
    class SpectrumAuction
    {
        private:
            ClockAuction::Storage auctionStorage_;
            uint32_t auctionIdCounter_;

            void InitializeAuctionIdCounter();
            void StoreAuctionIdCounter();

            StaticAuctionState staticAuctionState;
            DynamicAuctionState dynamicAuctionState;

        public:
            SpectrumAuction(shim_ctx_ptr_t ctx);

            int createAuction(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
            int getAuctionDetails(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
    };
}

typedef int (ClockAuction::SpectrumAuction::*spectrumAuctionFunctionP)(const std::string&, std::string&, ClockAuction::ErrorReport&);

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
            uint32_t auctionIdCounter_;
            void InitializeAuctionIdCounter();
            void IncrementAndStoreAuctionIdCounter();

            StaticAuctionState staticAuctionState_;
            DynamicAuctionState dynamicAuctionState_;
            ClockAuction::Storage auctionStorage_;

            void storeAuctionState();
            bool loadAuctionState();

            void evaluateClockRound();
            void evaluateAssignmentRound();

        public:
            SpectrumAuction(shim_ctx_ptr_t ctx);
            ErrorReport er_;

            bool createAuction(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
            bool getAuctionDetails(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
            bool getAuctionStatus(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
            bool startNextRound(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
            bool endRound(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
    };
}

typedef bool (ClockAuction::SpectrumAuction::*spectrumAuctionFunctionP)(const std::string&, std::string&, ClockAuction::ErrorReport&);

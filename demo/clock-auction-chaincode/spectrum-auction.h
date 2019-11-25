/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include "error-codes.h"

namespace ClockAuction
{
    class SpectrumAuction
    {
        private:

        public:
            SpectrumAuction();

            int createAuction(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
            int getAuctionDetails(const std::string& inputString, std::string& outputString, ClockAuction::ErrorReport& er);
    };
}

typedef int (ClockAuction::SpectrumAuction::*spectrumAuctionFunctionP)(const std::string&, std::string&, ClockAuction::ErrorReport&);

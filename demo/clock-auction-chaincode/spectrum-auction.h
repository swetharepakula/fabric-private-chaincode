/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include "error_codes.h"

typedef int (*spectrumAuctionFunctionP)(std::string&);

namespace ClockAuction
{
    class SpectrumAuction
    {
        private:

        public:
            SpectrumAuction();
            void getAuctionDetails(std::string& inputString, std::string& outputString, ErrorReport& er);
    }
}

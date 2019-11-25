/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include <vector>
#include <map>
#include "spectrum-auction.h"

namespace ClockAuction
{
    class Dispatcher
    {
        private:
            const std::string functionName_;
            const std::vector<std::string> functionParameters_;
            uint8_t* response_;
            const uint32_t max_response_len_;
            uint32_t* actual_response_len_;

            std::string responseString_;

            //static std::map<std::string, spectrumAuctionFunctionP> fMap_;
            ClockAuction::SpectrumAuction spectrumAuction_;
            ClockAuction::ErrorReport errorReport_;
                        

        public:
            Dispatcher(const std::string& functionName,
                    const std::vector<std::string>& functionParameters,
                    uint8_t* response,
                    const uint32_t max_response_len,
                    uint32_t* actual_response_len);

    };
}

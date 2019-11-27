/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common.h"
#include "error-codes.h"

namespace ClockAuction 
{
    class Channel
    {
        private:
            uint32_t id_;
            std::string name_;
            uint32_t impairment_;

            ErrorReport er_;

        public:
            Channel();
            Channel(uint32_t id, std::string& name, uint32_t impairment);
            bool toJson(std::string& jsonString);
            bool fromJson(const std::string& jsonString);
    };

    class Territory
    {
        private:
            uint32_t id_;
            std::string name_;
            bool isHighDemand_;
            double minPrice_;
            std::vector<Channel> channels_;
            
            ErrorReport er_;

        public:
            Territory();
            Territory(uint32_t id, std::string& name, bool isHighDemand, double minPrice, std::vector<Channel>& channels);
            bool toJson(std::string& jsonString);
            bool fromJson(const std::string& jsonString);
    };

    //class Territories
    //{
    //    std::vector<Territory> territories;
    //};
}

/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "error-codes.h"

namespace ClockAuction
{
    class Eligibility
    {
        private:
            uint32_t bidderId_;
            uint32_t number_;
            ErrorReport er_;
        public:
            //Eligibility();
            //Eligibility(uint32_t bidderId, uint32_t number);
            bool toJsonObject(JSON_Object* root_object) const;
            bool fromJsonObject(const JSON_Object* root_object);
    };
}

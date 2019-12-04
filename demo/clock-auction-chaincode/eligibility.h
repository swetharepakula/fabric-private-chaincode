/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ClockAuction
{
    class Eligibility
    {
        uint32_t bidderId_;
        uint32_t number_;

        Eligibility(uint32_t bidderId, uint32_t number);
    };
}

/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "common.h"
#include "eligibility.h"

//ClockAuction::Eligibility::Eligibility(uint32_t bidderId, uint32_t number) : bidderId_(bidderId), number_(number)
//{
//    // TODO: change constructor to include a bidder-checking function
//}

bool ClockAuction::Eligibility::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_number(root_object, "bidderId", bidderId_);
    json_object_set_number(root_object, "number", number_);
    return true;
}

bool ClockAuction::Eligibility::fromJsonObject(const JSON_Object* root_object)
{
    {
        bidderId_ = json_object_get_number(root_object, "bidderId");
        if(bidderId_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
    }
    {
        number_ = json_object_get_number(root_object, "number");
        if(number_ == 0)
        {
            er_.set(EC_INVALID_INPUT, "line: " + std::to_string(__LINE__));
            return false;
        }
    }
    return true;
}


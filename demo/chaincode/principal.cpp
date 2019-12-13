/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "principal.h"

ClockAuction::Principal::Principal()
{}

ClockAuction::Principal::Principal(std::string m, std::string d) : mspId_(m), dn_(d)
{}

bool ClockAuction::Principal::toJsonObject(JSON_Object* root_object) const
{
    json_object_set_string(root_object, "mspId", mspId_.c_str());
    json_object_set_string(root_object, "dn", dn_.c_str());
    return true;
}

bool ClockAuction::Principal::fromJsonObject(const JSON_Object* root_object)
{
    //{
    //    id_ = json_object_get_number(root_object, "id");
    //    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, id_ == 0);
    //}
    {
        const char* str = json_object_get_string(root_object, "mspId");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, str == 0);
        mspId_ = std::string(str);
    }
    {
        const char* str = json_object_get_string(root_object, "dn");
        FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, str == 0);
        dn_ = std::string(str);
    }
    //{
    //    const char* str = json_object_get_string(root_object, "name");
    //    FAST_FAIL_CHECK(er_, EC_INVALID_INPUT, str == 0);
    //    name_ = std::string(str);
    //}
    return true;
}


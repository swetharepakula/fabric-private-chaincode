/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>

void ClockAuction::ErrorReport::set(error_codes_e, std::string& s)
{
    ec_ = e;
    errorString_ = s;
}

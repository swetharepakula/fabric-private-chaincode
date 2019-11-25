/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>
#include "error-codes.h"

ClockAuction::ErrorReport::ErrorReport()
{}

void ClockAuction::ErrorReport::set(error_codes_e e, std::string& s)
{
    ec_ = e;
    errorString_ = s;
}

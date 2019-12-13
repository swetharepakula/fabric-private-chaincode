/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common.h"
#include "principal.h"
#include "error-codes.h"

namespace ClockAuction
{
    class Bidder
    {
        private:
            uint32_t id_;
            std::string displayName_;
            Principal principal_;
        public:
            bool toJsonObject(JSON_Object* root_object) const;
            bool fromJsonObject(const JSON_Object* root_object);
            ErrorReport er_;
    };
}

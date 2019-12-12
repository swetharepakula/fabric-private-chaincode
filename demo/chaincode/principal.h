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
    class Principal
    {
        private:
            uint32_t id_;
            std::string mspId_;
            std::string dn_;
            std::string name_;

            ErrorReport er_;

        public:
            //Principal(uint32_t id, std::string& mspId, std::string& dn, std::string& name);
            //bool toJson(std::string& jsonString);
            //bool fromJson(std::string& jsonString);
            bool toJsonObject(JSON_Object* root_object) const;
            bool fromJsonObject(const JSON_Object* root_object);
    };
}

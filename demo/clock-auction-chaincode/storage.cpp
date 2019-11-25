/*
 * Copyright 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"
#include "storage.h"

ClockAuction::Storage::Storage(shim_ctx_ptr_t ctx) : ctx_(ctx)
{}

void ClockAuction::Storage::ledgerPrivatePutString(const std::string& key, const std::string& value)
{
    ledgerPrivatePutBinary((const uint8_t*)key.c_str(), key.length(), (const uint8_t*)value.c_str(), value.length());
}

void ClockAuction::Storage::ledgerPrivateGetString(const std::string& key, std::string& value)
{
    uint32_t actualValueLength = 0;
    value.clear();

    //try the fast path
    ledgerPrivateGetBinary((const uint8_t*)key.c_str(), key.length(), (uint8_t*)value.c_str(), value.capacity(), &actualValueLength);
    if(actualValueLength > 0) //string retrieved, return
        return;
    
    //slow path: zero string means either no value, or value is larger than current capacity
    int power = 0;
    while(value.capacity() > (1 << power++));
    for(; power < 17; power++) //keep doubling capacity up to 2^17 (128K)
    {
        uint32_t newCapacity = (1 << power);
        LOG_DEBUG("retry get with capacity %u", newCapacity);
        try
        {
            value.reserve(newCapacity);
        }
        catch(...)
        {
            LOG_ERROR("capacity change (%u) failed, assuming no key available", newCapacity);
            return;
        }

        ledgerPrivateGetBinary((const uint8_t*)key.c_str(), key.length(), (uint8_t*)value.c_str(), value.capacity(), &actualValueLength);
        if(actualValueLength > 0) //string retrieved, return
            return;
    }
}

void ClockAuction::Storage::ledgerPrivatePutBinary(const uint8_t* key, const uint32_t keyLength, const uint8_t* value, const uint32_t valueLength)
{
    uint8_t* pvalue = (uint8_t*)value; //TODO: fix drop const
    put_state((const char*)key, pvalue, valueLength, ctx_);
}

void ClockAuction::Storage::ledgerPrivateGetBinary(const uint8_t* key, const uint32_t keyLength, uint8_t* value, const uint32_t valueLength, uint32_t* actualValueLength)
{
    get_state((const char*)key, value, valueLength, actualValueLength, ctx_);
}


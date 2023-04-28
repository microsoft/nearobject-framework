// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "Utils.hxx"
#include "pch.h"

uint32_t
HstringToUint32(winrt::hstring hstring)
{
    return static_cast<uint32_t>(std::stoi(winrt::to_string(hstring)));
}

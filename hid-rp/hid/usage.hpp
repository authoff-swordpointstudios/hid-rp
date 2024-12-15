/// @file
///
/// @author Benedek Kupper
/// @date   2022
///
/// @copyright
///         This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
///         If a copy of the MPL was not distributed with this file, You can obtain one at
///         https://mozilla.org/MPL/2.0/.
///
#ifndef __HID_USAGE_HPP_
#define __HID_USAGE_HPP_
#if __cplusplus < 201703L
    #error "This code requires C++17 or higher. Please upgrade your compiler."
#elif __cplusplus >= 202002L
    #include "hid/platform/c++20/usage.hpp" // C++20 or higher
#else
    #include "hid/platform/c++17/usage.hpp" // C++17
#endif
#endif // __HID_RDF_USAGE_HPP_

/*
 Copyright 2025 Ivan Somov

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#pragma once

namespace kopter {

/**
 * @brief Utility functions for calculating CRC checksums.
 */
struct CRCUtils {
    /**
     * @brief Computes the CRC16-CCITT checksum over a byte buffer.
     *
     * This implementation uses the CRC-CCITT (XModem) polynomial 0x1021 with a configurable seed.
     * The function processes the input data byte by byte and returns a 16-bit CRC value.
     *
     * @param data Pointer to the input buffer.
     * @param length Number of bytes in the input buffer.
     * @param seed Initial CRC value. Default is 0xFFFF.
     * @return uint16_t The computed CRC16 checksum.
     */
    static uint16_t crc16_ccitt(const uint8_t *data, size_t length, uint16_t seed = 0xFFFF);
};

} // namespace kopter
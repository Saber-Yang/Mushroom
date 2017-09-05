/**
 *    > Author:        UncP
 *    > Github:  www.github.com/UncP/Mushroom
 *    > License:      BSD-3
 *    > Time:    2017-09-05 16:28:54
**/

#include <cstring>

#include "../include/utility.hpp"

namespace Mushroom {

inline uint32_t DecodeFixed32(const char* ptr) {
  uint32_t result;
  memcpy(&result, ptr, sizeof(result));
  return result;
}

inline void EncodeFixed32(char* buf, uint32_t value) {
  memcpy(buf, &value, sizeof(value));
}

uint32_t Hash(const char* data, size_t n, uint32_t seed) {
  // Similar to murmur hash
  const uint32_t m = 0xc6a4a793;
  const uint32_t r = 24;
  const char* limit = data + n;
  uint32_t h = static_cast<uint32_t>(seed ^ (n * m));

  // Pick up four bytes at a time
  while (data + 4 <= limit) {
    uint32_t w = DecodeFixed32(data);
    data += 4;
    h += w;
    h *= m;
    h ^= (h >> 16);
  }

  // Pick up remaining bytes
  switch (limit - data) {
    // Note: The original hash implementation used data[i] << shift, which
    // promotes the char to int and then performs the shift. If the char is
    // negative, the shift is undefined behavior in C++. The hash algorithm is
    // part of the format definition, so we cannot change it; to obtain the same
    // behavior in a legal way we just cast to uint32_t, which will do
    // sign-extension. To guarantee compatibility with architectures where chars
    // are unsigned we first cast the char to int8_t.
    case 3:
      h += static_cast<uint32_t>(static_cast<int8_t>(data[2])) << 16;
    // fall through
    case 2:
      h += static_cast<uint32_t>(static_cast<int8_t>(data[1])) << 8;
    // fall through
    case 1:
      h += static_cast<uint32_t>(static_cast<int8_t>(data[0]));
      h *= m;
      h ^= (h >> r);
      break;
  }
  return h;
}

} // namespace Mushroom
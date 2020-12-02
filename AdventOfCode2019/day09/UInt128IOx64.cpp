// File:UInt128IOx64.cpp
#include "stdafx.h"

#ifdef _M_X64

#include "int128.h"

static const _uint128 _0(0);
static const _uint128 _10(10);
static const _uint128 _16(16);
static const _uint128 _8(16);

char*_ui128toa(_uint128 value, char *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  char *s = str;
  const _uint128 r(radix);
  while (value != _0) {
    const unsigned int c = value % r;
    *(s++) = radixLetter(c);
    value /= r;
  }
  if (s == str) {
    return strcpy(str, "0");
  }
  else {
    *s = 0;
    return _strrev(str);
  }
}

wchar_t *_ui128tow(_uint128 value, wchar_t *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  wchar_t *s = str;
  const _uint128 r(radix);
  while (value != _0) {
    const unsigned int c = value % r;
    *(s++) = wradixLetter(c);
    value /= r;
  }
  if (s == str) {
    return wcscpy(str, L"0");
  }
  else {
    *s = 0;
    return _wcsrev(str);
  }
}

const char *_uint128::parseDec(const char *str) {
  *this = 0;
  while (isdigit(*str)) {
    const unsigned int d = *(str++) - '0';
    *this *= _10;
    *this += d;
  }
  return str;
}

const char *_uint128::parseHex(const char *str) {
  *this = 0;
  while (isxdigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _16;
    *this += d;
  }
  return str;
}

const char *_uint128::parseOct(const char *str) {
  *this = 0;
  while (isodigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _8;
    *this += d;
  }
  return str;
}

_uint128::_uint128(const char *str) {
  assert(isdigit(*str));
  if (*str == '0') {
    switch (str[1]) {
    case 'x':
      parseHex(str + 2);
      break;
    case 0:
      *this = 0;
      break;
    default:
      parseOct(str + 1);
      break;
    }
  }
  else {
    parseDec(str);
  }
}

#endif // _M_X64


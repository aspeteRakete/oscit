/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef OSCIT_INCLUDE_OSCIT_HASH_VALUE_H_
#define OSCIT_INCLUDE_OSCIT_HASH_VALUE_H_
#include "oscit/value.h"

namespace oscit {

/** HashValue is just a wrapper around an immutable const char*.*/
class HashValue : public Value
{
 public:
  HashValue() : Value(TypeTag("H")) {}

  /** Create a new HashValue with an initial key/value pair.
   */
  HashValue(const char *key, const Value &value) : Value(TypeTag("H")) {
    set(key, value);
  }

  /** Create a new HashValue with an initial key/value pair.
   */
  HashValue(const std::string &key, const Value &value) : Value(TypeTag("H")) {
    set(key, value);
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const Value &value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, value));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const Value &value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, value)));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *key4, const Value &value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, HashValue(key4, value))));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, const Value &value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, HashValue(key4, HashValue(key5, value)))));
  }

  // FIXME: we can remove the ctors below if we remove explicit in Value ctor (after audit).


  /** Create a new HashValue with an initial key/value pair.
   */
  HashValue(const char *key, const char *value) : Value(TypeTag("H")) {
    set(key, Value(value));
  }

  /** Create a new HashValue with an initial key/value pair.
   */
  HashValue(const std::string &key, const char *value) : Value(TypeTag("H")) {
    set(key, value);
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, value));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, value)));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *key4, const char *value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, HashValue(key4, value))));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, const char *value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, HashValue(key4, HashValue(key5, value)))));
  }

  /** Create a new HashValue with an initial key/value pair.
   */
  HashValue(const char *key, Real value) : Value(TypeTag("H")) {
    set(key, Value(value));
  }

  /** Create a new HashValue with an initial key/value pair.
   */
  HashValue(const std::string &key, Real value) : Value(TypeTag("H")) {
    set(key, value);
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, Real value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, value));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, Real value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, value)));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *key4, Real value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, HashValue(key4, value))));
  }

  /** Create a new HashValue with a list of initial key/value pair.
   */
  HashValue(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, Real value) : Value(TypeTag("H")) {
    set(key1, HashValue(key2, HashValue(key3, HashValue(key4, HashValue(key5, value)))));
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_HASH_VALUE_H_

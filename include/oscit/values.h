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

#ifndef OSCIT_INCLUDE_OSCIT_VALUES_H_
#define OSCIT_INCLUDE_OSCIT_VALUES_H_

#include "oscit/conf.h"
#include "oscit/thash.h"
#include "oscit/value.h"
#include "oscit/list_value.h"
#include "oscit/real_value.h"
#include "oscit/string_value.h"
#include "oscit/error_value.h"
#include "oscit/hash_value.h"
#include "oscit/matrix_value.h"
#include "oscit/midi_value.h"

// FIXME: replace by enums in oscit namespace

#define NoIO(info)  Value(info)
#define NoIOTypeId STRING_TYPE_TAG_ID

#define BangIO(info)  ListValue("T").push_back(info)
#define BangIOTypeId H("Ts")

#define RealIO(info) Value(0.0).push_back(info)
#define RealIOTypeId H("fs")

#define StringIO(info) Value("").push_back(info)
#define StringIOTypeId H("ss")

#define RangeIO(min, max, info) \
  Value(0.0).push_back(min).push_back(max).push_back(info)
#define RangeIOTypeId H("fffs")

#define SelectIO(values, info) \
  Value("").push_back(values).push_back(info)
#define SelectIOTypeId H("sss")

#define HashIO(info) Value('H').push_back(info)
#define HashIOTypeId H("Hs")

#define MatrixIO(info) MatrixValue().push_back(info)
#define MatrixIOTypeId H("Ms")

#define MidiIO(info) MidiValue().push_back(info)
#define MidiIOTypeId H("ms")

#define AnyIO(info) Value('*').push_back(info)
#define AnyIOTypeId H("*s")


#endif // OSCIT_INCLUDE_OSCIT_VALUES_H_

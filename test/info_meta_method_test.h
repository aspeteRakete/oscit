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

#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class InfoMetaMethodTest : public TestHelper
{
public:
  void test_info( void ) {
    Root root(Oscit::no_io("This is the root node."));
    root.adopt(new Object("foo"));
    Value res;

    res = root.call(INFO_PATH, Value(""));
    assert_true(res.is_list());
    assert_equal("", res[0].str());
    assert_equal("This is the root node.", res[1].str());

    res = root.call(INFO_PATH, Value("/foo"));
    assert_true(res.is_list());
    assert_equal("/foo", res[0].str());
    assert_equal(Oscit::default_io()[Oscit::INFO].str(), res[1].str());

    res = root.call(INFO_PATH, Value("/blah"));
    assert_equal("/blah", res[0].str());
    assert_true(res[1].is_error());
    assert_equal(NOT_FOUND_ERROR, res[1].error_code());
  }

  void test_info_with_nil( void ) {
    Root root(Oscit::no_io("This is the root node."));
    Value res;

    res = root.call(INFO_PATH, gNilValue);
    assert_true(res.is_nil());
  }
};
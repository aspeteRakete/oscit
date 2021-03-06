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
#include "oscit/values.h"

class ErrorValueTest : public TestHelper
{
public:
  void test_create( void ) {
    Value v(BAD_REQUEST_ERROR, "foo");

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_true());
    assert_false(v.is_false());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_true (v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_midi());
    assert_false(v.is_any());

    assert_equal("foo", v.error_message());
    assert_equal(BAD_REQUEST_ERROR, v.error_code());

    assert_equal("E", v.type_tag());
    assert_equal(ERROR_TYPE_TAG_ID, v.type_id());

    assert_equal(ERROR_TYPE_TAG_ID, hashId("E"));

  }

  void test_create_error_value( void ) {
    ErrorValue v(BAD_REQUEST_ERROR, "bad");
    ErrorValue v2;

    assert_true(v.is_error());
    assert_true(v2.is_error());


    assert_equal("bad", v.error_message());
    assert_equal("unknown error", v2.error_message());
    assert_equal(UNKNOWN_ERROR, v2.error_code());
  }

  void test_create_error_value_std_string( void ) {
    ErrorValue v(BAD_REQUEST_ERROR, std::string("bad"));
    Value v2(BAD_REQUEST_ERROR, std::string("bad2"));

    assert_true(v.is_error());
    assert_true(v2.is_error());


    assert_equal("bad", v.error_message());
    assert_equal("bad2", v2.error_message());
    assert_equal(BAD_REQUEST_ERROR, v.error_code());
    assert_equal(BAD_REQUEST_ERROR, v2.error_code());
  }

  void test_copy( void ) {
    Value v(BAD_REQUEST_ERROR, "foo");
    Value v2(v);
    Value v3;

    assert_true(v2.is_error());

    v3 = v;

    assert_true(v3.is_error());

    assert_equal("foo", v2.error_message());
    assert_equal(BAD_REQUEST_ERROR, v2.error_code());
    assert_equal("foo", v3.error_message());
    assert_equal(BAD_REQUEST_ERROR, v3.error_code());

    v.error_->set_message("bar");
    assert_equal("bar", v.error_message());
    assert_equal("bar", v2.error_message());
    assert_equal("bar", v3.error_message());
  }

  void test_set( void ) {
    Value v;

    v.set(BAD_REQUEST_ERROR, "foobar");
    assert_true(v.is_error());
    assert_equal("foobar", v.error_message());
  }

  void test_append( void ) {
    ErrorValue v(BAD_REQUEST_ERROR, "bzz");
    assert_equal("bzz", v.error_message());
    v.append(" says the bee");
    assert_equal("bzz says the bee", v.error_message());
  }

  // set_type_tag does not exist for errors.
  // void test_set_type_tag( void ) {}

  void test_set_type( void ) {
    Value v;

    v.set_type(ERROR_VALUE);
    assert_true(v.is_error());
    assert_equal(UNKNOWN_ERROR, v.error_code());
  }

  void test_to_json( void ) {
    Value v(NOT_FOUND_ERROR, "banana");
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("\"404 banana\"", os.str());
    assert_equal("\"404 banana\"", v.to_json());
  }

  void test_can_receive( void ) {
    Object object("foo", HashValue(Oscit::TYPE, Oscit::SIGNATURE, "E").set(Oscit::INFO, "Receives error values."));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_true (object.can_receive(gBangValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_true (object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_false(object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
    assert_false(object.can_receive(MidiValue()));
  }

  void test_equal( void ) {
    Value a(NOT_FOUND_ERROR, "my keys");
    Value b(NOT_FOUND_ERROR, "my keys");
    assert_equal(a, b);
    a.error_->set_message("my health");
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }

  void test_stream_char( void ) {
    Value s(BAD_REQUEST_ERROR, "Hello");
    s << " " << "World!";
    assert_equal("Hello World!", s.error_message());
  }

  void test_stream_ints( void ) {
    Value s(BAD_REQUEST_ERROR, "");
    s << 44 << " " << 55;
    assert_equal("44 55", s.error_message());
  }

  void test_stream_double( void ) {
    Value s(BAD_REQUEST_ERROR, "");
    s << 4.4 << " " << 55;
    assert_equal("4.4 55", s.error_message());
  }

  void test_stream_other_value( void ) {
    Value s(BAD_REQUEST_ERROR, "");
    Value list(Json("[1,2,3]"));
    s << 4.4 << " " << list;
    assert_equal("4.4 [1, 2, 3]", s.error_message());
  }

  void test_create_varargs( void ) {
    FValue s(BAD_REQUEST_ERROR, "I am %i not '%s'.", 1337, "Superman");
    assert_equal("I am 1337 not 'Superman'.", s.error_message());
  }

  void test_contains_error( void ) {
    Value e(Json("hop"));
    assert_false(e.contains_error());
    e = ErrorValue(BAD_REQUEST_ERROR, "some message");
    assert_true(e.contains_error());
    e = HashValue("some key", "deep", "inside", 45.4);
    assert_false(e.contains_error());

    e = JsonValue("[1,2,3]");
    e.push_back(HashValue("some key", "deep", "inside", ErrorValue(BAD_REQUEST_ERROR, "some message")));
    assert_true(e.contains_error());
  }
};
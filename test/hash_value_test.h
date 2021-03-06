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

class HashValueTest : public TestHelper
{
public:
  void test_is_hash( void ) {
    Value v(TypeTag("H"));
    Value res;

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_true());
    assert_false(v.is_false());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_true (v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_midi());

    assert_false(v.get("foo", &res));
    v.set("foo",3.5);
    assert_true(v.get("foo", &res));
    assert_true(res.is_real());
    assert_equal(3.5, res.r);

    assert_equal("H", v.type_tag());
    assert_equal(HASH_TYPE_TAG_ID, v.type_id());

    assert_equal(HASH_TYPE_TAG_ID, hashId("H"));
  }

  void test_create_set( void ) {
    HashValue v;

    assert_true(v.is_hash());

    assert_equal("H", v.type_tag());
  }

  void test_create_key_value( void ) {
    HashValue v("do", Value(261.626));
    assert_equal("{\"do\":261.626}", v.to_json());
  }

  void test_create_nested_keys( void ) {
    HashValue v2("do", "re", Value(293.665));
    HashValue v3("do", "re", "mi", Value(329.628));
    HashValue v4("do", "re", "mi", "fa", Value(349.228));
    HashValue v5("do", "re", "mi", "fa", "sol", Value(391.995));

    assert_equal("{\"do\":{\"re\":293.665}}", v2.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":329.628}}}", v3.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":{\"fa\":349.228}}}}", v4.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":{\"fa\":{\"sol\":391.995}}}}}", v5.to_json());
  }

  void test_create_nested_keys_real( void ) {
    HashValue v1("do", 261.626);
    HashValue v2("do", "re", 293.665);
    HashValue v3("do", "re", "mi", 329.628);
    HashValue v4("do", "re", "mi", "fa", 349.228);
    HashValue v5("do", "re", "mi", "fa", "sol", 391.995);

    assert_equal("{\"do\":261.626}", v1.to_json());
    assert_equal("{\"do\":{\"re\":293.665}}", v2.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":329.628}}}", v3.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":{\"fa\":349.228}}}}", v4.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":{\"fa\":{\"sol\":391.995}}}}}", v5.to_json());
  }

  void test_create_nested_keys_string( void ) {
    HashValue v1("do", "C");
    HashValue v2("do", "re", "D");
    HashValue v3("do", "re", "mi", "E");
    HashValue v4("do", "re", "mi", "fa", "F");
    HashValue v5("do", "re", "mi", "fa", "sol", "G");

    assert_equal("{\"do\":\"C\"}", v1.to_json());
    assert_equal("{\"do\":{\"re\":\"D\"}}", v2.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":\"E\"}}}", v3.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":{\"fa\":\"F\"}}}}", v4.to_json());
    assert_equal("{\"do\":{\"re\":{\"mi\":{\"fa\":{\"sol\":\"G\"}}}}}", v5.to_json());
  }

  void test_set( void ) {
    Value v;

    v.set("nice", "friends").set("feared", "enemy");

    assert_true(v.is_hash());
    assert_equal("enemy", v["feared"].str());
  }

  void test_set_tag( void ) {
    Value v;

    v.set_type_tag("H");
    assert_true(v.is_hash());
  }

  void test_set_type( void ) {
    Value v;

    v.set_type(HASH_VALUE);
    assert_true(v.is_hash());
  }

  void test_key_iterator( void ) {
    Value v;
    Value res;

    HashIterator it  = v.begin();
    HashIterator end = v.end();

    assert_true(it == end);

    v.set("a", 1);
    v.set("c", 3);
    v.set("b", 2);

    it  = v.begin();
    end = v.end();

    while( it != end ) {
      std::string key(*it++);
      assert_true(v.get(key, &res));
      assert_true(res.is_real());
      if (key == "a") {
        assert_equal(1.0, res.r);
      } else if (key == "b") {
        assert_equal(2.0, res.r);
      } else if (key == "c") {
        assert_equal(3.0, res.r);
      } else {
        assert_equal("wrong key !", key);
      }
    }
  }

  void test_read( void ) {
    Value v('H');
    Hash &l = *v.hash_;
    Value res;

    assert_false(v.get("one", &res));

    l.set("one", Value(1.0));

    assert_true(v.get("one", &res));
    assert_equal(1.0, res.r);
  }

  void test_set_real( void ) {
    Value v;
    v.set("one", 1.34);
    assert_true(v.is_hash());
    assert_equal(1.34, v["one"].r); // this is a dangerous syntax: v["real"] can return gNilValue...

    v.set("two", 3.45);
    assert_true(v["two"].is_real());
  }

  void test_set_string( void ) {
    Value v(1.0);
    v.set("one", "first");
    assert_true(v.is_hash());
    assert_true(v["one"].is_string());
    assert_equal("first", v["one"].str());
  }

  void test_set_list( void ) {
    Value v;
    Value l;
    l.set("one").push_back(2.0);
    v.set("list", l);
    assert_true(v.is_hash());

    assert_true(v["list"].is_list());
    assert_equal(2, v["list"].size());

    assert_true(v["list"][0].is_string());
    assert_equal("one", v["list"][0].str());

    assert_true(v["list"][1].is_real());
    assert_equal(2.0, v["list"][1].r);
  }

  void test_set_hash( void ) {
    Value v;
    Hash h(20);
    h.set("one", Value(1.0));
    v.set(h);
    assert_true(v.is_hash());
    assert_equal(1.0, v["one"].r);
    h.set("one", Value(2.0));
    // copy, not sharing
    assert_equal(1.0, v["one"].r);
  }

  void test_to_json( void ) {
    Value v;
    Value jobs;
    v.set("name", "Joe");
    v.set("age", 34.0);
    jobs.set("dad").push_back("husband").push_back("lover").push_back(-666);
    v.set("job", jobs);

    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("{\"name\":\"Joe\", \"age\":34, \"job\":[\"dad\", \"husband\", \"lover\", -666]}", os.str());
    assert_equal("{\"name\":\"Joe\", \"age\":34, \"job\":[\"dad\", \"husband\", \"lover\", -666]}", v.to_json());
  }

  void test_lazy_json( void ) {
    Value v;
    Value jobs;
    v.set("name", "Joe");
    v.set("age", 34.0);
    jobs.set("dad").push_back("husband").push_back("lover").push_back(-666);
    v.set("job", jobs);

    assert_equal("name:\"Joe\" age:34 job:[\"dad\", \"husband\", \"lover\", -666]", v.lazy_json());
  }

  void test_from_json_strict( void ) {
    Value v(Json("{\"week_start_on\":1.0, monday:\"Lundi\"}"));
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_equal("Lundi", v["monday"].str());
  }

  void test_from_json_lazy( void ) {
    Value v(Json("week_start_on:1.0 monday:\"Lundi\""));
    assert_equal("{\"week_start_on\":1, \"monday\":\"Lundi\"}", v.to_json());
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_equal("Lundi", v["monday"].str());
  }

  void test_from_json_hash_in_hash( void ) {
    Value v(Json("week_start_on:1.0 weeks:{monday:\"Lundi\" friday:\"free\"}"));
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_equal("Lundi", v["weeks"]["monday"].str());
  }

  void test_from_json_hash_in_hash_curly( void ) {
    Value v(Json("{week_start_on:1.0 weeks:{monday:\"Lundi\" friday:\"free\"}}"));
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_true(v["weeks"].is_hash());
    assert_equal("free", v["weeks"]["friday"].str());
  }

  void test_from_json_empty_hash( void ) {
    Value v(Json("{}"));
    assert_true(v.is_hash());
  }

  void test_from_json_empty_key( void ) {
    Value v(Json("{\"\":null}"));
    assert_true(v.is_hash());
  }

  void test_can_receive( void ) {
    Object object("foo", Oscit::hash_io("bar"));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_true (object.can_receive(gBangValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_false(object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_true (object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
    assert_false(object.can_receive(MidiValue()));
  }

  void test_equal( void ) {
    Value a(Json("one: 1 two: 2"));
    Value b(Json("two: 2 one: 1"));
    assert_equal(a, b);
    a.set(Json("one: 1 two: 3"));
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }

  void test_deep_merge( void ) {
    Value a(Json("{one:{x:4 y:10} two:{x:5 y:8}}"));
    Value b(Json("{one:{x:7} two:null}"));
    a.deep_merge(b);
    assert_equal("{\"one\":{\"x\":7, \"y\":10}}", a.to_json());
  }

  void test_deep_merge_on_copy( void ) {
    Value a(Json("{one:{x:4 y:10} two:{x:5 y:8}}"));
    Value b(Json("{one:{x:7} two:null}"));
    Value c(a.to_json()); // this is how we make a copy
    c.deep_merge(b);
    assert_equal("{\"one\":{\"x\":4, \"y\":10}, \"two\":{\"x\":5, \"y\":8}}", a.to_json());
    assert_equal("{\"one\":{\"x\":7, \"y\":10}}", c.to_json());
  }

  void test_copy_on_write( void ) {
    JsonValue original("{one:1 two:2 nested:{a:1 b:2}}");
    Hash *hash_ptr = original.hash_;

    Value copy(original);

    // the hash is shared
    assert_equal(hash_ptr, copy.hash_);
    assert_equal(hash_ptr, original.hash_);

    // this should force "original" to have a new version
    original.set("one", Value(3));
    assert_true(hash_ptr != original.hash_);

    assert_equal(hash_ptr, copy.hash_);

    assert_equal("{\"one\":3, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", original.to_json());
    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", copy.to_json());

    assert_equal(original["nested"].hash_, copy["nested"].hash_);
  }

  void test_cow_remove( void ) {
    JsonValue original("{one:1 two:2 nested:{a:1 b:2}}");
    Value copy(original);

    // this should force "original" to have a new version
    original.remove("one");

    assert_equal("{\"two\":2, \"nested\":{\"a\":1, \"b\":2}}", original.to_json());
    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", copy.to_json());

    assert_equal(original["nested"].hash_, copy["nested"].hash_);
  }

  void test_cow_deep_merge( void ) {
    JsonValue original("{one:1 two:2 nested:{a:1 b:2}}");
    Value copy(original);

    // this should force "original" to have a new version
    original.deep_merge(JsonValue("{nested:{c:4}}"));

    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":2, \"c\":4}}", original.to_json());
    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", copy.to_json());
  }

  void test_copy_from_operator_equal( void ) {
    JsonValue original("{one:1 two:2 nested:{a:1 b:2}}");
    Hash *hash_ptr = original.hash_;

    Value copy;
    copy = original;

    // the hash is shared
    assert_equal(hash_ptr, copy.hash_);
    assert_equal(hash_ptr, original.hash_);

    // this should force "copy" to have a new version
    copy.set("one", Value(3));
    assert_true(hash_ptr != copy.hash_);

    assert_equal(hash_ptr, original.hash_);

    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", original.to_json());
    assert_equal("{\"one\":3, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", copy.to_json());

    assert_equal(original["nested"].hash_, copy["nested"].hash_);
  }

  void test_write_deeply_nested( void ) {
    JsonValue original("{one:1 two:2 nested:{a:1 b:2}}");
    Value copy(original);

    // this should force "original" to have a new version of 'nested' hash
    original.set("nested", "b", Value(3));

    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":3}}", original.to_json());
    assert_equal("{\"one\":1, \"two\":2, \"nested\":{\"a\":1, \"b\":2}}", copy.to_json());
  }

  void test_integer_key( void ) {
    Value a(Json("1:\"one\" two: 2"));
    assert_equal("{\"1\":\"one\", \"two\":2}", a.to_json());
  }

  void test_has_key( void ) {
    Value a(Json("one: 1 two: 2"));
    Value b;
    assert_false(b.has_key("one"));

    assert_true(a.has_key("one"));
    assert_false(a.has_key("four"));

    assert_false(b.has_key(std::string("one")));

    assert_true(a.has_key(std::string("one")));
    assert_false(a.has_key(std::string("four")));
  }

  void test_remove( void ) {
    Value a(Json("one: 1 two: 2"));
    Value b;
    assert_true(a.has_key("one"));

    a.remove("one");
    a.remove("xyz"); // should just ignore
    assert_false(a.has_key("one"));
    assert_true(a.has_key("two"));
  }
};

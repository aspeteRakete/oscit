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

#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "mock/dummy_script.h"

class ScriptTest : public TestHelper {
public:
  void test_script( void ) {
    DummyScript script;
    Value res = script.script(Value("This script will compile fine."));
    assert_true(res.is_string());
    assert_equal("This script will compile fine.", res.str());
    assert_true(script.script_ok());
    res.set_nil();
    res = script.script(gNilValue);
    assert_true(res.is_string());
    assert_equal("This script will compile fine.", res.str());
  }

  void test_bad_script( void ) {
    DummyScript script;
    Value res = script.script(Value("This script will raise a [compilation error]."));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("Compilation error near ' a [compilation error].'.", res.error_message());
    assert_false(script.script_ok());
    res.set_nil();
    res = script.script(gNilValue);
    assert_true(res.is_string());
    assert_equal("This script will raise a [compilation error].", res.str());
    // compile a correct script
    res = script.script(Value("This script will compile fine."));
    assert_true(res.is_string());
    assert_equal("This script will compile fine.", res.str());
    assert_true(script.script_ok());
  }

  void test_file( void ) {
    DummyScript script;
    Value res = script.file(Value(fixture_path("script_test_file.txt")));
    assert_true(res.is_string());
    assert_equal(fixture_path("script_test_file.txt"), res.str());
    assert_true(script.script_ok());
    res.set_nil();
    res = script.file(gNilValue);
    assert_true(res.is_string());
    assert_equal(fixture_path("script_test_file.txt"), res.str());
    res = script.script(gNilValue);
    assert_equal("On a dark, rainy day,\nWe will go out to fish.\nWith the elements we play\nAnd fishes end served on a dish.\n", res.str());
  }

  void test_change_content_should_be_saved_in_file( void ) {
    DummyScript script;
    std::string new_file(fixture_path("script_test_new_file.txt"));
    Value res = script.file(Value(new_file));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal(std::string("Could not find '").append(new_file).append("'."), res.error_message());
    assert_false(script.script_ok());
    res = script.script(gNilValue);
    assert_true(res.is_string());
    assert_equal("", res.str());

    res = script.script(Value("Foo bar baz !"));
    assert_true(res.is_string());
    assert_equal("Foo bar baz !", res.str());

    std::ifstream in(new_file.c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("Foo bar baz !", oss.str());

    remove(new_file.c_str());
  }

  void test_file_not_found( void ) {
    DummyScript script;
    std::string not_found(fixture_path("script_test_file_not_found.txt"));
    Value res = script.file(Value(not_found));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal(std::string("Could not find '").append(not_found).append("'."), res.error_message());
    assert_false(script.script_ok());
    res = script.file(gNilValue);
    assert_true(res.is_string());
    assert_equal(not_found, res.str());
    // load another existing file
    res = script.file(Value(fixture_path("script_test_file.txt")));
    assert_true(res.is_string());
    assert_equal(fixture_path("script_test_file.txt"), res.str());
    assert_true(script.script_ok());
  }

  void test_bad_file( void ) {
    DummyScript script;
    std::string bad_file(fixture_path("script_test_bad_file.txt"));
    Value res = script.file(Value(bad_file));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("Compilation error near \'he [compilation error] \'.", res.error_message());
    assert_false(script.script_ok());

    // keeps track of the last error message (TODO: split these long tests into simpler ones)
    assert_equal("Compilation error near \'he [compilation error] \'.", script.last_error().error_message());

    res.set_nil();
    res = script.file(gNilValue);
    assert_true(res.is_string());
    assert_equal(bad_file, res.str());
    assert_equal("On a dark, rainy day,\nWe will go out to fish.\nWith the [compilation error] #)IOYLNK$\n#:L)(_)*@#)(()}#\n#\n#&#()&U)$", script.script(gNilValue).str());
    // load a correct file
    res = script.file(Value(fixture_path("script_test_file.txt")));
    assert_true(res.is_string());
    assert_equal(fixture_path("script_test_file.txt"), res.str());
    assert_true(script.script_ok());
  }

  void test_reload( void ) {
    DummyScript script;
    std::string reload_file(fixture_path("script_test_reload.txt"));
    script.file(Value(reload_file));

    assert_equal(1.0, script.reload(gNilValue).r);
    Value res = script.reload(Value(0.002)); // 1 [ms]

    std::ofstream out(reload_file.c_str(), std::ios::out);
      out << "This is a nice script.";
    out.close();

    // too soon, should not reload
    script.reload_script(1);
    assert_false(script.script_ok());
    assert_equal("", script.script(gNilValue).str());

    // reload
    script.reload_script(2);
    assert_true(script.script_ok());
    assert_equal("This is a nice script.", script.script(gNilValue).str());

    remove(reload_file.c_str());
  }

  void test_inspect( void ) {
    DummyScript script;
    std::string new_file(fixture_path("script_test_new_file.txt"));
    script.file(Value(new_file));
    Value res;
    script.inspect(&res);
    assert_equal(std::string("script:\"\" file:\"").append(new_file).append("\" reload:1"), res.lazy_json());
  }
};
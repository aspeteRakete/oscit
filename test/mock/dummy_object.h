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

#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a value."

class DummyObject : public Object
{
public:
  /** Class signature. */
  TYPED("Object.DummyObject")

  DummyObject(const char *name, Real value) : Object(name, Oscit::range_io(DUMMY_OBJECT_INFO, 0, 127)), value_(value) {}
  DummyObject(const char *name, Value value, const Value &attrs) : Object(name, attrs), value_(value) {}
  DummyObject(const char *name, Real value, const Value &attrs) : Object(name, attrs), value_(value) {}
  DummyObject(const char *name, const char *value, const Value &attrs) : Object(name, attrs), value_(value) {}
  DummyObject(const char *name, const char *value, const char *attrs) : Object(name, JsonValue(attrs)), value_(JsonValue(value)) {}
  DummyObject(const char *name, const std::string &value, const Value &attrs) : Object(name, attrs), value_(value) {}

  virtual ~DummyObject() {}

  virtual const Value trigger(const Value &val) {
//    std::cout << "DummyObject->trigger(" << val << ")\n";

    if (!val.is_nil()) {
      value_ = val;
    }

    return value_;
  }

  virtual bool build_child(const std::string &name, const Value &attrs, Value *error, ObjectHandle *handle) {
    Object *obj = NULL;
    if (name == "special") {
      obj = adopt(new Object("special"));
    } else if (name == "AgeOf") {
      obj = adopt(new Object(name));
      obj->adopt(new DummyObject("Capitain", 78.0));
    } else if (name == "error") {
      error->set(INTERNAL_SERVER_ERROR, "You should not try to build errors !");
    }
    if (obj) {
      handle->hold(obj);
      return true;
    } else {
      return false;
    }
  }

  Real real() {
    return value_.r;
  }

  Value value_;
};

class DummyObject2 : public Object {
public:
  TYPED("Object.DummyObject2")

  DummyObject2(const char * name, const char *value) : Object(name, Oscit::select_io("Set color mode.", "rgb,rgba,yuv")), value_(value) {}

  DummyObject2(const char * name, const char *value, const Value &attrs) : Object(name, attrs), value_(value) {}

  virtual ~DummyObject2() {}

  virtual const std::string inspect() {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.c_str();
    return os.str();
  }

  virtual const Value trigger(const Value &val) {
    if (val.is_string())
      value_.set(val.c_str());
    return value_;
  }

private:
  StringValue value_;
};

class SubDummyObject : public DummyObject {
public:
  TYPED("Object.DummyObject.SubDummyObject")

  SubDummyObject(const char *name, Real value) : DummyObject(name, value) {}
};

#endif // _DUMMY_OBJECT_H_
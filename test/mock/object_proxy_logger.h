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

#ifndef OSCIT_TEST_MOCK_OBJECT_PROXY_LOGGER_H_
#define OSCIT_TEST_MOCK_OBJECT_PROXY_LOGGER_H_

#include "oscit/object.h"
#include "mock/logger.h"
#include "oscit/object_proxy.h"

class ObjectProxyLogger : public ObjectProxy, protected MockLogger {
public:
  ObjectProxyLogger(const char *name, const Value &attrs, std::ostringstream *stream) :
                ObjectProxy(name, attrs),
                MockLogger(name, stream) {}

  ObjectProxyLogger(const std::string &name, const Value &attrs) :
                ObjectProxy(name, attrs),
                MockLogger(name.c_str(), NULL) {}

  /** This method should be implemented in order to give a visual feedback to
   * users that the value has changed (GUI object notification).
   */
  virtual void value_changed() {
    log("value_changed", value_);
  }

  virtual void adopted() {
    ObjectProxy::adopted();
    if (root_proxy_) {
      log("adopted", root_proxy_->class_name());
    } else {
      log("adopted");
    }
  }

  time_t latency() const {
    return latency_;
  }

  TimeRef &time_ref() {
    return time_ref_;
  }

  RootProxy *root_proxy() {
    return root_proxy_;
  }

  void set_stream(std::ostream *stream) {
    MockLogger::set_stream(stream);
  }

};


#endif // OSCIT_TEST_MOCK_OBJECT_PROXY_LOGGER_H_
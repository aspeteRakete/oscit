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

#include "oscit/zeroconf_browser.h"

#include <stdio.h>

#include <string>

#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"
#include "oscit/command.h"

namespace oscit {

void ZeroConfBrowser::set_command(Command *command) {
  command_ = command;
  start();
}

void ZeroConfBrowser::do_adopt_proxy_factory(ProxyFactory *factory) {
  if (running_ && !proxy_factory_) {
    std::cerr << "WARNING: " << "browser allready running when adopting proxy factory, you should adopt the factory before starting the browser.\n";
  }
  if (proxy_factory_) {
    delete proxy_factory_;
  }
  proxy_factory_ = factory;
}


void ZeroConfBrowser::add_proxy(const Location &location) {
  if (!proxy_factory_) return;

  if (command_) {
    if (!command_->find_proxy(location)) {
      RootProxy *proxy = command_->adopt_proxy(proxy_factory_->build_and_init_root_proxy(location));
      if (proxy) added_proxy(proxy);
    }
  } else {
    std::cerr << "Cannot build proxy: ProxyFactory set but Command is NULL\n";
  }
}

void ZeroConfBrowser::remove_proxy(const Location &location) {
  if (command_) {
    RootProxy *proxy = command_->find_proxy(location);
    if (proxy) {
      proxy->detach();
      delete_proxy(proxy);
    }
  }
}

void ZeroConfBrowser::get_protocol_from_service_type() {
  size_t dot_index = service_type_.find(".");
  if (dot_index != std::string::npos) {
    protocol_ = service_type_.substr(1, dot_index - 1);
  } else {
    // Bad service type
    std::cerr << "Could not get protocol from service type: " << service_type_ << "\n";
  }
}


void ZeroConfBrowser::add_device(const Location &location) {
  // only record first entry in case there are more then one network interfaces
  if (!found_devices_.has_key(location.name())) {
    found_devices_.set(location.name(), location);
    add_proxy(location);
  }
}

void ZeroConfBrowser::remove_device(const char *name) {
  Location location;
  if (found_devices_.get(name, &location)) {
    found_devices_.remove(name);
    remove_proxy(location);
  }
}

bool ZeroConfBrowser::get_location_from_name(const char *service_name, Location *location) const {
  return found_devices_.get(service_name, location);
}


} // oscit
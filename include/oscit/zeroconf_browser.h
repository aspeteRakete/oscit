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

#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_BROWSER_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_BROWSER_H_
#include <string>

#include "oscit/mutex.h"
#include "oscit/location.h"
#include "oscit/root_proxy.h"

namespace oscit {

#define FOUND_DEVICE_HASH_SIZE 100

class ProxyFactory;
class Command;

/** This class let's you easily found applications providing a certain
 * service. To make sure that everything works as expected, you should
 * do all the setup work (set ProxyFactory for example), BEFORE the
 * browser is adopted by a command (since this operation starts the
 * service browsing).
 */
class ZeroConfBrowser : public Mutex {
 public:
  ZeroConfBrowser(const char *service_type);

  virtual ~ZeroConfBrowser();

  /** This method is called just after a new proxy has been added to the list.
   */
  virtual void added_proxy(RootProxy *proxy) {}

  /** This method is called so that you have an opportunity to delete it cleanly.
   */
  virtual void delete_proxy(RootProxy *proxy) {
    delete proxy;
  }

  template<class T>
  T *adopt_proxy_factory(T *factory) {
    do_adopt_proxy_factory(factory);
    return factory;
  }

  /** FIXME: make this internal and use command->adopt_browser
   */
  void set_command(Command *command);

  bool get_location_from_name(const char *service_name, Location *location) const;

  /** @internal. */
  void add_device(const Location &location);

  /** @internal. */
  void remove_device(const char *name);

 protected:

  /** This method is called when the command_ is set. Sub-classes can call this
   * method if they want to use the browser without creating proxies (without a command).
   */
  virtual void start();

  /** This method *must* be called from sub-classes in their destructors to
   * make sure the callbacks (add_device, remove_device) are not called in the
	 * middle of a class destruction.
	 */
  virtual void stop();

  /** Return true if the browser is running (searching for devices).
   */
  bool is_running() {
    return running_;
  }

  void set_running(bool is_running) {
    running_ = is_running;
  }

  void get_protocol_from_service_type();

  void add_proxy(const Location &location);

  void remove_proxy(const Location &location);

  /** This value is on if the browser is running (listening for new devices).
   */
  bool          running_;

  /** Protocol used in communication (usually 'oscit').
   */
  std::string   protocol_;

  /** Service-type to browse.
   */
  std::string   service_type_;

  /** Pointer to a Command that can be used to build proxy objects.
   */
  Command      *command_;

  /** Factory to use to bild proxy objects. If this value is NULL, the
   * browser does not build proxies.
   */
  ProxyFactory *proxy_factory_;

 private:
  void do_adopt_proxy_factory(ProxyFactory *factory);

  class Implementation;
  Implementation *impl_;

  THash<std::string, Location> found_devices_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_REGISTRATION_H_

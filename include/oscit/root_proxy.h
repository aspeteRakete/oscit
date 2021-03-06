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

/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_

#include "oscit/root.h"
#include "oscit/location.h"

namespace oscit {

class ObjectProxy;
class ProxyFactory;

/** This class helps maintain a 'ghost' tree that mirrors a remote 'real' tree. It is usually used
 * as an element from an interface to interact with some remote device.
 */
class RootProxy : public Root {
public:

  /** Class signature. */
  TYPED("Object.Root.RootProxy")

  /** Create a proxy from a remote end_point. Do not build meta methods.
   */
  RootProxy(const Location &remote_location) :
            Root(false), remote_location_(remote_location),
            command_(NULL), proxy_factory_(NULL) {}

  RootProxy(const Location &remote_location, ProxyFactory *proxy_factory) :
            Root(false), remote_location_(remote_location),
            command_(NULL), proxy_factory_(NULL) {
    set_proxy_factory(proxy_factory);
  }

  virtual ~RootProxy() {
    // unregister from command_
    detach();
  }

  /** This method is used to send a message to the remote tree.
   */
  void send_to_remote(const char *path, const Value &val) {
    if (command_) {
      command_->send(remote_location_, path, val);
    }
  }

  /** Keep proxy in sync by parsing replies and sending new queries.
   */
  void handle_reply(const std::string &path, const Value &val);

  /** Set proxy's factory (used to create new object proxies).
   *  this method registers the RootProxy into the given factory.
   */
  void set_proxy_factory(ProxyFactory *factory);

  /** Returns the location of the original tree this proxy mirrors.
   */
  const Location &remote_location() const {
    return remote_location_;
  }

  /** Dynamically build a child from the given name. If type is empty, we build dummy
   * object proxies that will try to get a "type" from the remote end.
   */
  virtual bool build_child(const std::string &name, const Value &attrs, Value *error, ObjectHandle *handle);

  ProxyFactory *proxy_factory() {
    return proxy_factory_;
  }

  /** IF the RootProxy was adopted by a Command, this removes the link
   * making it an orphan.
   *
   * Used by ZeroConfBrowser before deletion.
   */
  void detach() {
    set_command(NULL);
  }

private:
  friend class Command; // set_command

  /** Set proxy's command (communication channel).
   *  this method registers the RootProxy into the given command.
   */
  void set_command(Command *command);

  /** Called by ~Command to avoid further 'unregister' calls.
   */
  void unlink_command() {
    command_ = NULL;
  }

  /** Get information on the direct children of the remote root: this method should not
   * be used (automatically called during startup).
   * The argument is just here to give the same signature as ObjectProxy::sync_children.
   *
   */
  void sync_children(bool forced = true) {
    send_to_remote(LIST_WITH_ATTRIBUTES_PATH, Value(url()));
  }

  void build_children_from_attributes(Object *base, const Value &attrss);

  /** Reference to the original tree this root proxies. When the RootProxy is adopted
   *  by a command, this is used as key to route 'reply' messages.
   */
  Location remote_location_;

  /** This is the link to the original 'remote'. It is used to send queries to the
   *  remote tree.
   */
  Command *command_;

  /** The link to the proxy factory is used by the RootProxy when it needs to create
   *  new ObjectProxies. */
  ProxyFactory *proxy_factory_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_
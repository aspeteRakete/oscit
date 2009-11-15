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
class RootProxy : Root {
public:
  /** Create a proxy from a remote end_point. Do not build meta methods.
   */
  RootProxy(const Location &remote_location, ProxyFactory *factory) : Root(false), remote_location_(remote_location), command_(NULL) {}

  virtual ~RootProxy() {
    // unregister from command_
    set_command(NULL);
  }

  /** This method is used to send a message to the remote tree.
   */
  virtual void send_to_remote(const std::string &path, const Value &val) {}

  /** Keep proxy in sync by parsing replies and sending new queries.
   */
  virtual void handle_reply(const std::string &path, const Value &val) {
    // ObjectProxy *find_or_build_object_proxy_at(path);
  }

  ObjectProxy *build_object_proxy(const std::string &name, const Value &type);

  /** Set proxy's command (communication channel).
   *  this method registers the RootProxy into the given command.
   */
  void set_command(Command *command);

  /** Set proxy's factory (used to create new object proxies).
   *  this method registers the RootProxy into the given factory.
   */
  void set_proxy_factory(ProxyFactory *factory);

  /** Return a unique identifier for an endpoint.
   *  this method should be part of an abstract Location class
   *  that could cleanly encapsulate an IpEndPoint, a Pipe or any
   *  other way to connect two places.
   */
  const std::string &endpoint_id();

private:

  /** Get an object proxy at a defined url.
   */
  // ObjectProxy *find_or_build_object_proxy_at(const std::string &path) {
  //   Object * object = object_at(path);
  //
  //   if (object == NULL) {
  //     // build object
  //     ObjectProxy *object ..
  //     size_t pos = path.rfind("/");
  //     if (pos != std::string::npos) {
  //       // "/foo/bar" ==> "/foo/bar", "/foo", ""
  //       // find or build parent
  //       Object * parent = find_or_build_object_proxy_at(path.substr(0, pos));
  //       parent->adopt(build_object_proxy(path.substr(pos)))
  //     } else {
  //       // root object "foo"... error ?
  //     }
  //   }
  //   return object;
  // }

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
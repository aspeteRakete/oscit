/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "oscit/root_proxy.h"

#include <string>
#include <iostream>

#include "oscit/command.h"
#include "oscit/object_proxy.h"
#include "oscit/proxy_factory.h"

namespace oscit {

void RootProxy::set_command(Command *command) {
  if (command_) command_->unregister_proxy(this);
  command_ = command;
  if (command) {
    command->register_proxy(this);
    command->send(remote_location_, REGISTER_PATH, gNilValue);
    sync_children();
  }
}

void RootProxy::set_proxy_factory(ProxyFactory *factory) {
  if (proxy_factory_) proxy_factory_->unregister_proxy(this);
  proxy_factory_ = factory;
  if (proxy_factory_) proxy_factory_->register_proxy(this);
}


void RootProxy::build_children_from_types(Object *parent, const Value &types) {
  if (!types.is_list()) {
    std::cerr << "Cannot handle " << LIST_WITH_TYPE_PATH << " reply: invalid argument: " << types << "\n";
    return;
  }
  if (!proxy_factory_) {
    std::cerr << "Cannot handle " << LIST_WITH_TYPE_PATH << " reply: no ProxyFactory !\n";
    return;
  }

  int types_count = types.size();
  Value name_with_type;
  ObjectProxy *object_proxy;
  bool has_children;


  for(int i=0; i < types_count; ++i) {
    name_with_type = types[i];
    if (name_with_type.size() < 2 || !name_with_type[0].is_string()) {
      std::cerr << "Invalid type in " << LIST_WITH_TYPE_PATH << " reply argument: " << name_with_type << "\n";
    } else {
      std::string name = name_with_type[0].str();
      if (name.at(name.length()-1) == '/') {
        has_children = true;
        name = name.substr(0, name.length() - 1);
      } else {
        has_children = false;
      }

      if (!parent->child(name)) {
        // child does not exist yet, first ask parent if it can build it
        Value error;
        Object *object = parent->build_child(name, name_with_type[1], &error);
        if (object) {
          object_proxy = TYPE_CAST(ObjectProxy, object);
          if (!object_proxy) {
            std::cerr << parent->url() << " has built an invalid object through 'build_child'. Should be an ObjectProxy, found a " << object->class_name() << "\n";
          } else {
            object_proxy->set_need_sync(has_children);
          }
        } else if (error.is_error()) {
          std::cerr << parent->url() << "Returned an error while trying to build " << name << ": " << error << "\n";
        } else {
          object_proxy = proxy_factory_->build_object_proxy(parent, name, name_with_type[1]);
          if (object_proxy) {
            parent->adopt(object_proxy);
            object_proxy->set_need_sync(has_children);
          }
        }
      }
    }
  }
}

Object *RootProxy::build_child(const std::string &name, const Value &type, Value *error, const Mutex *context) {
  if (!proxy_factory_) {
    std::cerr << "Cannot build child /" << name << " : no ProxyFactory !\n";
    return NULL;
  }

  Object *object = proxy_factory_->build_object_proxy(this, name, type);

  if (object) adopt(object, context);

  return object;
}

void RootProxy::handle_reply(const std::string &path, const Value &val) {
  if (path == LIST_WITH_TYPE_PATH) {
    if (val.size() < 2 || !val[0].is_string() || !val[1].is_list()) {
      std::cerr << "Invalid argument in " << LIST_WITH_TYPE_PATH << " reply: " << val << "\n";
      return;
    }

    Object *parent = object_at(val[0].str());
    if (!parent) {
      std::cerr << "Invalid parent path " << val[0].str() << " in " << LIST_WITH_TYPE_PATH << " reply: unknown path.\n";
      return;
    }

    build_children_from_types(parent, val[1]);
  } else if (path == TYPE_PATH) {
    if (val.size() < 2 || !val[0].is_string()) {
      std::cerr << "Invalid argument in " << TYPE_PATH << " reply: " << val << "\n";
      return;
    }

    Value error;
    Object *target = find_or_build_object_at(val[0].str(), &error);
    ObjectProxy *object_proxy = TYPE_CAST(ObjectProxy, target);
    if (object_proxy) {
      object_proxy->set_type(val[1]);
    }
  } else {
    // Find target
    Object *target = object_at(path);

    if (target && target->can_receive(val)) {
      ObjectProxy *object_proxy = TYPE_CAST(ObjectProxy, target);
      // TODO: Should we use 'safe' here ? (mutex)
      if (object_proxy) object_proxy->handle_value_change(val);
    } else {
      std::cerr << "could not route '" << path << "' reply to '" << val << "'\n";
    }
  }
}

} // oscit

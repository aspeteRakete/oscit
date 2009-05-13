#include "oscit/command.h"
#include "oscit/root.h"
#include "oscit/object.h"
#include "oscit/zeroconf.h"

namespace oscit {

Command::Command(const char *protocol) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE), 
                                root_(NULL),
                                protocol_(protocol),
                                service_type_(""),
                                port_(0),
                                zeroconf_registration_(NULL) {}

Command::Command(const char *protocol, const char *service_type, uint16_t port) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                protocol_(protocol),
                                service_type_(service_type),
                                port_(port),
                                zeroconf_registration_(NULL) {}

Command::Command(Root *root, const char *protocol) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                protocol_(protocol),
                                service_type_(""),
                                port_(0),
                                zeroconf_registration_(NULL) {}

Command::Command(Root *root, const char *protocol, const char *service_type, uint16_t port) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                protocol_(protocol),
                                service_type_(service_type),
                                port_(port),
                                zeroconf_registration_(NULL) {}

Command::~Command() {
  kill();
  if (zeroconf_registration_ != NULL) delete zeroconf_registration_;
  
  if (root_) {
    root_->unregister_command(this);
    root_ = NULL;
  }
  string_iterator it;
  string_iterator end = remote_objects_.end();

  // destroy all references to remote objects
  for(it = remote_objects_.begin(); it != end; it++) {
    Object * object;
    if (remote_objects_.get(*it, &object)) {
      delete object;
    }
  }
}

Object *Command::remote_object(const Url &remote_url, Value *error) {
  Object *object;
  if (remote_objects_.get(remote_url.str(), &object)) {
    return object;
  } else {
    return build_remote_object(remote_url, error);
  }
}

void Command::publish_service() {
  if (zeroconf_registration_ == NULL && service_type_ != "") {
    std::string name(root_->name());
    if (name == "") {
      name = "Generic oscit device";
    }
    zeroconf_registration_ = new ZeroConfRegistration(name.c_str(), service_type_.c_str(), port_);
  }
}

} // oscit

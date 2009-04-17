#ifndef _OSCIT_ALIAS_H_
#define _OSCIT_ALIAS_H_

#include "oscit/root.h"

namespace oscit {
/** This object triggers another object's trigger when called. It's used in Group to expose functionalities. */
class Alias : public Object
{
public:
  Alias() : original_(NULL) {}
  
  Alias(const char *name, Object *object) : Object(name), original_(NULL) {
    set_original(object);
  }
  
  Alias(const std::string &name, Object *object) : Object(name), original_(NULL) {
    set_original(object);
  }
  
  virtual ~Alias() {
    // We unregister to tell the object that it should not delete this alias on destruction.
    if (original_) original_->unregister_alias(this);
  }
  
  /** Class signature. */
  virtual uint class_type() {
    return H("Alias");
  }
  
  virtual const Value trigger(const Value &val) {
    return original_ ? original_->safe_trigger(val) : gNilValue;
  }
  
  void unlink_original() {
    original_ = NULL;
  }
  
  inline bool connected() {
    return original_ != NULL;
  }
  
  /** Set new original object from url. */
  void set_original(const std::string &url) {
    Object *original = root_->object_at(url);
    if (original) set_original(original);
  }
  
  /** Set new original object from an object pointer. */
  void set_original(Object *object) {
    if (original_) original_->unregister_alias(this);
    original_ = object;
    set_type_tag_id(original_->type_tag_id());
    // We register so that the alias dies with the original object.
    if (original_) original_->register_alias(this);
  }
  
protected:
  Object *original_; /**< Original object pointed to by the alias. */
};

} // namespace oscit
#endif // _OSCIT_ALIAS_H_
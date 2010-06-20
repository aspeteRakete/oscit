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


#ifndef OSCIT_INCLUDE_OSCIT_OBJECT_H_
#define OSCIT_INCLUDE_OSCIT_OBJECT_H_

#include <stdlib.h> // atoi
#include <list>
#include <string>

#include "oscit/typed.h"
#include "oscit/observable.h"
#include "oscit/values.h"
#include "oscit/thash.h"
#include "oscit/mutex.h"
#include "oscit/location.h"
#include "oscit/c_reference_counted.h"
#include "oscit/c_tvector.h"
#include "oscit/c_thash.h"

namespace oscit {

/** Used to append the "-1" or "-2" to osc methods.
 */
#define OSC_NEXT_NAME_BUFFER_SIZE 20

#define DEFAULT_TYPE NoIO("No information on this node.")

class Root;
class Alias;
class ObjectProxy;
class ObjectHandle;

class Object : public Typed, public Observable, public CReferenceCounted {
 public:
  /** Class signature. */
  TYPED("Object")

  explicit Object() : root_(NULL), parent_(NULL), children_(20), context_(NULL), keep_last_(false),
    type_(DEFAULT_TYPE) {
    sync_type_id();
    name_ = "";
    url_  = name_;
  }

  explicit Object(const char *name) : root_(NULL), parent_(NULL),
    children_(20), name_(name), url_(name), context_(NULL), keep_last_(false),
    type_(DEFAULT_TYPE) {
    sync_type_id();
  }

  explicit Object(const std::string &name) : root_(NULL), parent_(NULL),
    children_(20), name_(name), url_(name), context_(NULL), keep_last_(false),
    type_(DEFAULT_TYPE) {
    sync_type_id();
  }

  explicit Object(const Value &type) : root_(NULL), parent_(NULL),
    children_(20), context_(NULL), keep_last_(false), type_(type) {
    sync_type_id();
    name_ = "";
    url_  = name_;
  }

  Object(const char *name, const Value &type, bool keep_last = false) : root_(NULL), parent_(NULL),
    children_(20), name_(name), url_(name), context_(NULL), keep_last_(keep_last),
    type_(type) {
    sync_type_id();
  }

  Object(const std::string &name, const Value &type, bool keep_last = false) : root_(NULL),
    parent_(NULL), children_(20), name_(name), url_(name), context_(NULL),
    keep_last_(keep_last), type_(type) {
    sync_type_id();
  }

  Object(Object *parent, const char *name) : root_(NULL), parent_(NULL),
    children_(20), name_(name), context_(NULL), keep_last_(false),
    type_(DEFAULT_TYPE) {
    sync_type_id();
    parent->adopt(this);
  }

  Object(Object *parent, const char *name, const Value &type) : root_(NULL),
    parent_(NULL), children_(20), name_(name), context_(NULL), keep_last_(false),
    type_(type) {
    sync_type_id();
    parent->adopt(this);
  }

  Object(Object *parent, const std::string &name, const Value &type) :
    root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL),
    keep_last_(false), type_(type) {
    sync_type_id();
    parent->adopt(this);
  }

  virtual ~Object();

  /** Clear all children (delete).
   * TODO: make thread safe
   */
  void clear();

  /** This is the operation executed when the object is called.
   * In order to benefit from return value optimization and avoid too many copy
   * you have to use Value v = xxx.trigger(val).
   * @todo why is the return value const ?
   */
  virtual const Value trigger(const Value &val) {
    return gNilValue;
  }

  /** Dynamically build a child from the given name. This method is called whenever
   * a sub-node or branch is not found and this is the last found object along
   * the path.
   */
  virtual bool build_child(const std::string &name, const Value &type, Value *error, ObjectHandle *handle) {
    return false;
  }

  /** Lock resource (used by commands). */
  inline void lock() {
    if (context_) context_->lock();
  }

  /** Unlock resource (used by commands). */
  inline void unlock() {
    if (context_) context_->unlock();
  }

  /** Inform the object of an alias linked to this object (has to be deleted
   * on destruction).
   */
  void register_alias(Alias *alias);

  /** Inform the object that an alias no longer exists. */
  void unregister_alias(Alias *alias);

  /** Return the object's unique url. */
  inline const std::string &url() const {
    return url_;
  }

  /** Set name from string. If the name is not unique in the parent's scope,
   *  the name is changed as "name-1", "name-2", etc.
   */
  void set_name(const char* name) { set_name(std::string(name)); }


  /** Set name from string. If the name is not unique in the parent's scope,
   *  the name is changed as "name-1", "name-2", etc.
   */
  void set_name(const std::string &name) {
    if (name == "") return;
    name_ = name;  // FIXME: gsub(/[^a-zA-Z\-0-9_],"")
    set_parent(parent_);  // reset parent so name is verified in parent
    moved();
  }

  void set_keep_last(bool should_keep_last) {
    if (should_keep_last != keep_last_) {
      keep_last_ = should_keep_last;
      set_parent(parent_); // reset
    }
  }
  /** Return name of object. */
  inline const std::string name() const {
    return name_;
  }

  /** ========================== REPLIES TO META METHODS ======================
   * The replies to meta methods are implemented as virtuals so that objects
   * that inherit from osc::Object just need to overwrite these in order to
   * return more meaningful information / content.
   */

  /** List sub-nodes.
   * This method is used as a reply to the /.list meta method.
   * The format of the reply is a list of names with the type:
   * [name, name, ...].
   */
  const Value list() const;

  /** Shortcut to call multiple methods on an object. This method simply calls from_hash.
   * @param val Using "obj.set(tempo:45 rubato:1.5)" is equivalent to calling
   *            "obj.tempo(45)" and "obj.rubato(1.5)".
   * @return    a hash with the result for each call.
   */
  const Value set(const Value &val) {
    Value result;
    from_hash(val, &result);
    return result;
  }

  /** Update an object from a hash, inserting results for each call
   * in the results hash.
   */
  virtual void from_hash(const Value &hash, Value *results);

  /** Shortcut to call multiple methods on an object.
   * @param val Using "obj.set(tempo:45 rubato:1.5)" is equivalent to calling
   *            "obj.tempo(45)" and "obj.rubato(1.5)".
   * @return true on success, false if any call failed.
   */
  bool set_all_ok(const Value &val);

  /** This is the prefered way to insert new objects in the tree since it clearly
   * highlights ownership in the parent.
   * TODO: make sure a parent is not adopted by it's child.
   * TODO: if we want to make this thread safe, we need a handle.
   */
  template<class T>
  T *adopt(T *object) {
    object->set_parent(this);
    return object;
  }

  /** Return a hash representing the current object. The
   * default behavior is to build a hash by sending 'insert_in_hash'
   * on the children objects. This method is not const because
   * the objects might need to trigger to get their current value.
   */
  virtual void insert_in_hash(Value *result);

  /** Return a hash representing the current object. The
   * default behavior is to build a hash by sending 'to_hash'
   * on the children objects. This method is just a convenient method
   * that calls insert_in_hash.
   */
  const Value to_hash() {
    Value result;
    insert_in_hash(&result);
    return result;
  }

  /** List sub-nodes with their current value and type.
   * This method is used as a reply to the /.list_with_type meta method.
   * The format of the reply is a list of names with the type:
   * [name, current, unit, ...], [name, current, unit, ...], etc.
   *
   * This method is not 'const' because it triggers the object to get the current value.
   */
  const Value list_with_type();

  /** List full tree under this node.
   *  @param base_length is the length of the url for the initial call
   *                     (removed from results).
   *  @param tree returned value.
   */
  void tree(size_t base_length, Value *tree) const;

  /** Human readable information method.
   *  Called as a response to "/.info '/this/url'".
   */
  const Value &info() const {
    return type_.last();
  }

  /** Type information on node.
   */
  const Value &type() const {
    return type_;
  }

  /** Type information with current value (used to automatically generate the correct control).
   * Called during response to "/.type '/this/url'".
   */
  const Value type_with_current_value();

  /** Set meta type (signature, range, units). The type should be immutable.
   *  this method is not a good idea.
   */
  // bool set_type(const Value &type) {
  //   if (type.type_id() != type_.type_id()) {
  //     return false;
  //   } else {
  //     type_ = type;
  //     sync_type_id();
  //     return true;
  //   }
  // }

  /** Return the list of children as a hash.
   * FIXME: if this is a const how does the user of this method RW lock ?
   */
  const CTHash<std::string, Object *> &children() const {
    return children_;
  }

  /** Return first child.
   */
  bool first_child(ObjectHandle *handle) {
    return get_child(0, handle);
  }

  /** Tries to find a child named 'name'.
   */
  bool get_child(const std::string &name, ObjectHandle *handle);

  /** Tries to find the child at a specific index. Returns
   * false on failure.
   */
  bool get_child(size_t index, ObjectHandle *handle);

  /** Return the number of children objects.
   */
  size_t children_count() {
    ScopedRead lock(children_vector_);
    return children_vector_.size();
  }


  Object *parent() {
    return parent_;
  }

  /** This method is called whenever the parent changes. It can be
   * used to finish initializing the object.
   */
  virtual void adopted() {}

  /** Set object's new parent.
   */
  void set_parent(Object *parent);

  /** Parent changed, set new context. */
  virtual void set_context(Mutex *context) { context_ = context; }

  /** Return the type tag signature id (uint) of the trigger method of this
   * object (what it wants to receive as arguments).
   *
   * TODO: rename, it's confusing with type().type_id(). Use accept_type_id() instead ?
   */
  inline TypeTagID type_id() {
    return type_id_;
  }

  bool accept_any_type() {
    return type_id_ == ANY_TYPE_TAG_ID;
  }

  inline bool can_receive(const Value &val) {
    if (type_id() == NO_TYPE_TAG_ID) return false;
    if (val.type_id() == type_id() || accept_any_type()) {
      return true;
    } else if (val.is_nil()) {
      return true;
    } else if (!val.is_list() || !type_[0].is_list()) {
      return false;
    } else {
      // first elements should match
      const char * c  = type_[0].type_tag();
      const char * cl = val.type_tag();
      while (*c) {
        if (*c != *cl) return false;
        ++c;
        ++cl;
      }
      return true;
    }
  }

 protected:

  /** Set object's new root.
  */
  void set_root(Root *root);

  /** Child sends a notification to the parent when it's name changes so that
   *  the parent/root keep their url hash in sync.
   */
  void register_child(Object *child);

  /** Free the child from the list of children.
   */
  void unregister_child(Object *child);

  /** Update cached url, notify root_ of the position change.
   */
  void moved();

  /** Add '-1', '-2', ... at the end of the current name. bob --> bob-1
   */
  void find_next_name() {
    size_t pos = name_.find('-');
    char buffer[OSC_NEXT_NAME_BUFFER_SIZE];
    if (pos == std::string::npos) {
      name_.append("-1");
    } else {
      std::string baseName = name_.substr(0, pos - 1);
      int i = atoi(name_.substr(pos + 1, std::string::npos).c_str());
      snprintf(buffer, OSC_NEXT_NAME_BUFFER_SIZE, "%i", i+1);
      name_ = baseName.append(buffer);
    }
  }

  void observer_lock() {
    // FIXME: the need for this = bad design !
  }

  void observer_unlock() {
    // FIXME: the need for this = bad design !
  }

 private:
  friend class ObjectProxy;

  /** Keep type_id_ in sync with type_.
   */
  void sync_type_id() {
    type_id_ = type_.size() > 0 ? type_[0].type_id() : NO_TYPE_TAG_ID;
  }

  /** List of aliases to destroy when
   * this node disappears.
   * FIXME: clarify usage and RW lock need.
   */
  std::list<Alias *>          aliases_;


 protected:

  /** Root object used to access application methods/
   */
  Root *root_;

  /** Pointer to parent object.
   * Can be NULL if the object does not have a parent.
   */
  Object *parent_;

  /** Hash with pointers to children objects or methods.
   * The children objects unregister themselves when they die or change
   * their parent by calling 'unregister_child'.
   */
  CTHash<std::string, Object *> children_;

  /** List of children as a vector for faster enumeration type of access.
   */
  CTVector<Object*> children_vector_;

  /** Unique name in parent's context.
   */
  std::string name_;

  /** Absolute path to object (cached).
   * FIXME: clarify usage and RW lock need.
   */
  std::string url_;

  /** Mutex to make sure only one thread is using a given context at a time.
   * FIXME: remove.
   */
  Mutex *context_;

  /** Flag to force this object at the end of the children list.
   * If more then one child has this setting, they are kept in insertion order at
   * the end.
   */
  bool keep_last_;

 private:

  /** Value that holds type information on the 'trigger' method of this
   * object.
   * If the type_ is not a list, this means the object is not callable and
   * the type_ member only holds a string with general information on the
   * object.
   * If the type_ is a list, the first element is the actual type of Value
   * the object can receive in it's 'trigger' method and the other elements
   * are information on the range of allowed values and such.
   * The last element of type_ is always an information string.
   * FIXME: clarify usage and RW lock need.
   */
  Value type_;

  /** Identifier for the type of the values the element can receive.
   * The value is a hash of the type tag list (multiple osc type tags) such
   * as "f" (single Real) or "fss" (Real, String, String).
   * This is cached from the first element in type_.
   * FIXME: clarify usage and RW lock need.
   */
  TypeTagID type_id_;
};

}  // namespace osc

#endif  // OSCIT_INCLUDE_OSCIT_OBJECT_H_

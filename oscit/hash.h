#ifndef _OSCIT_HASH_H_
#define _OSCIT_HASH_H_
#include "oscit/values.h"

namespace oscit {
class Value;
typedef std::list<std::string>::const_iterator HashIterator;

/** A Hash is just a reference counted THash<std::string,Value>. */
class Hash : public ReferenceCounted, public THash<std::string,Value>
{
 public:
  explicit Hash(size_t size) : THash<std::string,Value>(size) {}
};

} // oscit
#endif // _OSCIT_HASH_H_
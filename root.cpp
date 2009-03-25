#include "oscit/root.h"

namespace oscit {
Root::~Root()
{
  std::list<OscSend*>::iterator it  = observers_.begin();
  std::list<OscSend*>::iterator end = observers_.end();

  while (it != end) delete *it++;
  
  if (osc_in_) delete osc_in_;
  clear();
  root_ = NULL; // avoid call to unregister_object in ~Object
}
} // namespace oscit
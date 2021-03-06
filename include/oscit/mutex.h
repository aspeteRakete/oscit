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

#ifndef OSCIT_INCLUDE_OSCIT_MUTEX_H_
#define OSCIT_INCLUDE_OSCIT_MUTEX_H_
#include <pthread.h>
#include <assert.h>
#include <cstdio>

#include "oscit/non_copyable.h"
#include "oscit/typed.h"

namespace oscit {

/** Simple wrapper class for a fast mutex.
 * FIXME: Can we remove 'Typed' ?
 */
class Mutex : public Typed, private NonCopyable {
public:
  TYPED("Mutex")

  Mutex() {
    int status = pthread_mutex_init(&mutex_, NULL);
    if (status != 0) {
      fprintf(stderr, "Could not initialize mutex (%s)\n", strerror(status));
    }
  }

  // FIXME: remove virtual, typed and sub-classes (Thread, ZeroConf..)
  virtual ~Mutex() {
    int status = pthread_mutex_destroy(&mutex_);
    if (status != 0) {
      fprintf(stderr, "Could not destroy mutex (%s)\n", strerror(status));
      assert(false);
    }
  }

  /** If the mutex is locked by another thread, waits until it is unlocked, lock and continue.
    * If the mutex is not locked. Lock and continue.
    * If this thread locked it: bang! */
  inline void lock() {
    pthread_mutex_lock(&mutex_);
  }

  /** Release the lock so others can work on the data. */
  inline void unlock() {
    pthread_mutex_unlock(&mutex_);
  }

 private:
  pthread_mutex_t mutex_;
};


/** Scoped Mutex Lock for exclusive access to a resource.
 */
class ScopedLock : private NonCopyable {
public:
  ScopedLock(Mutex *mutex) : mutex_ptr_(mutex) {
    mutex_ptr_->lock();
  }

  ScopedLock(Mutex &mutex) : mutex_ptr_(&mutex) {
    mutex_ptr_->lock();
  }

  ~ScopedLock() {
    mutex_ptr_->unlock();
  }
 private:
  Mutex *mutex_ptr_;
};


/** Scoped Mutex Unlock to release exclusive access to a resource.
 */
class ScopedUnlock : private NonCopyable {
public:
  ScopedUnlock(Mutex *mutex) : mutex_ptr_(mutex) {
    mutex_ptr_->unlock();
  }

  ScopedUnlock(Mutex &mutex) : mutex_ptr_(&mutex) {
    mutex_ptr_->unlock();
  }

  ~ScopedUnlock() {
    mutex_ptr_->lock();
  }
 private:
  Mutex *mutex_ptr_;
};


} // oscit

#endif // OSCIT_INCLUDE_OSCIT_MUTEX_H_

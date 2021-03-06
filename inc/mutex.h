#ifndef MUTEX_H
#define MUTEX_H

/**
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if (defined __qdsp6__) || (defined __hexagon__)
#include "qurt_mutex.h"

#define RW_MUTEX_T                  qurt_mutex_t
#define RW_MUTEX_CTOR(mut)          qurt_mutex_init(& (mut))
#define RW_MUTEX_LOCK_READ(mut)     qurt_mutex_lock(& (mut))
#define RW_MUTEX_UNLOCK_READ(mut)   qurt_mutex_unlock(& (mut))
#define RW_MUTEX_LOCK_WRITE(mut)    qurt_mutex_lock(& (mut))
#define RW_MUTEX_UNLOCK_WRITE(mut)  qurt_mutex_unlock(& (mut))
#define RW_MUTEX_DTOR(mut)          qurt_mutex_destroy(& (mut))

#elif (1 == __linux) || (1 == __linux__) || (1 == __gnu_linux__) || (1 == linux)

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* asserts may be compiled out, this should always be present */
#define ABORT_FAIL( ff ) \
   do {\
      if(! (ff) ) {\
         fprintf(stderr, "assertion \"%s\" failed: file \"%s\", line %d\n", #ff, __FILE__, __LINE__);\
         abort();\
      }\
   } while(0)

#define RW_MUTEX_T                  pthread_rwlock_t
#define RW_MUTEX_CTOR(mut)          ABORT_FAIL(0 == pthread_rwlock_init( & (mut), 0))
#define RW_MUTEX_LOCK_READ(mut)     ABORT_FAIL(0 == pthread_rwlock_rdlock( & (mut)))
#define RW_MUTEX_UNLOCK_READ(mut)   ABORT_FAIL(0 == pthread_rwlock_unlock( & (mut)))
#define RW_MUTEX_LOCK_WRITE(mut)    ABORT_FAIL(0 == pthread_rwlock_wrlock( & (mut)))
#define RW_MUTEX_UNLOCK_WRITE(mut)  ABORT_FAIL(0 == pthread_rwlock_unlock( & (mut)))
#define RW_MUTEX_DTOR(mut)          ABORT_FAIL(0 == pthread_rwlock_destroy( & (mut)))


#else

#include "AEEstd.h"

#define RW_MUTEX_T uint32
#define RW_MUTEX_CTOR(mut) mut = 0
#define RW_MUTEX_LOCK_READ(mut) \
   do {\
      assert(STD_BIT_TEST(&mut, 1) == 0); \
      assert(STD_BIT_TEST(&mut, 2) == 0); \
      STD_BIT_SET(&mut, 1); \
   } while (0)

#define RW_MUTEX_UNLOCK_READ(mut) \
   do {\
      assert(STD_BIT_TEST(&mut, 1)); \
      assert(STD_BIT_TEST(&mut, 2) == 0); \
      STD_BIT_CLEAR(&mut, 1); \
   } while (0)

#define RW_MUTEX_LOCK_WRITE(mut) \
   do {\
      assert(STD_BIT_TEST(&mut, 1) == 0); \
      assert(STD_BIT_TEST(&mut, 2) == 0); \
      STD_BIT_SET(&mut, 2); \
   } while (0)

#define RW_MUTEX_UNLOCK_WRITE(mut) \
   do {\
      assert(STD_BIT_TEST(&mut, 1) == 0); \
      assert(STD_BIT_TEST(&mut, 2)); \
      STD_BIT_CLEAR(&mut, 2); \
   } while (0)

#define RW_MUTEX_DTOR(mut) mut = 0

#endif
#endif //MUTEX_H

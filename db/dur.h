// @file dur.h durability support

#pragma once

#include "diskloc.h"

namespace mongo { 

    namespace dur { 

        /** call writing...() to declare "i'm about to write to x and it should be logged for redo." 
            
            failure to call writing...() is checked in _DEBUG mode by using a read only mapped view
            (i.e., you'll segfault if you don't...)
        */


#if !defined(_DURABLE)

        inline void* writingPtr(void *x, size_t len) { return x; }
        inline DiskLoc& writingDiskLoc(DiskLoc& d) { return d; }
        inline int& writingInt(int& d) { return d; }
        template <typename T> inline T* writing(T *x) { return x; }
        inline void assertReading(void *p) { }
        template <typename T> inline T* writingNoLog(T *x) { return x; }
#else

        void* writingPtr(void *x, size_t len);

        inline DiskLoc& writingDiskLoc(DiskLoc& d) {
            return *((DiskLoc*) writingPtr(&d, sizeof(d)));
        }

        inline int& writingInt(int& d) {
            return *((int*) writingPtr(&d, sizeof(d)));
        }

        template <typename T> 
        inline 
        T* writing(T *x) { 
            return (T*) writingPtr(x, sizeof(T));
        }

        void assertReading(void *p);

        /** declare our intent to write, but it doesn't have to be journaled, as this write is 
            something 'unimportant'.
        */
        template <typename T> 
        inline 
        T* writingNoLog(T *x) { 
            log() << "todo dur" << endl;
            return (T*) writingPtr(x, sizeof(T));
        }

#endif

    }

    inline DiskLoc& DiskLoc::writing() { 
        return dur::writingDiskLoc(*this);
    }

}
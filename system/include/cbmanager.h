/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_GOLDFISH_OPENGL_SYSTEM_CBMANAGER_CBMANAGER_H
#define ANDROID_GOLDFISH_OPENGL_SYSTEM_CBMANAGER_CBMANAGER_H

#include <memory>
#include <system/graphics.h>
#include "gralloc_cb.h"

namespace android {

class CbManager {
public:
    CbManager();

    class CbManagerImpl {
    public:
        virtual ~CbManagerImpl() {}
        virtual const cb_handle_t* allocateBuffer(int width,
                                                  int height,
                                                  int format) = 0;
        virtual void freeBuffer(const cb_handle_t* h) = 0;

        virtual int lockBuffer(cb_handle_t& handle,
             const int usage,
             const int left, const int top, const int width, const int height,
             void** vaddr) = 0;

        virtual int lockYCbCrBuffer(cb_handle_t& handle,
             const int usage,
             const int left, const int top, const int width, const int height,
             android_ycbcr* ycbcr) = 0;

        virtual int unlockBuffer(cb_handle_t& handle) = 0;
    };

    const cb_handle_t* allocateBuffer(int width, int height, int format) {
        return mImpl->allocateBuffer(width, height, format);
    }

    void freeBuffer(const cb_handle_t* h) {
        mImpl->freeBuffer(h);
    }

    int lockBuffer(cb_handle_t& handle,
             const int usage,
             const int left, const int top, const int width, const int height,
             void** vaddr) {
        return mImpl->lockBuffer(handle, usage, left, top, width, height, vaddr);
    }

    int lockBuffer(buffer_handle_t h,
             const int usage,
             const int left, const int top, const int width, const int height,
             void** vaddr) {
        cb_handle_t* cb = cb_handle_t::from_unconst(h);
        if (cb) {
            return lockBuffer(*cb, usage, left, top, width, height, vaddr);
        } else {
            return -1;
        }
    }


    int lockYCbCrBuffer(cb_handle_t& handle,
                        const int usage,
                        const int left, const int top, const int width, const int height,
                        android_ycbcr* ycbcr) {
        return mImpl->lockYCbCrBuffer(handle, usage, left, top, width, height, ycbcr);
    }

    int lockYCbCrBuffer(buffer_handle_t h,
                        const int usage,
                        const int left, const int top, const int width, const int height,
                        struct android_ycbcr* ycbcr) {
        cb_handle_t* cb = cb_handle_t::from_unconst(h);
        if (cb) {
            return lockYCbCrBuffer(*cb, usage, left, top, width, height, ycbcr);
        } else {
            return -1;
        }
    }

    int unlockBuffer(cb_handle_t& handle) {
        return mImpl->unlockBuffer(handle);
    }

    int unlockBuffer(buffer_handle_t h) {
        cb_handle_t* cb = cb_handle_t::from_unconst(h);
        if (cb) {
            return unlockBuffer(*cb);
        } else {
            return -1;
        }
    }

    static uint64_t getOffset(const buffer_handle_t h) {
        const cb_handle_t* cb = cb_handle_t::from(h);
        return cb ? cb->getMmapedOffset() : -1;
    }

private:
    std::unique_ptr<CbManagerImpl> mImpl;
};

}  // namespace android

#endif  // ANDROID_GOLDFISH_OPENGL_SYSTEM_CBMANAGER_CBMANAGER_H

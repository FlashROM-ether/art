/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef ART_SRC_HEAP_BITMAP_H_
#define ART_SRC_HEAP_BITMAP_H_

#include "space_bitmap.h"

namespace art {
  class Heap;
  class SpaceBitmap;

  class HeapBitmap {
   public:
    bool Test(const Object* obj) {
      SpaceBitmap* bitmap = GetSpaceBitmap(obj);
      DCHECK(bitmap != NULL);
      return bitmap->Test(obj);
    }

    void Clear(const Object* obj) {
      SpaceBitmap* bitmap = GetSpaceBitmap(obj);
      DCHECK(bitmap != NULL)
        << "tried to clear object "
        << reinterpret_cast<const void*>(obj)
        << " which did not belong to any bitmaps";
      return bitmap->Clear(obj);
    }

    void Set(const Object* obj) {
      SpaceBitmap* bitmap = GetSpaceBitmap(obj);
      DCHECK(bitmap != NULL)
        << "tried to mark object "
        << reinterpret_cast<const void*>(obj)
        << " which did not belong to any bitmaps";
      bitmap->Set(obj);
    }

    SpaceBitmap* GetSpaceBitmap(const Object* obj) {
      // TODO: C++0x auto
      for (BitmapVec::iterator cur = bitmaps_.begin(); cur != bitmaps_.end(); ++cur) {
        if ((*cur)->HasAddress(obj)) {
          return *cur;
        }
      }
      return NULL;
    }

    void Walk(SpaceBitmap::Callback* callback, void* arg) {
      // TODO: C++0x auto
      for (BitmapVec::iterator cur = bitmaps_.begin(); cur != bitmaps_.end(); ++cur) {
        (*cur)->Walk(callback, arg);
      }
    }

   private:
    void AddSpaceBitmap(SpaceBitmap* space) {
      bitmaps_.push_back(space);
    }

    typedef std::vector<SpaceBitmap*> BitmapVec;
    BitmapVec bitmaps_;

    friend class Heap;
  };
}  // namespace art

#endif  // ART_SRC_HEAP_BITMAP_H_

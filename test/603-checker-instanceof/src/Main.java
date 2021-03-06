/*
 * Copyright (C) 2016 The Android Open Source Project
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

class SuperClass {
}

class ChildClass extends SuperClass {
}

public class Main {

  /// CHECK-START:    void Main.main(java.lang.String[]) builder (after)
  /// CHECK:          BoundType  klass:SuperClass can_be_null:false exact:false

  /// CHECK-START:    void Main.main(java.lang.String[]) builder (after)
  /// CHECK-NOT:      BoundType  klass:SuperClass can_be_null:false exact:true
  public static void main(String[] args) {
    Object obj = new ChildClass();

    // We need a fixed point iteration to hit the bogus type update
    // of 'obj' below, so create a loop that updates the type of 'obj'.
    for (int i = 1; i < 1; i++) {
      obj = new Object();
    }

    if (obj instanceof SuperClass) {
      // We used to wrongly type obj as an exact SuperClass from this point,
      // meaning we were statically determining that the following instanceof
      // would always fail.
      if (!(obj instanceof ChildClass)) {
        throw new Error("Expected a ChildClass, got " + obj.getClass());
      }
    }
  }
}

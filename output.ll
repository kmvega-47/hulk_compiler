; ModuleID = 'hulk_module'
source_filename = "hulk_module"

%A = type { i32 }
%B = type { i32 }

declare double @_hulk_sqrt(double)

declare double @_hulk_sin(double)

declare double @_hulk_cos(double)

declare double @_hulk_exp(double)

declare double @_hulk_log(double, double)

declare double @_hulk_rand()

declare double @_hulk_pow(double, double)

declare void @_hulk_print_number(double)

declare void @_hulk_print_string(ptr)

declare void @_hulk_print_bool(i1)

declare ptr @_hulk_concat(ptr, ptr, i32)

declare ptr @_hulk_alloc(i32)

declare void @_hulk_free(ptr)

declare ptr @_hulk_number_to_string(double)

define ptr @_new_A(ptr %0) {
entry:
  %type_id_ptr = getelementptr inbounds %A, ptr %0, i32 0, i32 0
  store i32 0, ptr %type_id_ptr, align 4
  ret ptr %0
}

define ptr @_new_B(ptr %0) {
entry:
  %type_id_ptr = getelementptr inbounds %B, ptr %0, i32 0, i32 0
  store i32 1, ptr %type_id_ptr, align 4
  %1 = call ptr @_new_A(ptr %0)
  ret ptr %0
}

define i32 @main() {
entry:
  %PI = alloca double, align 8
  store double 0x400921FB54442D18, ptr %PI, align 8
  %E = alloca double, align 8
  store double 0x4005BF0A8B145769, ptr %E, align 8
  %0 = call ptr @_hulk_alloc(i32 ptrtoint (ptr getelementptr (%B, ptr null, i32 1) to i32))
  %1 = call ptr @_new_B(ptr %0)
  %x = alloca ptr, align 8
  store ptr %1, ptr %x, align 8
  %x1 = load ptr, ptr %x, align 8
  %type_id = load i32, ptr %x1, align 4
  %is_result = icmp eq i32 %type_id, 1
  call void @_hulk_print_bool(i1 %is_result)
  %x2 = load ptr, ptr %x, align 8
  %type_id3 = load i32, ptr %x2, align 4
  %is_result4 = icmp eq i32 %type_id3, 0
  call void @_hulk_print_bool(i1 %is_result4)
  %x5 = load ptr, ptr %x, align 8
  call void @_hulk_print_bool(i1 true)
  %2 = call ptr @_hulk_alloc(i32 ptrtoint (ptr getelementptr (%A, ptr null, i32 1) to i32))
  %3 = call ptr @_new_A(ptr %2)
  %y = alloca ptr, align 8
  store ptr %3, ptr %y, align 8
  %y6 = load ptr, ptr %y, align 8
  call void @_hulk_print_bool(i1 true)
  %y7 = load ptr, ptr %y, align 8
  call void @_hulk_print_bool(i1 false)
  %4 = call ptr @_hulk_alloc(i32 ptrtoint (ptr getelementptr (%B, ptr null, i32 1) to i32))
  %5 = call ptr @_new_B(ptr %4)
  %y8 = alloca ptr, align 8
  store ptr %5, ptr %y8, align 8
  %y9 = load ptr, ptr %y8, align 8
  call void @_hulk_print_bool(i1 true)
  call void @_hulk_free(ptr %0)
  call void @_hulk_free(ptr %2)
  call void @_hulk_free(ptr %4)
  ret i32 0
}

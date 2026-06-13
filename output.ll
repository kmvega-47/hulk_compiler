; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@str = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.1 = private unnamed_addr constant [32 x i8] c"Linea1\0ALinea2\09Tabulado\22Comilla\22\00", align 1
@str.2 = private unnamed_addr constant [128 x i8] c"Este es un string mas largo que contiene multiples palabras para probar que el manejo de cadenas funciona correctamente en HULK\00", align 1

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

define i32 @main() {
entry:
  %PI = alloca double, align 8
  store double 0x400921FB54442D18, ptr %PI, align 8
  %E = alloca double, align 8
  store double 0x4005BF0A8B145769, ptr %E, align 8
  call void @_hulk_print_bool(i1 true)
  call void @_hulk_print_bool(i1 false)
  call void @_hulk_print_number(double 4.200000e+01)
  call void @_hulk_print_number(double 3.141600e+00)
  call void @_hulk_print_string(ptr @str)
  call void @_hulk_print_string(ptr @str.1)
  call void @_hulk_print_string(ptr @str.2)
  ret i32 0
}

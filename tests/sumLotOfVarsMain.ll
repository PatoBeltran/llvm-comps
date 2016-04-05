; ModuleID = 'sumLotOfVarsMain.c'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.11.0"

; Function Attrs: nounwind ssp uwtable
define i64 @inter(i64 %p, i64 %q, i64 %r) #0 {
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  %c = alloca i64, align 8
  %d = alloca i64, align 8
  %e = alloca i64, align 8
  %f = alloca i64, align 8
  %g = alloca i64, align 8
  %h = alloca i64, align 8
  %i = alloca i64, align 8
  %j = alloca i64, align 8
  %k = alloca i64, align 8
  %l = alloca i64, align 8
  %m = alloca i64, align 8
  %n = alloca i64, align 8
  %o = alloca i64, align 8
  store i64 %p, i64* %1, align 8
  store i64 %q, i64* %2, align 8
  store i64 %r, i64* %3, align 8
  store i64 10, i64* %a, align 8
  store i64 5, i64* %b, align 8
  store i64 4, i64* %c, align 8
  %4 = load i64, i64* %a, align 8
  %5 = load i64, i64* %b, align 8
  %6 = add nsw i64 %4, %5
  store i64 %6, i64* %d, align 8
  %7 = load i64, i64* %c, align 8
  %8 = add nsw i64 %7, 3
  store i64 %8, i64* %e, align 8
  %9 = load i64, i64* %a, align 8
  store i64 %9, i64* %f, align 8
  %10 = load i64, i64* %b, align 8
  %11 = load i64, i64* %f, align 8
  %12 = add nsw i64 %10, %11
  store i64 %12, i64* %g, align 8
  %13 = load i64, i64* %a, align 8
  %14 = add nsw i64 %13, 3
  store i64 %14, i64* %h, align 8
  %15 = load i64, i64* %b, align 8
  %16 = mul nsw i64 %15, 2
  store i64 %16, i64* %i, align 8
  %17 = load i64, i64* %e, align 8
  %18 = load i64, i64* %g, align 8
  %19 = add nsw i64 %17, %18
  store i64 %19, i64* %j, align 8
  %20 = load i64, i64* %c, align 8
  %21 = load i64, i64* %b, align 8
  %22 = mul nsw i64 %20, %21
  store i64 %22, i64* %k, align 8
  %23 = load i64, i64* %b, align 8
  %24 = sub nsw i64 10, %23
  store i64 %24, i64* %l, align 8
  %25 = load i64, i64* %g, align 8
  %26 = load i64, i64* %a, align 8
  %27 = sub nsw i64 %25, %26
  store i64 %27, i64* %m, align 8
  %28 = load i64, i64* %b, align 8
  %29 = mul nsw i64 4, %28
  store i64 %29, i64* %n, align 8
  %30 = load i64, i64* %c, align 8
  %31 = mul nsw i64 10, %30
  store i64 %31, i64* %o, align 8
  %32 = load i64, i64* %d, align 8
  %33 = load i64, i64* %c, align 8
  %34 = add nsw i64 %32, %33
  %35 = load i64, i64* %a, align 8
  %36 = add nsw i64 %34, %35
  %37 = load i64, i64* %b, align 8
  %38 = add nsw i64 %36, %37
  %39 = load i64, i64* %e, align 8
  %40 = load i64, i64* %f, align 8
  %41 = mul nsw i64 %39, %40
  %42 = load i64, i64* %g, align 8
  %43 = add nsw i64 %41, %42
  %44 = sub nsw i64 %38, %43
  %45 = load i64, i64* %h, align 8
  %46 = load i64, i64* %i, align 8
  %47 = add nsw i64 %45, %46
  %48 = load i64, i64* %j, align 8
  %49 = load i64, i64* %k, align 8
  %50 = sub nsw i64 %48, %49
  %51 = mul nsw i64 %47, %50
  %52 = load i64, i64* %l, align 8
  %53 = load i64, i64* %m, align 8
  %54 = add nsw i64 %52, %53
  %55 = load i64, i64* %n, align 8
  %56 = add nsw i64 %54, %55
  %57 = load i64, i64* %o, align 8
  %58 = sub nsw i64 %56, %57
  %59 = mul nsw i64 %51, %58
  %60 = load i64, i64* %1, align 8
  %61 = add nsw i64 %59, %60
  %62 = add nsw i64 %44, %61
  %63 = load i64, i64* %2, align 8
  %64 = add nsw i64 %62, %63
  %65 = load i64, i64* %3, align 8
  %66 = add nsw i64 %64, %65
  ret i64 %66
}

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 10, i32* %a, align 4
  %2 = load i32, i32* %a, align 4
  %3 = add nsw i32 3, %2
  ret i32 %3
}

attributes #0 = { nounwind ssp uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 7.3.0 (clang-703.0.29)"}

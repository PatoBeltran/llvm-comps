; ModuleID = 'variable2.c'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.11.0"

; Function Attrs: nounwind ssp uwtable
define i64 @inter() #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  %c = alloca i64, align 8
  %d = alloca i64, align 8
  store i64 10, i64* %a, align 8
  store i64 5, i64* %b, align 8
  store i64 4, i64* %c, align 8
  store i64 4, i64* %a, align 8
  %1 = load i64, i64* %a, align 8
  %2 = load i64, i64* %b, align 8
  %3 = add nsw i64 %1, %2
  store i64 %3, i64* %d, align 8
  store i64 5, i64* %a, align 8
  %4 = load i64, i64* %d, align 8
  %5 = load i64, i64* %c, align 8
  %6 = add nsw i64 %4, %5
  %7 = load i64, i64* %a, align 8
  %8 = add nsw i64 %6, %7
  %9 = load i64, i64* %b, align 8
  %10 = add nsw i64 %8, %9
  ret i64 %10
}

attributes #0 = { nounwind ssp uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 7.3.0 (clang-703.0.29)"}

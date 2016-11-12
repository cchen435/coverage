; ModuleID = 'struct.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.rect = type { i32, i32 }

; Function Attrs: nounwind uwtable
define i32 @area(i64 %r.coerce) #0 {
entry:
  %r = alloca %struct.rect, align 8
  %w = alloca i32, align 4
  %l = alloca i32, align 4
  %size = alloca i32, align 4
  %0 = bitcast %struct.rect* %r to i64*
  store i64 %r.coerce, i64* %0, align 1
  %a = getelementptr inbounds %struct.rect* %r, i32 0, i32 0
  %1 = load i32* %a, align 4
  store i32 %1, i32* %w, align 4
  %b = getelementptr inbounds %struct.rect* %r, i32 0, i32 1
  %2 = load i32* %b, align 4
  store i32 %2, i32* %l, align 4
  %3 = load i32* %w, align 4
  %4 = load i32* %l, align 4
  %mul = mul nsw i32 %3, %4
  store i32 %mul, i32* %size, align 4
  %5 = load i32* %size, align 4
  ret i32 %5
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %w = alloca i32, align 4
  %l = alloca i32, align 4
  %size = alloca i32, align 4
  %r = alloca %struct.rect, align 4
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 20, i32* %w, align 4
  store i32 30, i32* %l, align 4
  store i32 0, i32* %size, align 4
  %0 = load i32* %w, align 4
  %a = getelementptr inbounds %struct.rect* %r, i32 0, i32 0
  store i32 %0, i32* %a, align 4
  %1 = load i32* %l, align 4
  %b = getelementptr inbounds %struct.rect* %r, i32 0, i32 1
  store i32 %1, i32* %b, align 4
  %2 = bitcast %struct.rect* %r to i64*
  %3 = load i64* %2, align 1
  %call = call i32 @area(i64 %3)
  store i32 %call, i32* %size, align 4
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.2 (tags/RELEASE_352/final)"}

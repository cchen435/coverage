; ModuleID = 'global-struct.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.rect = type { i32, i32, i32 }

@r = common global %struct.rect zeroinitializer, align 4

; Function Attrs: nounwind uwtable
define i32 @area(i64 %shape.coerce0, i32 %shape.coerce1) #0 {
entry:
  %shape = alloca %struct.rect, align 8
  %coerce = alloca { i64, i32 }, align 8
  %w = alloca i32, align 4
  %l = alloca i32, align 4
  %size = alloca i32, align 4
  %0 = getelementptr { i64, i32 }* %coerce, i32 0, i32 0
  store i64 %shape.coerce0, i64* %0
  %1 = getelementptr { i64, i32 }* %coerce, i32 0, i32 1
  store i32 %shape.coerce1, i32* %1
  %2 = bitcast %struct.rect* %shape to i8*
  %3 = bitcast { i64, i32 }* %coerce to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 12, i32 8, i1 false)
  %a = getelementptr inbounds %struct.rect* %shape, i32 0, i32 0
  %4 = load i32* %a, align 4
  store i32 %4, i32* %w, align 4
  %b = getelementptr inbounds %struct.rect* %shape, i32 0, i32 1
  %5 = load i32* %b, align 4
  store i32 %5, i32* %l, align 4
  %6 = load i32* %w, align 4
  %7 = load i32* %l, align 4
  %mul = mul nsw i32 %6, %7
  store i32 %mul, i32* %size, align 4
  %8 = load i32* %size, align 4
  ret i32 %8
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %w = alloca i32, align 4
  %l = alloca i32, align 4
  %size = alloca i32, align 4
  %r.coerce = alloca { i64, i32 }
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 20, i32* %w, align 4
  store i32 30, i32* %l, align 4
  store i32 0, i32* %size, align 4
  %0 = load i32* %w, align 4
  store i32 %0, i32* getelementptr inbounds (%struct.rect* @r, i32 0, i32 0), align 4
  %1 = load i32* %l, align 4
  store i32 %1, i32* getelementptr inbounds (%struct.rect* @r, i32 0, i32 1), align 4
  %2 = bitcast { i64, i32 }* %r.coerce to i8*
  %3 = bitcast %struct.rect* @r to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 12, i32 0, i1 false)
  %4 = getelementptr { i64, i32 }* %r.coerce, i32 0, i32 0
  %5 = load i64* %4, align 1
  %6 = getelementptr { i64, i32 }* %r.coerce, i32 0, i32 1
  %7 = load i32* %6, align 1
  %call = call i32 @area(i64 %5, i32 %7)
  store i32 %call, i32* %size, align 4
  %8 = load i32* %size, align 4
  store i32 %8, i32* getelementptr inbounds (%struct.rect* @r, i32 0, i32 2), align 4
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.2 (tags/RELEASE_352/final)"}

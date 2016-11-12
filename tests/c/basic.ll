; ModuleID = 'basic.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [17 x i8] c"Hello World: %d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @func(i32 %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  %0 = load i32* %a.addr, align 4
  %add = add nsw i32 %0, 3
  store i32 %add, i32* %b, align 4
  %1 = load i32* %a.addr, align 4
  %add1 = add nsw i32 %1, 4
  store i32 %add1, i32* %a.addr, align 4
  %2 = load i32* %a.addr, align 4
  %mul = mul nsw i32 %2, 8
  store i32 %mul, i32* %c, align 4
  %3 = load i32* %b, align 4
  %cmp = icmp sgt i32 %3, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %4 = load i32* %a.addr, align 4
  %5 = load i32* %b, align 4
  %add2 = add nsw i32 %4, %5
  store i32 %add2, i32* %d, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %6 = load i32* %a.addr, align 4
  %7 = load i32* %b, align 4
  %mul3 = mul nsw i32 %6, %7
  store i32 %mul3, i32* %d, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %8 = load i32* %d, align 4
  %9 = load i32* %a.addr, align 4
  %mul4 = mul nsw i32 %8, %9
  store i32 %mul4, i32* %e, align 4
  %10 = load i32* %e, align 4
  ret i32 %10
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %a = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call i32 @func(i32 10)
  store i32 %call, i32* %a, align 4
  %0 = load i32* %a, align 4
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([17 x i8]* @.str, i32 0, i32 0), i32 %0)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.2 (tags/RELEASE_352/final)"}

; ModuleID = 'basic.c'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

; Function Attrs: nounwind ssp uwtable
define i32 @add(i32* %a, i32* %b, i32* %c, i32* %d, i32 %size) #0 {
entry:
  %a.addr = alloca i32*, align 8
  %b.addr = alloca i32*, align 8
  %c.addr = alloca i32*, align 8
  %d.addr = alloca i32*, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %l = alloca i32, align 4
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  store i32* %a, i32** %a.addr, align 8
  store i32* %b, i32** %b.addr, align 8
  store i32* %c, i32** %c.addr, align 8
  store i32* %d, i32** %d.addr, align 8
  store i32 %size, i32* %size.addr, align 4
  store i32 0, i32* %i, align 4
  %0 = load i32* %size.addr, align 4
  store i32 %0, i32* %j, align 4
  %1 = load i32* %i, align 4
  %2 = load i32* %j, align 4
  %add = add nsw i32 %1, %2
  store i32 %add, i32* %k, align 4
  %3 = load i32* %m, align 4
  %4 = load i32* %k, align 4
  %add1 = add nsw i32 %3, %4
  %5 = load i32* %i, align 4
  %add2 = add nsw i32 %add1, %5
  %6 = load i32* %j, align 4
  %add3 = add nsw i32 %add2, %6
  store i32 %add3, i32* %l, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %7 = load i32* %i, align 4
  %8 = load i32* %size.addr, align 4
  %div = sdiv i32 %8, 2
  %cmp = icmp slt i32 %7, %div
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %9 = load i32* %i, align 4
  %mul = mul nsw i32 2, %9
  %idxprom = sext i32 %mul to i64
  %10 = load i32** %a.addr, align 8
  %arrayidx = getelementptr inbounds i32* %10, i64 %idxprom
  %11 = load i32* %arrayidx, align 4, !coverage !1
  %12 = load i32* %i, align 4
  %mul4 = mul nsw i32 2, %12
  %add5 = add nsw i32 %mul4, 1
  %idxprom6 = sext i32 %add5 to i64
  %13 = load i32** %b.addr, align 8
  %arrayidx7 = getelementptr inbounds i32* %13, i64 %idxprom6
  %14 = load i32* %arrayidx7, align 4, !coverage !2
  %add8 = add nsw i32 %11, %14
  %15 = load i32* %i, align 4
  %mul9 = mul nsw i32 %15, 4
  %idxprom10 = sext i32 %mul9 to i64
  %16 = load i32** %a.addr, align 8
  %arrayidx11 = getelementptr inbounds i32* %16, i64 %idxprom10
  %17 = load i32* %arrayidx11, align 4, !coverage !3
  %add12 = add nsw i32 %add8, %17
  %18 = load i32* %i, align 4
  %idxprom13 = sext i32 %18 to i64
  %19 = load i32** %c.addr, align 8
  %arrayidx14 = getelementptr inbounds i32* %19, i64 %idxprom13
  store i32 %add12, i32* %arrayidx14, align 4
  %20 = load i32* %i, align 4
  %mul15 = mul nsw i32 2, %20
  %add16 = add nsw i32 %mul15, 1
  %idxprom17 = sext i32 %add16 to i64
  %21 = load i32** %a.addr, align 8
  %arrayidx18 = getelementptr inbounds i32* %21, i64 %idxprom17
  %22 = load i32* %arrayidx18, align 4, !coverage !2
  %23 = load i32* %i, align 4
  %mul19 = mul nsw i32 2, %23
  %idxprom20 = sext i32 %mul19 to i64
  %24 = load i32** %b.addr, align 8
  %arrayidx21 = getelementptr inbounds i32* %24, i64 %idxprom20
  %25 = load i32* %arrayidx21, align 4, !coverage !1
  %add22 = add nsw i32 %22, %25
  %26 = load i32* %i, align 4
  %idxprom23 = sext i32 %26 to i64
  %27 = load i32** %d.addr, align 8
  %arrayidx24 = getelementptr inbounds i32* %27, i64 %idxprom23
  store i32 %add22, i32* %arrayidx24, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %28 = load i32* %i, align 4
  %inc = add nsw i32 %28, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %29 = load i32* %i, align 4
  %30 = load i32* %j, align 4
  %cmp25 = icmp sgt i32 %29, %30
  br i1 %cmp25, label %if.then, label %if.else

if.then:                                          ; preds = %for.end
  %31 = load i32* %j, align 4
  %add26 = add nsw i32 %31, 4
  store i32 %add26, i32* %i, align 4
  %32 = load i32* %j, align 4
  %33 = load i32* %k, align 4
  %add27 = add nsw i32 %32, %33
  store i32 %add27, i32* %j, align 4
  br label %if.end

if.else:                                          ; preds = %for.end
  %34 = load i32* %m, align 4
  %add28 = add nsw i32 %34, 4
  store i32 %add28, i32* %l, align 4
  %35 = load i32* %k, align 4
  %36 = load i32* %i, align 4
  %add29 = add nsw i32 %35, %36
  store i32 %add29, i32* %n, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %37 = load i32* %l, align 4
  %38 = load i32* %m, align 4
  %cmp30 = icmp sgt i32 %37, %38
  br i1 %cmp30, label %if.then31, label %if.end34

if.then31:                                        ; preds = %if.end
  %39 = load i32* %j, align 4
  %add32 = add nsw i32 %39, 4
  store i32 %add32, i32* %i, align 4
  %40 = load i32* %j, align 4
  %41 = load i32* %k, align 4
  %add33 = add nsw i32 %40, %41
  store i32 %add33, i32* %j, align 4
  br label %if.end34

if.end34:                                         ; preds = %if.then31, %if.end
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end34
  %42 = load i32* %i, align 4
  %43 = load i32* %j, align 4
  %cmp35 = icmp slt i32 %42, %43
  br i1 %cmp35, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %44 = load i32* %i, align 4
  %inc36 = add nsw i32 %44, 1
  store i32 %inc36, i32* %i, align 4
  %45 = load i32* %n, align 4
  %46 = load i32* %m, align 4
  %add37 = add nsw i32 %46, %45
  store i32 %add37, i32* %m, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.2 (tags/RELEASE_352/final)"}
!1 = metadata !{metadata !"2*i"}
!2 = metadata !{metadata !"2*i+1"}
!3 = metadata !{metadata !"i*4"}

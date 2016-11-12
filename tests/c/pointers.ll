; ModuleID = 'pointers.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@main.array_a = private unnamed_addr constant [10 x i32] [i32 5, i32 4, i32 3, i32 2, i32 0, i32 8, i32 6, i32 9, i32 1, i32 7], align 16
@main.array_b = private unnamed_addr constant [10 x i32] [i32 5, i32 4, i32 3, i32 2, i32 0, i32 8, i32 6, i32 9, i32 1, i32 7], align 16

; Function Attrs: nounwind uwtable
define void @swap(i32* %a, i32 %i, i32 %j) #0 {
entry:
  %a.addr = alloca i32*, align 8
  %i.addr = alloca i32, align 4
  %j.addr = alloca i32, align 4
  %tmp = alloca i32, align 4
  store i32* %a, i32** %a.addr, align 8
  store i32 %i, i32* %i.addr, align 4
  store i32 %j, i32* %j.addr, align 4
  %0 = load i32* %i.addr, align 4
  %idxprom = sext i32 %0 to i64
  %1 = load i32** %a.addr, align 8
  %arrayidx = getelementptr inbounds i32* %1, i64 %idxprom
  %2 = load i32* %arrayidx, align 4
  store i32 %2, i32* %tmp, align 4
  %3 = load i32* %j.addr, align 4
  %idxprom1 = sext i32 %3 to i64
  %4 = load i32** %a.addr, align 8
  %arrayidx2 = getelementptr inbounds i32* %4, i64 %idxprom1
  %5 = load i32* %arrayidx2, align 4
  %6 = load i32* %i.addr, align 4
  %idxprom3 = sext i32 %6 to i64
  %7 = load i32** %a.addr, align 8
  %arrayidx4 = getelementptr inbounds i32* %7, i64 %idxprom3
  store i32 %5, i32* %arrayidx4, align 4
  %8 = load i32* %tmp, align 4
  %9 = load i32* %j.addr, align 4
  %idxprom5 = sext i32 %9 to i64
  %10 = load i32** %a.addr, align 8
  %arrayidx6 = getelementptr inbounds i32* %10, i64 %idxprom5
  store i32 %8, i32* %arrayidx6, align 4
  ret void
}

; Function Attrs: nounwind uwtable
define void @add(i32* %a, i32* %b, i32* %c, i32 %size) #0 {
entry:
  %a.addr = alloca i32*, align 8
  %b.addr = alloca i32*, align 8
  %c.addr = alloca i32*, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %a, i32** %a.addr, align 8
  store i32* %b, i32** %b.addr, align 8
  store i32* %c, i32** %c.addr, align 8
  store i32 %size, i32* %size.addr, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32* %i, align 4
  %1 = load i32* %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32* %i, align 4
  %idxprom = sext i32 %2 to i64
  %3 = load i32** %a.addr, align 8
  %arrayidx = getelementptr inbounds i32* %3, i64 %idxprom
  %4 = load i32* %arrayidx, align 4
  %5 = load i32* %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %6 = load i32** %b.addr, align 8
  %arrayidx2 = getelementptr inbounds i32* %6, i64 %idxprom1
  %7 = load i32* %arrayidx2, align 4
  %add = add nsw i32 %4, %7
  %8 = load i32* %i, align 4
  %idxprom3 = sext i32 %8 to i64
  %9 = load i32** %c.addr, align 8
  %arrayidx4 = getelementptr inbounds i32* %9, i64 %idxprom3
  store i32 %add, i32* %arrayidx4, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %10 = load i32* %i, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @sort(i32* %a, i32 %size) #0 {
entry:
  %a.addr = alloca i32*, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32* %a, i32** %a.addr, align 8
  store i32 %size, i32* %size.addr, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc7, %entry
  %0 = load i32* %i, align 4
  %1 = load i32* %size.addr, align 4
  %sub = sub nsw i32 %1, 1
  %cmp = icmp slt i32 %0, %sub
  br i1 %cmp, label %for.body, label %for.end9

for.body:                                         ; preds = %for.cond
  %2 = load i32* %i, align 4
  %add = add nsw i32 %2, 1
  store i32 %add, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %3 = load i32* %j, align 4
  %4 = load i32* %size.addr, align 4
  %cmp2 = icmp slt i32 %3, %4
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %5 = load i32* %i, align 4
  %idxprom = sext i32 %5 to i64
  %6 = load i32** %a.addr, align 8
  %arrayidx = getelementptr inbounds i32* %6, i64 %idxprom
  %7 = load i32* %arrayidx, align 4
  %8 = load i32* %j, align 4
  %idxprom4 = sext i32 %8 to i64
  %9 = load i32** %a.addr, align 8
  %arrayidx5 = getelementptr inbounds i32* %9, i64 %idxprom4
  %10 = load i32* %arrayidx5, align 4
  %cmp6 = icmp sgt i32 %7, %10
  br i1 %cmp6, label %if.then, label %if.end

if.then:                                          ; preds = %for.body3
  %11 = load i32** %a.addr, align 8
  %12 = load i32* %i, align 4
  %13 = load i32* %j, align 4
  call void @swap(i32* %11, i32 %12, i32 %13)
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body3
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %14 = load i32* %j, align 4
  %inc = add nsw i32 %14, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc7

for.inc7:                                         ; preds = %for.end
  %15 = load i32* %i, align 4
  %inc8 = add nsw i32 %15, 1
  store i32 %inc8, i32* %i, align 4
  br label %for.cond

for.end9:                                         ; preds = %for.cond
  ret i32 0
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %array_a = alloca [10 x i32], align 16
  %array_b = alloca [10 x i32], align 16
  %array_c = alloca [10 x i32], align 16
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %0 = bitcast [10 x i32]* %array_a to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([10 x i32]* @main.array_a to i8*), i64 40, i32 16, i1 false)
  %1 = bitcast [10 x i32]* %array_b to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* bitcast ([10 x i32]* @main.array_b to i8*), i64 40, i32 16, i1 false)
  %arraydecay = getelementptr inbounds [10 x i32]* %array_a, i32 0, i32 0
  %call = call i32 @sort(i32* %arraydecay, i32 10)
  %arraydecay1 = getelementptr inbounds [10 x i32]* %array_a, i32 0, i32 0
  %arraydecay2 = getelementptr inbounds [10 x i32]* %array_b, i32 0, i32 0
  %arraydecay3 = getelementptr inbounds [10 x i32]* %array_c, i32 0, i32 0
  call void @add(i32* %arraydecay1, i32* %arraydecay2, i32* %arraydecay3, i32 10)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.2 (tags/RELEASE_352/final)"}

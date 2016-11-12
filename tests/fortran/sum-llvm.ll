; ModuleID = 'sum.f90'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64--linux-gnu"

module asm "\09.ident\09\22GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4 LLVM: 3.5.2svn\22"

%struct.__st_parameter_dt = type { %struct.__st_parameter_common, i64, i64*, i64*, i8*, i8*, i32, i32, i8*, i8*, i32, i32, i8*, [256 x i8], i32*, i64, i8*, i32, i32, i8*, i8*, i32, i32, i8*, i8*, i32, i32, i8*, i8*, i32, [4 x i8] }
%struct.__st_parameter_common = type { i32, i32, i8*, i32, i32, i8*, i32* }

@.cst = private constant [8 x i8] c"sum.f90\00", align 8
@.cst1 = private constant [18 x i8] c"Enter Two Numbers:", align 8
@.cst2 = private constant [15 x i8] c"The total is : ", align 8

; Function Attrs: nounwind uwtable
define void @sum_() unnamed_addr #0 {
entry:
  %answer = alloca float
  %x = alloca float
  %y = alloca float
  %dt_parm.0 = alloca %struct.__st_parameter_dt
  %dt_parm.1 = alloca %struct.__st_parameter_dt
  %dt_parm.2 = alloca %struct.__st_parameter_dt
  %dt_parm.3 = alloca %struct.__st_parameter_dt
  %"alloca point" = bitcast i32 0 to i32
  %"ssa point" = bitcast i32 0 to i32
  br label %"2"

"2":                                              ; preds = %entry
  %0 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %1 = getelementptr inbounds %struct.__st_parameter_common* %0, i32 0, i32 2
  store i8* getelementptr inbounds ([8 x i8]* @.cst, i64 0, i64 0), i8** %1, align 8
  %2 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %3 = getelementptr inbounds %struct.__st_parameter_common* %2, i32 0, i32 3
  store i32 3, i32* %3, align 8
  %4 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %5 = getelementptr inbounds %struct.__st_parameter_common* %4, i32 0, i32 0
  store i32 128, i32* %5, align 8
  %6 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %7 = getelementptr inbounds %struct.__st_parameter_common* %6, i32 0, i32 1
  store i32 6, i32* %7, align 4
  call void @_gfortran_st_write(%struct.__st_parameter_dt* %dt_parm.0) #1
  call void @_gfortran_transfer_character_write(%struct.__st_parameter_dt* %dt_parm.0, i8* getelementptr inbounds ([18 x i8]* @.cst1, i64 0, i64 0), i32 18) #1
  call void @_gfortran_st_write_done(%struct.__st_parameter_dt* %dt_parm.0) #1
  %8 = bitcast %struct.__st_parameter_dt* %dt_parm.0 to i8*
  call void @llvm.lifetime.end(i64 480, i8* %8)
  %9 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.1, i32 0, i32 0
  %10 = getelementptr inbounds %struct.__st_parameter_common* %9, i32 0, i32 2
  store i8* getelementptr inbounds ([8 x i8]* @.cst, i64 0, i64 0), i8** %10, align 8
  %11 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.1, i32 0, i32 0
  %12 = getelementptr inbounds %struct.__st_parameter_common* %11, i32 0, i32 3
  store i32 4, i32* %12, align 8
  %13 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.1, i32 0, i32 0
  %14 = getelementptr inbounds %struct.__st_parameter_common* %13, i32 0, i32 0
  store i32 128, i32* %14, align 8
  %15 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.1, i32 0, i32 0
  %16 = getelementptr inbounds %struct.__st_parameter_common* %15, i32 0, i32 1
  store i32 5, i32* %16, align 4
  call void @_gfortran_st_read(%struct.__st_parameter_dt* %dt_parm.1) #1
  call void bitcast (void (%struct.__st_parameter_dt*, i8*, i32)* @_gfortran_transfer_real to void (%struct.__st_parameter_dt*, float*, i32)*)(%struct.__st_parameter_dt* %dt_parm.1, float* %x, i32 4) #1
  call void @_gfortran_st_read_done(%struct.__st_parameter_dt* %dt_parm.1) #1
  %17 = bitcast %struct.__st_parameter_dt* %dt_parm.1 to i8*
  call void @llvm.lifetime.end(i64 480, i8* %17)
  %18 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.2, i32 0, i32 0
  %19 = getelementptr inbounds %struct.__st_parameter_common* %18, i32 0, i32 2
  store i8* getelementptr inbounds ([8 x i8]* @.cst, i64 0, i64 0), i8** %19, align 8
  %20 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.2, i32 0, i32 0
  %21 = getelementptr inbounds %struct.__st_parameter_common* %20, i32 0, i32 3
  store i32 5, i32* %21, align 8
  %22 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.2, i32 0, i32 0
  %23 = getelementptr inbounds %struct.__st_parameter_common* %22, i32 0, i32 0
  store i32 128, i32* %23, align 8
  %24 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.2, i32 0, i32 0
  %25 = getelementptr inbounds %struct.__st_parameter_common* %24, i32 0, i32 1
  store i32 5, i32* %25, align 4
  call void @_gfortran_st_read(%struct.__st_parameter_dt* %dt_parm.2) #1
  call void bitcast (void (%struct.__st_parameter_dt*, i8*, i32)* @_gfortran_transfer_real to void (%struct.__st_parameter_dt*, float*, i32)*)(%struct.__st_parameter_dt* %dt_parm.2, float* %y, i32 4) #1
  call void @_gfortran_st_read_done(%struct.__st_parameter_dt* %dt_parm.2) #1
  %26 = bitcast %struct.__st_parameter_dt* %dt_parm.2 to i8*
  call void @llvm.lifetime.end(i64 480, i8* %26)
  %27 = load float* %x, align 4
  %28 = load float* %y, align 4
  %29 = fadd float %27, %28
  store float %29, float* %answer, align 4
  %30 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.3, i32 0, i32 0
  %31 = getelementptr inbounds %struct.__st_parameter_common* %30, i32 0, i32 2
  store i8* getelementptr inbounds ([8 x i8]* @.cst, i64 0, i64 0), i8** %31, align 8
  %32 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.3, i32 0, i32 0
  %33 = getelementptr inbounds %struct.__st_parameter_common* %32, i32 0, i32 3
  store i32 7, i32* %33, align 8
  %34 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.3, i32 0, i32 0
  %35 = getelementptr inbounds %struct.__st_parameter_common* %34, i32 0, i32 0
  store i32 128, i32* %35, align 8
  %36 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.3, i32 0, i32 0
  %37 = getelementptr inbounds %struct.__st_parameter_common* %36, i32 0, i32 1
  store i32 6, i32* %37, align 4
  call void @_gfortran_st_write(%struct.__st_parameter_dt* %dt_parm.3) #1
  call void @_gfortran_transfer_character_write(%struct.__st_parameter_dt* %dt_parm.3, i8* getelementptr inbounds ([15 x i8]* @.cst2, i64 0, i64 0), i32 15) #1
  call void bitcast (void (%struct.__st_parameter_dt*, i8*, i32)* @_gfortran_transfer_real_write to void (%struct.__st_parameter_dt*, float*, i32)*)(%struct.__st_parameter_dt* %dt_parm.3, float* %answer, i32 4) #1
  call void @_gfortran_st_write_done(%struct.__st_parameter_dt* %dt_parm.3) #1
  %38 = bitcast %struct.__st_parameter_dt* %dt_parm.3 to i8*
  call void @llvm.lifetime.end(i64 480, i8* %38)
  %39 = bitcast float* %answer to i8*
  call void @llvm.lifetime.end(i64 4, i8* %39)
  %40 = bitcast float* %x to i8*
  call void @llvm.lifetime.end(i64 4, i8* %40)
  %41 = bitcast float* %y to i8*
  call void @llvm.lifetime.end(i64 4, i8* %41)
  br label %return

return:                                           ; preds = %"2"
  ret void
}

declare void @_gfortran_st_write(%struct.__st_parameter_dt*)

declare void @_gfortran_transfer_character_write(%struct.__st_parameter_dt*, i8*, i32)

declare void @_gfortran_st_write_done(%struct.__st_parameter_dt*)

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

declare void @_gfortran_st_read(%struct.__st_parameter_dt*)

declare void @_gfortran_transfer_real(%struct.__st_parameter_dt*, i8*, i32)

declare void @_gfortran_st_read_done(%struct.__st_parameter_dt*)

declare void @_gfortran_transfer_real_write(%struct.__st_parameter_dt*, i8*, i32)

attributes #0 = { nounwind uwtable "no-frame-pointer-elim-non-leaf"="true" }
attributes #1 = { nounwind }

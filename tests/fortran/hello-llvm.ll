; ModuleID = 'hello.f90'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64--linux-gnu"

module asm "\09.ident\09\22GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4 LLVM: 3.5.2svn\22"

%struct.__st_parameter_dt = type { %struct.__st_parameter_common, i64, i64*, i64*, i8*, i8*, i32, i32, i8*, i8*, i32, i32, i8*, [256 x i8], i32*, i64, i8*, i32, i32, i8*, i8*, i32, i32, i8*, i8*, i32, i32, i8*, i8*, i32, [4 x i8] }
%struct.__st_parameter_common = type { i32, i32, i8*, i32, i32, i8*, i32* }

@.cst = private constant [10 x i8] c"hello.f90\00", align 8
@.cst1 = private constant [11 x i8] c"Hello World", align 8
@options.1.1882 = internal constant [7 x i32] [i32 68, i32 1023, i32 0, i32 0, i32 1, i32 1, i32 0], align 16

; Function Attrs: nounwind uwtable
define internal void @MAIN__() #0 {
entry:
  %dt_parm.0 = alloca %struct.__st_parameter_dt
  %"alloca point" = bitcast i32 0 to i32
  %"ssa point" = bitcast i32 0 to i32
  br label %"2"

"2":                                              ; preds = %entry
  %0 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %1 = getelementptr inbounds %struct.__st_parameter_common* %0, i32 0, i32 2
  store i8* getelementptr inbounds ([10 x i8]* @.cst, i64 0, i64 0), i8** %1, align 8
  %2 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %3 = getelementptr inbounds %struct.__st_parameter_common* %2, i32 0, i32 3
  store i32 2, i32* %3, align 8
  %4 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %5 = getelementptr inbounds %struct.__st_parameter_common* %4, i32 0, i32 0
  store i32 128, i32* %5, align 8
  %6 = getelementptr inbounds %struct.__st_parameter_dt* %dt_parm.0, i32 0, i32 0
  %7 = getelementptr inbounds %struct.__st_parameter_common* %6, i32 0, i32 1
  store i32 6, i32* %7, align 4
  call void @_gfortran_st_write(%struct.__st_parameter_dt* %dt_parm.0) #1
  call void @_gfortran_transfer_character_write(%struct.__st_parameter_dt* %dt_parm.0, i8* getelementptr inbounds ([11 x i8]* @.cst1, i64 0, i64 0), i32 11) #1
  call void @_gfortran_st_write_done(%struct.__st_parameter_dt* %dt_parm.0) #1
  %8 = bitcast %struct.__st_parameter_dt* %dt_parm.0 to i8*
  call void @llvm.lifetime.end(i64 480, i8* %8)
  call void bitcast (void (...)* @sum_ to void ()*)() #1
  br label %return

return:                                           ; preds = %"2"
  ret void
}

declare void @_gfortran_st_write(%struct.__st_parameter_dt*)

declare void @_gfortran_transfer_character_write(%struct.__st_parameter_dt*, i8*, i32)

declare void @_gfortran_st_write_done(%struct.__st_parameter_dt*)

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

declare void @sum_(...)

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) unnamed_addr #0 {
entry:
  %argc_addr = alloca i32, align 4
  %argv_addr = alloca i8**, align 8
  %"<retval>" = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  store i32 %argc, i32* %argc_addr, align 1
  store i8** %argv, i8*** %argv_addr, align 1
  %0 = load i32* %argc_addr, align 4
  %1 = load i8*** %argv_addr, align 8
  %"ssa point" = bitcast i32 0 to i32
  br label %"2"

"2":                                              ; preds = %entry
  call void @_gfortran_set_args(i32 %0, i8** %1) #1
  call void @_gfortran_set_options(i32 7, i32* getelementptr inbounds ([7 x i32]* @options.1.1882, i64 0, i64 0)) #1
  call void @MAIN__() #0
  br label %"3"

"3":                                              ; preds = %"2"
  store i32 0, i32* %"<retval>", align 1
  br label %return

return:                                           ; preds = %"3"
  %2 = load i32* %"<retval>", align 4
  ret i32 %2
}

declare void @_gfortran_set_args(i32, i8**)

declare void @_gfortran_set_options(i32, i32*)

attributes #0 = { nounwind uwtable "no-frame-pointer-elim-non-leaf"="true" }
attributes #1 = { nounwind }

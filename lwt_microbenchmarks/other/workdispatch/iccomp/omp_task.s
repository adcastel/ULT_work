# mark_description "Intel(R) C Intel(R) 64 Compiler for applications running on Intel(R) 64, Version 16.0.0.109 Build 20150815";
# mark_description "-Wall -O0 -S -fsource-asm -openmp";
	.file "omp_task.c"
	.text
..TXTST0:
# -- Begin  init
	.section .rodata, "a"
	.align 4
	.align 4
.L_2il0floatpacket.0:
	.long	0x42c80000
	.type	.L_2il0floatpacket.0,@object
	.size	.L_2il0floatpacket.0,4
	.text
# mark_begin;

	.globl init
# --- init(float *, int)
init:
# parameter 1: %rdi
# parameter 2: %esi
..B1.1:                         # Preds ..B1.0

### {

	.cfi_startproc
..___tag_value_init.1:
..L2:
                                                          #28.1
        pushq     %rbp                                          #28.1
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #28.1
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $32, %rsp                                     #28.1
        movq      %rdi, -24(%rbp)                               #28.1
        movl      %esi, -16(%rbp)                               #28.1

###     int i = 0;

        movl      $0, -32(%rbp)                                 #29.11

###     for (i = 0; i < n; i++) {

        movl      $0, -32(%rbp)                                 #30.10
        movl      -32(%rbp), %eax                               #30.17
        movl      -16(%rbp), %edx                               #30.21
        cmpl      %edx, %eax                                    #30.21
        jge       ..B1.3        # Prob 50%                      #30.21
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B1.2:                         # Preds ..B1.2 ..B1.1

###         v[i] = i + 100.0f;

        movl      -32(%rbp), %eax                               #31.16
        cvtsi2ss  %eax, %xmm0                                   #31.16
        movss     .L_2il0floatpacket.0(%rip), %xmm1             #31.20
        addss     %xmm1, %xmm0                                  #31.20
        movl      -32(%rbp), %eax                               #31.11
        movslq    %eax, %rax                                    #31.9
        imulq     $4, %rax, %rax                                #31.9
        addq      -24(%rbp), %rax                               #31.9
        movss     %xmm0, (%rax)                                 #31.9
        movl      $1, %eax                                      #30.24
        addl      -32(%rbp), %eax                               #30.24
        movl      %eax, -32(%rbp)                               #30.24
        movl      -32(%rbp), %eax                               #30.17
        movl      -16(%rbp), %edx                               #30.21
        cmpl      %edx, %eax                                    #30.21
        jl        ..B1.2        # Prob 50%                      #30.21
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B1.3:                         # Preds ..B1.2 ..B1.1

###     }
### }

        leave                                                   #33.1
	.cfi_restore 6
        ret                                                     #33.1
	.cfi_endproc
                                # LOE
# mark_end;
	.type	init,@function
	.size	init,.-init
	.data
# -- End  init
	.text
# -- Begin  check
	.section .rodata, "a"
	.align 4
.L_2il0floatpacket.1:
	.long	0x3f666666
	.type	.L_2il0floatpacket.1,@object
	.size	.L_2il0floatpacket.1,4
	.align 4
.L_2__STRING.0:
	.long	1680169846
	.long	809319517
	.long	174469166
	.byte	0
	.type	.L_2__STRING.0,@object
	.size	.L_2__STRING.0,13
	.text
# mark_begin;

	.globl check
# --- check(float *, int)
check:
# parameter 1: %rdi
# parameter 2: %esi
..B2.1:                         # Preds ..B2.0

### {

	.cfi_startproc
..___tag_value_check.8:
..L9:
                                                          #38.1
        pushq     %rbp                                          #38.1
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #38.1
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $32, %rsp                                     #38.1
        movq      %rdi, -24(%rbp)                               #38.1
        movl      %esi, -16(%rbp)                               #38.1

###     int i = 0;

        movl      $0, -32(%rbp)                                 #39.11

###     for (i = 0; i < n; i++) {

        movl      $0, -32(%rbp)                                 #40.10
        movl      -32(%rbp), %eax                               #40.17
        movl      -16(%rbp), %edx                               #40.21
        cmpl      %edx, %eax                                    #40.21
        jl        ..B2.3        # Prob 50%                      #40.21
        jmp       ..B2.5        # Prob 100%                     #40.21
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B2.2:                         # Preds ..B2.8 ..B2.3
        movl      $1, %eax                                      #40.24
        addl      -32(%rbp), %eax                               #40.24
        movl      %eax, -32(%rbp)                               #40.24
        movl      -32(%rbp), %eax                               #40.17
        movl      -16(%rbp), %edx                               #40.21
        cmpl      %edx, %eax                                    #40.21
        jge       ..B2.5        # Prob 50%                      #40.21
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B2.3:                         # Preds ..B2.2 ..B2.1

###         if (v[i] != (i+100.0f)*0.9f) {

        movl      -32(%rbp), %eax                               #41.15
        movslq    %eax, %rax                                    #41.13
        imulq     $4, %rax, %rax                                #41.13
        addq      -24(%rbp), %rax                               #41.13
        movss     (%rax), %xmm0                                 #41.13
        movl      -32(%rbp), %eax                               #41.22
        cvtsi2ss  %eax, %xmm1                                   #41.22
        movss     .L_2il0floatpacket.0(%rip), %xmm2             #41.24
        addss     %xmm2, %xmm1                                  #41.24
        movss     .L_2il0floatpacket.1(%rip), %xmm2             #41.32
        mulss     %xmm2, %xmm1                                  #41.32
        ucomiss   %xmm1, %xmm0                                  #41.32
        jp        ..B2.4        # Prob 0%                       #41.32
        je        ..B2.2        # Prob 50%                      #41.32
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B2.4:                         # Preds ..B2.3

###             printf("v[%d]<=0.0f\n", i);

        movl      $.L_2__STRING.0, %eax                         #42.13
        movl      -32(%rbp), %edx                               #42.13
        movq      %rax, %rdi                                    #42.13
        movl      %edx, %esi                                    #42.13
        movl      $0, %eax                                      #42.13
..___tag_value_check.13:
#       printf(const char *__restrict__, ...)
        call      printf                                        #42.13
..___tag_value_check.14:
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip eax
..B2.8:                         # Preds ..B2.4
        movl      %eax, -28(%rbp)                               #42.13
        jmp       ..B2.2        # Prob 100%                     #42.13
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B2.5:                         # Preds ..B2.2 ..B2.1

###         }
###     }
### }

        leave                                                   #45.1
	.cfi_restore 6
        ret                                                     #45.1
	.cfi_endproc
                                # LOE
# mark_end;
	.type	check,@function
	.size	check,.-check
	.data
# -- End  check
	.text
# -- Begin  vectorscal
	.text
# mark_begin;

	.globl vectorscal
# --- vectorscal(float *, float, int, int)
vectorscal:
# parameter 1: %rdi
# parameter 2: %xmm0
# parameter 3: %esi
# parameter 4: %edx
..B3.1:                         # Preds ..B3.0

### void vectorscal(float *v,float value,int init,int gran){

	.cfi_startproc
..___tag_value_vectorscal.17:
..L18:
                                                         #47.56
        pushq     %rbp                                          #47.56
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #47.56
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $48, %rsp                                     #47.56
        movq      %rdi, -40(%rbp)                               #47.56
        movss     %xmm0, -32(%rbp)                              #47.56
        movl      %esi, -24(%rbp)                               #47.56
        movl      %edx, -16(%rbp)                               #47.56

### 	int i=init;

        movl      -24(%rbp), %eax                               #48.8
        movl      %eax, -48(%rbp)                               #48.7

### 	int ii=i+gran;

        movl      -16(%rbp), %eax                               #49.11
        addl      -48(%rbp), %eax                               #49.11
        movl      %eax, -44(%rbp)                               #49.8

### 	for(i=init;i<ii;i++){

        movl      -24(%rbp), %eax                               #50.8
        movl      %eax, -48(%rbp)                               #50.6
        movl      -48(%rbp), %eax                               #50.13
        movl      -44(%rbp), %edx                               #50.15
        cmpl      %edx, %eax                                    #50.15
        jge       ..B3.3        # Prob 50%                      #50.15
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B3.2:                         # Preds ..B3.2 ..B3.1

### 		v[i]*=value;

        movl      -48(%rbp), %eax                               #51.5
        movslq    %eax, %rax                                    #51.3
        imulq     $4, %rax, %rax                                #51.3
        addq      -40(%rbp), %rax                               #51.3
        movss     (%rax), %xmm0                                 #51.3
        movss     -32(%rbp), %xmm1                              #51.9
        mulss     %xmm1, %xmm0                                  #51.3
        movl      -48(%rbp), %eax                               #51.5
        movslq    %eax, %rax                                    #51.3
        imulq     $4, %rax, %rax                                #51.3
        addq      -40(%rbp), %rax                               #51.3
        movss     %xmm0, (%rax)                                 #51.3
        movl      $1, %eax                                      #50.18
        addl      -48(%rbp), %eax                               #50.18
        movl      %eax, -48(%rbp)                               #50.18
        movl      -48(%rbp), %eax                               #50.13
        movl      -44(%rbp), %edx                               #50.15
        cmpl      %edx, %eax                                    #50.15
        jl        ..B3.2        # Prob 50%                      #50.15
                                # LOE rbx rbp rsp r12 r13 r14 r15 rip
..B3.3:                         # Preds ..B3.2 ..B3.1

### 	}
### 
### }

        leave                                                   #54.1
	.cfi_restore 6
        ret                                                     #54.1
	.cfi_endproc
                                # LOE
# mark_end;
	.type	vectorscal,@function
	.size	vectorscal,.-vectorscal
	.data
# -- End  vectorscal
	.text
# -- Begin  main, L_main_61__par_region0_2.2, L_main_84__ptask3_2.3, L_main_78__par_region1_2.4
	.bss
	.align 4
	.align 4
___kmpv_zeromain_0:
	.type	___kmpv_zeromain_0,@object
	.size	___kmpv_zeromain_0,4
	.space 4	# pad
	.align 4
___kmpv_zeromain_1:
	.type	___kmpv_zeromain_1,@object
	.size	___kmpv_zeromain_1,4
	.space 4	# pad
	.section .rodata, "a"
	.space 3, 0x00 	# pad
	.align 8
.L_2il0floatpacket.5:
	.long	0x00000000,0x40490000
	.type	.L_2il0floatpacket.5,@object
	.size	.L_2il0floatpacket.5,8
	.align 8
.L_2il0floatpacket.6:
	.long	0x00000000,0x40488000
	.type	.L_2il0floatpacket.6,@object
	.size	.L_2il0floatpacket.6,8
	.align 4
.L_2__STRING.1:
	.long	622879781
	.long	1713709156
	.long	1713724192
	.long	622865696
	.long	622878054
	.long	1917001830
	.long	1702125925
	.long	694560040
	.word	10
	.type	.L_2__STRING.1,@object
	.size	.L_2__STRING.1,34
	.text
# mark_begin;

	.globl main
# --- main(int, char **)
main:
# parameter 1: %edi
# parameter 2: %rsi
..B4.1:                         # Preds ..B4.0

### {

	.cfi_startproc
..___tag_value_main.24:
..L25:
                                                         #57.1
        pushq     %rbp                                          #57.1
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #57.1
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $464, %rsp                                    #57.1
        movq      %r15, -8(%rbp)                                #57.1
        movq      %r14, -16(%rbp)                               #57.1
        movq      %r13, -24(%rbp)                               #57.1
        movq      %r12, -32(%rbp)                               #57.1
        movq      %rbx, -40(%rbp)                               #57.1
        movl      %edi, -280(%rbp)                              #57.1
        movq      %rsi, -272(%rbp)                              #57.1
        movl      $.2.4_2_kmpc_loc_struct_pack.1, %eax          #57.1
        movl      $0, %edx                                      #57.1
        movq      %rax, %rdi                                    #57.1
        movq      %rdx, %rsi                                    #57.1
        movl      $0, %eax                                      #57.1
..___tag_value_main.29:
        call      __kmpc_begin                                  #57.1
..___tag_value_main.30:
	.cfi_offset 3, -56
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
                                # LOE rbp rsp rip
..B4.2:                         # Preds ..B4.1

###     int i, r, nthreads, t;
###     double *time, *creation;
###     float *v;
###     #pragma omp parallel

        movl      $.2.4_2_kmpc_loc_struct_pack.12, %eax         #61.5
        movq      %rax, %rdi                                    #61.5
..___tag_value_main.36:
        call      __kmpc_global_thread_num                      #61.5
..___tag_value_main.37:
                                # LOE rbp rsp rip eax
..B4.64:                        # Preds ..B4.2
        movl      %eax, -448(%rbp)                              #61.5
        movl      -448(%rbp), %eax                              #61.5
        movl      %eax, -444(%rbp)                              #61.5
        movl      $.2.4_2_kmpc_loc_struct_pack.42, %eax         #61.5
        movq      %rax, %rdi                                    #61.5
        movl      $0, %eax                                      #61.5
..___tag_value_main.38:
        call      __kmpc_ok_to_fork                             #61.5
..___tag_value_main.39:
                                # LOE rbp rsp rip eax
..B4.63:                        # Preds ..B4.64
        movl      %eax, -440(%rbp)                              #61.5
                                # LOE rbp rsp rip
..B4.3:                         # Preds ..B4.63
        movl      -440(%rbp), %eax                              #61.5
        testl     %eax, %eax                                    #61.5
        jne       ..B4.7        # Prob 50%                      #61.5
                                # LOE rbp rsp rip
..B4.4:                         # Preds ..B4.3
        movl      $.2.4_2_kmpc_loc_struct_pack.42, %eax         #61.5
        movl      -444(%rbp), %edx                              #61.5
        movq      %rax, %rdi                                    #61.5
        movl      %edx, %esi                                    #61.5
        movl      $0, %eax                                      #61.5
..___tag_value_main.40:
        call      __kmpc_serialized_parallel                    #61.5
..___tag_value_main.41:
                                # LOE rbp rsp rip
..B4.5:                         # Preds ..B4.4
        lea       -444(%rbp), %rax                              #61.5
        movl      $___kmpv_zeromain_0, %edx                     #61.5
        lea       -432(%rbp), %rcx                              #61.5
        movq      %rax, %rdi                                    #61.5
        movq      %rdx, %rsi                                    #61.5
        movq      %rcx, %rdx                                    #61.5
..___tag_value_main.42:
        call      L_main_61__par_region0_2.2                    #61.5
..___tag_value_main.43:
                                # LOE rbp rsp rip
..B4.6:                         # Preds ..B4.5
        movl      $.2.4_2_kmpc_loc_struct_pack.42, %eax         #61.5
        movl      -444(%rbp), %edx                              #61.5
        movq      %rax, %rdi                                    #61.5
        movl      %edx, %esi                                    #61.5
        movl      $0, %eax                                      #61.5
..___tag_value_main.44:
        call      __kmpc_end_serialized_parallel                #61.5
..___tag_value_main.45:
        jmp       ..B4.8        # Prob 100%                     #61.5
                                # LOE rbp rsp rip
..B4.7:                         # Preds ..B4.3
        movl      $.2.4_2_kmpc_loc_struct_pack.42, %eax         #61.5
        movl      $1, %edx                                      #61.5
        movl      $L_main_61__par_region0_2.2, %ecx             #61.5
        lea       -432(%rbp), %rbx                              #61.5
        movq      %rax, %rdi                                    #61.5
        movl      %edx, %esi                                    #61.5
        movq      %rcx, %rdx                                    #61.5
        movq      %rbx, %rcx                                    #61.5
        movl      $0, %eax                                      #61.5
..___tag_value_main.46:
        call      __kmpc_fork_call                              #61.5
..___tag_value_main.47:
                                # LOE rbp rsp rip
..B4.8:                         # Preds ..B4.7 ..B4.6

###     {
###     #pragma omp master
###         {
###             nthreads = omp_get_num_threads();
###         }
###     }
###     int ntasks = (argc > 1) ? atoi(argv[1]) : NUM_ELEMS;

        movl      -280(%rbp), %eax                              #68.19
        cmpl      $1, %eax                                      #68.26
        jg        ..B4.10       # Prob 50%                      #68.26
                                # LOE rbp rsp rip
..B4.9:                         # Preds ..B4.8
        movl      $100, -424(%rbp)                              #68.47
        jmp       ..B4.11       # Prob 100%                     #68.47
                                # LOE rbp rsp rip
..B4.10:                        # Preds ..B4.8
        movl      $8, %eax                                      #68.31
        addq      -272(%rbp), %rax                              #68.31
        movq      (%rax), %rax                                  #68.31
        movq      %rax, %rdi                                    #68.31
..___tag_value_main.48:
#       atoi(const char *)
        call      atoi                                          #68.31
..___tag_value_main.49:
                                # LOE rbp rsp rip eax
..B4.65:                        # Preds ..B4.10
        movl      %eax, -428(%rbp)                              #68.31
        movl      -428(%rbp), %eax                              #68.31
        movl      %eax, -424(%rbp)                              #68.47
                                # LOE rbp rsp rip
..B4.11:                        # Preds ..B4.65 ..B4.9
        movl      -424(%rbp), %eax                              #68.47
        movl      %eax, -420(%rbp)                              #68.16

###     int granularity = (argc > 2) ? atoi(argv[2]) : 1;

        movl      -280(%rbp), %eax                              #69.24
        cmpl      $2, %eax                                      #69.31
        jg        ..B4.13       # Prob 50%                      #69.31
                                # LOE rbp rsp rip
..B4.12:                        # Preds ..B4.11
        movl      $1, -412(%rbp)                                #69.52
        jmp       ..B4.14       # Prob 100%                     #69.52
                                # LOE rbp rsp rip
..B4.13:                        # Preds ..B4.11
        movl      $16, %eax                                     #69.36
        addq      -272(%rbp), %rax                              #69.36
        movq      (%rax), %rax                                  #69.36
        movq      %rax, %rdi                                    #69.36
..___tag_value_main.50:
#       atoi(const char *)
        call      atoi                                          #69.36
..___tag_value_main.51:
                                # LOE rbp rsp rip eax
..B4.66:                        # Preds ..B4.13
        movl      %eax, -416(%rbp)                              #69.36
        movl      -416(%rbp), %eax                              #69.36
        movl      %eax, -412(%rbp)                              #69.52
                                # LOE rbp rsp rip
..B4.14:                        # Preds ..B4.66 ..B4.12
        movl      -412(%rbp), %eax                              #69.52
        movl      %eax, -408(%rbp)                              #69.21

###     int rep = (argc > 3) ? atoi(argv[3]) : TIMES;

        movl      -280(%rbp), %eax                              #70.16
        cmpl      $3, %eax                                      #70.23
        jg        ..B4.16       # Prob 50%                      #70.23
                                # LOE rbp rsp rip
..B4.15:                        # Preds ..B4.14
        movl      $50, -400(%rbp)                               #70.44
        jmp       ..B4.17       # Prob 100%                     #70.44
                                # LOE rbp rsp rip
..B4.16:                        # Preds ..B4.14
        movl      $24, %eax                                     #70.28
        addq      -272(%rbp), %rax                              #70.28
        movq      (%rax), %rax                                  #70.28
        movq      %rax, %rdi                                    #70.28
..___tag_value_main.52:
#       atoi(const char *)
        call      atoi                                          #70.28
..___tag_value_main.53:
                                # LOE rbp rsp rip eax
..B4.67:                        # Preds ..B4.16
        movl      %eax, -404(%rbp)                              #70.28
        movl      -404(%rbp), %eax                              #70.28
        movl      %eax, -400(%rbp)                              #70.44
                                # LOE rbp rsp rip
..B4.17:                        # Preds ..B4.67 ..B4.15
        movl      -400(%rbp), %eax                              #70.44
        movl      %eax, -396(%rbp)                              #70.13

###     time = malloc(sizeof (double)*rep);

        movl      -396(%rbp), %eax                              #71.12
        movslq    %eax, %rax                                    #71.12
        imulq     $8, %rax, %rax                                #71.12
        movq      %rax, %rdi                                    #71.12
..___tag_value_main.54:
#       malloc(size_t)
        call      malloc                                        #71.12
..___tag_value_main.55:
                                # LOE rax rbp rsp rip
..B4.68:                        # Preds ..B4.17
        movq      %rax, -216(%rbp)                              #71.12
                                # LOE rbp rsp rip
..B4.18:                        # Preds ..B4.68
        movq      -216(%rbp), %rax                              #71.12
        movq      %rax, -208(%rbp)                              #71.5

###     creation = malloc(sizeof (double)*rep);

        movl      -396(%rbp), %eax                              #72.16
        movslq    %eax, %rax                                    #72.16
        imulq     $8, %rax, %rax                                #72.16
        movq      %rax, %rdi                                    #72.16
..___tag_value_main.56:
#       malloc(size_t)
        call      malloc                                        #72.16
..___tag_value_main.57:
                                # LOE rax rbp rsp rip
..B4.69:                        # Preds ..B4.18
        movq      %rax, -200(%rbp)                              #72.16
                                # LOE rbp rsp rip
..B4.19:                        # Preds ..B4.69
        movq      -200(%rbp), %rax                              #72.16
        movq      %rax, -192(%rbp)                              #72.5

###     v = malloc(sizeof (float)*ntasks*granularity);

        movl      -408(%rbp), %eax                              #73.9
        movslq    %eax, %rax                                    #73.9
        movl      -420(%rbp), %edx                              #73.9
        movslq    %edx, %rdx                                    #73.9
        imulq     $4, %rdx, %rdx                                #73.9
        imulq     %rax, %rdx                                    #73.9
        movq      %rdx, %rdi                                    #73.9
..___tag_value_main.58:
#       malloc(size_t)
        call      malloc                                        #73.9
..___tag_value_main.59:
                                # LOE rax rbp rsp rip
..B4.70:                        # Preds ..B4.19
        movq      %rax, -184(%rbp)                              #73.9
                                # LOE rbp rsp rip
..B4.20:                        # Preds ..B4.70
        movq      -184(%rbp), %rax                              #73.9
        movq      %rax, -176(%rbp)                              #73.5

###     int total = ntasks*granularity;

        movl      -408(%rbp), %eax                              #74.24
        imull     -420(%rbp), %eax                              #74.24
        movl      %eax, -392(%rbp)                              #74.15

###     for (r = 0; r < rep; r++) {

        movl      $0, -388(%rbp)                                #75.10
        movl      -388(%rbp), %eax                              #75.17
        movl      -396(%rbp), %edx                              #75.21
        cmpl      %edx, %eax                                    #75.21
        jge       ..B4.31       # Prob 50%                      #75.21
                                # LOE rbp rsp rip
..B4.21:                        # Preds ..B4.30 ..B4.20

###     init(v, total);

        movq      -176(%rbp), %rax                              #76.5
        movl      -392(%rbp), %edx                              #76.5
        movq      %rax, %rdi                                    #76.5
        movl      %edx, %esi                                    #76.5
..___tag_value_main.60:
#       init(float *, int)
        call      init                                          #76.5
..___tag_value_main.61:
                                # LOE rbp rsp rip
..B4.22:                        # Preds ..B4.21

###         time[r] = omp_get_wtime();

..___tag_value_main.62:
#       omp_get_wtime(void)
        call      omp_get_wtime                                 #77.19
..___tag_value_main.63:
                                # LOE rbp rsp rip xmm0
..B4.71:                        # Preds ..B4.22
        movsd     %xmm0, -136(%rbp)                             #77.19
                                # LOE rbp rsp rip
..B4.23:                        # Preds ..B4.71
        movl      -388(%rbp), %eax                              #77.14
        movslq    %eax, %rax                                    #77.9
        imulq     $8, %rax, %rax                                #77.9
        addq      -208(%rbp), %rax                              #77.9
        movsd     -136(%rbp), %xmm0                             #77.19
        movsd     %xmm0, (%rax)                                 #77.9

### 	#pragma omp parallel

        movl      $.2.4_2_kmpc_loc_struct_pack.87, %eax         #78.2
        movq      %rax, %rdi                                    #78.2
        movl      $0, %eax                                      #78.2
..___tag_value_main.64:
        call      __kmpc_ok_to_fork                             #78.2
..___tag_value_main.65:
                                # LOE rbp rsp rip eax
..B4.72:                        # Preds ..B4.23
        movl      %eax, -380(%rbp)                              #78.2
                                # LOE rbp rsp rip
..B4.24:                        # Preds ..B4.72
        movl      -380(%rbp), %eax                              #78.2
        testl     %eax, %eax                                    #78.2
        jne       ..B4.28       # Prob 50%                      #78.2
                                # LOE rbp rsp rip
..B4.25:                        # Preds ..B4.24
        movl      $.2.4_2_kmpc_loc_struct_pack.87, %eax         #78.2
        movl      -444(%rbp), %edx                              #78.2
        movq      %rax, %rdi                                    #78.2
        movl      %edx, %esi                                    #78.2
        movl      $0, %eax                                      #78.2
..___tag_value_main.66:
        call      __kmpc_serialized_parallel                    #78.2
..___tag_value_main.67:
                                # LOE rbp rsp rip
..B4.26:                        # Preds ..B4.25
        addq      $-16, %rsp                                    #78.2
        lea       -444(%rbp), %rax                              #78.2
        movl      $___kmpv_zeromain_1, %edx                     #78.2
        lea       -372(%rbp), %rcx                              #78.2
        lea       -392(%rbp), %rbx                              #78.2
        lea       -408(%rbp), %rsi                              #78.2
        lea       -176(%rbp), %rdi                              #78.2
        lea       -192(%rbp), %r8                               #78.2
        movq      %r8, (%rsp)                                   #78.2
        lea       -388(%rbp), %r8                               #78.2
        movq      %r8, 8(%rsp)                                  #78.2
        movq      %rdi, -88(%rbp)                               #78.2
        movq      %rax, %rdi                                    #78.2
        movq      %rsi, -80(%rbp)                               #78.2
        movq      %rdx, %rsi                                    #78.2
        movq      %rcx, %rdx                                    #78.2
        movq      %rbx, %rcx                                    #78.2
        movq      -80(%rbp), %rax                               #78.2
        movq      %rax, %r8                                     #78.2
        movq      -88(%rbp), %rax                               #78.2
        movq      %rax, %r9                                     #78.2
..___tag_value_main.68:
        call      L_main_78__par_region1_2.4                    #78.2
..___tag_value_main.69:
                                # LOE rbp rsp rip
..B4.73:                        # Preds ..B4.26
        addq      $16, %rsp                                     #78.2
                                # LOE rbp rsp rip
..B4.27:                        # Preds ..B4.73
        movl      $.2.4_2_kmpc_loc_struct_pack.87, %eax         #78.2
        movl      -444(%rbp), %edx                              #78.2
        movq      %rax, %rdi                                    #78.2
        movl      %edx, %esi                                    #78.2
        movl      $0, %eax                                      #78.2
..___tag_value_main.70:
        call      __kmpc_end_serialized_parallel                #78.2
..___tag_value_main.71:
        jmp       ..B4.29       # Prob 100%                     #78.2
                                # LOE rbp rsp rip
..B4.28:                        # Preds ..B4.24
        addq      $-32, %rsp                                    #78.2
        movl      $.2.4_2_kmpc_loc_struct_pack.87, %eax         #78.2
        movl      $6, %edx                                      #78.2
        movl      $L_main_78__par_region1_2.4, %ecx             #78.2
        lea       -372(%rbp), %rbx                              #78.2
        lea       -392(%rbp), %rsi                              #78.2
        lea       -408(%rbp), %rdi                              #78.2
        lea       -176(%rbp), %r8                               #78.2
        movq      %r8, (%rsp)                                   #78.2
        lea       -192(%rbp), %r8                               #78.2
        movq      %r8, 8(%rsp)                                  #78.2
        lea       -388(%rbp), %r8                               #78.2
        movq      %r8, 16(%rsp)                                 #78.2
        movq      %rdi, -72(%rbp)                               #78.2
        movq      %rax, %rdi                                    #78.2
        movq      %rsi, -64(%rbp)                               #78.2
        movl      %edx, %esi                                    #78.2
        movq      %rcx, %rdx                                    #78.2
        movq      %rbx, %rcx                                    #78.2
        movq      -64(%rbp), %rax                               #78.2
        movq      %rax, %r8                                     #78.2
        movq      -72(%rbp), %rax                               #78.2
        movq      %rax, %r9                                     #78.2
        movl      $0, %eax                                      #78.2
..___tag_value_main.72:
        call      __kmpc_fork_call                              #78.2
..___tag_value_main.73:
                                # LOE rbp rsp rip
..B4.74:                        # Preds ..B4.28
        addq      $32, %rsp                                     #78.2
                                # LOE rbp rsp rip
..B4.29:                        # Preds ..B4.74 ..B4.27

### 	{
### 		#pragma omp single
### 		{
### 			double auxtime=omp_get_wtime();
### 	        	for (i = 0; i < total; i+=granularity) {
### 				#pragma omp task firstprivate(i)
### 				{
###                 		//	v[i] *= 0.9f;
###                 			vectorscal(v,0.9f,i,granularity);
### 				}
###             		}
### 			creation[r]=omp_get_wtime()-auxtime;
### 		}
### 	}
###         time[r] = omp_get_wtime() - time[r];

..___tag_value_main.74:
#       omp_get_wtime(void)
        call      omp_get_wtime                                 #93.19
..___tag_value_main.75:
                                # LOE rbp rsp rip xmm0
..B4.75:                        # Preds ..B4.29
        movsd     %xmm0, -96(%rbp)                              #93.19
                                # LOE rbp rsp rip
..B4.30:                        # Preds ..B4.75
        movl      -388(%rbp), %eax                              #93.42
        movslq    %eax, %rax                                    #93.37
        imulq     $8, %rax, %rax                                #93.37
        addq      -208(%rbp), %rax                              #93.37
        movsd     -96(%rbp), %xmm0                              #93.19
        movsd     (%rax), %xmm1                                 #93.37
        subsd     %xmm1, %xmm0                                  #93.37
        movl      -388(%rbp), %eax                              #93.14
        movslq    %eax, %rax                                    #93.9
        imulq     $8, %rax, %rax                                #93.9
        addq      -208(%rbp), %rax                              #93.9
        movsd     %xmm0, (%rax)                                 #93.9
        movl      $1, %eax                                      #75.26
        addl      -388(%rbp), %eax                              #75.26
        movl      %eax, -388(%rbp)                              #75.26
        movl      -388(%rbp), %eax                              #75.17
        movl      -396(%rbp), %edx                              #75.21
        cmpl      %edx, %eax                                    #75.21
        jl        ..B4.21       # Prob 50%                      #75.21
                                # LOE rbp rsp rip
..B4.31:                        # Preds ..B4.30 ..B4.20

###     }
###  double min, max, avg, aux, sigma, dev;
###     min = time[0];

        movq      -208(%rbp), %rax                              #96.11
        movsd     (%rax), %xmm0                                 #96.11
        movsd     %xmm0, -168(%rbp)                             #96.5

###     max = time[0];

        movq      -208(%rbp), %rax                              #97.11
        movsd     (%rax), %xmm0                                 #97.11
        movsd     %xmm0, -160(%rbp)                             #97.5

###     aux = time[0];

        movq      -208(%rbp), %rax                              #98.11
        movsd     (%rax), %xmm0                                 #98.11
        movsd     %xmm0, -152(%rbp)                             #98.5

###     double avgcreation = creation[0];

        movq      -192(%rbp), %rax                              #99.26
        movsd     (%rax), %xmm0                                 #99.26
        movsd     %xmm0, -144(%rbp)                             #99.24

###     for (t = 1; t < TIMES; t++) {

        movl      $1, -384(%rbp)                                #100.10
        movl      -384(%rbp), %eax                              #100.17
        cmpl      $50, %eax                                     #100.21
        jge       ..B4.33       # Prob 50%                      #100.21
                                # LOE rbp rsp rip
..B4.32:                        # Preds ..B4.32 ..B4.31

###         if (time[t] < min) min = time[t];

        movl      -384(%rbp), %eax                              #101.18
        movslq    %eax, %rax                                    #101.13
        imulq     $8, %rax, %rax                                #101.13
        addq      -208(%rbp), %rax                              #101.13
        movsd     (%rax), %xmm0                                 #101.13
        movsd     -168(%rbp), %xmm1                             #101.23
        minsd     %xmm1, %xmm0                                  #101.9
        movsd     %xmm0, -168(%rbp)                             #101.28

###         if (time[t] > max) max = time[t];

        movl      -384(%rbp), %eax                              #102.18
        movslq    %eax, %rax                                    #102.13
        imulq     $8, %rax, %rax                                #102.13
        addq      -208(%rbp), %rax                              #102.13
        movsd     (%rax), %xmm0                                 #102.13
        movsd     -160(%rbp), %xmm1                             #102.23
        maxsd     %xmm1, %xmm0                                  #102.9
        movsd     %xmm0, -160(%rbp)                             #102.28

###         aux += time[t];

        movl      -384(%rbp), %eax                              #103.21
        movslq    %eax, %rax                                    #103.16
        imulq     $8, %rax, %rax                                #103.16
        addq      -208(%rbp), %rax                              #103.16
        movsd     -152(%rbp), %xmm0                             #103.9
        movsd     (%rax), %xmm1                                 #103.16
        addsd     %xmm1, %xmm0                                  #103.9
        movsd     %xmm0, -152(%rbp)                             #103.9

### 	avgcreation+=creation[t];

        movl      -384(%rbp), %eax                              #104.24
        movslq    %eax, %rax                                    #104.15
        imulq     $8, %rax, %rax                                #104.15
        addq      -192(%rbp), %rax                              #104.15
        movsd     -144(%rbp), %xmm0                             #104.2
        movsd     (%rax), %xmm1                                 #104.15
        addsd     %xmm1, %xmm0                                  #104.2
        movsd     %xmm0, -144(%rbp)                             #104.2
        movl      $1, %eax                                      #100.28
        addl      -384(%rbp), %eax                              #100.28
        movl      %eax, -384(%rbp)                              #100.28
        movl      -384(%rbp), %eax                              #100.17
        cmpl      $50, %eax                                     #100.21
        jl        ..B4.32       # Prob 50%                      #100.21
                                # LOE rbp rsp rip
..B4.33:                        # Preds ..B4.32 ..B4.31

###     }
###     avg = aux / TIMES;

        movsd     -152(%rbp), %xmm0                             #106.11
        movsd     .L_2il0floatpacket.5(%rip), %xmm1             #106.17
        divsd     %xmm1, %xmm0                                  #106.17
        movsd     %xmm0, -128(%rbp)                             #106.5

###     for (t = 0; t < TIMES; t++) {

        movl      $0, -384(%rbp)                                #107.10
        movl      -384(%rbp), %eax                              #107.17
        cmpl      $50, %eax                                     #107.21
        jge       ..B4.35       # Prob 50%                      #107.21
                                # LOE rbp rsp rip
..B4.34:                        # Preds ..B4.34 ..B4.33

###         sigma = sigma + ((time[t] - avg)*(time[t] - avg));

        movl      -384(%rbp), %eax                              #108.32
        movslq    %eax, %rax                                    #108.27
        imulq     $8, %rax, %rax                                #108.27
        addq      -208(%rbp), %rax                              #108.27
        movsd     (%rax), %xmm0                                 #108.27
        movsd     -128(%rbp), %xmm1                             #108.37
        subsd     %xmm1, %xmm0                                  #108.37
        movl      -384(%rbp), %eax                              #108.48
        movslq    %eax, %rax                                    #108.43
        imulq     $8, %rax, %rax                                #108.43
        addq      -208(%rbp), %rax                              #108.43
        movsd     (%rax), %xmm1                                 #108.43
        movsd     -128(%rbp), %xmm2                             #108.53
        subsd     %xmm2, %xmm1                                  #108.53
        mulsd     %xmm1, %xmm0                                  #108.53
        movsd     -120(%rbp), %xmm1                             #108.17
        addsd     %xmm0, %xmm1                                  #108.53
        movsd     %xmm1, -120(%rbp)                             #108.9
        movl      $1, %eax                                      #107.28
        addl      -384(%rbp), %eax                              #107.28
        movl      %eax, -384(%rbp)                              #107.28
        movl      -384(%rbp), %eax                              #107.17
        cmpl      $50, %eax                                     #107.21
        jl        ..B4.34       # Prob 50%                      #107.21
                                # LOE rbp rsp rip
..B4.35:                        # Preds ..B4.34 ..B4.33

###     }
### #ifndef VERBOSE
###     dev = sqrt(sigma / (TIMES - 1));

        movsd     -120(%rbp), %xmm0                             #111.11
        movsd     .L_2il0floatpacket.6(%rip), %xmm1             #111.11
        divsd     %xmm1, %xmm0                                  #111.11
..___tag_value_main.76:
#       sqrt(double)
        call      sqrt                                          #111.11
..___tag_value_main.77:
                                # LOE rbp rsp rip xmm0
..B4.76:                        # Preds ..B4.35
        movsd     %xmm0, -112(%rbp)                             #111.11
                                # LOE rbp rsp rip
..B4.36:                        # Preds ..B4.76
        movsd     -112(%rbp), %xmm0                             #111.11
        movsd     %xmm0, -104(%rbp)                             #111.5

### #else
###     dev = sqrt(sigma);
### #endif
###     printf("%d %d %f [%f - %f] %f Create(%f)\n",

        movl      $.L_2__STRING.1, %eax                         #115.5
        movl      -432(%rbp), %edx                              #115.5
        movl      -420(%rbp), %ecx                              #115.5
        movsd     -128(%rbp), %xmm0                             #115.5
        movsd     -168(%rbp), %xmm1                             #115.5
        movsd     -160(%rbp), %xmm2                             #115.5
        movsd     -104(%rbp), %xmm3                             #115.5
        movsd     -144(%rbp), %xmm4                             #115.5
        movl      -396(%rbp), %ebx                              #115.5
        cvtsi2sd  %ebx, %xmm5                                   #115.5
        divsd     %xmm5, %xmm4                                  #115.5
        movq      %rax, %rdi                                    #115.5
        movl      %edx, %esi                                    #115.5
        movl      %ecx, %edx                                    #115.5
        movl      $5, %eax                                      #115.5
..___tag_value_main.78:
#       printf(const char *__restrict__, ...)
        call      printf                                        #115.5
..___tag_value_main.79:
                                # LOE rbp rsp rip eax
..B4.77:                        # Preds ..B4.36
        movl      %eax, -376(%rbp)                              #115.5
                                # LOE rbp rsp rip
..B4.37:                        # Preds ..B4.77

###             nthreads, ntasks, avg, min, max, dev, avgcreation/rep);
###     check(v, total);

        movq      -176(%rbp), %rax                              #117.5
        movl      -392(%rbp), %edx                              #117.5
        movq      %rax, %rdi                                    #117.5
        movl      %edx, %esi                                    #117.5
..___tag_value_main.80:
#       check(float *, int)
        call      check                                         #117.5
..___tag_value_main.81:
                                # LOE rbp rsp rip
..B4.38:                        # Preds ..B4.37

###     free(v);

        movq      -176(%rbp), %rax                              #118.5
        movq      %rax, %rdi                                    #118.5
..___tag_value_main.82:
#       free(void *)
        call      free                                          #118.5
..___tag_value_main.83:
                                # LOE rbp rsp rip
..B4.39:                        # Preds ..B4.38

###     free(time);

        movq      -208(%rbp), %rax                              #119.5
        movq      %rax, %rdi                                    #119.5
..___tag_value_main.84:
#       free(void *)
        call      free                                          #119.5
..___tag_value_main.85:
                                # LOE rbp rsp rip
..B4.40:                        # Preds ..B4.39

###     
###     return 0;

        movl      $.2.4_2_kmpc_loc_struct_pack.20, %eax         #121.12
        movq      %rax, %rdi                                    #121.12
        movl      $0, %eax                                      #121.12
..___tag_value_main.86:
        call      __kmpc_end                                    #121.12
..___tag_value_main.87:
                                # LOE rbp rsp rip
..B4.41:                        # Preds ..B4.40
        movl      $0, %eax                                      #121.12
        movq      -40(%rbp), %rbx                               #121.12
	.cfi_restore 3
        movq      -32(%rbp), %r12                               #121.12
	.cfi_restore 12
        movq      -24(%rbp), %r13                               #121.12
	.cfi_restore 13
        movq      -16(%rbp), %r14                               #121.12
	.cfi_restore 14
        movq      -8(%rbp), %r15                                #121.12
	.cfi_restore 15
        leave                                                   #121.12
	.cfi_restore 6
        ret                                                     #121.12
	.cfi_offset 3, -56
	.cfi_offset 6, -16
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
                                # LOE
..B4.42:                        # Preds ..B4.46 ..B4.44
        movl      $0, %eax                                      #61.5
        movq      -40(%rbp), %rbx                               #61.5
	.cfi_restore 3
        movq      -32(%rbp), %r12                               #61.5
	.cfi_restore 12
        movq      -24(%rbp), %r13                               #61.5
	.cfi_restore 13
        movq      -16(%rbp), %r14                               #61.5
	.cfi_restore 14
        movq      -8(%rbp), %r15                                #61.5
	.cfi_restore 15
        leave                                                   #61.5
	.cfi_restore 6
        ret                                                     #61.5
                                # LOE
L_main_61__par_region0_2.2:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
..B4.43:                        # Preds ..B4.0
        pushq     %rbp                                          #61.5
	.cfi_def_cfa 7, 16
        movq      %rsp, %rbp                                    #61.5
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $464, %rsp                                    #61.5
        movq      %r15, -8(%rbp)                                #61.5
        movq      %r14, -16(%rbp)                               #61.5
        movq      %r13, -24(%rbp)                               #61.5
        movq      %r12, -32(%rbp)                               #61.5
        movq      %rbx, -40(%rbp)                               #61.5
        movq      %rdi, -304(%rbp)                              #61.5
        movq      %rsi, -296(%rbp)                              #61.5
        movq      %rdx, -288(%rbp)                              #61.5
        movq      -304(%rbp), %rax                              #61.5
        movl      (%rax), %eax                                  #61.5
        movl      %eax, -456(%rbp)                              #61.5
        movl      $.2.4_2_kmpc_loc_struct_pack.28, %eax         #63.5
        movl      -456(%rbp), %edx                              #63.5
        movq      %rax, %rdi                                    #63.5
        movl      %edx, %esi                                    #63.5
        movl      $0, %eax                                      #63.5
..___tag_value_main.110:
        call      __kmpc_master                                 #63.5
..___tag_value_main.111:
	.cfi_offset 3, -56
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
                                # LOE rbp rsp rip eax
..B4.78:                        # Preds ..B4.43
        movl      %eax, -452(%rbp)                              #63.5
                                # LOE rbp rsp rip
..B4.44:                        # Preds ..B4.78
        movl      -452(%rbp), %eax                              #63.5
        cmpl      $1, %eax                                      #63.5
        jne       ..B4.42       # Prob 50%                      #63.5
                                # LOE rbp rsp rip
..B4.45:                        # Preds ..B4.44
..___tag_value_main.117:
#       omp_get_num_threads(void)
        call      omp_get_num_threads                           #65.24
..___tag_value_main.118:
                                # LOE rbp rsp rip eax
..B4.79:                        # Preds ..B4.45
        movl      %eax, -436(%rbp)                              #65.24
                                # LOE rbp rsp rip
..B4.46:                        # Preds ..B4.79
        movq      -288(%rbp), %rax                              #65.13
        movl      -436(%rbp), %edx                              #65.24
        movl      %edx, (%rax)                                  #65.13
        movl      $.2.4_2_kmpc_loc_struct_pack.28, %eax         #63.5
        movl      -456(%rbp), %edx                              #63.5
        movq      %rax, %rdi                                    #63.5
        movl      %edx, %esi                                    #63.5
        movl      $0, %eax                                      #63.5
..___tag_value_main.119:
        call      __kmpc_end_master                             #63.5
..___tag_value_main.120:
        jmp       ..B4.42       # Prob 100%                     #63.5
                                # LOE rbp rsp rip
..B4.47:                        # Preds ..B4.50
        movl      $0, %eax                                      #78.2
        movq      -40(%rbp), %rbx                               #78.2
	.cfi_restore 3
        movq      -32(%rbp), %r12                               #78.2
	.cfi_restore 12
        movq      -24(%rbp), %r13                               #78.2
	.cfi_restore 13
        movq      -16(%rbp), %r14                               #78.2
	.cfi_restore 14
        movq      -8(%rbp), %r15                                #78.2
	.cfi_restore 15
        leave                                                   #78.2
	.cfi_restore 6
        ret                                                     #78.2
                                # LOE
L_main_78__par_region1_2.4:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %rcx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 16 + %rbp
# parameter 8: 24 + %rbp
..B4.48:                        # Preds ..B4.0
        pushq     %rbp                                          #78.2
	.cfi_def_cfa 7, 16
        movq      %rsp, %rbp                                    #78.2
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $464, %rsp                                    #78.2
        movq      %r15, -8(%rbp)                                #78.2
        movq      %r14, -16(%rbp)                               #78.2
        movq      %r13, -24(%rbp)                               #78.2
        movq      %r12, -32(%rbp)                               #78.2
        movq      %rbx, -40(%rbp)                               #78.2
        movq      %rdi, -352(%rbp)                              #78.2
        movq      %rsi, -344(%rbp)                              #78.2
        movq      %rdx, -336(%rbp)                              #78.2
        movq      %rcx, -328(%rbp)                              #78.2
        movq      %r8, -320(%rbp)                               #78.2
        movq      %r9, -312(%rbp)                               #78.2
        movq      -352(%rbp), %rax                              #78.2
        movl      (%rax), %eax                                  #78.2
        movl      %eax, -464(%rbp)                              #78.2
        movl      $.2.4_2_kmpc_loc_struct_pack.71, %eax         #80.3
        movl      -464(%rbp), %edx                              #80.3
        movq      %rax, %rdi                                    #80.3
        movl      %edx, %esi                                    #80.3
        movl      $0, %eax                                      #80.3
..___tag_value_main.131:
        call      __kmpc_single                                 #80.3
..___tag_value_main.132:
	.cfi_offset 3, -56
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
                                # LOE rbp rsp rip eax
..B4.80:                        # Preds ..B4.48
        movl      %eax, -460(%rbp)                              #80.3
                                # LOE rbp rsp rip
..B4.49:                        # Preds ..B4.80
        movl      -460(%rbp), %eax                              #80.3
        cmpl      $1, %eax                                      #80.3
        je        ..B4.51       # Prob 50%                      #80.3
                                # LOE rbp rsp rip
..B4.50:                        # Preds ..B4.58 ..B4.49
        movl      $.2.4_2_kmpc_loc_struct_pack.79, %eax         #80.3
        movl      -464(%rbp), %edx                              #80.3
        movq      %rax, %rdi                                    #80.3
        movl      %edx, %esi                                    #80.3
        movl      $0, %eax                                      #80.3
..___tag_value_main.138:
        call      __kmpc_barrier                                #80.3
..___tag_value_main.139:
        jmp       ..B4.47       # Prob 100%                     #80.3
                                # LOE rbp rsp rip
..B4.51:                        # Preds ..B4.49
..___tag_value_main.140:
#       omp_get_wtime(void)
        call      omp_get_wtime                                 #82.19
..___tag_value_main.141:
                                # LOE rbp rsp rip xmm0
..B4.81:                        # Preds ..B4.51
        movsd     %xmm0, -256(%rbp)                             #82.19
                                # LOE rbp rsp rip
..B4.52:                        # Preds ..B4.81
        movsd     -256(%rbp), %xmm0                             #82.19
        movsd     %xmm0, -248(%rbp)                             #82.18
        movq      -336(%rbp), %rax                              #83.16
        movl      $0, (%rax)                                    #83.16
        movq      -336(%rbp), %rax                              #83.11
        movl      (%rax), %eax                                  #83.23
        movq      -328(%rbp), %rdx                              #83.11
        movl      (%rdx), %edx                                  #83.27
        cmpl      %edx, %eax                                    #83.27
        jl        ..B4.54       # Prob 50%                      #83.27
        jmp       ..B4.57       # Prob 100%                     #83.27
                                # LOE rbp rsp rip
..B4.53:                        # Preds ..B4.56
        movq      -336(%rbp), %rax                              #83.34
        movq      -320(%rbp), %rdx                              #83.34
        movl      (%rdx), %edx                                  #83.37
        addl      (%rax), %edx                                  #83.34
        movq      -336(%rbp), %rax                              #83.34
        movl      %edx, (%rax)                                  #83.34
        movq      -336(%rbp), %rax                              #83.11
        movl      (%rax), %eax                                  #83.23
        movq      -328(%rbp), %rdx                              #83.11
        movl      (%rdx), %edx                                  #83.27
        cmpl      %edx, %eax                                    #83.27
        jl        ..B4.55       # Prob 50%                      #83.27
        jmp       ..B4.57       # Prob 100%                     #83.27
                                # LOE rbp rsp rip
..B4.54:                        # Preds ..B4.52
..B4.55:                        # Preds ..B4.53 ..B4.54
        movl      $.2.4_2_kmpc_loc_struct_pack.63, %eax         #84.5
        movl      -464(%rbp), %edx                              #84.5
        movl      $1, %ecx                                      #84.5
        movl      $24, %ebx                                     #84.5
        movl      $16, %esi                                     #84.5
        movl      $L_main_84__ptask3_2.3, %edi                  #84.5
        movq      %rdi, -56(%rbp)                               #84.5
        movq      %rax, %rdi                                    #84.5
        movq      %rsi, -48(%rbp)                               #84.5
        movl      %edx, %esi                                    #84.5
        movl      %ecx, %edx                                    #84.5
        movq      %rbx, %rcx                                    #84.5
        movq      -48(%rbp), %rax                               #84.5
        movq      %rax, %r8                                     #84.5
        movq      -56(%rbp), %rax                               #84.5
        movq      %rax, %r9                                     #84.5
        movl      $0, %eax                                      #84.5
..___tag_value_main.142:
        call      __kmpc_omp_task_alloc                         #84.5
..___tag_value_main.143:
                                # LOE rax rbp rsp rip
..B4.82:                        # Preds ..B4.55
        movq      %rax, -232(%rbp)                              #84.5
                                # LOE rbp rsp rip
..B4.56:                        # Preds ..B4.82
        movq      -232(%rbp), %rax                              #84.5
        movq      (%rax), %rax                                  #84.5
        movq      %rax, -224(%rbp)                              #84.5
        movq      -312(%rbp), %rax                              #84.5
        movq      -224(%rbp), %rdx                              #84.5
        movq      %rax, (%rdx)                                  #84.5
        movq      -320(%rbp), %rax                              #84.5
        movq      -224(%rbp), %rdx                              #84.5
        movq      %rax, 8(%rdx)                                 #84.5
        movq      -336(%rbp), %rax                              #84.5
        movq      -232(%rbp), %rdx                              #84.5
        movl      (%rax), %eax                                  #84.5
        movl      %eax, 20(%rdx)                                #84.5
        movl      $.2.4_2_kmpc_loc_struct_pack.63, %eax         #84.5
        movl      -464(%rbp), %edx                              #84.5
        movq      -232(%rbp), %rcx                              #84.5
        movq      %rax, %rdi                                    #84.5
        movl      %edx, %esi                                    #84.5
        movq      %rcx, %rdx                                    #84.5
        movl      $0, %eax                                      #84.5
..___tag_value_main.144:
        call      __kmpc_omp_task                               #84.5
..___tag_value_main.145:
        jmp       ..B4.53       # Prob 100%                     #84.5
                                # LOE rbp rsp rip
..B4.57:                        # Preds ..B4.53 ..B4.52
..___tag_value_main.146:
#       omp_get_wtime(void)
        call      omp_get_wtime                                 #90.16
..___tag_value_main.147:
                                # LOE rbp rsp rip xmm0
..B4.83:                        # Preds ..B4.57
        movsd     %xmm0, -240(%rbp)                             #90.16
                                # LOE rbp rsp rip
..B4.58:                        # Preds ..B4.83
        movsd     -240(%rbp), %xmm0                             #90.16
        movsd     -248(%rbp), %xmm1                             #90.32
        subsd     %xmm1, %xmm0                                  #90.32
        movq      16(%rbp), %rax                                #90.4
        movq      24(%rbp), %rdx                                #90.4
        movl      (%rdx), %edx                                  #90.13
        movslq    %edx, %rdx                                    #90.4
        imulq     $8, %rdx, %rdx                                #90.4
        addq      (%rax), %rdx                                  #90.4
        movsd     %xmm0, (%rdx)                                 #90.4
        movl      $.2.4_2_kmpc_loc_struct_pack.71, %eax         #80.3
        movl      -464(%rbp), %edx                              #80.3
        movq      %rax, %rdi                                    #80.3
        movl      %edx, %esi                                    #80.3
        movl      $0, %eax                                      #80.3
..___tag_value_main.148:
        call      __kmpc_end_single                             #80.3
..___tag_value_main.149:
        jmp       ..B4.50       # Prob 100%                     #80.3
                                # LOE rbp rsp rip
..B4.59:                        # Preds ..B4.60
        movl      $0, %eax                                      #84.5
        movq      -40(%rbp), %rbx                               #84.5
	.cfi_restore 3
        movq      -32(%rbp), %r12                               #84.5
	.cfi_restore 12
        movq      -24(%rbp), %r13                               #84.5
	.cfi_restore 13
        movq      -16(%rbp), %r14                               #84.5
	.cfi_restore 14
        movq      -8(%rbp), %r15                                #84.5
	.cfi_restore 15
        leave                                                   #84.5
	.cfi_restore 6
        ret                                                     #84.5
                                # LOE
L_main_84__ptask3_2.3:
# parameter 1: %edi
# parameter 2: %rsi
..B4.60:                        # Preds ..B4.0
        pushq     %rbp                                          #84.5
	.cfi_def_cfa 7, 16
        movq      %rsp, %rbp                                    #84.5
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        subq      $464, %rsp                                    #84.5
        movq      %r15, -8(%rbp)                                #84.5
        movq      %r14, -16(%rbp)                               #84.5
        movq      %r13, -24(%rbp)                               #84.5
        movq      %r12, -32(%rbp)                               #84.5
        movq      %rbx, -40(%rbp)                               #84.5
        movl      %edi, -368(%rbp)                              #84.5
        movq      %rsi, -360(%rbp)                              #84.5
        movq      -360(%rbp), %rax                              #84.5
        movq      (%rax), %rax                                  #84.5
        movq      %rax, -264(%rbp)                              #84.5
        movq      -264(%rbp), %rax                              #87.20
        movq      (%rax), %rax                                  #87.20
        movq      (%rax), %rax                                  #87.20
        movss     .L_2il0floatpacket.1(%rip), %xmm0             #87.20
        movq      -360(%rbp), %rdx                              #87.20
        movl      20(%rdx), %edx                                #87.20
        movq      -264(%rbp), %rcx                              #87.20
        movq      8(%rcx), %rcx                                 #87.20
        movl      (%rcx), %ecx                                  #87.20
        movq      %rax, %rdi                                    #87.20
        movl      %edx, %esi                                    #87.20
        movl      %ecx, %edx                                    #87.20
..___tag_value_main.160:
#       vectorscal(float *, float, int, int)
        call      vectorscal                                    #87.20
..___tag_value_main.161:
        jmp       ..B4.59       # Prob 100%                     #87.20
	.cfi_endproc
                                # LOE rbp rsp rip
# mark_end;
	.type	main,@function
	.size	main,.-main
	.data
	.align 4
	.align 4
.2.4_2_kmpc_loc_struct_pack.1:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.0
	.align 4
.2.4_2__kmpc_loc_pack.0:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	53
	.byte	55
	.byte	59
	.byte	53
	.byte	55
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.12:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.11
	.align 4
.2.4_2__kmpc_loc_pack.11:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	54
	.byte	49
	.byte	59
	.byte	54
	.byte	49
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.42:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.41
	.align 4
.2.4_2__kmpc_loc_pack.41:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	54
	.byte	49
	.byte	59
	.byte	54
	.byte	55
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.87:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.86
	.align 4
.2.4_2__kmpc_loc_pack.86:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	55
	.byte	56
	.byte	59
	.byte	57
	.byte	50
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.20:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.19
	.align 4
.2.4_2__kmpc_loc_pack.19:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	49
	.byte	50
	.byte	49
	.byte	59
	.byte	49
	.byte	50
	.byte	49
	.byte	59
	.byte	59
	.space 1, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.28:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.27
	.align 4
.2.4_2__kmpc_loc_pack.27:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	54
	.byte	51
	.byte	59
	.byte	54
	.byte	54
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.71:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.70
	.align 4
.2.4_2__kmpc_loc_pack.70:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	56
	.byte	48
	.byte	59
	.byte	56
	.byte	48
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.79:
	.long	0
	.long	322
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.78
	.align 4
.2.4_2__kmpc_loc_pack.78:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	56
	.byte	48
	.byte	59
	.byte	56
	.byte	48
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.4_2_kmpc_loc_struct_pack.63:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.4_2__kmpc_loc_pack.62
	.align 4
.2.4_2__kmpc_loc_pack.62:
	.byte	59
	.byte	117
	.byte	110
	.byte	107
	.byte	110
	.byte	111
	.byte	119
	.byte	110
	.byte	59
	.byte	109
	.byte	97
	.byte	105
	.byte	110
	.byte	59
	.byte	56
	.byte	52
	.byte	59
	.byte	56
	.byte	52
	.byte	59
	.byte	59
	.data
# -- End  main, L_main_61__par_region0_2.2, L_main_84__ptask3_2.3, L_main_78__par_region1_2.4
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
# End

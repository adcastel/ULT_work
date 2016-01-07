# mark_description "Intel(R) C Intel(R) 64 Compiler XE for applications running on Intel(R) 64, Version 15.0.3.187 Build 2015040";
# mark_description "7";
# mark_description "-S -fsource-asm -fopenmp -O0";
	.file "parallel_for.c"
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
init:
# parameter 1: %rdi
# parameter 2: %esi
..B1.1:                         # Preds ..B1.0

### {

..___tag_value_init.1:                                          #28.1
..L2:
                                                          #28.1
        pushq     %rbp                                          #28.1
..___tag_value_init.3:                                          #
        movq      %rsp, %rbp                                    #28.1
..___tag_value_init.4:                                          #
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
..___tag_value_init.6:                                          #
        ret                                                     #33.1
..___tag_value_init.7:                                          #
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
check:
# parameter 1: %rdi
# parameter 2: %esi
..B2.1:                         # Preds ..B2.0

### {

..___tag_value_check.8:                                         #38.1
..L9:
                                                          #38.1
        pushq     %rbp                                          #38.1
..___tag_value_check.10:                                        #
        movq      %rsp, %rbp                                    #38.1
..___tag_value_check.11:                                        #
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
..___tag_value_check.13:                                        #42.13
        call      printf                                        #42.13
..___tag_value_check.14:                                        #
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
..___tag_value_check.15:                                        #
        ret                                                     #45.1
..___tag_value_check.16:                                        #
                                # LOE
# mark_end;
	.type	check,@function
	.size	check,.-check
	.data
# -- End  check
	.text
# -- Begin  main, L_main_52__par_region0_2.2, L_main_67__par_loop1_2.3
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
.L_2il0floatpacket.4:
	.long	0x00000000,0x40490000
	.type	.L_2il0floatpacket.4,@object
	.size	.L_2il0floatpacket.4,8
	.align 8
.L_2il0floatpacket.5:
	.long	0x00000000,0x40488000
	.type	.L_2il0floatpacket.5,@object
	.size	.L_2il0floatpacket.5,8
	.align 4
.L_2__STRING.1:
	.long	622879781
	.long	1713709156
	.long	1713724192
	.long	622865696
	.long	622878054
	.word	2662
	.byte	0
	.type	.L_2__STRING.1,@object
	.size	.L_2__STRING.1,23
	.text
# mark_begin;

	.globl main
main:
# parameter 1: %edi
# parameter 2: %rsi
..B3.1:                         # Preds ..B3.0

### {

..___tag_value_main.17:                                         #48.1
..L18:
                                                         #48.1
        pushq     %rbp                                          #48.1
..___tag_value_main.19:                                         #
        movq      %rsp, %rbp                                    #48.1
..___tag_value_main.20:                                         #
        subq      $400, %rsp                                    #48.1
        movq      %r15, -8(%rbp)                                #48.1
        movq      %r14, -16(%rbp)                               #48.1
        movq      %r13, -24(%rbp)                               #48.1
        movq      %r12, -32(%rbp)                               #48.1
        movq      %rbx, -40(%rbp)                               #48.1
        movl      %edi, -208(%rbp)                              #48.1
        movq      %rsi, -200(%rbp)                              #48.1
        movl      $.2.3_2_kmpc_loc_struct_pack.1, %eax          #48.1
        movl      $0, %edx                                      #48.1
        movq      %rax, %rdi                                    #48.1
        movq      %rdx, %rsi                                    #48.1
        movl      $0, %eax                                      #48.1
..___tag_value_main.22:                                         #48.1
        call      __kmpc_begin                                  #48.1
..___tag_value_main.23:                                         #
                                # LOE rbp rsp rip
..B3.2:                         # Preds ..B3.1

###     int i, r, t,nthreads;
###     double *time;
###     float *v;
###     #pragma omp parallel

        movl      $.2.3_2_kmpc_loc_struct_pack.12, %eax         #52.5
        movq      %rax, %rdi                                    #52.5
..___tag_value_main.29:                                         #52.5
        call      __kmpc_global_thread_num                      #52.5
..___tag_value_main.30:                                         #
                                # LOE rbp rsp rip eax
..B3.55:                        # Preds ..B3.2
        movl      %eax, -360(%rbp)                              #52.5
        movl      -360(%rbp), %eax                              #52.5
        movl      %eax, -356(%rbp)                              #52.5
        movl      $.2.3_2_kmpc_loc_struct_pack.36, %eax         #52.5
        movq      %rax, %rdi                                    #52.5
        movl      $0, %eax                                      #52.5
..___tag_value_main.31:                                         #52.5
        call      __kmpc_ok_to_fork                             #52.5
..___tag_value_main.32:                                         #
                                # LOE rbp rsp rip eax
..B3.54:                        # Preds ..B3.55
        movl      %eax, -352(%rbp)                              #52.5
                                # LOE rbp rsp rip
..B3.3:                         # Preds ..B3.54
        movl      -352(%rbp), %eax                              #52.5
        testl     %eax, %eax                                    #52.5
        jne       ..B3.7        # Prob 50%                      #52.5
                                # LOE rbp rsp rip
..B3.4:                         # Preds ..B3.3
        movl      $.2.3_2_kmpc_loc_struct_pack.36, %eax         #52.5
        movl      -356(%rbp), %edx                              #52.5
        movq      %rax, %rdi                                    #52.5
        movl      %edx, %esi                                    #52.5
        movl      $0, %eax                                      #52.5
..___tag_value_main.33:                                         #52.5
        call      __kmpc_serialized_parallel                    #52.5
..___tag_value_main.34:                                         #
                                # LOE rbp rsp rip
..B3.5:                         # Preds ..B3.4
        lea       -356(%rbp), %rax                              #52.5
        movl      $___kmpv_zeromain_0, %edx                     #52.5
        lea       -332(%rbp), %rcx                              #52.5
        movq      %rax, %rdi                                    #52.5
        movq      %rdx, %rsi                                    #52.5
        movq      %rcx, %rdx                                    #52.5
..___tag_value_main.35:                                         #52.5
        call      L_main_52__par_region0_2.2                    #52.5
..___tag_value_main.36:                                         #
                                # LOE rbp rsp rip
..B3.6:                         # Preds ..B3.5
        movl      $.2.3_2_kmpc_loc_struct_pack.36, %eax         #52.5
        movl      -356(%rbp), %edx                              #52.5
        movq      %rax, %rdi                                    #52.5
        movl      %edx, %esi                                    #52.5
        movl      $0, %eax                                      #52.5
..___tag_value_main.37:                                         #52.5
        call      __kmpc_end_serialized_parallel                #52.5
..___tag_value_main.38:                                         #
        jmp       ..B3.8        # Prob 100%                     #52.5
                                # LOE rbp rsp rip
..B3.7:                         # Preds ..B3.3
        movl      $.2.3_2_kmpc_loc_struct_pack.36, %eax         #52.5
        movl      $1, %edx                                      #52.5
        movl      $L_main_52__par_region0_2.2, %ecx             #52.5
        lea       -332(%rbp), %rbx                              #52.5
        movq      %rax, %rdi                                    #52.5
        movl      %edx, %esi                                    #52.5
        movq      %rcx, %rdx                                    #52.5
        movq      %rbx, %rcx                                    #52.5
        movl      $0, %eax                                      #52.5
..___tag_value_main.39:                                         #52.5
        call      __kmpc_fork_call                              #52.5
..___tag_value_main.40:                                         #
                                # LOE rbp rsp rip
..B3.8:                         # Preds ..B3.7 ..B3.6

###     {
###     #pragma omp master
###         {
###             nthreads = omp_get_num_threads();
###         }
###     }
###     int n = (argc > 1) ? atoi(argv[1]) : NUM_ELEMS;

        movl      -208(%rbp), %eax                              #59.14
        cmpl      $1, %eax                                      #59.21
        jg        ..B3.10       # Prob 50%                      #59.21
                                # LOE rbp rsp rip
..B3.9:                         # Preds ..B3.8
        movl      $5017600, -324(%rbp)                          #59.42
        jmp       ..B3.11       # Prob 100%                     #59.42
                                # LOE rbp rsp rip
..B3.10:                        # Preds ..B3.8
        movl      $8, %eax                                      #59.26
        addq      -200(%rbp), %rax                              #59.26
        movq      (%rax), %rax                                  #59.26
        movq      %rax, %rdi                                    #59.26
..___tag_value_main.41:                                         #59.26
        call      atoi                                          #59.26
..___tag_value_main.42:                                         #
                                # LOE rbp rsp rip eax
..B3.56:                        # Preds ..B3.10
        movl      %eax, -328(%rbp)                              #59.26
        movl      -328(%rbp), %eax                              #59.26
        movl      %eax, -324(%rbp)                              #59.42
                                # LOE rbp rsp rip
..B3.11:                        # Preds ..B3.56 ..B3.9
        movl      -324(%rbp), %eax                              #59.42
        movl      %eax, -320(%rbp)                              #59.11

###     int rep = (argc > 2) ? atoi(argv[2]) : TIMES;

        movl      -208(%rbp), %eax                              #60.16
        cmpl      $2, %eax                                      #60.23
        jg        ..B3.13       # Prob 50%                      #60.23
                                # LOE rbp rsp rip
..B3.12:                        # Preds ..B3.11
        movl      $50, -312(%rbp)                               #60.44
        jmp       ..B3.14       # Prob 100%                     #60.44
                                # LOE rbp rsp rip
..B3.13:                        # Preds ..B3.11
        movl      $16, %eax                                     #60.28
        addq      -200(%rbp), %rax                              #60.28
        movq      (%rax), %rax                                  #60.28
        movq      %rax, %rdi                                    #60.28
..___tag_value_main.43:                                         #60.28
        call      atoi                                          #60.28
..___tag_value_main.44:                                         #
                                # LOE rbp rsp rip eax
..B3.57:                        # Preds ..B3.13
        movl      %eax, -316(%rbp)                              #60.28
        movl      -316(%rbp), %eax                              #60.28
        movl      %eax, -312(%rbp)                              #60.44
                                # LOE rbp rsp rip
..B3.14:                        # Preds ..B3.57 ..B3.12
        movl      -312(%rbp), %eax                              #60.44
        movl      %eax, -308(%rbp)                              #60.13

###     time = malloc(sizeof (double)*rep);

        movl      -308(%rbp), %eax                              #61.12
        movslq    %eax, %rax                                    #61.12
        imulq     $8, %rax, %rax                                #61.12
        movq      %rax, %rdi                                    #61.12
..___tag_value_main.45:                                         #61.12
        call      malloc                                        #61.12
..___tag_value_main.46:                                         #
                                # LOE rax rbp rsp rip
..B3.58:                        # Preds ..B3.14
        movq      %rax, -192(%rbp)                              #61.12
                                # LOE rbp rsp rip
..B3.15:                        # Preds ..B3.58
        movq      -192(%rbp), %rax                              #61.12
        movq      %rax, -184(%rbp)                              #61.5

###     v = malloc(sizeof (float)*n);

        movl      -320(%rbp), %eax                              #62.9
        movslq    %eax, %rax                                    #62.9
        imulq     $4, %rax, %rax                                #62.9
        movq      %rax, %rdi                                    #62.9
..___tag_value_main.47:                                         #62.9
        call      malloc                                        #62.9
..___tag_value_main.48:                                         #
                                # LOE rax rbp rsp rip
..B3.59:                        # Preds ..B3.15
        movq      %rax, -176(%rbp)                              #62.9
                                # LOE rbp rsp rip
..B3.16:                        # Preds ..B3.59
        movq      -176(%rbp), %rax                              #62.9
        movq      %rax, -168(%rbp)                              #62.5

###     
###     for (r = 0; r < rep; r++) {

        movl      $0, -304(%rbp)                                #64.10
        movl      -304(%rbp), %eax                              #64.17
        movl      -308(%rbp), %edx                              #64.21
        cmpl      %edx, %eax                                    #64.21
        jge       ..B3.27       # Prob 50%                      #64.21
                                # LOE rbp rsp rip
..B3.17:                        # Preds ..B3.26 ..B3.16

###     init(v, n);

        movq      -168(%rbp), %rax                              #65.5
        movl      -320(%rbp), %edx                              #65.5
        movq      %rax, %rdi                                    #65.5
        movl      %edx, %esi                                    #65.5
..___tag_value_main.49:                                         #65.5
        call      init                                          #65.5
..___tag_value_main.50:                                         #
                                # LOE rbp rsp rip
..B3.18:                        # Preds ..B3.17

###         time[r] = omp_get_wtime();

..___tag_value_main.51:                                         #66.19
        call      omp_get_wtime                                 #66.19
..___tag_value_main.52:                                         #
                                # LOE rbp rsp rip xmm0
..B3.60:                        # Preds ..B3.18
        movsd     %xmm0, -136(%rbp)                             #66.19
                                # LOE rbp rsp rip
..B3.19:                        # Preds ..B3.60
        movl      -304(%rbp), %eax                              #66.14
        movslq    %eax, %rax                                    #66.9
        imulq     $8, %rax, %rax                                #66.9
        addq      -184(%rbp), %rax                              #66.9
        movsd     -136(%rbp), %xmm0                             #66.19
        movsd     %xmm0, (%rax)                                 #66.9

### 	    #pragma omp parallel for

        movl      $.2.3_2_kmpc_loc_struct_pack.47, %eax         #67.6
        movq      %rax, %rdi                                    #67.6
        movl      $0, %eax                                      #67.6
..___tag_value_main.53:                                         #67.6
        call      __kmpc_ok_to_fork                             #67.6
..___tag_value_main.54:                                         #
                                # LOE rbp rsp rip eax
..B3.61:                        # Preds ..B3.19
        movl      %eax, -296(%rbp)                              #67.6
                                # LOE rbp rsp rip
..B3.20:                        # Preds ..B3.61
        movl      -296(%rbp), %eax                              #67.6
        testl     %eax, %eax                                    #67.6
        jne       ..B3.24       # Prob 50%                      #67.6
                                # LOE rbp rsp rip
..B3.21:                        # Preds ..B3.20
        movl      $.2.3_2_kmpc_loc_struct_pack.47, %eax         #67.6
        movl      -356(%rbp), %edx                              #67.6
        movq      %rax, %rdi                                    #67.6
        movl      %edx, %esi                                    #67.6
        movl      $0, %eax                                      #67.6
..___tag_value_main.55:                                         #67.6
        call      __kmpc_serialized_parallel                    #67.6
..___tag_value_main.56:                                         #
                                # LOE rbp rsp rip
..B3.22:                        # Preds ..B3.21
        lea       -356(%rbp), %rax                              #67.6
        movl      $___kmpv_zeromain_1, %edx                     #67.6
        lea       -320(%rbp), %rcx                              #67.6
        lea       -168(%rbp), %rbx                              #67.6
        lea       -288(%rbp), %rsi                              #67.6
        lea       -284(%rbp), %rdi                              #67.6
        movq      %rdi, -88(%rbp)                               #67.6
        movq      %rax, %rdi                                    #67.6
        movq      %rsi, -80(%rbp)                               #67.6
        movq      %rdx, %rsi                                    #67.6
        movq      %rcx, %rdx                                    #67.6
        movq      %rbx, %rcx                                    #67.6
        movq      -80(%rbp), %rax                               #67.6
        movq      %rax, %r8                                     #67.6
        movq      -88(%rbp), %rax                               #67.6
        movq      %rax, %r9                                     #67.6
..___tag_value_main.57:                                         #67.6
        call      L_main_67__par_loop1_2.3                      #67.6
..___tag_value_main.58:                                         #
                                # LOE rbp rsp rip
..B3.23:                        # Preds ..B3.22
        movl      $.2.3_2_kmpc_loc_struct_pack.47, %eax         #67.6
        movl      -356(%rbp), %edx                              #67.6
        movq      %rax, %rdi                                    #67.6
        movl      %edx, %esi                                    #67.6
        movl      $0, %eax                                      #67.6
..___tag_value_main.59:                                         #67.6
        call      __kmpc_end_serialized_parallel                #67.6
..___tag_value_main.60:                                         #
        jmp       ..B3.25       # Prob 100%                     #67.6
                                # LOE rbp rsp rip
..B3.24:                        # Preds ..B3.20
        addq      $-16, %rsp                                    #67.6
        movl      $.2.3_2_kmpc_loc_struct_pack.47, %eax         #67.6
        movl      $4, %edx                                      #67.6
        movl      $L_main_67__par_loop1_2.3, %ecx               #67.6
        lea       -320(%rbp), %rbx                              #67.6
        lea       -168(%rbp), %rsi                              #67.6
        lea       -288(%rbp), %rdi                              #67.6
        lea       -284(%rbp), %r8                               #67.6
        movq      %r8, (%rsp)                                   #67.6
        movq      %rdi, -72(%rbp)                               #67.6
        movq      %rax, %rdi                                    #67.6
        movq      %rsi, -64(%rbp)                               #67.6
        movl      %edx, %esi                                    #67.6
        movq      %rcx, %rdx                                    #67.6
        movq      %rbx, %rcx                                    #67.6
        movq      -64(%rbp), %rax                               #67.6
        movq      %rax, %r8                                     #67.6
        movq      -72(%rbp), %rax                               #67.6
        movq      %rax, %r9                                     #67.6
        movl      $0, %eax                                      #67.6
..___tag_value_main.61:                                         #67.6
        call      __kmpc_fork_call                              #67.6
..___tag_value_main.62:                                         #
                                # LOE rbp rsp rip
..B3.62:                        # Preds ..B3.24
        addq      $16, %rsp                                     #67.6
                                # LOE rbp rsp rip
..B3.25:                        # Preds ..B3.62 ..B3.23

###             for (i = 0; i < n; i++) {
### 		//printf("Thread %d\n", omp_get_thread_num());
###                 v[i] *= 0.9f;
###             }
###         time[r] = omp_get_wtime() - time[r];

..___tag_value_main.63:                                         #72.19
        call      omp_get_wtime                                 #72.19
..___tag_value_main.64:                                         #
                                # LOE rbp rsp rip xmm0
..B3.63:                        # Preds ..B3.25
        movsd     %xmm0, -96(%rbp)                              #72.19
                                # LOE rbp rsp rip
..B3.26:                        # Preds ..B3.63
        movl      -304(%rbp), %eax                              #72.42
        movslq    %eax, %rax                                    #72.37
        imulq     $8, %rax, %rax                                #72.37
        addq      -184(%rbp), %rax                              #72.37
        movsd     -96(%rbp), %xmm0                              #72.19
        movsd     (%rax), %xmm1                                 #72.37
        subsd     %xmm1, %xmm0                                  #72.37
        movl      -304(%rbp), %eax                              #72.14
        movslq    %eax, %rax                                    #72.9
        imulq     $8, %rax, %rax                                #72.9
        addq      -184(%rbp), %rax                              #72.9
        movsd     %xmm0, (%rax)                                 #72.9
        movl      $1, %eax                                      #64.26
        addl      -304(%rbp), %eax                              #64.26
        movl      %eax, -304(%rbp)                              #64.26
        movl      -304(%rbp), %eax                              #64.17
        movl      -308(%rbp), %edx                              #64.21
        cmpl      %edx, %eax                                    #64.21
        jl        ..B3.17       # Prob 50%                      #64.21
                                # LOE rbp rsp rip
..B3.27:                        # Preds ..B3.26 ..B3.16

###     }
###  double min, max, avg, aux, sigma, dev;
###     min = time[0];

        movq      -184(%rbp), %rax                              #75.11
        movsd     (%rax), %xmm0                                 #75.11
        movsd     %xmm0, -160(%rbp)                             #75.5

###     max = time[0];

        movq      -184(%rbp), %rax                              #76.11
        movsd     (%rax), %xmm0                                 #76.11
        movsd     %xmm0, -152(%rbp)                             #76.5

###     aux = time[0];

        movq      -184(%rbp), %rax                              #77.11
        movsd     (%rax), %xmm0                                 #77.11
        movsd     %xmm0, -144(%rbp)                             #77.5

###     for (t = 1; t < TIMES; t++) {

        movl      $1, -300(%rbp)                                #78.10
        movl      -300(%rbp), %eax                              #78.17
        cmpl      $50, %eax                                     #78.21
        jge       ..B3.29       # Prob 50%                      #78.21
                                # LOE rbp rsp rip
..B3.28:                        # Preds ..B3.28 ..B3.27

###         if (time[t] < min) min = time[t];

        movl      -300(%rbp), %eax                              #79.18
        movslq    %eax, %rax                                    #79.13
        imulq     $8, %rax, %rax                                #79.13
        addq      -184(%rbp), %rax                              #79.13
        movsd     (%rax), %xmm0                                 #79.13
        movsd     -160(%rbp), %xmm1                             #79.23
        minsd     %xmm1, %xmm0                                  #79.9
        movsd     %xmm0, -160(%rbp)                             #79.28

###         if (time[t] > max) max = time[t];

        movl      -300(%rbp), %eax                              #80.18
        movslq    %eax, %rax                                    #80.13
        imulq     $8, %rax, %rax                                #80.13
        addq      -184(%rbp), %rax                              #80.13
        movsd     (%rax), %xmm0                                 #80.13
        movsd     -152(%rbp), %xmm1                             #80.23
        maxsd     %xmm1, %xmm0                                  #80.9
        movsd     %xmm0, -152(%rbp)                             #80.28

###         aux += time[t];

        movl      -300(%rbp), %eax                              #81.21
        movslq    %eax, %rax                                    #81.16
        imulq     $8, %rax, %rax                                #81.16
        addq      -184(%rbp), %rax                              #81.16
        movsd     -144(%rbp), %xmm0                             #81.9
        movsd     (%rax), %xmm1                                 #81.16
        addsd     %xmm1, %xmm0                                  #81.9
        movsd     %xmm0, -144(%rbp)                             #81.9
        movl      $1, %eax                                      #78.28
        addl      -300(%rbp), %eax                              #78.28
        movl      %eax, -300(%rbp)                              #78.28
        movl      -300(%rbp), %eax                              #78.17
        cmpl      $50, %eax                                     #78.21
        jl        ..B3.28       # Prob 50%                      #78.21
                                # LOE rbp rsp rip
..B3.29:                        # Preds ..B3.28 ..B3.27

###     }
###     avg = aux / TIMES;

        movsd     -144(%rbp), %xmm0                             #83.11
        movsd     .L_2il0floatpacket.4(%rip), %xmm1             #83.17
        divsd     %xmm1, %xmm0                                  #83.17
        movsd     %xmm0, -128(%rbp)                             #83.5

###     for (t = 0; t < TIMES; t++) {

        movl      $0, -300(%rbp)                                #84.10
        movl      -300(%rbp), %eax                              #84.17
        cmpl      $50, %eax                                     #84.21
        jge       ..B3.31       # Prob 50%                      #84.21
                                # LOE rbp rsp rip
..B3.30:                        # Preds ..B3.30 ..B3.29

###         sigma = sigma + ((time[t] - avg)*(time[t] - avg));

        movl      -300(%rbp), %eax                              #85.32
        movslq    %eax, %rax                                    #85.27
        imulq     $8, %rax, %rax                                #85.27
        addq      -184(%rbp), %rax                              #85.27
        movsd     (%rax), %xmm0                                 #85.27
        movsd     -128(%rbp), %xmm1                             #85.37
        subsd     %xmm1, %xmm0                                  #85.37
        movl      -300(%rbp), %eax                              #85.48
        movslq    %eax, %rax                                    #85.43
        imulq     $8, %rax, %rax                                #85.43
        addq      -184(%rbp), %rax                              #85.43
        movsd     (%rax), %xmm1                                 #85.43
        movsd     -128(%rbp), %xmm2                             #85.53
        subsd     %xmm2, %xmm1                                  #85.53
        mulsd     %xmm1, %xmm0                                  #85.53
        movsd     -120(%rbp), %xmm1                             #85.17
        addsd     %xmm0, %xmm1                                  #85.53
        movsd     %xmm1, -120(%rbp)                             #85.9
        movl      $1, %eax                                      #84.28
        addl      -300(%rbp), %eax                              #84.28
        movl      %eax, -300(%rbp)                              #84.28
        movl      -300(%rbp), %eax                              #84.17
        cmpl      $50, %eax                                     #84.21
        jl        ..B3.30       # Prob 50%                      #84.21
                                # LOE rbp rsp rip
..B3.31:                        # Preds ..B3.30 ..B3.29

###     }
### #ifndef VERBOSE
###     dev = sqrt(sigma / (TIMES - 1));

        movsd     -120(%rbp), %xmm0                             #88.11
        movsd     .L_2il0floatpacket.5(%rip), %xmm1             #88.11
        divsd     %xmm1, %xmm0                                  #88.11
..___tag_value_main.65:                                         #88.11
        call      sqrt                                          #88.11
..___tag_value_main.66:                                         #
                                # LOE rbp rsp rip xmm0
..B3.64:                        # Preds ..B3.31
        movsd     %xmm0, -112(%rbp)                             #88.11
                                # LOE rbp rsp rip
..B3.32:                        # Preds ..B3.64
        movsd     -112(%rbp), %xmm0                             #88.11
        movsd     %xmm0, -104(%rbp)                             #88.5

### #else
###     dev = sqrt(sigma);
### #endif
###     printf("%d %d %f [%f - %f] %f\n",

        movl      $.L_2__STRING.1, %eax                         #92.5
        movl      -332(%rbp), %edx                              #92.5
        movl      -320(%rbp), %ecx                              #92.5
        movsd     -128(%rbp), %xmm0                             #92.5
        movsd     -160(%rbp), %xmm1                             #92.5
        movsd     -152(%rbp), %xmm2                             #92.5
        movsd     -104(%rbp), %xmm3                             #92.5
        movq      %rax, %rdi                                    #92.5
        movl      %edx, %esi                                    #92.5
        movl      %ecx, %edx                                    #92.5
        movl      $4, %eax                                      #92.5
..___tag_value_main.67:                                         #92.5
        call      printf                                        #92.5
..___tag_value_main.68:                                         #
                                # LOE rbp rsp rip eax
..B3.65:                        # Preds ..B3.32
        movl      %eax, -292(%rbp)                              #92.5
                                # LOE rbp rsp rip
..B3.33:                        # Preds ..B3.65

###             nthreads, n, avg, min, max, dev);
###     check(v, n);

        movq      -168(%rbp), %rax                              #94.5
        movl      -320(%rbp), %edx                              #94.5
        movq      %rax, %rdi                                    #94.5
        movl      %edx, %esi                                    #94.5
..___tag_value_main.69:                                         #94.5
        call      check                                         #94.5
..___tag_value_main.70:                                         #
                                # LOE rbp rsp rip
..B3.34:                        # Preds ..B3.33

###     free(v);

        movq      -168(%rbp), %rax                              #95.5
        movq      %rax, %rdi                                    #95.5
..___tag_value_main.71:                                         #95.5
        call      free                                          #95.5
..___tag_value_main.72:                                         #
                                # LOE rbp rsp rip
..B3.35:                        # Preds ..B3.34

###     free(time);

        movq      -184(%rbp), %rax                              #96.5
        movq      %rax, %rdi                                    #96.5
..___tag_value_main.73:                                         #96.5
        call      free                                          #96.5
..___tag_value_main.74:                                         #
                                # LOE rbp rsp rip
..B3.36:                        # Preds ..B3.35

###     
###     return 0;

        movl      $.2.3_2_kmpc_loc_struct_pack.20, %eax         #98.12
        movq      %rax, %rdi                                    #98.12
        movl      $0, %eax                                      #98.12
..___tag_value_main.75:                                         #98.12
        call      __kmpc_end                                    #98.12
..___tag_value_main.76:                                         #
                                # LOE rbp rsp rip
..B3.37:                        # Preds ..B3.36
        movl      $0, %eax                                      #98.12
        movq      -40(%rbp), %rbx                               #98.12
..___tag_value_main.77:                                         #
        movq      -32(%rbp), %r12                               #98.12
..___tag_value_main.78:                                         #
        movq      -24(%rbp), %r13                               #98.12
..___tag_value_main.79:                                         #
        movq      -16(%rbp), %r14                               #98.12
..___tag_value_main.80:                                         #
        movq      -8(%rbp), %r15                                #98.12
..___tag_value_main.81:                                         #
        leave                                                   #98.12
..___tag_value_main.82:                                         #
        ret                                                     #98.12
..___tag_value_main.83:                                         #
                                # LOE
..B3.38:                        # Preds ..B3.42 ..B3.40
        movl      $0, %eax                                      #52.5
        movq      -40(%rbp), %rbx                               #52.5
..___tag_value_main.89:                                         #
        movq      -32(%rbp), %r12                               #52.5
..___tag_value_main.90:                                         #
        movq      -24(%rbp), %r13                               #52.5
..___tag_value_main.91:                                         #
        movq      -16(%rbp), %r14                               #52.5
..___tag_value_main.92:                                         #
        movq      -8(%rbp), %r15                                #52.5
..___tag_value_main.93:                                         #
        leave                                                   #52.5
..___tag_value_main.94:                                         #
        ret                                                     #52.5
                                # LOE
L_main_52__par_region0_2.2:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
..B3.39:                        # Preds ..B3.0
        pushq     %rbp                                          #52.5
..___tag_value_main.96:                                         #
        movq      %rsp, %rbp                                    #52.5
..___tag_value_main.97:                                         #
        subq      $400, %rsp                                    #52.5
        movq      %r15, -8(%rbp)                                #52.5
        movq      %r14, -16(%rbp)                               #52.5
        movq      %r13, -24(%rbp)                               #52.5
        movq      %r12, -32(%rbp)                               #52.5
        movq      %rbx, -40(%rbp)                               #52.5
        movq      %rdi, -232(%rbp)                              #52.5
        movq      %rsi, -224(%rbp)                              #52.5
        movq      %rdx, -216(%rbp)                              #52.5
        movq      -232(%rbp), %rax                              #52.5
        movl      (%rax), %eax                                  #52.5
        movl      %eax, -388(%rbp)                              #52.5
        movl      $.2.3_2_kmpc_loc_struct_pack.28, %eax         #54.5
        movl      -388(%rbp), %edx                              #54.5
        movq      %rax, %rdi                                    #54.5
        movl      %edx, %esi                                    #54.5
        movl      $0, %eax                                      #54.5
..___tag_value_main.99:                                         #54.5
        call      __kmpc_master                                 #54.5
..___tag_value_main.100:                                        #
                                # LOE rbp rsp rip eax
..B3.66:                        # Preds ..B3.39
        movl      %eax, -384(%rbp)                              #54.5
                                # LOE rbp rsp rip
..B3.40:                        # Preds ..B3.66
        movl      -384(%rbp), %eax                              #54.5
        cmpl      $1, %eax                                      #54.5
        jne       ..B3.38       # Prob 50%                      #54.5
                                # LOE rbp rsp rip
..B3.41:                        # Preds ..B3.40
..___tag_value_main.106:                                        #56.24
        call      omp_get_num_threads                           #56.24
..___tag_value_main.107:                                        #
                                # LOE rbp rsp rip eax
..B3.67:                        # Preds ..B3.41
        movl      %eax, -336(%rbp)                              #56.24
                                # LOE rbp rsp rip
..B3.42:                        # Preds ..B3.67
        movq      -216(%rbp), %rax                              #56.13
        movl      -336(%rbp), %edx                              #56.24
        movl      %edx, (%rax)                                  #56.13
        movl      $.2.3_2_kmpc_loc_struct_pack.28, %eax         #54.5
        movl      -388(%rbp), %edx                              #54.5
        movq      %rax, %rdi                                    #54.5
        movl      %edx, %esi                                    #54.5
        movl      $0, %eax                                      #54.5
..___tag_value_main.108:                                        #54.5
        call      __kmpc_end_master                             #54.5
..___tag_value_main.109:                                        #
        jmp       ..B3.38       # Prob 100%                     #54.5
                                # LOE rbp rsp rip
..B3.43:                        # Preds ..B3.49
        movl      $0, -380(%rbp)                                #67.6
        movq      -248(%rbp), %rax                              #67.6
        movl      (%rax), %eax                                  #68.37
        movl      $1, %edx                                      #67.6
        subl      %edx, %eax                                    #67.6
        movl      %eax, -376(%rbp)                              #67.6
        movq      -248(%rbp), %rax                              #67.6
        movl      (%rax), %eax                                  #68.37
        movl      $1, %edx                                      #67.6
        subl      %edx, %eax                                    #67.6
        movl      %eax, -372(%rbp)                              #67.6
        movl      $0, -368(%rbp)                                #67.6
        movl      $1, -364(%rbp)                                #67.6
        addq      $-32, %rsp                                    #67.6
        movl      $.2.3_2_kmpc_loc_struct_pack.47, %eax         #67.6
        movl      -400(%rbp), %edx                              #67.6
        movl      $34, %ecx                                     #67.6
        lea       -368(%rbp), %rbx                              #67.6
        lea       -380(%rbp), %rsi                              #67.6
        lea       -376(%rbp), %rdi                              #67.6
        lea       -364(%rbp), %r8                               #67.6
        movq      %r8, (%rsp)                                   #67.6
        movl      $1, 8(%rsp)                                   #67.6
        movl      $1, 16(%rsp)                                  #67.6
        movq      %rdi, -56(%rbp)                               #67.6
        movq      %rax, %rdi                                    #67.6
        movq      %rsi, -48(%rbp)                               #67.6
        movl      %edx, %esi                                    #67.6
        movl      %ecx, %edx                                    #67.6
        movq      %rbx, %rcx                                    #67.6
        movq      -48(%rbp), %rax                               #67.6
        movq      %rax, %r8                                     #67.6
        movq      -56(%rbp), %rax                               #67.6
        movq      %rax, %r9                                     #67.6
        movl      $0, %eax                                      #67.6
..___tag_value_main.110:                                        #67.6
        call      __kmpc_for_static_init_4                      #67.6
..___tag_value_main.111:                                        #
                                # LOE rbp rsp rip
..B3.68:                        # Preds ..B3.43
        addq      $32, %rsp                                     #67.6
                                # LOE rbp rsp rip
..B3.44:                        # Preds ..B3.68
        movl      -380(%rbp), %eax                              #67.6
        movl      %eax, -348(%rbp)                              #67.6
        movl      -376(%rbp), %eax                              #67.6
        movl      %eax, -344(%rbp)                              #67.6
        movl      -364(%rbp), %eax                              #67.6
        movl      %eax, -340(%rbp)                              #67.6
        movl      -348(%rbp), %eax                              #67.6
        movl      -372(%rbp), %edx                              #67.6
        cmpl      %edx, %eax                                    #67.6
        jg        ..B3.47       # Prob 50%                      #67.6
                                # LOE rbp rsp rip
..B3.45:                        # Preds ..B3.44
        movl      -344(%rbp), %eax                              #67.6
        movl      -372(%rbp), %edx                              #67.6
        cmpl      %edx, %eax                                    #67.6
        jle       ..B3.50       # Prob 50%                      #67.6
                                # LOE rbp rsp rip
..B3.46:                        # Preds ..B3.45
        movl      -372(%rbp), %eax                              #67.6
        movl      %eax, -344(%rbp)                              #67.6
        jmp       ..B3.50       # Prob 100%                     #67.6
                                # LOE rbp rsp rip
..B3.47:                        # Preds ..B3.51 ..B3.50 ..B3.44
        movl      $.2.3_2_kmpc_loc_struct_pack.47, %eax         #67.6
        movl      -400(%rbp), %edx                              #67.6
        movq      %rax, %rdi                                    #67.6
        movl      %edx, %esi                                    #67.6
        movl      $0, %eax                                      #67.6
..___tag_value_main.112:                                        #67.6
        call      __kmpc_for_static_fini                        #67.6
..___tag_value_main.113:                                        #
                                # LOE rbp rsp rip
..B3.48:                        # Preds ..B3.47 ..B3.49
        movl      $0, %eax                                      #67.6
        movq      -40(%rbp), %rbx                               #67.6
..___tag_value_main.114:                                        #
        movq      -32(%rbp), %r12                               #67.6
..___tag_value_main.115:                                        #
        movq      -24(%rbp), %r13                               #67.6
..___tag_value_main.116:                                        #
        movq      -16(%rbp), %r14                               #67.6
..___tag_value_main.117:                                        #
        movq      -8(%rbp), %r15                                #67.6
..___tag_value_main.118:                                        #
        leave                                                   #67.6
..___tag_value_main.119:                                        #
        ret                                                     #67.6
                                # LOE
L_main_67__par_loop1_2.3:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %rcx
# parameter 5: %r8
# parameter 6: %r9
..B3.49:                        # Preds ..B3.0
        pushq     %rbp                                          #67.6
..___tag_value_main.121:                                        #
        movq      %rsp, %rbp                                    #67.6
..___tag_value_main.122:                                        #
        subq      $400, %rsp                                    #67.6
        movq      %r15, -8(%rbp)                                #67.6
        movq      %r14, -16(%rbp)                               #67.6
        movq      %r13, -24(%rbp)                               #67.6
        movq      %r12, -32(%rbp)                               #67.6
        movq      %rbx, -40(%rbp)                               #67.6
        movq      %rdi, -280(%rbp)                              #67.6
        movq      %rsi, -272(%rbp)                              #67.6
        movq      %rdx, -264(%rbp)                              #67.6
        movq      %rcx, -256(%rbp)                              #67.6
        movq      %r8, -248(%rbp)                               #67.6
        movq      %r9, -240(%rbp)                               #67.6
        movq      -280(%rbp), %rax                              #67.6
        movl      (%rax), %eax                                  #67.6
        movl      %eax, -400(%rbp)                              #67.6
        movq      -264(%rbp), %rax                              #67.6
        movl      (%rax), %eax                                  #67.6
        movl      %eax, -396(%rbp)                              #67.6
        movl      $0, -392(%rbp)                                #68.18
        movq      -248(%rbp), %rax                              #68.18
        movl      -396(%rbp), %edx                              #68.18
        movl      %edx, (%rax)                                  #68.18
        movl      -392(%rbp), %eax                              #68.37
        movq      -248(%rbp), %rdx                              #68.13
        movl      (%rdx), %edx                                  #68.37
        cmpl      %edx, %eax                                    #68.37
        jl        ..B3.43       # Prob 50%                      #68.37
        jmp       ..B3.48       # Prob 100%                     #68.37
..___tag_value_main.124:                                        #
                                # LOE rbp rsp rip
..B3.50:                        # Preds ..B3.46 ..B3.45
        movl      -348(%rbp), %eax                              #67.6
        movl      %eax, -392(%rbp)                              #67.6
        movl      -392(%rbp), %eax                              #68.37
        movl      -344(%rbp), %edx                              #67.6
        cmpl      %edx, %eax                                    #68.37
        jg        ..B3.47       # Prob 50%                      #68.37
                                # LOE rbp rsp rip
..B3.51:                        # Preds ..B3.51 ..B3.50
        movq      -256(%rbp), %rax                              #70.17
        movl      -392(%rbp), %edx                              #70.19
        movslq    %edx, %rdx                                    #70.17
        imulq     $4, %rdx, %rdx                                #70.17
        addq      (%rax), %rdx                                  #70.17
        movss     (%rdx), %xmm0                                 #70.17
        movss     .L_2il0floatpacket.1(%rip), %xmm1             #70.25
        mulss     %xmm1, %xmm0                                  #70.17
        movq      -256(%rbp), %rax                              #70.17
        movl      -392(%rbp), %edx                              #70.19
        movslq    %edx, %rdx                                    #70.17
        imulq     $4, %rdx, %rdx                                #70.17
        addq      (%rax), %rdx                                  #70.17
        movss     %xmm0, (%rdx)                                 #70.17
        movl      $1, %eax                                      #68.32
        addl      -392(%rbp), %eax                              #68.32
        movl      %eax, -392(%rbp)                              #68.32
        movl      -392(%rbp), %eax                              #68.37
        movl      -344(%rbp), %edx                              #68.13
        cmpl      %edx, %eax                                    #68.37
        jle       ..B3.51       # Prob 50%                      #68.37
        jmp       ..B3.47       # Prob 100%                     #68.37
..___tag_value_main.129:                                        #
                                # LOE rbp rsp rip
# mark_end;
	.type	main,@function
	.size	main,.-main
	.data
	.align 4
	.align 4
.2.3_2_kmpc_loc_struct_pack.1:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.3_2__kmpc_loc_pack.0
	.align 4
.2.3_2__kmpc_loc_pack.0:
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
	.byte	52
	.byte	56
	.byte	59
	.byte	52
	.byte	56
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.3_2_kmpc_loc_struct_pack.12:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.3_2__kmpc_loc_pack.11
	.align 4
.2.3_2__kmpc_loc_pack.11:
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
	.byte	50
	.byte	59
	.byte	53
	.byte	50
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.3_2_kmpc_loc_struct_pack.36:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.3_2__kmpc_loc_pack.35
	.align 4
.2.3_2__kmpc_loc_pack.35:
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
	.byte	50
	.byte	59
	.byte	53
	.byte	56
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.3_2_kmpc_loc_struct_pack.47:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.3_2__kmpc_loc_pack.46
	.align 4
.2.3_2__kmpc_loc_pack.46:
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
	.byte	55
	.byte	59
	.byte	55
	.byte	49
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.3_2_kmpc_loc_struct_pack.20:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.3_2__kmpc_loc_pack.19
	.align 4
.2.3_2__kmpc_loc_pack.19:
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
	.byte	57
	.byte	56
	.byte	59
	.byte	57
	.byte	56
	.byte	59
	.byte	59
	.space 3, 0x00 	# pad
	.align 4
.2.3_2_kmpc_loc_struct_pack.28:
	.long	0
	.long	2
	.long	0
	.long	0
	.quad	.2.3_2__kmpc_loc_pack.27
	.align 4
.2.3_2__kmpc_loc_pack.27:
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
	.byte	52
	.byte	59
	.byte	53
	.byte	55
	.byte	59
	.byte	59
	.data
# -- End  main, L_main_52__par_region0_2.2, L_main_67__par_loop1_2.3
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
	.4byte 0x00000014
	.8byte 0x7801000100000000
	.8byte 0x0000019008070c10
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x0000001c
	.8byte ..___tag_value_init.1
	.8byte ..___tag_value_init.7-..___tag_value_init.1
	.byte 0x04
	.4byte ..___tag_value_init.3-..___tag_value_init.1
	.2byte 0x100e
	.byte 0x04
	.4byte ..___tag_value_init.4-..___tag_value_init.3
	.4byte 0x8610060c
	.2byte 0x0402
	.4byte ..___tag_value_init.6-..___tag_value_init.4
	.2byte 0x00c6
	.4byte 0x0000002c
	.4byte 0x0000004c
	.8byte ..___tag_value_check.8
	.8byte ..___tag_value_check.16-..___tag_value_check.8
	.byte 0x04
	.4byte ..___tag_value_check.10-..___tag_value_check.8
	.2byte 0x100e
	.byte 0x04
	.4byte ..___tag_value_check.11-..___tag_value_check.10
	.4byte 0x8610060c
	.2byte 0x0402
	.4byte ..___tag_value_check.15-..___tag_value_check.11
	.2byte 0x00c6
	.4byte 0x000000f4
	.4byte 0x0000007c
	.8byte ..___tag_value_main.17
	.8byte ..___tag_value_main.129-..___tag_value_main.17
	.byte 0x04
	.4byte ..___tag_value_main.19-..___tag_value_main.17
	.2byte 0x100e
	.byte 0x04
	.4byte ..___tag_value_main.20-..___tag_value_main.19
	.4byte 0x8610060c
	.2byte 0x0402
	.4byte ..___tag_value_main.23-..___tag_value_main.20
	.8byte 0x048e058d068c0783
	.2byte 0x038f
	.byte 0x04
	.4byte ..___tag_value_main.77-..___tag_value_main.23
	.2byte 0x04c3
	.4byte ..___tag_value_main.78-..___tag_value_main.77
	.2byte 0x04cc
	.4byte ..___tag_value_main.79-..___tag_value_main.78
	.2byte 0x04cd
	.4byte ..___tag_value_main.80-..___tag_value_main.79
	.2byte 0x04ce
	.4byte ..___tag_value_main.81-..___tag_value_main.80
	.2byte 0x04cf
	.4byte ..___tag_value_main.82-..___tag_value_main.81
	.2byte 0x04c6
	.4byte ..___tag_value_main.83-..___tag_value_main.82
	.8byte 0x058d068c02860783
	.4byte 0x038f048e
	.byte 0x04
	.4byte ..___tag_value_main.89-..___tag_value_main.83
	.2byte 0x04c3
	.4byte ..___tag_value_main.90-..___tag_value_main.89
	.2byte 0x04cc
	.4byte ..___tag_value_main.91-..___tag_value_main.90
	.2byte 0x04cd
	.4byte ..___tag_value_main.92-..___tag_value_main.91
	.2byte 0x04ce
	.4byte ..___tag_value_main.93-..___tag_value_main.92
	.2byte 0x04cf
	.4byte ..___tag_value_main.94-..___tag_value_main.93
	.2byte 0x04c6
	.4byte ..___tag_value_main.96-..___tag_value_main.94
	.4byte 0x0410070c
	.4byte ..___tag_value_main.97-..___tag_value_main.96
	.4byte 0x8610060c
	.2byte 0x0402
	.4byte ..___tag_value_main.100-..___tag_value_main.97
	.8byte 0x048e058d068c0783
	.2byte 0x038f
	.byte 0x04
	.4byte ..___tag_value_main.114-..___tag_value_main.100
	.2byte 0x04c3
	.4byte ..___tag_value_main.115-..___tag_value_main.114
	.2byte 0x04cc
	.4byte ..___tag_value_main.116-..___tag_value_main.115
	.2byte 0x04cd
	.4byte ..___tag_value_main.117-..___tag_value_main.116
	.2byte 0x04ce
	.4byte ..___tag_value_main.118-..___tag_value_main.117
	.2byte 0x04cf
	.4byte ..___tag_value_main.119-..___tag_value_main.118
	.2byte 0x04c6
	.4byte ..___tag_value_main.121-..___tag_value_main.119
	.4byte 0x0410070c
	.4byte ..___tag_value_main.122-..___tag_value_main.121
	.4byte 0x8610060c
	.2byte 0x0402
	.4byte ..___tag_value_main.124-..___tag_value_main.122
	.8byte 0x048e058d068c0783
	.2byte 0x038f
	.byte 0x00
# End

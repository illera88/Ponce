/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
NN_null = 0,            // Unknown Operation
NN_aaa,                 // ASCII Adjust after Addition
NN_aad,                 // ASCII Adjust AX before Division
NN_aam,                 // ASCII Adjust AX after Multiply
NN_aas,                 // ASCII Adjust AL after Subtraction
NN_adc,                 // Add with Carry
NN_add,                 // Add
NN_and,                 // Logical AND
NN_arpl,                // Adjust RPL Field of Selector
NN_bound,               // Check Array Index Against Bounds
NN_bsf,                 // Bit Scan Forward
NN_bsr,                 // Bit Scan Reverse
NN_bt,                  // Bit Test
NN_btc,                 // Bit Test and Complement
NN_btr,                 // Bit Test and Reset
NN_bts,                 // Bit Test and Set
NN_call,                // Call Procedure
NN_callfi,              // Indirect Call Far Procedure
NN_callni,              // Indirect Call Near Procedure
NN_cbw,                 // AL -> AX (with sign)
NN_cwde,                // AX -> EAX (with sign)
NN_cdqe,                // EAX -> RAX (with sign)
NN_clc,                 // Clear Carry Flag
NN_cld,                 // Clear Direction Flag
NN_cli,                 // Clear Interrupt Flag
NN_clts,                // Clear Task-Switched Flag in CR0
NN_cmc,                 // Complement Carry Flag
NN_cmp,                 // Compare Two Operands
NN_cmps,                // Compare Strings
NN_cwd,                 // AX -> DX:AX (with sign)
NN_cdq,                 // EAX -> EDX:EAX (with sign)
NN_cqo,                 // RAX -> RDX:RAX (with sign)
NN_daa,                 // Decimal Adjust AL after Addition
NN_das,                 // Decimal Adjust AL after Subtraction
NN_dec,                 // Decrement by 1
NN_div,                 // Unsigned Divide
NN_enterw,              // Make Stack Frame for Procedure Parameters
NN_enter,               // Make Stack Frame for Procedure Parameters
NN_enterd,              // Make Stack Frame for Procedure Parameters
NN_enterq,              // Make Stack Frame for Procedure Parameters
NN_hlt,                 // Halt
NN_idiv,                // Signed Divide
NN_imul,                // Signed Multiply
NN_in,                  // Input from Port
NN_inc,                 // Increment by 1
NN_ins,                 // Input Byte(s) from Port to String
NN_int,                 // Call to Interrupt Procedure
NN_into,                // Call to Interrupt Procedure if Overflow Flag = 1
NN_int3,                // Trap to Debugger
NN_iretw,               // Interrupt Return
NN_iret,                // Interrupt Return
NN_iretd,               // Interrupt Return (use32)
NN_iretq,               // Interrupt Return (use64)
NN_ja,                  // Jump if Above (CF=0 & ZF=0)
NN_jae,                 // Jump if Above or Equal (CF=0)
NN_jb,                  // Jump if Below (CF=1)
NN_jbe,                 // Jump if Below or Equal (CF=1 | ZF=1)
NN_jc,                  // Jump if Carry (CF=1)
NN_jcxz,                // Jump if CX is 0
NN_jecxz,               // Jump if ECX is 0
NN_jrcxz,               // Jump if RCX is 0
NN_je,                  // Jump if Equal (ZF=1)
NN_jg,                  // Jump if Greater (ZF=0 & SF=OF)
NN_jge,                 // Jump if Greater or Equal (SF=OF)
NN_jl,                  // Jump if Less (SF!=OF)
NN_jle,                 // Jump if Less or Equal (ZF=1 | SF!=OF)
NN_jna,                 // Jump if Not Above (CF=1 | ZF=1)
NN_jnae,                // Jump if Not Above or Equal (CF=1)
NN_jnb,                 // Jump if Not Below (CF=0)
NN_jnbe,                // Jump if Not Below or Equal (CF=0 & ZF=0)
NN_jnc,                 // Jump if Not Carry (CF=0)
NN_jne,                 // Jump if Not Equal (ZF=0)
NN_jng,                 // Jump if Not Greater (ZF=1 | SF!=OF)
NN_jnge,                // Jump if Not Greater or Equal (ZF=1)
NN_jnl,                 // Jump if Not Less (SF=OF)
NN_jnle,                // Jump if Not Less or Equal (ZF=0 & SF=OF)
NN_jno,                 // Jump if Not Overflow (OF=0)
NN_jnp,                 // Jump if Not Parity (PF=0)
NN_jns,                 // Jump if Not Sign (SF=0)
NN_jnz,                 // Jump if Not Zero (ZF=0)
NN_jo,                  // Jump if Overflow (OF=1)
NN_jp,                  // Jump if Parity (PF=1)
NN_jpe,                 // Jump if Parity Even (PF=1)
NN_jpo,                 // Jump if Parity Odd  (PF=0)
NN_js,                  // Jump if Sign (SF=1)
NN_jz,                  // Jump if Zero (ZF=1)
NN_jmp,                 // Jump
NN_jmpfi,               // Indirect Far Jump
NN_jmpni,               // Indirect Near Jump
NN_jmpshort,            // Jump Short (not used)
NN_lahf,                // Load Flags into AH Register
NN_lar,                 // Load Access Right Byte
NN_lea,                 // Load Effective Address
NN_leavew,              // High Level Procedure Exit
NN_leave,               // High Level Procedure Exit
NN_leaved,              // High Level Procedure Exit
NN_leaveq,              // High Level Procedure Exit
NN_lgdt,                // Load Global Descriptor Table Register
NN_lidt,                // Load Interrupt Descriptor Table Register
NN_lgs,                 // Load Full Pointer to GS:xx
NN_lss,                 // Load Full Pointer to SS:xx
NN_lds,                 // Load Full Pointer to DS:xx
NN_les,                 // Load Full Pointer to ES:xx
NN_lfs,                 // Load Full Pointer to FS:xx
NN_lldt,                // Load Local Descriptor Table Register
NN_lmsw,                // Load Machine Status Word
NN_lock,                // Assert LOCK# Signal Prefix
NN_lods,                // Load String
NN_loopw,               // Loop while ECX != 0
NN_loop,                // Loop while CX != 0
NN_loopd,               // Loop while ECX != 0
NN_loopq,               // Loop while RCX != 0
NN_loopwe,              // Loop while CX != 0 and ZF=1
NN_loope,               // Loop while rCX != 0 and ZF=1
NN_loopde,              // Loop while ECX != 0 and ZF=1
NN_loopqe,              // Loop while RCX != 0 and ZF=1
NN_loopwne,             // Loop while CX != 0 and ZF=0
NN_loopne,              // Loop while rCX != 0 and ZF=0
NN_loopdne,             // Loop while ECX != 0 and ZF=0
NN_loopqne,             // Loop while RCX != 0 and ZF=0
NN_lsl,                 // Load Segment Limit
NN_ltr,                 // Load Task Register
NN_mov,                 // Move Data
NN_movsp,               // Move to/from Special Registers
NN_movs,                // Move Byte(s) from String to String
NN_movsx,               // Move with Sign-Extend
NN_movzx,               // Move with Zero-Extend
NN_mul,                 // Unsigned Multiplication of AL or AX
NN_neg,                 // Two's Complement Negation
NN_nop,                 // No Operation
NN_not,                 // One's Complement Negation
NN_or,                  // Logical Inclusive OR
NN_out,                 // Output to Port
NN_outs,                // Output Byte(s) to Port
NN_pop,                 // Pop a word from the Stack
NN_popaw,               // Pop all General Registers
NN_popa,                // Pop all General Registers
NN_popad,               // Pop all General Registers (use32)
NN_popaq,               // Pop all General Registers (use64)
NN_popfw,               // Pop Stack into Flags Register
NN_popf,                // Pop Stack into Flags Register
NN_popfd,               // Pop Stack into Eflags Register
NN_popfq,               // Pop Stack into Rflags Register
NN_push,                // Push Operand onto the Stack
NN_pushaw,              // Push all General Registers
NN_pusha,               // Push all General Registers
NN_pushad,              // Push all General Registers (use32)
NN_pushaq,              // Push all General Registers (use64)
NN_pushfw,              // Push Flags Register onto the Stack
NN_pushf,               // Push Flags Register onto the Stack
NN_pushfd,              // Push Flags Register onto the Stack (use32)
NN_pushfq,              // Push Flags Register onto the Stack (use64)
NN_rcl,                 // Rotate Through Carry Left
NN_rcr,                 // Rotate Through Carry Right
NN_rol,                 // Rotate Left
NN_ror,                 // Rotate Right
NN_rep,                 // Repeat String Operation
NN_repe,                // Repeat String Operation while ZF=1
NN_repne,               // Repeat String Operation while ZF=0
NN_retn,                // Return Near from Procedure
NN_retf,                // Return Far from Procedure
NN_sahf,                // Store AH into Flags Register
NN_sal,                 // Shift Arithmetic Left
NN_sar,                 // Shift Arithmetic Right
NN_shl,                 // Shift Logical Left
NN_shr,                 // Shift Logical Right
NN_sbb,                 // Integer Subtraction with Borrow
NN_scas,                // Compare String
NN_seta,                // Set Byte if Above (CF=0 & ZF=0)
NN_setae,               // Set Byte if Above or Equal (CF=0)
NN_setb,                // Set Byte if Below (CF=1)
NN_setbe,               // Set Byte if Below or Equal (CF=1 | ZF=1)
NN_setc,                // Set Byte if Carry (CF=1)
NN_sete,                // Set Byte if Equal (ZF=1)
NN_setg,                // Set Byte if Greater (ZF=0 & SF=OF)
NN_setge,               // Set Byte if Greater or Equal (SF=OF)
NN_setl,                // Set Byte if Less (SF!=OF)
NN_setle,               // Set Byte if Less or Equal (ZF=1 | SF!=OF)
NN_setna,               // Set Byte if Not Above (CF=1 | ZF=1)
NN_setnae,              // Set Byte if Not Above or Equal (CF=1)
NN_setnb,               // Set Byte if Not Below (CF=0)
NN_setnbe,              // Set Byte if Not Below or Equal (CF=0 & ZF=0)
NN_setnc,               // Set Byte if Not Carry (CF=0)
NN_setne,               // Set Byte if Not Equal (ZF=0)
NN_setng,               // Set Byte if Not Greater (ZF=1 | SF!=OF)
NN_setnge,              // Set Byte if Not Greater or Equal (ZF=1)
NN_setnl,               // Set Byte if Not Less (SF=OF)
NN_setnle,              // Set Byte if Not Less or Equal (ZF=0 & SF=OF)
NN_setno,               // Set Byte if Not Overflow (OF=0)
NN_setnp,               // Set Byte if Not Parity (PF=0)
NN_setns,               // Set Byte if Not Sign (SF=0)
NN_setnz,               // Set Byte if Not Zero (ZF=0)
NN_seto,                // Set Byte if Overflow (OF=1)
NN_setp,                // Set Byte if Parity (PF=1)
NN_setpe,               // Set Byte if Parity Even (PF=1)
NN_setpo,               // Set Byte if Parity Odd  (PF=0)
NN_sets,                // Set Byte if Sign (SF=1)
NN_setz,                // Set Byte if Zero (ZF=1)
NN_sgdt,                // Store Global Descriptor Table Register
NN_sidt,                // Store Interrupt Descriptor Table Register
NN_shld,                // Double Precision Shift Left
NN_shrd,                // Double Precision Shift Right
NN_sldt,                // Store Local Descriptor Table Register
NN_smsw,                // Store Machine Status Word
NN_stc,                 // Set Carry Flag
NN_std,                 // Set Direction Flag
NN_sti,                 // Set Interrupt Flag
NN_stos,                // Store String
NN_str,                 // Store Task Register
NN_sub,                 // Integer Subtraction
NN_test,                // Logical Compare
NN_verr,                // Verify a Segment for Reading
NN_verw,                // Verify a Segment for Writing
NN_wait,                // Wait until BUSY# Pin is Inactive (HIGH)
NN_xchg,                // Exchange Register/Memory with Register
NN_xlat,                // Table Lookup Translation
NN_xor,                 // Logical Exclusive OR

//
//      486 instructions
//

NN_cmpxchg,             // Compare and Exchange
NN_bswap,               // Swap bits in EAX
NN_xadd,                // t<-dest; dest<-src+dest; src<-t
NN_invd,                // Invalidate Data Cache
NN_wbinvd,              // Invalidate Data Cache (write changes)
NN_invlpg,              // Invalidate TLB entry

//
//      Pentium instructions
//

NN_rdmsr,               // Read Machine Status Register
NN_wrmsr,               // Write Machine Status Register
NN_cpuid,               // Get CPU ID
NN_cmpxchg8b,           // Compare and Exchange Eight Bytes
NN_rdtsc,               // Read Time Stamp Counter
NN_rsm,                 // Resume from System Management Mode

//
//      Pentium Pro instructions
//

NN_cmova,               // Move if Above (CF=0 & ZF=0)
NN_cmovb,               // Move if Below (CF=1)
NN_cmovbe,              // Move if Below or Equal (CF=1 | ZF=1)
NN_cmovg,               // Move if Greater (ZF=0 & SF=OF)
NN_cmovge,              // Move if Greater or Equal (SF=OF)
NN_cmovl,               // Move if Less (SF!=OF)
NN_cmovle,              // Move if Less or Equal (ZF=1 | SF!=OF)
NN_cmovnb,              // Move if Not Below (CF=0)
NN_cmovno,              // Move if Not Overflow (OF=0)
NN_cmovnp,              // Move if Not Parity (PF=0)
NN_cmovns,              // Move if Not Sign (SF=0)
NN_cmovnz,              // Move if Not Zero (ZF=0)
NN_cmovo,               // Move if Overflow (OF=1)
NN_cmovp,               // Move if Parity (PF=1)
NN_cmovs,               // Move if Sign (SF=1)
NN_cmovz,               // Move if Zero (ZF=1)
NN_fcmovb,              // Floating Move if Below
NN_fcmove,              // Floating Move if Equal
NN_fcmovbe,             // Floating Move if Below or Equal
NN_fcmovu,              // Floating Move if Unordered
NN_fcmovnb,             // Floating Move if Not Below
NN_fcmovne,             // Floating Move if Not Equal
NN_fcmovnbe,            // Floating Move if Not Below or Equal
NN_fcmovnu,             // Floating Move if Not Unordered
NN_fcomi,               // FP Compare, result in EFLAGS
NN_fucomi,              // FP Unordered Compare, result in EFLAGS
NN_fcomip,              // FP Compare, result in EFLAGS, pop stack
NN_fucomip,             // FP Unordered Compare, result in EFLAGS, pop stack
NN_rdpmc,               // Read Performance Monitor Counter

//
//      FPP instructuions
//

NN_fld,                 // Load Real
NN_fst,                 // Store Real
NN_fstp,                // Store Real and Pop
NN_fxch,                // Exchange Registers
NN_fild,                // Load Integer
NN_fist,                // Store Integer
NN_fistp,               // Store Integer and Pop
NN_fbld,                // Load BCD
NN_fbstp,               // Store BCD and Pop
NN_fadd,                // Add Real
NN_faddp,               // Add Real and Pop
NN_fiadd,               // Add Integer
NN_fsub,                // Subtract Real
NN_fsubp,               // Subtract Real and Pop
NN_fisub,               // Subtract Integer
NN_fsubr,               // Subtract Real Reversed
NN_fsubrp,              // Subtract Real Reversed and Pop
NN_fisubr,              // Subtract Integer Reversed
NN_fmul,                // Multiply Real
NN_fmulp,               // Multiply Real and Pop
NN_fimul,               // Multiply Integer
NN_fdiv,                // Divide Real
NN_fdivp,               // Divide Real and Pop
NN_fidiv,               // Divide Integer
NN_fdivr,               // Divide Real Reversed
NN_fdivrp,              // Divide Real Reversed and Pop
NN_fidivr,              // Divide Integer Reversed
NN_fsqrt,               // Square Root
NN_fscale,              // Scale:  st(0) <- st(0) * 2^st(1)
NN_fprem,               // Partial Remainder
NN_frndint,             // Round to Integer
NN_fxtract,             // Extract exponent and significand
NN_fabs,                // Absolute value
NN_fchs,                // Change Sign
NN_fcom,                // Compare Real
NN_fcomp,               // Compare Real and Pop
NN_fcompp,              // Compare Real and Pop Twice
NN_ficom,               // Compare Integer
NN_ficomp,              // Compare Integer and Pop
NN_ftst,                // Test
NN_fxam,                // Examine
NN_fptan,               // Partial tangent
NN_fpatan,              // Partial arctangent
NN_f2xm1,               // 2^x - 1
NN_fyl2x,               // Y * lg2(X)
NN_fyl2xp1,             // Y * lg2(X+1)
NN_fldz,                // Load +0.0
NN_fld1,                // Load +1.0
NN_fldpi,               // Load PI=3.14...
NN_fldl2t,              // Load lg2(10)
NN_fldl2e,              // Load lg2(e)
NN_fldlg2,              // Load lg10(2)
NN_fldln2,              // Load ln(2)
NN_finit,               // Initialize Processor
NN_fninit,              // Initialize Processor (no wait)
NN_fsetpm,              // Set Protected Mode
NN_fldcw,               // Load Control Word
NN_fstcw,               // Store Control Word
NN_fnstcw,              // Store Control Word (no wait)
NN_fstsw,               // Store Status Word
NN_fnstsw,              // Store Status Word (no wait)
NN_fclex,               // Clear Exceptions
NN_fnclex,              // Clear Exceptions (no wait)
NN_fstenv,              // Store Environment
NN_fnstenv,             // Store Environment (no wait)
NN_fldenv,              // Load Environment
NN_fsave,               // Save State
NN_fnsave,              // Save State (no wait)
NN_frstor,              // Restore State
NN_fincstp,             // Increment Stack Pointer
NN_fdecstp,             // Decrement Stack Pointer
NN_ffree,               // Free Register
NN_fnop,                // No Operation
NN_feni,                // (8087 only)
NN_fneni,               // (no wait) (8087 only)
NN_fdisi,               // (8087 only)
NN_fndisi,              // (no wait) (8087 only)

//
//      80387 instructions
//

NN_fprem1,              // Partial Remainder ( < half )
NN_fsincos,             // t<-cos(st); st<-sin(st); push t
NN_fsin,                // Sine
NN_fcos,                // Cosine
NN_fucom,               // Compare Unordered Real
NN_fucomp,              // Compare Unordered Real and Pop
NN_fucompp,             // Compare Unordered Real and Pop Twice

//
//      Instructions added 28.02.96
//

NN_setalc,              // Set AL to Carry Flag
NN_svdc,                // Save Register and Descriptor
NN_rsdc,                // Restore Register and Descriptor
NN_svldt,               // Save LDTR and Descriptor
NN_rsldt,               // Restore LDTR and Descriptor
NN_svts,                // Save TR and Descriptor
NN_rsts,                // Restore TR and Descriptor
NN_icebp,               // ICE Break Point
NN_loadall,             // Load the entire CPU state from ES:EDI

//
//      MMX instructions
//

NN_emms,                // Empty MMX state
NN_movd,                // Move 32 bits
NN_movq,                // Move 64 bits
NN_packsswb,            // Pack with Signed Saturation (Word->Byte)
NN_packssdw,            // Pack with Signed Saturation (Dword->Word)
NN_packuswb,            // Pack with Unsigned Saturation (Word->Byte)
NN_paddb,               // Packed Add Byte
NN_paddw,               // Packed Add Word
NN_paddd,               // Packed Add Dword
NN_paddsb,              // Packed Add with Saturation (Byte)
NN_paddsw,              // Packed Add with Saturation (Word)
NN_paddusb,             // Packed Add Unsigned with Saturation (Byte)
NN_paddusw,             // Packed Add Unsigned with Saturation (Word)
NN_pand,                // Bitwise Logical And
NN_pandn,               // Bitwise Logical And Not
NN_pcmpeqb,             // Packed Compare for Equal (Byte)
NN_pcmpeqw,             // Packed Compare for Equal (Word)
NN_pcmpeqd,             // Packed Compare for Equal (Dword)
NN_pcmpgtb,             // Packed Compare for Greater Than (Byte)
NN_pcmpgtw,             // Packed Compare for Greater Than (Word)
NN_pcmpgtd,             // Packed Compare for Greater Than (Dword)
NN_pmaddwd,             // Packed Multiply and Add
NN_pmulhw,              // Packed Multiply High
NN_pmullw,              // Packed Multiply Low
NN_por,                 // Bitwise Logical Or
NN_psllw,               // Packed Shift Left Logical (Word)
NN_pslld,               // Packed Shift Left Logical (Dword)
NN_psllq,               // Packed Shift Left Logical (Qword)
NN_psraw,               // Packed Shift Right Arithmetic (Word)
NN_psrad,               // Packed Shift Right Arithmetic (Dword)
NN_psrlw,               // Packed Shift Right Logical (Word)
NN_psrld,               // Packed Shift Right Logical (Dword)
NN_psrlq,               // Packed Shift Right Logical (Qword)
NN_psubb,               // Packed Subtract Byte
NN_psubw,               // Packed Subtract Word
NN_psubd,               // Packed Subtract Dword
NN_psubsb,              // Packed Subtract with Saturation (Byte)
NN_psubsw,              // Packed Subtract with Saturation (Word)
NN_psubusb,             // Packed Subtract Unsigned with Saturation (Byte)
NN_psubusw,             // Packed Subtract Unsigned with Saturation (Word)
NN_punpckhbw,           // Unpack High Packed Data (Byte->Word)
NN_punpckhwd,           // Unpack High Packed Data (Word->Dword)
NN_punpckhdq,           // Unpack High Packed Data (Dword->Qword)
NN_punpcklbw,           // Unpack Low Packed Data (Byte->Word)
NN_punpcklwd,           // Unpack Low Packed Data (Word->Dword)
NN_punpckldq,           // Unpack Low Packed Data (Dword->Qword)
NN_pxor,                // Bitwise Logical Exclusive Or

//
//      Undocumented Deschutes processor instructions
//

NN_fxsave,              // Fast save FP context
NN_fxrstor,             // Fast restore FP context

//      Pentium II instructions

NN_sysenter,            // Fast Transition to System Call Entry Point
NN_sysexit,             // Fast Transition from System Call Entry Point

//      3DNow! instructions

NN_pavgusb,             // Packed 8-bit Unsigned Integer Averaging
NN_pfadd,               // Packed Floating-Point Addition
NN_pfsub,               // Packed Floating-Point Subtraction
NN_pfsubr,              // Packed Floating-Point Reverse Subtraction
NN_pfacc,               // Packed Floating-Point Accumulate
NN_pfcmpge,             // Packed Floating-Point Comparison, Greater or Equal
NN_pfcmpgt,             // Packed Floating-Point Comparison, Greater
NN_pfcmpeq,             // Packed Floating-Point Comparison, Equal
NN_pfmin,               // Packed Floating-Point Minimum
NN_pfmax,               // Packed Floating-Point Maximum
NN_pi2fd,               // Packed 32-bit Integer to Floating-Point
NN_pf2id,               // Packed Floating-Point to 32-bit Integer
NN_pfrcp,               // Packed Floating-Point Reciprocal Approximation
NN_pfrsqrt,             // Packed Floating-Point Reciprocal Square Root Approximation
NN_pfmul,               // Packed Floating-Point Multiplication
NN_pfrcpit1,            // Packed Floating-Point Reciprocal First Iteration Step
NN_pfrsqit1,            // Packed Floating-Point Reciprocal Square Root First Iteration Step
NN_pfrcpit2,            // Packed Floating-Point Reciprocal Second Iteration Step
NN_pmulhrw,             // Packed Floating-Point 16-bit Integer Multiply with rounding
NN_femms,               // Faster entry/exit of the MMX or floating-point state
NN_prefetch,            // Prefetch at least a 32-byte line into L1 data cache
NN_prefetchw,           // Prefetch processor cache line into L1 data cache (mark as modified)


//      Pentium III instructions

NN_addps,               // Packed Single-FP Add
NN_addss,               // Scalar Single-FP Add
NN_andnps,              // Bitwise Logical And Not for Single-FP
NN_andps,               // Bitwise Logical And for Single-FP
NN_cmpps,               // Packed Single-FP Compare
NN_cmpss,               // Scalar Single-FP Compare
NN_comiss,              // Scalar Ordered Single-FP Compare and Set EFLAGS
NN_cvtpi2ps,            // Packed signed INT32 to Packed Single-FP conversion
NN_cvtps2pi,            // Packed Single-FP to Packed INT32 conversion
NN_cvtsi2ss,            // Scalar signed INT32 to Single-FP conversion
NN_cvtss2si,            // Scalar Single-FP to signed INT32 conversion
NN_cvttps2pi,           // Packed Single-FP to Packed INT32 conversion (truncate)
NN_cvttss2si,           // Scalar Single-FP to signed INT32 conversion (truncate)
NN_divps,               // Packed Single-FP Divide
NN_divss,               // Scalar Single-FP Divide
NN_ldmxcsr,             // Load Streaming SIMD Extensions Technology Control/Status Register
NN_maxps,               // Packed Single-FP Maximum
NN_maxss,               // Scalar Single-FP Maximum
NN_minps,               // Packed Single-FP Minimum
NN_minss,               // Scalar Single-FP Minimum
NN_movaps,              // Move Aligned Four Packed Single-FP
NN_movhlps,             // Move High to Low Packed Single-FP
NN_movhps,              // Move High Packed Single-FP
NN_movlhps,             // Move Low to High Packed Single-FP
NN_movlps,              // Move Low Packed Single-FP
NN_movmskps,            // Move Mask to Register
NN_movss,               // Move Scalar Single-FP
NN_movups,              // Move Unaligned Four Packed Single-FP
NN_mulps,               // Packed Single-FP Multiply
NN_mulss,               // Scalar Single-FP Multiply
NN_orps,                // Bitwise Logical OR for Single-FP Data
NN_rcpps,               // Packed Single-FP Reciprocal
NN_rcpss,               // Scalar Single-FP Reciprocal
NN_rsqrtps,             // Packed Single-FP Square Root Reciprocal
NN_rsqrtss,             // Scalar Single-FP Square Root Reciprocal
NN_shufps,              // Shuffle Single-FP
NN_sqrtps,              // Packed Single-FP Square Root
NN_sqrtss,              // Scalar Single-FP Square Root
NN_stmxcsr,             // Store Streaming SIMD Extensions Technology Control/Status Register
NN_subps,               // Packed Single-FP Subtract
NN_subss,               // Scalar Single-FP Subtract
NN_ucomiss,             // Scalar Unordered Single-FP Compare and Set EFLAGS
NN_unpckhps,            // Unpack High Packed Single-FP Data
NN_unpcklps,            // Unpack Low Packed Single-FP Data
NN_xorps,               // Bitwise Logical XOR for Single-FP Data
NN_pavgb,               // Packed Average (Byte)
NN_pavgw,               // Packed Average (Word)
NN_pextrw,              // Extract Word
NN_pinsrw,              // Insert Word
NN_pmaxsw,              // Packed Signed Integer Word Maximum
NN_pmaxub,              // Packed Unsigned Integer Byte Maximum
NN_pminsw,              // Packed Signed Integer Word Minimum
NN_pminub,              // Packed Unsigned Integer Byte Minimum
NN_pmovmskb,            // Move Byte Mask to Integer
NN_pmulhuw,             // Packed Multiply High Unsigned
NN_psadbw,              // Packed Sum of Absolute Differences
NN_pshufw,              // Packed Shuffle Word
NN_maskmovq,            // Byte Mask write
NN_movntps,             // Move Aligned Four Packed Single-FP Non Temporal
NN_movntq,              // Move 64 Bits Non Temporal
NN_prefetcht0,          // Prefetch to all cache levels
NN_prefetcht1,          // Prefetch to all cache levels
NN_prefetcht2,          // Prefetch to L2 cache
NN_prefetchnta,         // Prefetch to L1 cache
NN_sfence,              // Store Fence

// Pentium III Pseudo instructions

NN_cmpeqps,             // Packed Single-FP Compare EQ
NN_cmpltps,             // Packed Single-FP Compare LT
NN_cmpleps,             // Packed Single-FP Compare LE
NN_cmpunordps,          // Packed Single-FP Compare UNORD
NN_cmpneqps,            // Packed Single-FP Compare NOT EQ
NN_cmpnltps,            // Packed Single-FP Compare NOT LT
NN_cmpnleps,            // Packed Single-FP Compare NOT LE
NN_cmpordps,            // Packed Single-FP Compare ORDERED
NN_cmpeqss,             // Scalar Single-FP Compare EQ
NN_cmpltss,             // Scalar Single-FP Compare LT
NN_cmpless,             // Scalar Single-FP Compare LE
NN_cmpunordss,          // Scalar Single-FP Compare UNORD
NN_cmpneqss,            // Scalar Single-FP Compare NOT EQ
NN_cmpnltss,            // Scalar Single-FP Compare NOT LT
NN_cmpnless,            // Scalar Single-FP Compare NOT LE
NN_cmpordss,            // Scalar Single-FP Compare ORDERED

// AMD K7 instructions

NN_pf2iw,               // Packed Floating-Point to Integer with Sign Extend
NN_pfnacc,              // Packed Floating-Point Negative Accumulate
NN_pfpnacc,             // Packed Floating-Point Mixed Positive-Negative Accumulate
NN_pi2fw,               // Packed 16-bit Integer to Floating-Point
NN_pswapd,              // Packed Swap Double Word

// Undocumented FP instructions (thanks to norbert.juffa@adm.com)

NN_fstp1,               // Alias of Store Real and Pop
NN_fcom2,               // Alias of Compare Real
NN_fcomp3,              // Alias of Compare Real and Pop
NN_fxch4,               // Alias of Exchange Registers
NN_fcomp5,              // Alias of Compare Real and Pop
NN_ffreep,              // Free Register and Pop
NN_fxch7,               // Alias of Exchange Registers
NN_fstp8,               // Alias of Store Real and Pop
NN_fstp9,               // Alias of Store Real and Pop

// Pentium 4 instructions

NN_addpd,               // Add Packed Double-Precision Floating-Point Values
NN_addsd,               // Add Scalar Double-Precision Floating-Point Values
NN_andnpd,              // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
NN_andpd,               // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
NN_clflush,             // Flush Cache Line
NN_cmppd,               // Compare Packed Double-Precision Floating-Point Values
NN_cmpsd,               // Compare Scalar Double-Precision Floating-Point Values
NN_comisd,              // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
NN_cvtdq2pd,            // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
NN_cvtdq2ps,            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
NN_cvtpd2dq,            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
NN_cvtpd2pi,            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
NN_cvtpd2ps,            // Convert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
NN_cvtpi2pd,            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
NN_cvtps2dq,            // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
NN_cvtps2pd,            // Convert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
NN_cvtsd2si,            // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
NN_cvtsd2ss,            // Convert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
NN_cvtsi2sd,            // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
NN_cvtss2sd,            // Convert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
NN_cvttpd2dq,           // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
NN_cvttpd2pi,           // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
NN_cvttps2dq,           // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
NN_cvttsd2si,           // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
NN_divpd,               // Divide Packed Double-Precision Floating-Point Values
NN_divsd,               // Divide Scalar Double-Precision Floating-Point Values
NN_lfence,              // Load Fence
NN_maskmovdqu,          // Store Selected Bytes of Double Quadword
NN_maxpd,               // Return Maximum Packed Double-Precision Floating-Point Values
NN_maxsd,               // Return Maximum Scalar Double-Precision Floating-Point Value
NN_mfence,              // Memory Fence
NN_minpd,               // Return Minimum Packed Double-Precision Floating-Point Values
NN_minsd,               // Return Minimum Scalar Double-Precision Floating-Point Value
NN_movapd,              // Move Aligned Packed Double-Precision Floating-Point Values
NN_movdq2q,             // Move Quadword from XMM to MMX Register
NN_movdqa,              // Move Aligned Double Quadword
NN_movdqu,              // Move Unaligned Double Quadword
NN_movhpd,              // Move High Packed Double-Precision Floating-Point Values
NN_movlpd,              // Move Low Packed Double-Precision Floating-Point Values
NN_movmskpd,            // Extract Packed Double-Precision Floating-Point Sign Mask
NN_movntdq,             // Store Double Quadword Using Non-Temporal Hint
NN_movnti,              // Store Doubleword Using Non-Temporal Hint
NN_movntpd,             // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
NN_movq2dq,             // Move Quadword from MMX to XMM Register
NN_movsd,               // Move Scalar Double-Precision Floating-Point Values
NN_movupd,              // Move Unaligned Packed Double-Precision Floating-Point Values
NN_mulpd,               // Multiply Packed Double-Precision Floating-Point Values
NN_mulsd,               // Multiply Scalar Double-Precision Floating-Point Values
NN_orpd,                // Bitwise Logical OR of Double-Precision Floating-Point Values
NN_paddq,               // Add Packed Quadword Integers
NN_pause,               // Spin Loop Hint
NN_pmuludq,             // Multiply Packed Unsigned Doubleword Integers
NN_pshufd,              // Shuffle Packed Doublewords
NN_pshufhw,             // Shuffle Packed High Words
NN_pshuflw,             // Shuffle Packed Low Words
NN_pslldq,              // Shift Double Quadword Left Logical
NN_psrldq,              // Shift Double Quadword Right Logical
NN_psubq,               // Subtract Packed Quadword Integers
NN_punpckhqdq,          // Unpack High Data
NN_punpcklqdq,          // Unpack Low Data
NN_shufpd,              // Shuffle Packed Double-Precision Floating-Point Values
NN_sqrtpd,              // Compute Square Roots of Packed Double-Precision Floating-Point Values
NN_sqrtsd,              // Compute Square Rootof Scalar Double-Precision Floating-Point Value
NN_subpd,               // Subtract Packed Double-Precision Floating-Point Values
NN_subsd,               // Subtract Scalar Double-Precision Floating-Point Values
NN_ucomisd,             // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
NN_unpckhpd,            // Unpack and Interleave High Packed Double-Precision Floating-Point Values
NN_unpcklpd,            // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
NN_xorpd,               // Bitwise Logical OR of Double-Precision Floating-Point Values

// AMD syscall/sysret instructions

NN_syscall,             // Low latency system call
NN_sysret,              // Return from system call

// AMD64 instructions

NN_swapgs,              // Exchange GS base with KernelGSBase MSR

// New Pentium instructions (SSE3)

NN_movddup,             // Move One Double-FP and Duplicate
NN_movshdup,            // Move Packed Single-FP High and Duplicate
NN_movsldup,            // Move Packed Single-FP Low and Duplicate

// Missing AMD64 instructions

NN_movsxd,              // Move with Sign-Extend Doubleword
NN_cmpxchg16b,          // Compare and Exchange 16 Bytes

// SSE3 instructions

NN_addsubpd,            // Add /Sub packed DP FP numbers
NN_addsubps,            // Add /Sub packed SP FP numbers
NN_haddpd,              // Add horizontally packed DP FP numbers
NN_haddps,              // Add horizontally packed SP FP numbers
NN_hsubpd,              // Sub horizontally packed DP FP numbers
NN_hsubps,              // Sub horizontally packed SP FP numbers
NN_monitor,             // Set up a linear address range to be monitored by hardware
NN_mwait,               // Wait until write-back store performed within the range specified by the MONITOR instruction
NN_fisttp,              // Store ST in intXX (chop) and pop
NN_lddqu,               // Load unaligned integer 128-bit

// SSSE3 instructions

NN_psignb,              // Packed SIGN Byte
NN_psignw,              // Packed SIGN Word
NN_psignd,              // Packed SIGN Doubleword
NN_pshufb,              // Packed Shuffle Bytes
NN_pmulhrsw,            // Packed Multiply High with Round and Scale
NN_pmaddubsw,           // Multiply and Add Packed Signed and Unsigned Bytes
NN_phsubsw,             // Packed Horizontal Subtract and Saturate
NN_phaddsw,             // Packed Horizontal Add and Saturate
NN_phaddw,              // Packed Horizontal Add Word
NN_phaddd,              // Packed Horizontal Add Doubleword
NN_phsubw,              // Packed Horizontal Subtract Word
NN_phsubd,              // Packed Horizontal Subtract Doubleword
NN_palignr,             // Packed Align Right
NN_pabsb,               // Packed Absolute Value Byte
NN_pabsw,               // Packed Absolute Value Word
NN_pabsd,               // Packed Absolute Value Doubleword

// VMX instructions

NN_vmcall,              // Call to VM Monitor
NN_vmclear,             // Clear Virtual Machine Control Structure
NN_vmlaunch,            // Launch Virtual Machine
NN_vmresume,            // Resume Virtual Machine
NN_vmptrld,             // Load Pointer to Virtual Machine Control Structure
NN_vmptrst,             // Store Pointer to Virtual Machine Control Structure
NN_vmread,              // Read Field from Virtual Machine Control Structure
NN_vmwrite,             // Write Field from Virtual Machine Control Structure
NN_vmxoff,              // Leave VMX Operation
NN_vmxon,               // Enter VMX Operation

// Undefined Instruction

NN_ud2,                 // Undefined Instruction

// Added with x86-64

NN_rdtscp,              // Read Time-Stamp Counter and Processor ID

// Geode LX 3DNow! extensions

NN_pfrcpv,              // Reciprocal Approximation for a Pair of 32-bit Floats
NN_pfrsqrtv,            // Reciprocal Square Root Approximation for a Pair of 32-bit Floats

// SSE2 pseudoinstructions

NN_cmpeqpd,             // Packed Double-FP Compare EQ
NN_cmpltpd,             // Packed Double-FP Compare LT
NN_cmplepd,             // Packed Double-FP Compare LE
NN_cmpunordpd,          // Packed Double-FP Compare UNORD
NN_cmpneqpd,            // Packed Double-FP Compare NOT EQ
NN_cmpnltpd,            // Packed Double-FP Compare NOT LT
NN_cmpnlepd,            // Packed Double-FP Compare NOT LE
NN_cmpordpd,            // Packed Double-FP Compare ORDERED
NN_cmpeqsd,             // Scalar Double-FP Compare EQ
NN_cmpltsd,             // Scalar Double-FP Compare LT
NN_cmplesd,             // Scalar Double-FP Compare LE
NN_cmpunordsd,          // Scalar Double-FP Compare UNORD
NN_cmpneqsd,            // Scalar Double-FP Compare NOT EQ
NN_cmpnltsd,            // Scalar Double-FP Compare NOT LT
NN_cmpnlesd,            // Scalar Double-FP Compare NOT LE
NN_cmpordsd,            // Scalar Double-FP Compare ORDERED

// SSSE4.1 instructions

NN_blendpd,              // Blend Packed Double Precision Floating-Point Values
NN_blendps,              // Blend Packed Single Precision Floating-Point Values
NN_blendvpd,             // Variable Blend Packed Double Precision Floating-Point Values
NN_blendvps,             // Variable Blend Packed Single Precision Floating-Point Values
NN_dppd,                 // Dot Product of Packed Double Precision Floating-Point Values
NN_dpps,                 // Dot Product of Packed Single Precision Floating-Point Values
NN_extractps,            // Extract Packed Single Precision Floating-Point Value
NN_insertps,             // Insert Packed Single Precision Floating-Point Value
NN_movntdqa,             // Load Double Quadword Non-Temporal Aligned Hint
NN_mpsadbw,              // Compute Multiple Packed Sums of Absolute Difference
NN_packusdw,             // Pack with Unsigned Saturation
NN_pblendvb,             // Variable Blend Packed Bytes
NN_pblendw,              // Blend Packed Words
NN_pcmpeqq,              // Compare Packed Qword Data for Equal
NN_pextrb,               // Extract Byte
NN_pextrd,               // Extract Dword
NN_pextrq,               // Extract Qword
NN_phminposuw,           // Packed Horizontal Word Minimum
NN_pinsrb,               // Insert Byte
NN_pinsrd,               // Insert Dword
NN_pinsrq,               // Insert Qword
NN_pmaxsb,               // Maximum of Packed Signed Byte Integers
NN_pmaxsd,               // Maximum of Packed Signed Dword Integers
NN_pmaxud,               // Maximum of Packed Unsigned Dword Integers
NN_pmaxuw,               // Maximum of Packed Word Integers
NN_pminsb,               // Minimum of Packed Signed Byte Integers
NN_pminsd,               // Minimum of Packed Signed Dword Integers
NN_pminud,               // Minimum of Packed Unsigned Dword Integers
NN_pminuw,               // Minimum of Packed Word Integers
NN_pmovsxbw,             // Packed Move with Sign Extend
NN_pmovsxbd,             // Packed Move with Sign Extend
NN_pmovsxbq,             // Packed Move with Sign Extend
NN_pmovsxwd,             // Packed Move with Sign Extend
NN_pmovsxwq,             // Packed Move with Sign Extend
NN_pmovsxdq,             // Packed Move with Sign Extend
NN_pmovzxbw,             // Packed Move with Zero Extend
NN_pmovzxbd,             // Packed Move with Zero Extend
NN_pmovzxbq,             // Packed Move with Zero Extend
NN_pmovzxwd,             // Packed Move with Zero Extend
NN_pmovzxwq,             // Packed Move with Zero Extend
NN_pmovzxdq,             // Packed Move with Zero Extend
NN_pmuldq,               // Multiply Packed Signed Dword Integers
NN_pmulld,               // Multiply Packed Signed Dword Integers and Store Low Result
NN_ptest,                // Logical Compare
NN_roundpd,              // Round Packed Double Precision Floating-Point Values
NN_roundps,              // Round Packed Single Precision Floating-Point Values
NN_roundsd,              // Round Scalar Double Precision Floating-Point Values
NN_roundss,              // Round Scalar Single Precision Floating-Point Values

// SSSE4.2 instructions

NN_crc32,                // Accumulate CRC32 Value
NN_pcmpestri,            // Packed Compare Explicit Length Strings, Return Index
NN_pcmpestrm,            // Packed Compare Explicit Length Strings, Return Mask
NN_pcmpistri,            // Packed Compare Implicit Length Strings, Return Index
NN_pcmpistrm,            // Packed Compare Implicit Length Strings, Return Mask
NN_pcmpgtq,              // Compare Packed Data for Greater Than
NN_popcnt,               // Return the Count of Number of Bits Set to 1

// AMD SSE4a instructions

NN_extrq,                // Extract Field From Register
NN_insertq,              // Insert Field
NN_movntsd,              // Move Non-Temporal Scalar Double-Precision Floating-Point
NN_movntss,              // Move Non-Temporal Scalar Single-Precision Floating-Point
NN_lzcnt,                // Leading Zero Count

// xsave/xrstor instructions

NN_xgetbv,               // Get Value of Extended Control Register
NN_xrstor,               // Restore Processor Extended States
NN_xsave,                // Save Processor Extended States
NN_xsetbv,               // Set Value of Extended Control Register

// Intel Safer Mode Extensions (SMX)

NN_getsec,               // Safer Mode Extensions (SMX) Instruction

// AMD-V Virtualization ISA Extension

NN_clgi,                 // Clear Global Interrupt Flag
NN_invlpga,              // Invalidate TLB Entry in a Specified ASID
NN_skinit,               // Secure Init and Jump with Attestation
NN_stgi,                 // Set Global Interrupt Flag
NN_vmexit,               // Stop Executing Guest, Begin Executing Host
NN_vmload,               // Load State from VMCB
NN_vmmcall,              // Call VMM
NN_vmrun,                // Run Virtual Machine
NN_vmsave,               // Save State to VMCB

// VMX+ instructions

NN_invept,               // Invalidate Translations Derived from EPT
NN_invvpid,              // Invalidate Translations Based on VPID

// Intel Atom instructions

NN_movbe,                // Move Data After Swapping Bytes

// Intel AES instructions

NN_aesenc,                // Perform One Round of an AES Encryption Flow
NN_aesenclast,            // Perform the Last Round of an AES Encryption Flow
NN_aesdec,                // Perform One Round of an AES Decryption Flow
NN_aesdeclast,            // Perform the Last Round of an AES Decryption Flow
NN_aesimc,                // Perform the AES InvMixColumn Transformation
NN_aeskeygenassist,       // AES Round Key Generation Assist

// Carryless multiplication

NN_pclmulqdq,            // Carry-Less Multiplication Quadword

// Returns modifies by operand size prefixes

NN_retnw,               // Return Near from Procedure (use16)
NN_retnd,               // Return Near from Procedure (use32)
NN_retnq,               // Return Near from Procedure (use64)
NN_retfw,               // Return Far from Procedure (use16)
NN_retfd,               // Return Far from Procedure (use32)
NN_retfq,               // Return Far from Procedure (use64)

// RDRAND support

NN_rdrand,              // Read Random Number

// new GPR instructions

NN_adcx,                 // Unsigned Integer Addition of Two Operands with Carry Flag
NN_adox,                 // Unsigned Integer Addition of Two Operands with Overflow Flag
NN_andn,                 // Logical AND NOT
NN_bextr,                // Bit Field Extract
NN_blsi,                 // Extract Lowest Set Isolated Bit
NN_blsmsk,               // Get Mask Up to Lowest Set Bit
NN_blsr,                 // Reset Lowest Set Bit
NN_bzhi,                 // Zero High Bits Starting with Specified Bit Position
NN_clac,                 // Clear AC Flag in EFLAGS Register
NN_mulx,                 // Unsigned Multiply Without Affecting Flags
NN_pdep,                 // Parallel Bits Deposit
NN_pext,                 // Parallel Bits Extract
NN_rorx,                 // Rotate Right Logical Without Affecting Flags
NN_sarx,                 // Shift Arithmetically Right Without Affecting Flags
NN_shlx,                 // Shift Logically Left Without Affecting Flags
NN_shrx,                 // Shift Logically Right Without Affecting Flags
NN_stac,                 // Set AC Flag in EFLAGS Register
NN_tzcnt,                // Count the Number of Trailing Zero Bits
NN_xsaveopt,             // Save Processor Extended States Optimized
NN_invpcid,              // Invalidate Processor Context ID
NN_rdseed,               // Read Random Seed
NN_rdfsbase,             // Read FS Segment Base
NN_rdgsbase,             // Read GS Segment Base
NN_wrfsbase,             // Write FS Segment Base
NN_wrgsbase,             // Write GS Segment Base

// new AVX instructions
NN_vaddpd,               // Add Packed Double-Precision Floating-Point Values
NN_vaddps,               // Packed Single-FP Add
NN_vaddsd,               // Add Scalar Double-Precision Floating-Point Values
NN_vaddss,               // Scalar Single-FP Add
NN_vaddsubpd,            // Add /Sub packed DP FP numbers
NN_vaddsubps,            // Add /Sub packed SP FP numbers
NN_vaesdec,              // Perform One Round of an AES Decryption Flow
NN_vaesdeclast,          // Perform the Last Round of an AES Decryption Flow
NN_vaesenc,              // Perform One Round of an AES Encryption Flow
NN_vaesenclast,          // Perform the Last Round of an AES Encryption Flow
NN_vaesimc,              // Perform the AES InvMixColumn Transformation
NN_vaeskeygenassist,     // AES Round Key Generation Assist
NN_vandnpd,              // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
NN_vandnps,              // Bitwise Logical And Not for Single-FP
NN_vandpd,               // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
NN_vandps,               // Bitwise Logical And for Single-FP
NN_vblendpd,             // Blend Packed Double Precision Floating-Point Values
NN_vblendps,             // Blend Packed Single Precision Floating-Point Values
NN_vblendvpd,            // Variable Blend Packed Double Precision Floating-Point Values
NN_vblendvps,            // Variable Blend Packed Single Precision Floating-Point Values
NN_vbroadcastf128,       // Broadcast 128 Bits of Floating-Point Data
NN_vbroadcasti128,       // Broadcast 128 Bits of Integer Data
NN_vbroadcastsd,         // Broadcast Double-Precision Floating-Point Element
NN_vbroadcastss,         // Broadcast Single-Precision Floating-Point Element
NN_vcmppd,               // Compare Packed Double-Precision Floating-Point Values
NN_vcmpps,               // Packed Single-FP Compare
NN_vcmpsd,               // Compare Scalar Double-Precision Floating-Point Values
NN_vcmpss,               // Scalar Single-FP Compare
NN_vcomisd,              // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
NN_vcomiss,              // Scalar Ordered Single-FP Compare and Set EFLAGS
NN_vcvtdq2pd,            // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
NN_vcvtdq2ps,            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
NN_vcvtpd2dq,            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
NN_vcvtpd2ps,            // Convert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
NN_vcvtph2ps,            // Convert 16-bit FP Values to Single-Precision FP Values
NN_vcvtps2dq,            // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
NN_vcvtps2pd,            // Convert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
NN_vcvtps2ph,            // Convert Single-Precision FP value to 16-bit FP value
NN_vcvtsd2si,            // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
NN_vcvtsd2ss,            // Convert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
NN_vcvtsi2sd,            // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
NN_vcvtsi2ss,            // Scalar signed INT32 to Single-FP conversion
NN_vcvtss2sd,            // Convert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
NN_vcvtss2si,            // Scalar Single-FP to signed INT32 conversion
NN_vcvttpd2dq,           // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
NN_vcvttps2dq,           // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
NN_vcvttsd2si,           // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
NN_vcvttss2si,           // Scalar Single-FP to signed INT32 conversion (truncate)
NN_vdivpd,               // Divide Packed Double-Precision Floating-Point Values
NN_vdivps,               // Packed Single-FP Divide
NN_vdivsd,               // Divide Scalar Double-Precision Floating-Point Values
NN_vdivss,               // Scalar Single-FP Divide
NN_vdppd,                // Dot Product of Packed Double Precision Floating-Point Values
NN_vdpps,                // Dot Product of Packed Single Precision Floating-Point Values
NN_vextractf128,         // Extract Packed Floating-Point Values
NN_vextracti128,         // Extract Packed Integer Values
NN_vextractps,           // Extract Packed Floating-Point Values
NN_vfmadd132pd,          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
NN_vfmadd132ps,          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
NN_vfmadd132sd,          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
NN_vfmadd132ss,          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
NN_vfmadd213pd,          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
NN_vfmadd213ps,          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
NN_vfmadd213sd,          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
NN_vfmadd213ss,          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
NN_vfmadd231pd,          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
NN_vfmadd231ps,          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
NN_vfmadd231sd,          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
NN_vfmadd231ss,          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
NN_vfmaddsub132pd,       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
NN_vfmaddsub132ps,       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
NN_vfmaddsub213pd,       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
NN_vfmaddsub213ps,       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
NN_vfmaddsub231pd,       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
NN_vfmaddsub231ps,       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
NN_vfmsub132pd,          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
NN_vfmsub132ps,          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
NN_vfmsub132sd,          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
NN_vfmsub132ss,          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
NN_vfmsub213pd,          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
NN_vfmsub213ps,          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
NN_vfmsub213sd,          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
NN_vfmsub213ss,          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
NN_vfmsub231pd,          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
NN_vfmsub231ps,          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
NN_vfmsub231sd,          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
NN_vfmsub231ss,          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
NN_vfmsubadd132pd,       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
NN_vfmsubadd132ps,       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
NN_vfmsubadd213pd,       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
NN_vfmsubadd213ps,       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
NN_vfmsubadd231pd,       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
NN_vfmsubadd231ps,       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
NN_vfnmadd132pd,         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
NN_vfnmadd132ps,         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
NN_vfnmadd132sd,         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
NN_vfnmadd132ss,         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
NN_vfnmadd213pd,         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
NN_vfnmadd213ps,         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
NN_vfnmadd213sd,         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
NN_vfnmadd213ss,         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
NN_vfnmadd231pd,         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
NN_vfnmadd231ps,         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
NN_vfnmadd231sd,         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
NN_vfnmadd231ss,         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
NN_vfnmsub132pd,         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
NN_vfnmsub132ps,         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
NN_vfnmsub132sd,         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
NN_vfnmsub132ss,         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
NN_vfnmsub213pd,         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
NN_vfnmsub213ps,         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
NN_vfnmsub213sd,         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
NN_vfnmsub213ss,         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
NN_vfnmsub231pd,         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
NN_vfnmsub231ps,         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
NN_vfnmsub231sd,         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
NN_vfnmsub231ss,         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
NN_vgatherdps,           // Gather Packed SP FP Values Using Signed Dword Indices
NN_vgatherdpd,           // Gather Packed DP FP Values Using Signed Dword Indices
NN_vgatherqps,           // Gather Packed SP FP Values Using Signed Qword Indices
NN_vgatherqpd,           // Gather Packed DP FP Values Using Signed Qword Indices
NN_vhaddpd,              // Add horizontally packed DP FP numbers
NN_vhaddps,              // Add horizontally packed SP FP numbers
NN_vhsubpd,              // Sub horizontally packed DP FP numbers
NN_vhsubps,              // Sub horizontally packed SP FP numbers
NN_vinsertf128,          // Insert Packed Floating-Point Values
NN_vinserti128,          // Insert Packed Integer Values
NN_vinsertps,            // Insert Packed Single Precision Floating-Point Value
NN_vlddqu,               // Load Unaligned Packed Integer Values
NN_vldmxcsr,             // Load Streaming SIMD Extensions Technology Control/Status Register
NN_vmaskmovdqu,          // Store Selected Bytes of Double Quadword with NT Hint
NN_vmaskmovpd,           // Conditionally Load Packed Double-Precision Floating-Point Values
NN_vmaskmovps,           // Conditionally Load Packed Single-Precision Floating-Point Values
NN_vmaxpd,               // Return Maximum Packed Double-Precision Floating-Point Values
NN_vmaxps,               // Packed Single-FP Maximum
NN_vmaxsd,               // Return Maximum Scalar Double-Precision Floating-Point Value
NN_vmaxss,               // Scalar Single-FP Maximum
NN_vminpd,               // Return Minimum Packed Double-Precision Floating-Point Values
NN_vminps,               // Packed Single-FP Minimum
NN_vminsd,               // Return Minimum Scalar Double-Precision Floating-Point Value
NN_vminss,               // Scalar Single-FP Minimum
NN_vmovapd,              // Move Aligned Packed Double-Precision Floating-Point Values
NN_vmovaps,              // Move Aligned Four Packed Single-FP
NN_vmovd,                // Move 32 bits
NN_vmovddup,             // Move One Double-FP and Duplicate
NN_vmovdqa,              // Move Aligned Double Quadword
NN_vmovdqu,              // Move Unaligned Double Quadword
NN_vmovhlps,             // Move High to Low Packed Single-FP
NN_vmovhpd,              // Move High Packed Double-Precision Floating-Point Values
NN_vmovhps,              // Move High Packed Single-FP
NN_vmovlhps,             // Move Low to High Packed Single-FP
NN_vmovlpd,              // Move Low Packed Double-Precision Floating-Point Values
NN_vmovlps,              // Move Low Packed Single-FP
NN_vmovmskpd,            // Extract Packed Double-Precision Floating-Point Sign Mask
NN_vmovmskps,            // Move Mask to Register
NN_vmovntdq,             // Store Double Quadword Using Non-Temporal Hint
NN_vmovntdqa,            // Load Double Quadword Non-Temporal Aligned Hint
NN_vmovntpd,             // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
NN_vmovntps,             // Move Aligned Four Packed Single-FP Non Temporal
NN_vmovntsd,             // Move Non-Temporal Scalar Double-Precision Floating-Point
NN_vmovntss,             // Move Non-Temporal Scalar Single-Precision Floating-Point
NN_vmovq,                // Move 64 bits
NN_vmovsd,               // Move Scalar Double-Precision Floating-Point Values
NN_vmovshdup,            // Move Packed Single-FP High and Duplicate
NN_vmovsldup,            // Move Packed Single-FP Low and Duplicate
NN_vmovss,               // Move Scalar Single-FP
NN_vmovupd,              // Move Unaligned Packed Double-Precision Floating-Point Values
NN_vmovups,              // Move Unaligned Four Packed Single-FP
NN_vmpsadbw,             // Compute Multiple Packed Sums of Absolute Difference
NN_vmulpd,               // Multiply Packed Double-Precision Floating-Point Values
NN_vmulps,               // Packed Single-FP Multiply
NN_vmulsd,               // Multiply Scalar Double-Precision Floating-Point Values
NN_vmulss,               // Scalar Single-FP Multiply
NN_vorpd,                // Bitwise Logical OR of Double-Precision Floating-Point Values
NN_vorps,                // Bitwise Logical OR for Single-FP Data
NN_vpabsb,               // Packed Absolute Value Byte
NN_vpabsd,               // Packed Absolute Value Doubleword
NN_vpabsw,               // Packed Absolute Value Word
NN_vpackssdw,            // Pack with Signed Saturation (Dword->Word)
NN_vpacksswb,            // Pack with Signed Saturation (Word->Byte)
NN_vpackusdw,            // Pack with Unsigned Saturation
NN_vpackuswb,            // Pack with Unsigned Saturation (Word->Byte)
NN_vpaddb,               // Packed Add Byte
NN_vpaddd,               // Packed Add Dword
NN_vpaddq,               // Add Packed Quadword Integers
NN_vpaddsb,              // Packed Add with Saturation (Byte)
NN_vpaddsw,              // Packed Add with Saturation (Word)
NN_vpaddusb,             // Packed Add Unsigned with Saturation (Byte)
NN_vpaddusw,             // Packed Add Unsigned with Saturation (Word)
NN_vpaddw,               // Packed Add Word
NN_vpalignr,             // Packed Align Right
NN_vpand,                // Bitwise Logical And
NN_vpandn,               // Bitwise Logical And Not
NN_vpavgb,               // Packed Average (Byte)
NN_vpavgw,               // Packed Average (Word)
NN_vpblendd,             // Blend Packed Dwords
NN_vpblendvb,            // Variable Blend Packed Bytes
NN_vpblendw,             // Blend Packed Words
NN_vpbroadcastb,         // Broadcast a Byte Integer
NN_vpbroadcastd,         // Broadcast a Dword Integer
NN_vpbroadcastq,         // Broadcast a Qword Integer
NN_vpbroadcastw,         // Broadcast a Word Integer
NN_vpclmulqdq,           // Carry-Less Multiplication Quadword
NN_vpcmpeqb,             // Packed Compare for Equal (Byte)
NN_vpcmpeqd,             // Packed Compare for Equal (Dword)
NN_vpcmpeqq,             // Compare Packed Qword Data for Equal
NN_vpcmpeqw,             // Packed Compare for Equal (Word)
NN_vpcmpestri,           // Packed Compare Explicit Length Strings, Return Index
NN_vpcmpestrm,           // Packed Compare Explicit Length Strings, Return Mask
NN_vpcmpgtb,             // Packed Compare for Greater Than (Byte)
NN_vpcmpgtd,             // Packed Compare for Greater Than (Dword)
NN_vpcmpgtq,             // Compare Packed Data for Greater Than
NN_vpcmpgtw,             // Packed Compare for Greater Than (Word)
NN_vpcmpistri,           // Packed Compare Implicit Length Strings, Return Index
NN_vpcmpistrm,           // Packed Compare Implicit Length Strings, Return Mask
NN_vperm2f128,           // Permute Floating-Point Values
NN_vperm2i128,           // Permute Integer Values
NN_vpermd,               // Full Doublewords Element Permutation
NN_vpermilpd,            // Permute Double-Precision Floating-Point Values
NN_vpermilps,            // Permute Single-Precision Floating-Point Values
NN_vpermpd,              // Permute Double-Precision Floating-Point Elements
NN_vpermps,              // Permute Single-Precision Floating-Point Elements
NN_vpermq,               // Qwords Element Permutation
NN_vpextrb,              // Extract Byte
NN_vpextrd,              // Extract Dword
NN_vpextrq,              // Extract Qword
NN_vpextrw,              // Extract Word
NN_vpgatherdd,           // Gather Packed Dword Values Using Signed Dword Indices
NN_vpgatherdq,           // Gather Packed Qword Values Using Signed Dword Indices
NN_vpgatherqd,           // Gather Packed Dword Values Using Signed Qword Indices
NN_vpgatherqq,           // Gather Packed Qword Values Using Signed Qword Indices
NN_vphaddd,              // Packed Horizontal Add Doubleword
NN_vphaddsw,             // Packed Horizontal Add and Saturate
NN_vphaddw,              // Packed Horizontal Add Word
NN_vphminposuw,          // Packed Horizontal Word Minimum
NN_vphsubd,              // Packed Horizontal Subtract Doubleword
NN_vphsubsw,             // Packed Horizontal Subtract and Saturate
NN_vphsubw,              // Packed Horizontal Subtract Word
NN_vpinsrb,              // Insert Byte
NN_vpinsrd,              // Insert Dword
NN_vpinsrq,              // Insert Qword
NN_vpinsrw,              // Insert Word
NN_vpmaddubsw,           // Multiply and Add Packed Signed and Unsigned Bytes
NN_vpmaddwd,             // Packed Multiply and Add
NN_vpmaskmovd,           // Conditionally Store Dword Values Using Mask
NN_vpmaskmovq,           // Conditionally Store Qword Values Using Mask
NN_vpmaxsb,              // Maximum of Packed Signed Byte Integers
NN_vpmaxsd,              // Maximum of Packed Signed Dword Integers
NN_vpmaxsw,              // Packed Signed Integer Word Maximum
NN_vpmaxub,              // Packed Unsigned Integer Byte Maximum
NN_vpmaxud,              // Maximum of Packed Unsigned Dword Integers
NN_vpmaxuw,              // Maximum of Packed Word Integers
NN_vpminsb,              // Minimum of Packed Signed Byte Integers
NN_vpminsd,              // Minimum of Packed Signed Dword Integers
NN_vpminsw,              // Packed Signed Integer Word Minimum
NN_vpminub,              // Packed Unsigned Integer Byte Minimum
NN_vpminud,              // Minimum of Packed Unsigned Dword Integers
NN_vpminuw,              // Minimum of Packed Word Integers
NN_vpmovmskb,            // Move Byte Mask to Integer
NN_vpmovsxbd,            // Packed Move with Sign Extend
NN_vpmovsxbq,            // Packed Move with Sign Extend
NN_vpmovsxbw,            // Packed Move with Sign Extend
NN_vpmovsxdq,            // Packed Move with Sign Extend
NN_vpmovsxwd,            // Packed Move with Sign Extend
NN_vpmovsxwq,            // Packed Move with Sign Extend
NN_vpmovzxbd,            // Packed Move with Zero Extend
NN_vpmovzxbq,            // Packed Move with Zero Extend
NN_vpmovzxbw,            // Packed Move with Zero Extend
NN_vpmovzxdq,            // Packed Move with Zero Extend
NN_vpmovzxwd,            // Packed Move with Zero Extend
NN_vpmovzxwq,            // Packed Move with Zero Extend
NN_vpmuldq,              // Multiply Packed Signed Dword Integers
NN_vpmulhrsw,            // Packed Multiply High with Round and Scale
NN_vpmulhuw,             // Packed Multiply High Unsigned
NN_vpmulhw,              // Packed Multiply High
NN_vpmulld,              // Multiply Packed Signed Dword Integers and Store Low Result
NN_vpmullw,              // Packed Multiply Low
NN_vpmuludq,             // Multiply Packed Unsigned Doubleword Integers
NN_vpor,                 // Bitwise Logical Or
NN_vpsadbw,              // Packed Sum of Absolute Differences
NN_vpshufb,              // Packed Shuffle Bytes
NN_vpshufd,              // Shuffle Packed Doublewords
NN_vpshufhw,             // Shuffle Packed High Words
NN_vpshuflw,             // Shuffle Packed Low Words
NN_vpsignb,              // Packed SIGN Byte
NN_vpsignd,              // Packed SIGN Doubleword
NN_vpsignw,              // Packed SIGN Word
NN_vpslld,               // Packed Shift Left Logical (Dword)
NN_vpslldq,              // Shift Double Quadword Left Logical
NN_vpsllq,               // Packed Shift Left Logical (Qword)
NN_vpsllvd,              // Variable Bit Shift Left Logical (Dword)
NN_vpsllvq,              // Variable Bit Shift Left Logical (Qword)
NN_vpsllw,               // Packed Shift Left Logical (Word)
NN_vpsrad,               // Packed Shift Right Arithmetic (Dword)
NN_vpsravd,              // Variable Bit Shift Right Arithmetic
NN_vpsraw,               // Packed Shift Right Arithmetic (Word)
NN_vpsrld,               // Packed Shift Right Logical (Dword)
NN_vpsrldq,              // Shift Double Quadword Right Logical (Qword)
NN_vpsrlq,               // Packed Shift Right Logical (Qword)
NN_vpsrlvd,              // Variable Bit Shift Right Logical (Dword)
NN_vpsrlvq,              // Variable Bit Shift Right Logical (Qword)
NN_vpsrlw,               // Packed Shift Right Logical (Word)
NN_vpsubb,               // Packed Subtract Byte
NN_vpsubd,               // Packed Subtract Dword
NN_vpsubq,               // Subtract Packed Quadword Integers
NN_vpsubsb,              // Packed Subtract with Saturation (Byte)
NN_vpsubsw,              // Packed Subtract with Saturation (Word)
NN_vpsubusb,             // Packed Subtract Unsigned with Saturation (Byte)
NN_vpsubusw,             // Packed Subtract Unsigned with Saturation (Word)
NN_vpsubw,               // Packed Subtract Word
NN_vptest,               // Logical Compare
NN_vpunpckhbw,           // Unpack High Packed Data (Byte->Word)
NN_vpunpckhdq,           // Unpack High Packed Data (Dword->Qword)
NN_vpunpckhqdq,          // Unpack High Packed Data (Qword->Xmmword)
NN_vpunpckhwd,           // Unpack High Packed Data (Word->Dword)
NN_vpunpcklbw,           // Unpack Low Packed Data (Byte->Word)
NN_vpunpckldq,           // Unpack Low Packed Data (Dword->Qword)
NN_vpunpcklqdq,          // Unpack Low Packed Data (Qword->Xmmword)
NN_vpunpcklwd,           // Unpack Low Packed Data (Word->Dword)
NN_vpxor,                // Bitwise Logical Exclusive Or
NN_vrcpps,               // Packed Single-FP Reciprocal
NN_vrcpss,               // Scalar Single-FP Reciprocal
NN_vroundpd,             // Round Packed Double Precision Floating-Point Values
NN_vroundps,             // Round Packed Single Precision Floating-Point Values
NN_vroundsd,             // Round Scalar Double Precision Floating-Point Values
NN_vroundss,             // Round Scalar Single Precision Floating-Point Values
NN_vrsqrtps,             // Packed Single-FP Square Root Reciprocal
NN_vrsqrtss,             // Scalar Single-FP Square Root Reciprocal
NN_vshufpd,              // Shuffle Packed Double-Precision Floating-Point Values
NN_vshufps,              // Shuffle Single-FP
NN_vsqrtpd,              // Compute Square Roots of Packed Double-Precision Floating-Point Values
NN_vsqrtps,              // Packed Single-FP Square Root
NN_vsqrtsd,              // Compute Square Rootof Scalar Double-Precision Floating-Point Value
NN_vsqrtss,              // Scalar Single-FP Square Root
NN_vstmxcsr,             // Store Streaming SIMD Extensions Technology Control/Status Register
NN_vsubpd,               // Subtract Packed Double-Precision Floating-Point Values
NN_vsubps,               // Packed Single-FP Subtract
NN_vsubsd,               // Subtract Scalar Double-Precision Floating-Point Values
NN_vsubss,               // Scalar Single-FP Subtract
NN_vtestpd,              // Packed Double-Precision Floating-Point Bit Test
NN_vtestps,              // Packed Single-Precision Floating-Point Bit Test
NN_vucomisd,             // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
NN_vucomiss,             // Scalar Unordered Single-FP Compare and Set EFLAGS
NN_vunpckhpd,            // Unpack and Interleave High Packed Double-Precision Floating-Point Values
NN_vunpckhps,            // Unpack High Packed Single-FP Data
NN_vunpcklpd,            // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
NN_vunpcklps,            // Unpack Low Packed Single-FP Data
NN_vxorpd,               // Bitwise Logical OR of Double-Precision Floating-Point Values
NN_vxorps,               // Bitwise Logical XOR for Single-FP Data
NN_vzeroall,             // Zero All YMM Registers
NN_vzeroupper,           // Zero Upper Bits of YMM Registers

// Transactional Synchronization Extensions

NN_xabort,               // Transaction Abort
NN_xbegin,               // Transaction Begin
NN_xend,                 // Transaction End
NN_xtest,                // Test If In Transactional Execution

// Virtual PC synthetic instructions

NN_vmgetinfo,            // Virtual PC - Get VM Information
NN_vmsetinfo,            // Virtual PC - Set VM Information
NN_vmdxdsbl,             // Virtual PC - Disable Direct Execution
NN_vmdxenbl,             // Virtual PC - Enable Direct Execution
NN_vmcpuid,              // Virtual PC - Virtualized CPU Information
NN_vmhlt,                // Virtual PC - Halt
NN_vmsplaf,              // Virtual PC - Spin Lock Acquisition Failed
NN_vmpushfd,             // Virtual PC - Push virtualized flags register
NN_vmpopfd,              // Virtual PC - Pop virtualized flags register
NN_vmcli,                // Virtual PC - Clear Interrupt Flag
NN_vmsti,                // Virtual PC - Set Interrupt Flag
NN_vmiretd,              // Virtual PC - Return From Interrupt
NN_vmsgdt,               // Virtual PC - Store Global Descriptor Table
NN_vmsidt,               // Virtual PC - Store Interrupt Descriptor Table
NN_vmsldt,               // Virtual PC - Store Local Descriptor Table
NN_vmstr,                // Virtual PC - Store Task Register
NN_vmsdte,               // Virtual PC - Store to Descriptor Table Entry
NN_vpcext,               // Virtual PC - ISA extension

// FMA4
NN_vfmaddsubps,          // Multiply with Alternating Add/Subtract of Packed Single-Precision Floating-Point
NN_vfmaddsubpd,          // Multiply with Alternating Add/Subtract of Packed Double-Precision Floating-Point
NN_vfmsubaddps,          // Multiply with Alternating Subtract/Add of Packed Single-Precision Floating-Point
NN_vfmsubaddpd,          // Multiply with Alternating Subtract/Add of Packed Double-Precision Floating-Point
NN_vfmaddps,             // Multiply and Add Packed Single-Precision Floating-Point
NN_vfmaddpd,             // Multiply and Add Packed Double-Precision Floating-Point
NN_vfmaddss,             // Multiply and Add Scalar Single-Precision Floating-Point
NN_vfmaddsd,             // Multiply and Add Scalar Double-Precision Floating-Point
NN_vfmsubps,             // Multiply and Subtract Packed Single-Precision Floating-Point
NN_vfmsubpd,             // Multiply and Subtract Packed Double-Precision Floating-Point
NN_vfmsubss,             // Multiply and Subtract Scalar Single-Precision Floating-Point
NN_vfmsubsd,             // Multiply and Subtract Scalar Double-Precision Floating-Point
NN_vfnmaddps,            // Negative Multiply and Add Packed Single-Precision Floating-Point
NN_vfnmaddpd,            // Negative Multiply and Add Packed Double-Precision Floating-Point
NN_vfnmaddss,            // Negative Multiply and Add Scalar Single-Precision Floating-Point
NN_vfnmaddsd,            // Negative Multiply and Add Double Single-Precision Floating-Point
NN_vfnmsubps,            // Negative Multiply and Subtract Packed Single-Precision Floating-Point
NN_vfnmsubpd,            // Negative Multiply and Subtract Packed Double-Precision Floating-Point
NN_vfnmsubss,            // Negative Multiply and Subtract Scalar Single-Precision Floating-Point
NN_vfnmsubsd,            // Negative Multiply and Subtract Double Single-Precision Floating-Point

NN_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
I5_null = 0,            // Unknown Operation

//
//      Intel 8080-8085 instructions
//

I5_aci,
I5_adc, Z80_adc = I5_adc,
I5_add, Z80_add = I5_add,
I5_adi,
I5_ana,
I5_ani,
I5_call,
I5_cnz,
I5_cz,
I5_cnc,
I5_cc,
I5_cpo,
I5_cpe,
I5_cp,
I5_cm,
I5_cmc,
I5_cmp,
I5_cpi,
I5_cma,
I5_daa,
I5_dad,
I5_dcr,
I5_dcx,
I5_di, Z80_di = I5_di,
I5_ei, Z80_ei = I5_ei,
I5_halt,
I5_in, Z80_in = I5_in,
I5_inr,
I5_inx,
I5_jmp,
I5_jnz,
I5_jz,
I5_jnc,
I5_jc,
I5_jpo,
I5_jpe,
I5_jp,
I5_jm,
I5_lda,
I5_ldax,
I5_lhld,
I5_lxi,
I5_mov,
I5_mvi,
I5_nop,
I5_ora,
I5_ori,
I5_out, Z80_out = I5_out,
I5_pchl,
I5_pop, Z80_pop = I5_pop,
I5_push, Z80_push = I5_push,
I5_ret,
I5_rnz,
I5_rz,
I5_rnc,
I5_rc,
I5_rpo,
I5_rpe,
I5_rp,
I5_rm,
I5_ral,
I5_rlc,
I5_rar,
I5_rrc,
I5_rst,
I5_sbb,
I5_sbi,
I5_stc,
I5_sphl,
I5_sta,
I5_stax,
I5_shld,
I5_sui,
I5_sub, Z80_sub = I5_sub,
I5_xra,
I5_xri,
I5_xchg,
I5_xthl,

I5_rim,
I5_sim,

//
//      Z80 extensions
//

Z80_and,
Z80_bit,
Z80_call,
Z80_ccf,
Z80_cp,
Z80_cpd,
Z80_cpdr,
Z80_cpi,
Z80_cpir,
Z80_cpl,
Z80_dec,
Z80_djnz,
Z80_ex,
Z80_exx,
Z80_halt,
Z80_im,
Z80_inc,
Z80_ind,
Z80_indr,
Z80_ini,
Z80_inir,
Z80_jp,
Z80_jr,
Z80_ld,
Z80_ldd,
Z80_lddr,
Z80_ldi,
Z80_ldir,
Z80_neg,
Z80_or,
Z80_otdr,
Z80_otir,
Z80_outd,
Z80_outi,
Z80_res,
Z80_ret,
Z80_reti,
Z80_retn,
Z80_rl,
Z80_rla,
Z80_rlc,
Z80_rlca,
Z80_rld,
Z80_rr,
Z80_rra,
Z80_rrc,
Z80_rrca,
Z80_rrd,
Z80_scf,
Z80_sbc,
Z80_set,
Z80_sla,
Z80_sra,
Z80_srl,
Z80_xor,
Z80_inp,                // undocumented
Z80_outp,               // undocumented
Z80_srr,                // undocumented

//
//      HD64180 extensions
//

HD_in0,   Z80_in0   = HD_in0,
HD_mlt,   Z80_mlt   = HD_mlt,
HD_otim,  Z80_otim  = HD_otim,
HD_otimr, Z80_otimr = HD_otimr,
HD_otdm,  Z80_otdm  = HD_otdm,
HD_otdmr, Z80_otdmr = HD_otdmr,
HD_out0,  Z80_out0  = HD_out0,
HD_slp,   Z80_slp   = HD_slp,
HD_tst,   Z80_tst   = HD_tst,
HD_tstio, Z80_tstio = HD_tstio,

//
//      A80 special instructions
//

A80_lbcd,
A80_lded,
A80_lspd,
A80_lixd,
A80_liyd,
A80_sbcd,
A80_sded,
A80_sspd,
A80_sixd,
A80_siyd,
A80_xtix,
A80_xtiy,
A80_spix,
A80_spiy,
A80_pcix,
A80_pciy,
A80_mvra,
A80_mvia,
A80_mvar,
A80_mvai,
A80_addix,
A80_addiy,
A80_addc,
A80_addcix,
A80_addciy,
A80_subc,
A80_subcix,
A80_subciy,
A80_jrc,
A80_jrnc,
A80_jrz,
A80_jrnz,
A80_cmpi,
A80_cmpd,
A80_im0,
A80_im1,
A80_im2,
A80_otd,
A80_oti,

// Intel 8085 undocumented instructions
// (info from http://oak.oakland.edu/pub/cpm/maclib/i8085.lib)

I5_dsub,        // (HL) <- (HL)-(BC), affects all flags
I5_arhl,        // SHIFT HL RIGHT ONE BIT, (H7 IS DUPLICATED, L0 IS SHIFTED INTO CY)
I5_rdel,        // ROTATE DE LEFT ONE BIT THRU CY, (E0 RECEIVES CY, CY RECEIVES D7)
I5_ldhi,        // (DE) <- (HL)+arg
I5_ldsi,        // (DE) <- (SP)+arg
I5_shlx,        // ((DE)) <- (HL)
I5_lhlx,        // (HL) <- ((DE))
I5_rstv,        // RESTART 40H ON V (OVERFLOW)
I5_jx5,         // JUMP IF X5 SET
I5_jnx5,        // JUMP IF NOT X5 SET

// Z380 instructions

Z80_cplw,       // Complement HL register
Z80_swap,       // Swap upper register word with lower register word
Z80_inw,        // Input word
Z80_outw,       // Output word
Z80_ldw,        // Load word
Z80_addw,       // Add word
Z80_subw,       // Subtract word
Z80_adcw,       // Add with carry word
Z80_sbcw,       // Subtract with borrow word
Z80_andw,       // AND logical word
Z80_xorw,       // XOR logical word
Z80_orw,        // OR logical word
Z80_cpw,        // Compare word
Z80_ddir,       // Decoder directive
Z80_calr,       // Call relative
Z80_ldctl,      // Load control register
Z80_mtest,      // Mode test
Z80_exxx,       // Exchange Index Register with Alternate Bank
Z80_exxy,       // Exchange Index Register with Alternate Bank
Z80_exall,      // Exchange all registers with Alternate Bank
Z80_setc,       // Set control bit
Z80_resc,       // Reset control bit
Z80_rlcw,       // Rotate Left Circular Word
Z80_rrcw,       // Rotate Right Circular Word
Z80_rlw,        // Rotate Left Word
Z80_rrw,        // Rotate Right Word
Z80_slaw,       // Shift Left Arithmetic Word
Z80_sraw,       // Shift Right Arithmetic Word
Z80_srlw,       // Shift Right Logical Word
Z80_multw,      // Multiply Word
Z80_multuw,     // Multiply Word Unsigned
Z80_divuw,      // Divide unsigned
Z80_outaw,      // Output word direct to port address
Z80_inaw,       // Input word direct from port address
Z80_outa,       // Output byte direct to port address
Z80_ina,        // Input byte direct from port address
Z80_negw,       // Negate word
Z80_exts,       // Extend byte sign
Z80_extsw,      // Extend word sign
Z80_btest,      // Bank test
Z80_ldiw,       // Load and increment (word)
Z80_ldirw,      // Load and increment, repeat (word)
Z80_lddw,       // Load and decrement (word)
Z80_lddrw,      // Load and decrement, repeat (word)
Z80_iniw,       // Input and increment (word)
Z80_inirw,      // Input and increment, repeat (word)
Z80_indw,       // Input and decrement (word)
Z80_indrw,      // Input and decrement, repeat (word)
Z80_outiw,      // Output and increment (word)
Z80_otirw,      // Output and increment, repeat (word)
Z80_outdw,      // Output and decrement (word)
Z80_otdrw,      // Output and decrement, repeat (word)

// Gameboy instructions

GB_ldh,
GB_stop,

I5_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

I860_null = 0,          // Unknown Operation

//
//      Intel 860 XP instructions
//

I860_adds,
I860_addu,
I860_and,
I860_andh,
I860_andnot,
I860_andnoth,
I860_bc,
I860_bc_t,
I860_bla,
I860_bnc,
I860_bnc_t,
I860_br,
I860_bri,
I860_bte,
I860_btne,
I860_call,
I860_calli,
I860_fadd,
I860_faddp,
I860_faddz,
I860_famov,
I860_fiadd,
I860_fisub,
I860_fix,
I860_fld,
I860_flush,
I860_fmlow_dd,
I860_fmul,
I860_form,
I860_frcp,
I860_frsqr,
I860_fst,
I860_fsub,
I860_ftrunc,
I860_fxfr,
I860_fzchkl,
I860_fzchks,
I860_introvr,
I860_ixfr,
I860_ld_c,
I860_ld,
I860_ldint,
I860_ldio,
I860_lock,
I860_or,
I860_orh,
I860_pfadd,
I860_pfaddp,
I860_pfaddz,
I860_pfamov,
I860_pfeq,
I860_pfgt,
I860_pfiadd,
I860_pfisub,
I860_pfix,
I860_pfld,
I860_pfle,
I860_pfmul,
I860_pfmul3_dd,
I860_pform,
I860_pfsub,
I860_pftrunc,
I860_pfzchkl,
I860_pfzchks,
I860_pst_d,
I860_scyc,
I860_shl,
I860_shr,
I860_shra,
I860_shrd,
I860_st_c,
I860_st,
I860_stio,
I860_subs,
I860_subu,
I860_trap,
I860_unlock,
I860_xor,
I860_xorh,
//
// iNTEL 860 XP Pipelined F-P instructions
//
I860_r2p1,
I860_r2pt,
I860_r2ap1,
I860_r2apt,
I860_i2p1,
I860_i2pt,
I860_i2ap1,
I860_i2apt,
I860_rat1p2,
I860_m12apm,
I860_ra1p2,
I860_m12ttpa,
I860_iat1p2,
I860_m12tpm,
I860_ia1p2,
I860_m12tpa,
I860_r2s1,
I860_r2st,
I860_r2as1,
I860_r2ast,
I860_i2s1,
I860_i2st,
I860_i2as1,
I860_i2ast,
I860_rat1s2,
I860_m12asm,
I860_ra1s2,
I860_m12ttsa,
I860_iat1s2,
I860_m12tsm,
I860_ia1s2,
I860_m12tsa,
I860_mr2p1,
I860_mr2pt,
I860_mr2mp1,
I860_mr2mpt,
I860_mi2p1,
I860_mi2pt,
I860_mi2mp1,
I860_mi2mpt,
I860_mrmt1p2,
I860_mm12mpm,
I860_mrm1p2,
I860_mm12ttpm,
I860_mimt1p2,
I860_mm12tpm,
I860_mim1p2,
I860_mr2s1,
I860_mr2st,
I860_mr2ms1,
I860_mr2mst,
I860_mi2s1,
I860_mi2st,
I860_mi2ms1,
I860_mi2mst,
I860_mrmt1s2,
I860_mm12msm,
I860_mrm1s2,
I860_mm12ttsm,
I860_mimt1s2,
I860_mm12tsm,
I860_mim1s2,

I860_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
I51_null = 0,   // Unknown Operation

I51_acall,      // Absolute Call
I51_add,        // Add Second Operand to Acc
I51_addc,       // Add Second Operand to Acc with carry
I51_ajmp,       // Absolute Jump
I51_anl,        // Logical AND (op1 &= op2)
I51_cjne,       // Compare Operands and JNE
I51_clr,        // Clear Operand (0)
I51_cpl,        // Complement Operand
I51_da,         // Decimal Adjust Accumulator
I51_dec,        // Decrement Operand
I51_div,        // Divide Acc by B
I51_djnz,       // Decrement Operand and JNZ
I51_inc,        // Increment Operand
I51_jb,         // Jump if Bit is set
I51_jbc,        // Jump if Bit is set & clear Bit
I51_jc,         // Jump if Carry is set
I51_jmp,        // Jump indirect relative to Data Pointer
I51_jnb,        // Jump if Bit is clear
I51_jnc,        // Jump if Carry is clear
I51_jnz,        // Jump if Acc is not zero
I51_jz,         // Jump if Acc is zero
I51_lcall,      // Long Subroutine Call
I51_ljmp,       // Long Jump
I51_mov,        // Move (Op1 <- Op2)
I51_movc,       // Move code byte relative to second op to Acc
I51_movx,       // Move from/to external RAM
I51_mul,        // Multiply Acc by B
I51_nop,        // No operation
I51_orl,        // Logical OR (op1 |= op2)
I51_pop,        // Pop  from Stack and put in Direct RAM
I51_push,       // Push from Direct RAM to Stack
I51_ret,        // Return from subroutine
I51_reti,       // Return from Interrupt
I51_rl,         // Rotate Acc left
I51_rlc,        // Rotate Acc left through Carry
I51_rr,         // Rotate Acc right
I51_rrc,        // Rotate Acc right through Carry
I51_setb,       // Set Direct Bit
I51_sjmp,       // Short jump
I51_subb,       // Subtract Second Operand from Acc with Borrow
I51_swap,       // Swap nibbles of Acc
I51_xch,        // Exchange Operands
I51_xchd,       // Exchange Digit in Acc with Indirect RAM
I51_xrl,        // Exclusive OR (op1 ^= op2)

// 80251 instructions

I51_jsle,       // Jump if less than or equal (signed)
I51_jsg,        // Jump if greater than (signed)
I51_jle,        // Jump if less than or equal
I51_jg,         // Jump if greater than
I51_jsl,        // Jump if less than (signed)
I51_jsge,       // Jump if greater than or equal (signed)
I51_je,         // Jump if equal
I51_jne,        // Jump if not equal
I51_trap,       // Trap
I51_ejmp,       // Extended jump
I51_ecall,      // Extended call
I51_eret,       // Extended return
I51_movh,       // Move immediate 16-bit data to the high word of a dword (double-word) register
I51_movz,       // Move 8-bit register to 16-bit register with zero extension
I51_movs,       // Move 8-bit register to 16-bit register with sign extension
I51_srl,        // Shift logical right by 1 bit
I51_sra,        // Shift arithmetic right by 1 bit
I51_sll,        // Shift logical left by 1 bit
I51_sub,        // Subtract
I51_cmp,        // Compare

// 51mx instructions
I51_emov,       // Move (A <- @PRi+disp) or (@PRi+disp <- A)

I51_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
TMS_null = 0,   // Unknown Operation
TMS_abs,        // Absolute value of Acc
TMS_adcb,       // Add ACCB to Acc With Carry
TMS_add,        // Add to Acc
TMS_addb,       // Add ACCB to Acc
TMS_addc,       // Add to Acc With Carry
TMS_adds,       // Add to Acc With Sign-Extension Suppressed
TMS_addt,       // Add to Acc With Shift Specified by TREG1
TMS_adrk,       // Add to Aux Register With Short Immediate
TMS_and,        // AND With Acc
TMS_andb,       // AND ACCD With Acc
TMS_apac,       // Add P Register to Acc
TMS_apl,        // AND Data Memory Value With DBMR
TMS_apl2,       // AND Data Memory Value With Long Constant
TMS_b,          // Branch Unconditionally
TMS_bacc,       // Branch to Location (Acc)
TMS_baccd,      // Branch to Location (Acc) Delayed
TMS_banz,       // Branch on Aux Register Not Zero
TMS_banzd,      // Branch on Aux Register Not Zero Delayed
TMS_bcnd,       // Branch Conditionally
TMS_bcndd,      // Branch Conditionally Delayed
TMS_bd,         // Branch Unconditionally Delayed
TMS_bit,        // Test Bit
TMS_bitt,       // Test Bit Specified by TREG2
TMS_bldd,       // Block Move From Data Memory to Data Memory
TMS_bldp,       // Block Move From Data Memory to Program Memory
TMS_blpd,       // Block Move From Program Memory to Data Memory
TMS_bsar,       // Barrel Shift
TMS_cala,       // Call Subroutine at (Acc)
TMS_calad,      // Call Subroutine at (Acc) Delayed
TMS_call,       // Call Unconditionally
TMS_calld,      // Call Unconditionally Delayed
TMS_cc,         // Call Conditionally
TMS_ccd,        // Call Conditionally Delayed
TMS_clrc,       // Clear Control Bit
TMS_cmpl,       // Complement Acc
TMS_cmpr,       // Compare Aux Register With ARCR
TMS_cpl,        // Compare DBMR With Data Value
TMS_cpl2,       // Compare Long Immediate With Data Value
TMS_crgt,       // Test for Acc Greater Than ACCB
TMS_crlt,       // Test for Acc Smaller Than ACCB
TMS_dmov,       // Data Move in Data Memory
TMS_estop,      // Emulator Stop
TMS_exar,       // Exchange ACCB With Acc
TMS_idle,       // Idle Until Interrupt
TMS_idle2,      // Idle Until Interrupt - Low Power Mode
TMS_in,         // Input Data From Port
TMS_intr,       // Soft Interrupt
TMS_lacb,       // Load Acc With ACCB
TMS_lacc,       // Load Acc With Shift
TMS_lacl,       // Load Low Acc and Clear High Acc
TMS_lact,       // Load Acc With Shift Specified by TREG1
TMS_lamm,       // Load Acc With Memory-Mapped Register
TMS_lar,        // Load Aux Register
TMS_ldp,        // Load Data Memory Pointer
TMS_lmmr,       // Load Memory-Mapped Register
TMS_lph,        // Load Product High Register
TMS_lst,        // Load Status Register
TMS_lt,         // Load TREG0
TMS_lta,        // Load TREG0 and Accumulate Previous Product
TMS_ltd,        // Load TREG0,Accumulate Previous Product and Move Data
TMS_ltp,        // Load TREG0 and Store P -> Acc
TMS_lts,        // Load TREG0 and Subtract Previous Product
TMS_mac,        // Multiply and Accumulate
TMS_macd,       // Multiply and Accumulate With Data Move
TMS_madd,       // Multiply and Accumulate With Data Move and Dynamic Addressing
TMS_mads,       // Multiply and Accumulate With Dynamic Addressing
TMS_mar,        // Modify Aux Register
TMS_mpy,        // Multiply
TMS_mpya,       // Multiply and Accumulate Previous Product
TMS_mpys,       // Multiply and Subtract Previous Product
TMS_mpyu,       // Multiply Unsigned
TMS_neg,        // Negate Acc
TMS_nmi,        // Nonmaskable Interrupt
TMS_nop,        // No Operation
TMS_norm,       // Normalize Contents of Acc
TMS_opl,        // OR With DBMS
TMS_opl2,       // OR With Long Immediate
TMS_or,         // OR With Acc
TMS_orb,        // OR ACCB With Accumulator
TMS_out,        // Out Data to Port
TMS_pac,        // Load Acc <- P
TMS_pop,        // Pop Top of Stack to Low Acc
TMS_popd,       // Pop Top of Stack to Data Memory
TMS_pshd,       // Push Data Memory Value Onto Stack
TMS_push,       // Push Low Acc Onto Stack
TMS_ret,        // Return From Subroutine
TMS_retc,       // Return Conditionally
TMS_retcd,      // Return Conditionally Delayed
TMS_retd,       // Return From Subroutine Delayed
TMS_rete,       // Enable Interrupts and Return From Interrupt
TMS_reti,       // Return From Interrupt
TMS_rol,        // Rotate Acc Left
TMS_rolb,       // Rotate ACCB and Acc Left
TMS_ror,        // Rotate Acc Right
TMS_rorb,       // Rotate ACCB and Acc Right
TMS_rpt,        // Repeat Next Instruction
TMS_rptb,       // Repeat Block
TMS_rptz,       // Repeat Preceded by Clearing Acc and P
TMS_sacb,       // Store Acc in ACCB
TMS_sach,       // Store High Acc With Shift
TMS_sacl,       // Store Low Acc With Shift
TMS_samm,       // Store Acc in Memory-Mapped Register
TMS_sar,        // Store Aux Register
TMS_sath,       // Barrel Shift Acc as Specified by TREG1(4)
TMS_satl,       // Barrel Shift Acc as Specified by TREG1(3-0)
TMS_sbb,        // Subtract ACCB From Acc
TMS_sbbb,       // Subtract ACCB From Acc With Borrow
TMS_sbrk,       // Subtract From Aux Register Short Immediate
TMS_setc,       // Set Control Bit
TMS_sfl,        // Shift Acc Left
TMS_sflb,       // Shift ACCB and Acc Left
TMS_sfr,        // Shift Acc Right
TMS_sfrb,       // Shift ACCB and Acc Right
TMS_smmr,       // Store Memory-Mapped Register
TMS_spac,       // Subtract P From Acc
TMS_sph,        // Store High P Register
TMS_spl,        // Store Low P Register
TMS_splk,       // Store Parallel Long Immediate
TMS_spm,        // Store ACCB and Acc Right
TMS_sqra,       // Square and Accumulate Previous Product
TMS_sqrs,       // Square and Subtract Previous Product
TMS_sst,        // Store Status Register
TMS_sub,        // Subtract From Acc
TMS_subb,       // Subtract From Acc With Borrow
TMS_subc,       // Conditional Subtract
TMS_subs,       // Subtract From Acc With Sign-Extension Suppressed
TMS_subt,       // Subtract From Acc With Shift Specified by TREG1
TMS_tblr,       // Table Read
TMS_tblw,       // Table Write
TMS_trap,       // Software Interrupt
TMS_xc,         // Execute Conditionally
TMS_xor,        // Exclusive-OR With Acc
TMS_xorb,       // Exclusive-OR of ACCB With Acc
TMS_xpl,        // Exclusive-OR Data Memory Value
TMS_xpl2,       // Exclusive-OR Data Memory Value
TMS_zalr,       // Zero Low Acc Load High Acc With Rounding
TMS_zap,        // Zero Acc and P
TMS_zpr,        // Zero P Register

//
//      TMS320C2x instructions
//

TMS2_abs,               // Absolute value of accumulator
TMS2_add,               // Add to accumulator with shift
TMS2_addc,              // Add to accumulator with carry
TMS2_addh,              // Add to high accumulator
TMS2_addk,              // Add to accumulator short immediate
TMS2_adds,              // Add to low accumulator with sign extension suppressed
TMS2_addt,              // Add to accumulator with shift specified by T register
TMS2_adlk,              // Add to accumulator long immediate with shift
TMS2_adrk,              // Add to auxiliary register short immediate
TMS2_and,               // And with accumulator
TMS2_andk,              // And immediate with accumulator with shift
TMS2_apac,              // App P register to accumulator
TMS2_b,                 // Branch unconditionally
TMS2_bacc,              // Branch to address specified by accumulator
TMS2_banz,              // Bnrach on auxiliary register not zero
TMS2_bbnz,              // Branch if tc bit != 0
TMS2_bbz,               // Branch if tc bit = 0
TMS2_bc,                // Branch on carry
TMS2_bgez,              // Branch if accumulator >= 0
TMS2_bgz,               // Branch if accumulator > 0
TMS2_bioz,              // Branch on i/o status = 0
TMS2_bit,               // Test bit
TMS2_bitt,              // Test bit specifed by T register
TMS2_blez,              // Branch if accumulator <= 0
TMS2_blkd,              // Block move from data memory to data memory
TMS2_blkp,              // Block move from program memory to data memory
TMS2_blz,               // Branch if accumulator < 0
TMS2_bnc,               // Branch on no carry
TMS2_bnv,               // Branch if no overflow
TMS2_bnz,               // Branch if accumulator != 0
TMS2_bv,                // Branch on overflow
TMS2_bz,                // Branch if accumulator = 0
TMS2_cala,              // Call subroutine indirect
TMS2_call,              // Call subroutine
TMS2_cmpl,              // Complement accumulator
TMS2_cmpr,              // Compare auxiliary register with auxiliary register ar0
TMS2_cnfd,              // Configure block as data memory
TMS2_cnfp,              // Configure block as program memory
TMS2_conf,              // Configure block as data/program memory
TMS2_dint,              // Disable interrupt
TMS2_dmov,              // Data move in data memory
TMS2_eint,              // Enable interrupt
TMS2_fort,              // Format serial port registers
TMS2_idle,              // Idle until interrupt
TMS2_in,                // Input data from port
TMS2_lac,               // Load accumulator with shift
TMS2_lack,              // Load accumulator short immediate
TMS2_lact,              // Load accumulator with shift specified by T register
TMS2_lalk,              // Load accumulator long immediate with shift
TMS2_lar,               // Load auxiliary register
TMS2_lark,              // Load auxiliary register short immediate
TMS2_larp,              // Load auxiliary register pointer
TMS2_ldp,               // Load data memory page pointer
TMS2_ldpk,              // Load data memory page pointer immediate
TMS2_lph,               // Load high P register
TMS2_lrlk,              // Load auxiliary register long immediate
TMS2_lst,               // Load status register ST0
TMS2_lst1,              // Load status register ST1
TMS2_lt,                // Load T register
TMS2_lta,               // Load T register and accumulate previous product
TMS2_ltd,               // Load T register, accumulate previous product and move data
TMS2_ltp,               // Load T register and store P register in accumulator
TMS2_lts,               // Load T register and subtract previous product
TMS2_mac,               // Multiply and accumulate
TMS2_macd,              // Multiply and accumulate with data move
TMS2_mar,               // Modify auxiliary register
TMS2_mpy,               // Multiply (with T register, store product in P register)
TMS2_mpya,              // Multiply and accumulate previous product
TMS2_mpyk,              // Multiply immediate
TMS2_mpys,              // Multiply and subtract previous product
TMS2_mpyu,              // Multiply unsigned
TMS2_neg,               // Negate accumulator
TMS2_nop,               // No operation
TMS2_norm,              // Normalize contents of accumulator
TMS2_or,                // Or with accumulator
TMS2_ork,               // Or immediate with accumulator with shift
TMS2_out,               // Output data to port
TMS2_pac,               // Load accumulator with P register
TMS2_pop,               // Pop top of stack to low accumulator
TMS2_popd,              // Pop top of stack to data memory
TMS2_pshd,              // Push data memory value onto stack
TMS2_push,              // Push low accumulator onto stack
TMS2_rc,                // Reset carry bit
TMS2_ret,               // Return from subroutine
TMS2_rfsm,              // Reset serial port frame synchronization mode
TMS2_rhm,               // Reset hold mode
TMS2_rol,               // Rotate accumulator left
TMS2_ror,               // Rotate acuumulator right
TMS2_rovm,              // Reset overflow mode
TMS2_rpt,               // Repeat instruction as specified by data memory value
TMS2_rptk,              // Repeat instruction as specified by immediate value
TMS2_rsxm,              // Reset sign extension mode
TMS2_rtc,               // Reset test/control flag
TMS2_rtxm,              // Reset serial port transmit mode
TMS2_rxf,               // Reset external flag
TMS2_sach,              // Store high accumulator with shift
TMS2_sacl,              // Store low accumulator with shift
TMS2_sar,               // Store auxiliary register
TMS2_sblk,              // Subtract from accumulator long immediate with shift
TMS2_sbrk,              // Subtract from auxiliary register short immediate
TMS2_sc,                // Set carry bit
TMS2_sfl,               // Shift accumulator left
TMS2_sfr,               // Shift accumulator right
TMS2_sfsm,              // Set serial port frame synchronization mode
TMS2_shm,               // Set hold mode
TMS2_sovm,              // Set overflow mode
TMS2_spac,              // Subtract P register from accumulator
TMS2_sph,               // Store high P register
TMS2_spl,               // Store low P register
TMS2_spm,               // Set P register output shift mode
TMS2_sqra,              // Square and accumulate
TMS2_sqrs,              // Square and subtract previous product
TMS2_sst,               // Store status register ST0
TMS2_sst1,              // Store status register ST1
TMS2_ssxm,              // Set sign extension mode
TMS2_stc,               // Set test/control flag
TMS2_stxm,              // Set serial port transmit mode
TMS2_sub,               // Subtract from accumulator with shift
TMS2_subb,              // Subtract from accumulator with borrow
TMS2_subc,              // Conditional subtract
TMS2_subh,              // Subtract from high accumulator
TMS2_subk,              // Subtract from accumulator shoft immediate
TMS2_subs,              // Subtract from low accumulator with sign extension suppressed
TMS2_subt,              // Subtract from accumulator with shift specified by T register
TMS2_sxf,               // Set external flag
TMS2_tblr,              // Table read
TMS2_tblw,              // Table write
TMS2_trap,              // Software interrupt
TMS2_xor,               // Exclusive or with accumulator
TMS2_xork,              // Exclusive or immediate with accumulator with shift
TMS2_zac,               // Zero accumulator
TMS2_zalh,              // Zero low accumulator and load high accumulator
TMS2_zalr,              // Zero low accumulator and load high accumulator with rounding
TMS2_zals,              // Zero low accumulator and load high accumulator with sign extension suppressed

TMS_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
M65_null = 0,           // Unknown Operation

// NMOS instructions

M65_adc,                // A <- (A) + M + C
M65_anc,                // A <- A /\ M, C <- ~A7
M65_and,                // A <- (A) /\ M
M65_ane,                // M <-[(A)\/$EE] /\ (X)/\(M)
M65_arr,                // A <- [(A /\ M) >> 1]
M65_asl,                // C <- A7, A <- (A) << 1
M65_asr,                // A <- [(A /\ M) >> 1]
M65_bcc,                // if C=0, PC = PC + offset
M65_bcs,                // if C=1, PC = PC + offset
M65_beq,                // if Z=1, PC = PC + offset
M65_bit,                // Z <- ~(A /\ M) N<-M7 V<-M6
M65_bmi,                // if N=1, PC = PC + offset
M65_bne,                // if Z=0, PC = PC + offset
M65_bpl,                // if N=0, PC = PC + offset
M65_brk,                // Stack <- PC, PC <- ($fffe)
M65_bvc,                // if V=0, PC = PC + offset
M65_bvs,                // if V=1, PC = PC + offset
M65_clc,                // C <- 0
M65_cld,                // D <- 0
M65_cli,                // I <- 0
M65_clv,                // V <- 0
M65_cmp,                // (A - M) -> NZC
M65_cpx,                // (X - M) -> NZC
M65_cpy,                // (Y - M) -> NZC
M65_dcp,                // M <- (M)-1, (A-M) -> NZC
M65_dec,                // M <- (M) - 1
M65_dex,                // X <- (X) - 1
M65_dey,                // Y <- (Y) - 1
M65_eor,                // A <- (A) \-/ M
M65_inc,                // M <- (M) + 1
M65_inx,                // X <- (X) +1
M65_iny,                // Y <- (Y) + 1
M65_isb,                // M <- (M) - 1,A <- (A)-M-~C
M65_jmp,                // PC <- Address
M65_jmpi,               // (PC <- Address)
M65_jsr,                // Stack <- PC, PC <- Address
M65_lae,                // X,S,A <- (S /\ M)
M65_lax,                // A <- M, X <- M
M65_lda,                // A <- M
M65_ldx,                // X <- M
M65_ldy,                // Y <- M
M65_lsr,                // C <- A0, A <- (A) >> 1
M65_lxa,                // X04 <- (X04) /\ M04, A04 <- (A04) /\ M04
M65_nop,                // [no operation]
M65_ora,                // A <- (A) V M
M65_pha,                // Stack <- (A)
M65_php,                // Stack <- (P)
M65_pla,                // A <- (Stack)
M65_plp,                // A <- (Stack)
M65_rla,                // M <- (M << 1) /\ (A)
M65_rol,                // C <- A7 & A <- A << 1 + C
M65_ror,                // C<-A0 & A<- (A7=C + A>>1)
M65_rra,                // M <- (M >> 1) + (A) + C
M65_rti,                // P <- (Stack), PC <-(Stack)
M65_rts,                // PC <- (Stack)
M65_sax,                // M <- (A) /\ (X)
M65_sbc,                // A <- (A) - M - ~C
M65_sbx,                // X <- (X)/\(A) - M
M65_sec,                // C <- 1
M65_sed,                // D <- 1
M65_sei,                // I <- 1
M65_sha,                // M <- (A) /\ (X) /\ (PCH+1)
M65_shs,                // X <- (A) /\ (X), S <- (X), M <- (X) /\ (PCH+1)
M65_shx,                // M <- (X) /\ (PCH+1)
M65_shy,                // M <- (Y) /\ (PCH+1)
M65_slo,                // M <- (M >> 1) + A + C
M65_sre,                // M <- (M >> 1) \-/ A
M65_sta,                // M <- (A)
M65_stx,                // M <- (X)
M65_sty,                // M <- (Y)
M65_tax,                // X <- (A)
M65_tay,                // Y <- (A)
M65_tsx,                // X <- (S)
M65_txa,                // A <- (X)
M65_txs,                // S <- (X)
M65_tya,                // A <- (Y)

// CMOS instructions

M65_bbr0,               // Branch if bit 0 reset
M65_bbr1,               // Branch if bit 1 reset
M65_bbr2,               // Branch if bit 2 reset
M65_bbr3,               // Branch if bit 3 reset
M65_bbr4,               // Branch if bit 4 reset
M65_bbr5,               // Branch if bit 5 reset
M65_bbr6,               // Branch if bit 6 reset
M65_bbr7,               // Branch if bit 7 reset
M65_bbs0,               // Branch if bit 0 set
M65_bbs1,               // Branch if bit 1 set
M65_bbs2,               // Branch if bit 2 set
M65_bbs3,               // Branch if bit 3 set
M65_bbs4,               // Branch if bit 4 set
M65_bbs5,               // Branch if bit 5 set
M65_bbs6,               // Branch if bit 6 set
M65_bbs7,               // Branch if bit 7 set
M65_rmb0,               // Reset memory bit 0
M65_rmb1,               // Reset memory bit 1
M65_rmb2,               // Reset memory bit 2
M65_rmb3,               // Reset memory bit 3
M65_rmb4,               // Reset memory bit 4
M65_rmb5,               // Reset memory bit 5
M65_rmb6,               // Reset memory bit 6
M65_rmb7,               // Reset memory bit 7
M65_smb0,               // Set memory bit 0
M65_smb1,               // Set memory bit 1
M65_smb2,               // Set memory bit 2
M65_smb3,               // Set memory bit 3
M65_smb4,               // Set memory bit 4
M65_smb5,               // Set memory bit 5
M65_smb6,               // Set memory bit 6
M65_smb7,               // Set memory bit 7
M65_stz,                // Store zero
M65_tsb,                // Test and set bits
M65_trb,                // Test and reset bits
M65_phy,                // Push Y register
M65_ply,                // Pull Y register
M65_phx,                // Push X register
M65_plx,                // Pull X register
M65_bra,                // Branch always

M65_last,

    };



extern struct instruc_t Instructions[];

// The instruction types (``itype''s)
// m65* CPUs implements.
//
// One should _never_ put anything else in 'ins.hpp'
// but the instruction types: IDA possesses build scripts that 
// will parse ins.hpp files, and build a database of
// <cpu> + <itype> -> <autocomment>
// Where 'autocomment' is the contents of the comment string 
// associated to the itype.
enum m65_itype_t
{
  // http://www.westerndesigncenter.com/wdc/datasheets/Programmanual.pdf
  M65816_null=0, // Unknown Operation
  M65816_adc,    // Add with carry
  M65816_and,    // AND A with memory
  M65816_asl,    // Shift memory or A left
  M65816_bcc,    // Branch if carry clear
  M65816_bcs,    // Branch if carry set
  M65816_beq,    // Branch if equal
  M65816_bit,    // Test memory bits against A
  M65816_bmi,    // Branch if minus
  M65816_bne,    // Branch if not equal
  M65816_bpl,    // Branch if plus
  M65816_bra,    // Branch always
  M65816_brk,    // Software break
  M65816_brl,    // Branch always long
  M65816_bvc,    // Branch if overflow clear
  M65816_bvs,    // Branch if overflow set
  M65816_clc,    // Clear carry flag
  M65816_cld,    // Clear decimal mode flag
  M65816_cli,    // Clear interrupt disable flag
  M65816_clv,    // Clear overflow flag
  M65816_cmp,    // Compare A with memory
  M65816_cop,    // Co-processor enable
  M65816_cpx,    // Compare X with memory
  M65816_cpy,    // Compare Y with memory
  M65816_dec,    // Decrement
  M65816_dex,    // Decrement X
  M65816_dey,    // Decrement Y
  M65816_eor,    // XOR A with M
  M65816_inc,    // Increment
  M65816_inx,    // Increment X
  M65816_iny,    // Increment Y
  M65816_jml,    // Jump long (inter-bank)
  M65816_jmp,    // Jump
  M65816_jsl,    // Jump to subroutine long (inter-bank)
  M65816_jsr,    // Jump to subroutine
  M65816_lda,    // Load A from memory
  M65816_ldx,    // Load X from memory
  M65816_ldy,    // Load Y from memory
  M65816_lsr,    // Logical shift memory or A right
  M65816_mvn,    // Block move next
  M65816_mvp,    // Block move prev
  M65816_nop,    // Nop
  M65816_ora,    // Or A with memory
  M65816_pea,    // Push effective absolute address
  M65816_pei,    // Push effective indirect address
  M65816_per,    // Push effective PC-relative indirect address
  M65816_pha,    // Push A
  M65816_phb,    // Push B (data bank register)
  M65816_phd,    // Push D (direct page register)
  M65816_phk,    // Push K (program bank register)
  M65816_php,    // Push processor status
  M65816_phx,    // Push X
  M65816_phy,    // Push Y
  M65816_pla,    // Pull A
  M65816_plb,    // Pull B
  M65816_pld,    // Pull D
  M65816_plp,    // Pull processor status
  M65816_plx,    // Pull X
  M65816_ply,    // Pull Y
  M65816_rep,    // Reset status bits
  M65816_rol,    // Rotate memory or A left
  M65816_ror,    // Rotate memory or A right
  M65816_rti,    // Return from interrupt
  M65816_rtl,    // Return from subroutine long
  M65816_rts,    // Return from subroutine
  M65816_sbc,    // Subtract with borrow from A
  M65816_sec,    // Set carry flag
  M65816_sed,    // Set decimal mode flag
  M65816_sei,    // Set interrupt disable flag
  M65816_sep,    // Set status bits
  M65816_sta,    // Store A to memory
  M65816_stp,    // Stop processor
  M65816_stx,    // Store X to memory
  M65816_sty,    // Store Y to memory
  M65816_stz,    // Store zero to memory
  M65816_tax,    // Transfer A to X
  M65816_tay,    // Transfer A to Y
  M65816_tcd,    // Transfer 16-bit A to D (direct page register)
  M65816_tcs,    // Transfer A to S
  M65816_tdc,    // Transfer 16-bit D to A
  M65816_trb,    // Test and reset memory bits against A
  M65816_tsb,    // Test and set memory bits against A
  M65816_tsc,    // Transfer S to A
  M65816_tsx,    // Transfer S to X
  M65816_txa,    // Transfer X to A
  M65816_txs,    // Transfer X to S
  M65816_txy,    // Transfer X to Y
  M65816_tya,    // Transfer Y to A
  M65816_tyx,    // Transfer Y to X
  M65816_wai,    // Wait for interrupt
  M65816_wdm,    // Reserved
  M65816_xba,    // Exchange bytes in A
  M65816_xce,    // Exchange carry and emulation bits
  M65816_last
};

/*
 *      Interactive disassembler (IDA)
 *      Copyright (c) 1990-2008 Hex-Rays
 *      PDP11 module.
 *      Copyright (c) 1995-2006 by Iouri Kharon.
 *                        E-mail: yjh@styx.cabel.net
 *
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
pdp_null = 0,           // Unknown Operation

pdp_halt,       // Stop CPU
pdp_wait,       // Wait interrupt
pdp_rti,        // Interrupt return
pdp_bpt,        // Trap to Debbuger
pdp_iot,        // Trap to 20 (i/o)
pdp_reset,      // Reset CPU and device
pdp_rtt,        // Interrupt return and ignore dbg-flag
pdp_mfpt,       // Load Processor Type          (* hi model)
pdp_jmp,        // Absolute jmp
pdp_rts,        // Return into subroutine
pdp_spl,        // Set Prior.
pdp_nop,        // Not operation
pdp_clc,        // Clear C bit in PSW
pdp_clv,        // Clear V bit in PSW
pdp_clz,        // Clear Z bit in PSW
pdp_cln,        // Clear N bit in PSW
pdp_ccc,        // Clear All Condition bits in PSW
pdp_sec,        // Set C bit in PSW
pdp_sev,        // Set V bit in PSW
pdp_sez,        // Set Z bit in PSW
pdp_sen,        // Set N bit in PSW
pdp_scc,        // Set All Condition bits bit in PSW
pdp_swab,       // Exchange byte in word
pdp_br,         // Relative jmp
pdp_bne,        // Jump if Z=0
pdp_beq,        // Jump if Z=1
pdp_bge,        // Jump if N^V=0
pdp_blt,        // Jump if N^V=1
pdp_bgt,        // Jump if Z|(N^V)=0
pdp_ble,        // Jump if Z|(N^V)=1
pdp_jsr,        // Call procedure
pdp_clr,        // Clear operand
pdp_com,        // Inverse operand
pdp_inc,        // Increment operand
pdp_dec,        // Decrement operand
pdp_neg,        // op = -op
pdp_adc,        // Add with Carry
pdp_sbc,        // Substract with Carry
pdp_tst,        // Test operand
pdp_ror,        // Cyclic shift rignt
pdp_rol,        // Cyclic shift left
pdp_asr,        // Arifmetic shift rignt
pdp_asl,        // Arifmetic shift left
pdp_mark,       // Return and empty stack
pdp_mfpi,       // Load from previous instruction space (*hi model)
pdp_mtpi,       // Store to previous instruction space  (*hi model)
pdp_sxt,        // N=>op
pdp_mov,        // Move operand
pdp_cmp,        // Compare operands
pdp_bit,        // Check Bit's
pdp_bic,        // Clear Bit's
pdp_bis,        // Set Bit's
pdp_add,        // Add operands
pdp_sub,        // Substract operands
pdp_mul,        // Multiple Reg          (*eis)
pdp_div,        // Divide Reg            (*eis)
pdp_ash,        // Multistep shift       (*eis)
pdp_ashc,       // Multistep shift 2 reg (*eis)
pdp_xor,        // Exclusive or          (*eis)
pdp_fadd,       // Floating Add         (*fis)
pdp_fsub,       // Floating Subtract    (*fis)
pdp_fmul,       // Floating Multiple    (*fis)
pdp_fdiv,       // Floating Divide      (*fis)
pdp_sob,        //
pdp_bpl,        // Jump if N=0
pdp_bmi,        // Jump if N=1
pdp_bhi,        // Jump if (!C)&(!Z)=0
pdp_blos,       // Jump if C|Z=1
pdp_bvc,        // Jump if V=0
pdp_bvs,        // Jump if V=1
pdp_bcc,        // Jump if C=0
pdp_bcs,        // Jump if C=1
pdp_emt,        // Trap to System
pdp_trap,       // Trap to user/compiler
pdp_mtps,       // Store PSW            (*lsi11 only)
pdp_mfpd,       // Load from previous data space (*hi model)
pdp_mtpd,       // Store to previous data space  (*hi model)
pdp_mfps,       // Load  PSW            (*lsi11 only)
    // FPU instruction
pdp_cfcc,       // Copy cond.codes into FPS to PSW
pdp_setf,       // Set Float
pdp_seti,       // Set Integer
pdp_setd,       // Set Double
pdp_setl,       // Set Long Integer
pdp_ldfps,      // Load FPS
pdp_stfps,      // Store FPS
pdp_stst,       // Load interrupt status
pdp_clrd,       // Clear
pdp_tstd,       // Test
pdp_absd,       // op = mod(op)
pdp_negd,       // op = -op
pdp_muld,       // Multiple
pdp_modd,       // Get int. part
pdp_addd,       // Add
pdp_ldd,        // Load in Acc
pdp_subd,       // Substract
pdp_cmpd,       // Compare
pdp_std,        // Store into Acc
pdp_divd,       // Divide
pdp_stexp,      // Store exponent
pdp_stcdi,      // Store and convert double/float to integer/long
pdp_stcdf,      // Store and convert double/float to float/double
pdp_ldexp,      // Load exponent
pdp_ldcif,      // Load and convert integer/long to double/float
pdp_ldcfd,      // Load and convert float/double to double/float
pdp_call,       // Jsr PC,
pdp_return,     // RTS PC
pdp_compcc,     // Complex Condition Codes

pdp_last

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
mc_null = 0,           // Unknown Operation

mc_abcd,       // summa with BCD correction
mc_add,        // summa with Dn
mc_adda,       // summa with An
mc_addi,       // summa with immediate
mc_addq,       // -"-
mc_addx,       // summa 2 reg and const
mc_and,        // logical and
mc_andi,       // logical and with immediate
mc_asl,        // shift by count
mc_asr,        // -"-
mc_b,          // branch if cond code
mc_bchg,       // bit chang
mc_bclr,       // bit clear
mc_bftst,     // bit field test
mc_bfchg,      // bit field change
mc_bfclr,      // bit field set in 0
mc_bfset,      // bit field set in 1
mc_bfextu,     // bit field extract (unsigned)
mc_bfexts,     // bit field extract (signed)
mc_bfffo,      // bit field offset
mc_bfins,      // bit field include
mc_bgnd,       // CPU32- enter backgound mode
mc_bkpt,       // disable TRAP at any cycles
mc_bra,        // branch
mc_bset,       // bit set
mc_bsr,        // relative call
mc_btst,       // bit test
mc_callm,      // call module [descrtiptor] (020)
mc_cas,        // compare and swap
mc_cas2,       // -"-
mc_chk,        // compare and trap
mc_chk2,       // -"-
mc_cinv,       // invalidate cache
mc_clr,        // clear data
mc_cmp,        // compare data
mc_cmp2,       // -"-
mc_cmpa,       // compare address
mc_cmpi,       // compare immediate
mc_cmpm,       // compare memmory
mc_cpush,      // invalidate cache
mc_db,         // (djnz) if cond code
mc_divs,       // sign divide
mc_divsl,      // -"-
mc_divu,       // unsign didide
mc_divul,      // -"-
mc_eor,        // logical xor
mc_eori,       // logical xor with immediate
mc_exg,        // exchange registers
mc_ext,        // analog cwd
mc_extb,       // analog cwq
mc_fabs,       // FP abs
mc_facos,      // FP acos
mc_fadd,       // FP add
mc_fasin,      // FP asin
mc_fatan,      // FP atan
mc_fatanh,     // FP atanh
mc_fb,         // fBcc
mc_fcmp,       // FP cmp
mc_fcos,       // FP cos
mc_fcosh,      // FP cosh
mc_fdabs,      // FP abs double rounded
mc_fdadd,      // FP add
mc_fdb,        // fDBcc
mc_fddiv,      // FP div double rounded
mc_fdiv,       // FP div
mc_fdmove,     // FP mov double rounded
mc_fdmul,      // FP mul double rounded
mc_fdneg,      // FP neg double rounded
mc_fdsqrt,     // FP sqrt double rounded
mc_fdsub,      // FP sub double rounded
mc_fetox,      // FP e**x
mc_fetoxm1,    // FP e**x-1
mc_fgetexp,    // FP extract exponent
mc_fgetman,    // FP extract mantissa
mc_fint,       // FP get integer part
mc_fintrz,     // FP get integer part by round
mc_flog2,      // FP log[2]
mc_flog10,     // FP log[10]
mc_flogn,      // FP log[e]
mc_flognp1,    // FP log[e] x+1
mc_fmod,       // FP mod
mc_fmove,      // move float
mc_fmovecr,    // move floating const
mc_fmovem,     // move float register list
mc_fmul,       // FM mul
mc_fneg,       // FP neg
mc_fnop,       // float nop
mc_frem,       // FP rem
mc_frestore,   // restore status
mc_fs,         // fScc
mc_fsabs,      // FP abs single rounded
mc_fsadd,      // FP add single rounded
mc_fsave,      // Fsave status
mc_fscale,     // FP *2**x
mc_fsdiv,      // FP div single rounded
mc_fsgldiv,    // FP single div
mc_fsglmul,    // FP single mul
mc_fsin,       // FP sin
mc_fsincos,    // FP sin & cos
mc_fsinh,      // FP sinh
mc_fsmove,     // FP mov single rounded
mc_fsmul,      // FP mul single rounded
mc_fsneg,      // FP neg single rounded
mc_fsqrt,      // FP sqrt
mc_fssqrt,     // FP sqrt single rounded
mc_fssub,      // FP sub single rounded
mc_fsub,       // FP sub
mc_ftan,       // FP tan
mc_ftanh,      // FP tanh
mc_ftentox,    // FP 10**x
mc_ftrap,      // fTRAPcc
mc_ftst,       // FP tst
mc_ftwotox,    // FP 2**x
mc_halt,       // (coldfire) Halt the processor
mc_illegal,    // trap to vector
mc_jmp,        // absolute Jmp
mc_jsr,        // absolute jsr
mc_lea,        // load effective address
mc_link,       // change sp context
mc_lpstop,     // cpu32 - stop
mc_lsl,        // logical shift
mc_lsr,        // -"-
mc_mac,        // multiple and accumulate
mc_macl,       // multiple and accumulate with register load
mc_move,       // move memmory
mc_move16,     // move 16 byte
mc_movea,      // move move with address reg
mc_movec,      // move from/to control register
mc_movem,      // move register list
mc_movep,      // move with data register
mc_moveq,      // move with immediate
mc_moves,      // mtpd/mfpd
mc_msac,       // multiple and subtract
mc_msacl,      // multiple and subtract with register load
mc_muls,       // sign multiplication
mc_mulu,       // unsign multiplication
mc_nbcd,       // negative with BCD correct
mc_neg,        // negative
mc_negx,       // negative with immediate
mc_nop,        // null operation
mc_not,        // logical not
mc_or,         // logical or
mc_ori,        // logical or with immediate
mc_pack,       // UNP(bcd)+adj -> PACK(bcd)
mc_pea,        // lea into stack
mc_pb,         // 68851 pbcc
mc_pdb,        // 68851 pdbcc
mc_pflush,     // invalidate cache
mc_pflushr,    // 68851 load root entry
mc_ploadr,     // 68851
mc_ploadw,     // 68851
mc_pmove,      // move MMU register
mc_prestore,   // 68851
mc_psave,      // 68851
mc_ps,         // 68851 pscc
mc_ptestr,     // test a logical address
mc_ptestw,     // -"-
mc_ptrap,      // 68851 ptrapcc
mc_pulse,      // (coldfire) generate unique PST value
mc_pvalid,     // 68851 move AL bit
mc_rol,        // cycl. shift
mc_ror,        // -"-
mc_roxl,       // two op cycl shift
mc_roxr,       // -"-
mc_reset,      // reset
mc_rtd,        // mark
mc_rte,        // return with SR
mc_rtm,        // reload save module state
mc_rtr,        // return with CCR
mc_rts,        // return
mc_sbcd,       // substract with BCD correction
mc_s,          // 1 if cond code else 0
mc_stop,       // halt
mc_sub,        // substract
mc_suba,       // substract address
mc_subi,       // substract immediate
mc_subq,       // -"-
mc_subx,       // substract memmory
mc_swap,       // hi and low word in register
mc_tas,        // test byte sign
mc_tbl,        // CPU32-tblu/tblun/tbls/tblsn
mc_trap,       // analog int(86) (or trap if cond code)
mc_trapv,      // if V then trap
mc_tst,        // test destination
mc_unlk,       // destroy address ?
mc_unpk,       // PACK(bcd)+adj -> UNPACK(bcd)
mc_wddata,     // (coldfire) load WDDATA port (pins)
mc_wdebug,     // (coldfire) load debug register
mc_atrap,      // MacOS atrap

// new ColdFire instructions
mc_bitrev,     // bit reverse
mc_byterev,    // byte reverse
mc_ff1,        // find first one
mc_intouch,    // instruction fetch touch
mc_mov3q,      // move 3-bit data quick
mc_mvs,        // move with sign extend
mc_mvz,        // move with zero-fill
mc_sats,       // signed saturate
mc_movclr,     // move from accumulator and clear
mc_maaac,      // Multiply and Add to 1st Accumulator, Add to 2nd Accumulator
mc_masac,      // Multiply and Add to 1st Accumulator, Subtract from 2nd Accumulator
mc_msaac,      // Multiply and Subtract to 1st Accumulator, Add to 2nd Accumulator
mc_mssac,      // Multiply and Subtract to 1st Accumulator, Subtract to 2nd Accumulator

mc_remsl,      // Signed Divide Remainder
mc_remul,      // Unsigned Divide Remainder

mc_last
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
mc8_null = 0,           // Unknown Operation

mc8_aba,        // Add b to a                                  (11)
mc8_ab,         // Add b to index register
mc8_adc,        // Add with carry
mc8_add,        // Add without carry
mc8_addd,       // Add 16-bit to D                             (11)
mc8_ais,        // Add immediate value to stack pointer        (8)
mc8_aix,        // Add immediate value to index register (H:X) (8)
mc8_and,        // Logical AND
mc8_andcc,      // Logical AND immdiate with CCR (9)
mc8_asr,        // Ariphmetic shift right
mc8_bcc,        // Branch if carry bit clear
mc8_bclr,       // Clear bit in memory
mc8_bcs,        // Branch if carry bit set
mc8_beq,        // Branch if equal
mc8_bge,        // Branch if >= (signed)
mc8_bgt,        // Branch if > (signed)
mc8_bhcc,       // Branch if half carry bit clear (8)
mc8_bhcs,       // Branch if half carry bit set   (8)
mc8_bhi,        // Branch if > (unsigned)
mc8_bhs,        // Branch if >= (unsigned)        (11)
mc8_bih,        // Branch if IRQ pin high         (8)
mc8_bil,        // Branch if IRQ pin low          (8)
mc8_bit,        // Bit test                       (8)
mc8_ble,        // Branch if <= (signed)          (8)
mc8_blo,        // Branch if <  (unsigned)        (11)
mc8_bls,        // Branch if <= (unsigned)        (8)
mc8_blt,        // Branch if < (signed)           (8)
mc8_bmc,        // Branch if interrupt mask clear (8)
mc8_bmi,        // Branch if minus
mc8_bms,        // Branch if interrupt mask set   (8)
mc8_bne,        // Branch if not equal
mc8_bpl,        // Branch if not plus
mc8_bra,        // Branch always
mc8_brclr,      // Branch if bit (n) in memory clear
mc8_brn,        // Branch never
mc8_brset,      // Branch if bit (n) in memory set
mc8_bset,       // Set bit in memory
mc8_bsr,        // Branch to subroutine
mc8_bvc,        // Branch if overflow cleared     (11)
mc8_bvs,        // Branch if overflow set         (11)
mc8_cba,        // Compare A to B                 (11)
mc8_cbeq,       // Compare and branch if equal    (8)
mc8_clc,        // Clear carry bit
mc8_cli,        // Clear interrupt mask
mc8_clr,        // Clear
mc8_clv,        // Clear overflow flag            (11)
mc8_cmp,        // Compare accumulator with memory
mc8_com,        // Complement
mc8_cp,         // Compare index register 16 bit  (11)
mc8_cpd,        // Compare D 16 bit               (11)
mc8_cphx,       // Compare index register with memory          (8)
mc8_cpx,        // Compare X (index register low) with memory  (8)
mc8_cwai,       // Clear CC bits & wait for interrupt          (9)
mc8_daa,        // Decimal adjust accumulator
mc8_dbnz,       // Decrement and branch if not equal   (8)
mc8_de,         // Decrement index register            (11)
mc8_dec,        // Decrement
mc8_des,        // Decrement stack pointer             (11)
mc8_div,        // Divide                              (8)
mc8_eor,        // Excusive OR memory with accumulator
mc8_exg,        // Exchange register                   (9)
mc8_fdiv,       // Fractional divide 16 by 16          (11)
mc8_idiv,       // Integer divide 16 by 16             (11)
mc8_in,         // Increment index register            (11)
mc8_inc,        // Increment
mc8_ins,        // Increment stack pointer             (11)
mc8_jmp,        // Jump
mc8_jsr,        // Jump to subroutine
mc8_ld,         // Load index register                     (11)
mc8_lda,        // Load accumulator from memory
mc8_ldd,        // Load double accumulator                 (11)
mc8_ldhx,       // Load index register from memory         (8)
mc8_lds,        // Load stack pointer                      (11)
mc8_ldx,        // Load X (index register low) from memory (8)
mc8_lea,        // Load register from memory               (9)
mc8_lsl,        // Logical (or Ariphmetic) shift left
mc8_lsld,       // Logical (or Ariphmetic) shift left double (11)
mc8_lsr,        // Logical shift right
mc8_lsrd,       // Logical shift right double                (11)
mc8_mov,        // Move                          (8)
mc8_mul,        // Unsigned multiply
mc8_neg,        // Negate
mc8_nop,        // No Operation
mc8_nsa,        // Nibble swap accumulator       (8)
mc8_ora,        // Inclusive OR memory with accumulator
mc8_orcc,       // Inclusive OR immediate with CCR            (9)
mc8_psh,        // Push index register into stack (low first) (11)
mc8_psha,       // Push accumulator A into stack
mc8_pshb,       // Push accumulator B into stack              (11)
mc8_pshh,       // Push H (index register high) into stack    (8)
mc8_pshx,       // Push X (index register low) into stack     (8)
mc8_pul,        // Pull index register from stack (hi first)  (11)
mc8_pula,       // Pull accumulator A from stack
mc8_pulb,       // Pull accumulator B from stack              (11)
mc8_pulh,       // Pull H (index register high) from stack    (8)
mc8_pulx,       // Pull X (index register low) from stack     (8)
mc8_rol,        // Rotate left trough carry
mc8_ror,        // Rotate right trough carry
mc8_rsp,        // Reset stack pointer               (8)
mc8_rti,        // Reset from interrupt
mc8_rts,        // Reset from subroutine
mc8_sba,        // Subtract B from A                       (11)
mc8_sbc,        // Subtract with carry
mc8_sec,        // Set carry bit
mc8_sei,        // Set interrupt mask bit
mc8_sev,        // Set Overflow Flag                       (11)
mc8_sex,        // Sign extended                           (9)
mc8_slp,        // Sleep    (HD63701 extention)            (11)
mc8_st,         // Store index register in memory          (11)
mc8_sta,        // Store accumulator in memory
mc8_std,        // Store accumulators in memory            (11)
mc8_sthx,       // Store H:X (index reg)                   (8)
mc8_stop,       // Enable IRQ pin. Stop oscillator
mc8_sts,        // Store stack pointer                     (11)
mc8_stx,        // Store X (index register low) in memory  (8)
mc8_sub,        // Subtract
mc8_subd,       // Subtract 16-bit                         (11)
mc8_swi,        // Software interrupt
mc8_sync,       // Syncronisation with interrupt           (9)
mc8_tab,        // Transfer A to B                                (11)
mc8_tap,        // Transfer A to CCR
mc8_tax,        // Transfer accumulator to X (index register low) (8)
mc8_tba,        // Transfer B to A                                (11)
mc8_test,       // Address Bus Counts (in test mode)              (11)
mc8_tfr,        // Transfer register to register                  (9)
mc8_tpa,        // Transfer CCR to A
mc8_ts,         // Transfer SP to index reg                       (11)
mc8_tst,        // Test for negative or zero
mc8_tsx,        // Transfer SP to index reg                       (8)
mc8_txa,        // Transfer X (index reg low) to accumulator      (8)
mc8_txs,        // Transfer index reg X to SP
mc8_tys,        // Transfer index reg Y to SP                     (11)
mc8_wai,        // Wait for interrupt                   (11)
mc8_wait,       // Enable interrupt. Stop processor     (8)
mc8_xgd,        // Exchange D with index register       (11)
mc8_1,          // Special testing command              (11)
mc8_2,          // Special testing command              (11)
mc8_os9,        // OS9 system call                      (9)
// these instructions must be continuous (HD63701 extention)
mc8_aim,
mc8_oim,
mc8_eim,
mc8_tim,

// new S08 instructions
mc8_bgnd,       // Background                                     (8)
mc8_call,       // Call Subroutine                                (8)
mc8_rtc,        // Return fom Call                                (8)

// Pseudoinstructions
mc8_skip1,      // Skip one byte
mc8_skip2,      // Skip two bytes

mc8_last

    };

/*
 *      Interactive disassembler (IDA)
 *      Copyright (c) 1990-2008 Hex-Rays
 *      JVM module.
 *      Copyright (c) 1995-2006 by Iouri Kharon.
 *                        E-mail: yjh@styx.cabel.net
 *
 *      ALL RIGHTS RESERVED.
 *
 */



extern instruc_t Instructions[];
enum 
{
//j_null = 0,           // Unknown Operation
j_nop = 0,      //  0   //Do nothing
j_aconst_null,  //  1   //Push null object reference
j_iconst_m1,    //  2   //Push integer constant -1
j_iconst_0,     //  3   //Push the integer 0
j_iconst_1,     //  4   //Push the integer 1
j_iconst_2,     //  5   //Push the integer 2
j_iconst_3,     //  6   //Push the integer 3
j_iconst_4,     //  7   //Push the integer 4
j_iconst_5,     //  8   //Push the integer 5
j_lconst_0,     //  9   //Push the long integer 0
j_lconst_1,     // 10   //Push the long integer 1
j_fconst_0,     // 11   //Push the single-precision foating point 0.0
j_fconst_1,     // 12   //Push the single-precision foating point 1.0
j_fconst_2,     // 13   //Push the single-precision foating point 2.0
j_dconst_0,     // 14   //Push the double-precision foating point 2.0
j_dconst_1,     // 15   //Push the double-precision foating point 2.0
j_bipush,       // 16   //Push one byte signed integer
j_sipush,       // 17   //Push two-byte signed integer
j_ldc,          // 18   //Push item from constant pool (i1)
j_ldcw,         // 19   //Push item from constant pool (i2)
j_ldc2w,        // 20   //Push long or double from constant pool
j_iload,        // 21   //Push integer value of the local variable
j_lload,        // 22   //Push long value of the local variable
j_fload,        // 23   //Push single float value of the local variable
j_dload,        // 24   //Push double float value of the local variable
j_aload,        // 25   //Push object reference from the local variable
j_iload_0,      // 26   //Push integer value of the local variable #0
j_iload_1,      // 27   //Push integer value of the local variable #1
j_iload_2,      // 28   //Push integer value of the local variable #2
j_iload_3,      // 29   //Push integer value of the local variable #3
j_lload_0,      // 30   //Push long value of the local variable #0
j_lload_1,      // 31   //Push long value of the local variable #1
j_lload_2,      // 32   //Push long value of the local variable #2
j_lload_3,      // 33   //Push long value of the local variable #3
j_fload_0,      // 34   //Push single float value of the local variable #0
j_fload_1,      // 35   //Push single float value of the local variable #1
j_fload_2,      // 36   //Push single float value of the local variable #2
j_fload_3,      // 37   //Push single float value of the local variable #3
j_dload_0,      // 38   //Push double float value of the local variable #0
j_dload_1,      // 39   //Push double float value of the local variable #1
j_dload_2,      // 40   //Push double float value of the local variable #2
j_dload_3,      // 41   //Push double float value of the local variable #3
j_aload_0,      // 42   //Push object reference from the local variable #0
j_aload_1,      // 43   //Push object reference from the local variable #1
j_aload_2,      // 44   //Push object reference from the local variable #2
j_aload_3,      // 45   //Push object reference from the local variable #3
j_iaload,       // 46   //Push integer from array
j_laload,       // 47   //Push long from array
j_faload,       // 48   //Push single float from array
j_daload,       // 49   //Push double float from array
j_aaload,       // 50   //Push object refernce from array
j_baload,       // 51   //Push signed byte from array
j_caload,       // 52   //Push character from array
j_saload,       // 53   //Push short from array
j_istore,       // 54   //Pop integer value into local variable
j_lstore,       // 55   //Pop long value into local variable
j_fstore,       // 56   //Pop single float value into local variable
j_dstore,       // 57   //Pop double float value into local variable
j_astore,       // 58   //Pop object refernce into local variable
j_istore_0,     // 59   //Pop integer value into local variable #0
j_istore_1,     // 60   //Pop integer value into local variable #1
j_istore_2,     // 61   //Pop integer value into local variable #2
j_istore_3,     // 62   //Pop integer value into local variable #3
j_lstore_0,     // 63   //Pop long value into local variable #0
j_lstore_1,     // 64   //Pop long value into local variable #1
j_lstore_2,     // 65   //Pop long value into local variable #2
j_lstore_3,     // 66   //Pop long value into local variable #3
j_fstore_0,     // 67   //Pop single float value into local variable #0
j_fstore_1,     // 68   //Pop single float value into local variable #1
j_fstore_2,     // 69   //Pop single float value into local variable #2
j_fstore_3,     // 70   //Pop single float value into local variable #3
j_dstore_0,     // 71   //Pop double float value into local variable
j_dstore_1,     // 72   //Pop double float value into local variable #0
j_dstore_2,     // 73   //Pop double float value into local variable #1
j_dstore_3,     // 74   //Pop double float value into local variable #2
j_astore_0,     // 75   //Pop object refernce into local variable #0
j_astore_1,     // 76   //Pop object refernce into local variable #1
j_astore_2,     // 77   //Pop object refernce into local variable #2
j_astore_3,     // 78   //Pop object refernce into local variable #3
j_iastore,      // 79   //Pop integer from array
j_lastore,      // 80   //Pop long from array
j_fastore,      // 81   //Pop single float from array
j_dastore,      // 82   //Pop double float from array
j_aastore,      // 83   //Pop object refernce from array
j_bastore,      // 84   //Pop signed byte from array
j_castore,      // 85   //Pop character from array
j_sastore,      // 86   //Pop short from array
j_pop,          // 87   //Pop top stack word
j_pop2,         // 88   //Pop top two stack word
j_dup,          // 89   //Duplicate top stack word
j_dup_x1,       // 90   //Duplicate top stack word and put two down
j_dup_x2,       // 91   //Duplicate top stack word and put three down
j_dup2,         // 92   //Duplicate top two stack word
j_dup2_x1,      // 93   //Duplicate top two stack words and put two down
j_dup2_x2,      // 94   //Duplicate top two stack words and put three down
j_swap,         // 95   //Swap two top stack words
j_iadd,         // 96   //Integer add
j_ladd,         // 97   //Long add
j_fadd,         // 98   //Single float add
j_dadd,         // 99   //Double float add
j_isub,         //100   //Integer subtract
j_lsub,         //101   //Long subtract
j_fsub,         //102   //Single float subtract
j_dsub,         //103   //Double Float subtract
j_imul,         //104   //Integer multiply
j_lmul,         //105   //Long multiply
j_fmul,         //106   //Single float multiply
j_dmul,         //107   //Double Float multiply
j_idiv,         //108   //Integer divide
j_ldiv,         //109   //Long divide
j_fdiv,         //110   //Single float divide
j_ddiv,         //111   //Double Float divide
j_irem,         //112   //Integer reminder
j_lrem,         //113   //Long reminder
j_frem,         //114   //Single float reminder
j_drem,         //115   //Double Float reminder
j_ineg,         //116   //Integer negate
j_lneg,         //117   //Long negate
j_fneg,         //118   //Single float negate
j_dneg,         //119   //Double Float negate
j_ishl,         //120   //Integer shift left
j_lshl,         //121   //Long shift left
j_ishr,         //122   //Integer logical shift right
j_lshr,         //123   //Long logical shift right
j_iushr,        //124   //Integer arithmetic shift right
j_lushr,        //125   //Long arithmeticshift right
j_iand,         //126   //Integer boolean AND
j_land,         //127   //Long boolean AND
j_ior,          //128   //Integer boolean OR
j_lor,          //129   //Long boolean OR
j_ixor,         //130   //Integer boolean XOR
j_lxor,         //131   //Long boolean XOR
j_iinc,         //132   //Add 8-bit signed const to local variable
j_i2l,          //133   //Integer to Long conversion
j_i2f,          //134   //Integer to single float conversion
j_i2d,          //135   //Integer to double float conversion
j_l2i,          //136   //Long to Integer conversion
j_l2f,          //137   //Long to single float conversion
j_l2d,          //138   //Long to double float conversion
j_f2i,          //139   //Single float to Integer conversion
j_f2l,          //140   //Single float to Long conversion
j_f2d,          //141   //Single float to double float conversion
j_d2i,          //142   //Double float to Integer conversion
j_d2l,          //143   //Double float to Long conversion
j_d2f,          //144   //Double float to double float conversion
j_i2b,          //145   //Integer to signed byte conversion
j_i2c,          //146   //Integer to unsigned short conversion
j_i2s,          //147   //Integer to signed short conversion
j_lcmp,         //148   //Long compare
j_fcmpl,        //149   //Single float compare (-1 on NaN)
j_fcmpg,        //150   //Single float compare (1 on NaN)
j_dcmpl,        //151   //Double float compare (-1 on NaN)
j_dcmpg,        //152   //Double float compare (1 on NaN)
j_ifeq,         //153   //Branch if equal to 0
j_ifne,         //154   //Branch if not equal to 0
j_iflt,         //155   //Branch if less then 0
j_ifge,         //156   //Branch if greater than or equal to 0
j_ifgt,         //157   //Branch if greater than 0
j_ifle,         //158   //Branch if less than or equal to 0
j_if_icmpeq,    //159   //Branch if integers equal
j_if_icmpne,    //160   //Branch if integers not equal
j_if_icmplt,    //161   //Branch if integers less than
j_if_icmpge,    //162   //Branch if integers grater than or equal to
j_if_icmpgt,    //163   //Branch if integers grater than
j_if_icmple,    //164   //Branch if integers less than or equal to
j_if_acmpeq,    //165   //Branch if object references are equal
j_if_acmpne,    //166   //Branch if object references not equal
j_goto,         //167   //Branch always
j_jsr,          //168   //Jump subroutine
j_ret,          //169   //Return from subroutine
j_tableswitch,  //170   //Access jump table by index and jump
j_lookupswitch, //171   //Access jump table by key match and jump
j_ireturn,      //172   //Return integer from function
j_lreturn,      //173   //Return long from function
j_freturn,      //174   //Return single floatr from function
j_dreturn,      //175   //Return double float from function
j_areturn,      //176   //Return object reference from function
j_return,       //177   //Return (void) from procedure
j_getstatic,    //178   //Set static field from class
j_putstatic,    //179   //Set static field in class
j_getfield,     //180   //Fetch field from object
j_putfield,     //181   //Set field in object
j_invokevirtual, //182  //invoke instance method
j_invokespecial, //183  //invoke instance method (superclass/init/...)
j_invokestatic, //184  //invoke a class (static) method
j_invokeinterface, //185  //invoke interface method
j_invokedynamic, //186   // invoke instance method (select by paraneter)
j_new,          //187   //Create new object
j_newarray,     //188   //Allocate new array
j_anewarray,    //189   //Allocate new array of refernces to object
j_arraylength,  //190   //Get length of array
j_athrow,       //191   //Throw exception or error
j_checkcast,    //192   //Make sure object is of given type
j_instanceof,   //193   //Determine if an object is of given type
j_monitorenter, //194   //Enter monitored region of code
j_monitorexit,  //195   //Exit monitored region of code
j_wide,         //196   //wide (prefix of command)
j_multianewarray, //197 //Allocate new multi-dimensional array
j_ifnull,       //198   //Branch if NULL-ptr
j_ifnonnull,    //199   //Branch if not NULL-ptr
j_goto_w,       //200   //Branch always (wide index)
j_jsr_w,        //201   //Jump subroutine (wide index)
j_breakpoint,   //202   //Stop and pass control to breakpoint handler
//
j_lastnorm,
j_a_invokesuper = j_lastnorm,
j_a_invokevirtualobject,
j_a_invokeignored,
//bottom of table ! (emu)
j_a_software,
j_a_hardware,
//
j_last
    };

enum name_quick {
j_ldc_quick = j_lastnorm,       //203     (18)
j_ldcw_quick,                   //204     (19)
j_ldc2w_quick,                  //205     (20)
j_getfield_quick,               //206     (180)
j_putfield_quick,               //207     (181)
j_getfield2_quick,              //208
j_putfield2_quick,              //209
j_getstatic_quick,              //210     (178)
j_putstatic_quick,              //211     (179)
j_getstatic2_quick,             //212
j_putstatic2_quick,             //213
j_invokevirtual_quick,          //214     (182)
j_invokenonvirtual_quick,       //215     (183)
j_invokesuper_quick,            //216
j_invokestatic_quick,           //217     (184)
j_invokeinterface_quick,        //218     (185)
j_invokevirtualobject_quick,    //219
j_invokeignored_quick,          //220
j_new_quick,                    //221     (187)
j_anewarray_quick,              //222     (189)
j_multianewarray_quick,         //223     (197)
j_checkcast_quick,              //224     (192)
j_instanceof_quick,             //225     (193)
j_invokevirtual_quick_w,        //226
j_getfield_quick_w,             //227
j_putfield_quick_w,             //228
j_quick_last
    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
ARM_null = 0,           // Unknown Operation
ARM_ret,                // Return from Subroutine
ARM_nop,                // No Operation
ARM_b,                  // Branch
ARM_bl,                 // Branch with Link
ARM_asr,                // Arithmetic Shift Right
ARM_lsl,                // Logical Shift Left
ARM_lsr,                // Logical Shift Right
ARM_ror,                // Rotate Right
ARM_neg,                // Negate
ARM_and,                // 0 Rd = Op1 & Op2
ARM_eor,                // 1 Rd = Op1 ^ Op2
ARM_sub,                // 2 Rd = Op1 - Op2
ARM_rsb,                // 3 Rd = Op2 - Op1
ARM_add,                // 4 Rd = Op1 + Op2
ARM_adc,                // 5 Rd = Op1 + Op2 + C
ARM_sbc,                // 6 Rd = Op1 - Op2 + C - 1
ARM_rsc,                // 7 Rd = Op2 - Op1 + C - 1
ARM_tst,                // 8 Set cond. codes on Op1 & Op2
ARM_teq,                // 9 Set cond. codes on Op1 ^ Op2
ARM_cmp,                // A Set cond. codes on Op1 - Op2
ARM_cmn,                // B Set cond. codes on Op1 + Op2
ARM_orr,                // C Rd = Op1 | Op2
ARM_mov,                // D Rd = Op2
ARM_bic,                // E Rd = Op1 & ~Op2
ARM_mvn,                // F Rd = ~Op2
ARM_mrs,                // Transfer PSR to Register
ARM_msr,                // Transfer Register to PSR
ARM_mul,                // Multiply
ARM_mla,                // Multiply-Accumulate
ARM_ldr,                // Load from Memory
ARM_ldrpc,              // Indirect Jump
ARM_str,                // Store to Memory
ARM_ldm,                // Load Block from Memory
ARM_stm,                // Store Block to Memory
ARM_swp,                // Single Data Swap
ARM_svc,                // Supervisor call

// Version 4

ARM_smull,              // Signed Multiply long
ARM_smlal,              // Signed Multiply-Accumulate long
ARM_umull,              // Unsigned Multiply long
ARM_umlal,              // Unsigned Multiply-Accumulate long
ARM_bx,                 // Branch to/from Thumb mode
ARM_pop,                // Pop registers
ARM_push,               // Push registers
ARM_adr,                // Load address

// Version 5

ARM_bkpt,               // Breakpoint
ARM_blx1,               // Branch with Link and Exchange (immediate address)
ARM_blx2,               // Branch with Link and Exchange (register indirect)
ARM_clz,                // Count Leading Zeros

// Version 5E

ARM_ldrd,               // Load pair of registers
ARM_pld,                // Prepare to load data
ARM_qadd,               // Saturated addition
ARM_qdadd,              // Saturated addition with doubling
ARM_qdsub,              // Saturated subtraction with doubling
ARM_qsub,               // Saturated subtraction
ARM_smlabb,             // Signed multiply-accumulate (bottom*bottom)
ARM_smlatb,             // Signed multiply-accumulate (top*bottom)
ARM_smlabt,             // Signed multiply-accumulate (bottom*top)
ARM_smlatt,             // Signed multiply-accumulate (top*top)
ARM_smlalbb,            // Long signed multiply-accumulate (bottom*bottom)
ARM_smlaltb,            // Long signed multiply-accumulate (top*bottom)
ARM_smlalbt,            // Long signed multiply-accumulate (bottom*top)
ARM_smlaltt,            // Long signed multiply-accumulate (top*top)
ARM_smlawb,             // Wide signed multiply-accumulate (bottom)
ARM_smulwb,             // Wide signed multiply (bottom)
ARM_smlawt,             // Wide signed multiply-accumulate (top)
ARM_smulwt,             // Wide signed multiply (top)
ARM_smulbb,             // Signed multiply (bottom*bottom)
ARM_smultb,             // Signed multiply (top*bottom)
ARM_smulbt,             // Signed multiply (bottom*top)
ARM_smultt,             // Signed multiply (top*top)
ARM_strd,               // Store pair of registers

// Intel xScale coprocessor instructions

xScale_mia,             // Multiply-Internal Accumulate
xScale_miaph,           // Multiply-Internal Accumulate Packed HalfWords
xScale_miabb,           // Multiply-Internal Accumulate Bottom-Bottom Halves
xScale_miabt,           // Multiply-Internal Accumulate Bottom-Top Halves
xScale_miatb,           // Multiply-Internal Accumulate Top-Bottom Halves
xScale_miatt,           // Multiply-Internal Accumulate Top-Top Halves
xScale_mar,             // Move To Internal Accumulator
xScale_mra,             // Move From Internal Accumulator

// Macro instructions

ARM_movl,               // Move immediate to register
ARM_adrl,               // Load address
ARM_swbkpt,             // WinCE Debugger break

// Coprocessor instructions

ARM_cdp,                // Coprocessor Data Processing
ARM_cdp2,               // Coprocessor Data Processing
ARM_ldc,                // Load Coprocessor Register
ARM_ldc2,               // Load Coprocessor Register
ARM_stc,                // Store Coprocessor Register
ARM_stc2,               // Store Coprocessor Register
ARM_mrc,                // Move from Coprocessor to ARM Register
ARM_mrc2,               // Move from Coprocessor to ARM Register
ARM_mcr,                // Move from ARM to Coprocessor Register
ARM_mcr2,               // Move from ARM to Coprocessor Register
ARM_mcrr,               // Copy pair of registers to coprocessor (5E)
ARM_mrrc,               // Copy pair of registers from coprocessor (5E)

// VFP instructions

ARM_fabsd,              // Floating point Absolute Value, Double precision
ARM_fabss,              // Floating point Absolute Value, Single precision
ARM_faddd,              // Floating point Addition, Double precision
ARM_fadds,              // Floating point Addition, Single precision
ARM_fcmpd,              // Floating point Compare, Double precision
ARM_fcmps,              // Floating point Compare, Single precision
ARM_fcmped,             // Floating point Compare (NaN Exceptions), Double precision
ARM_fcmpes,             // Floating point Compare (NaN Exceptions), Single precision
ARM_fcmpezd,            // Floating point Compare (NaN Exceptions) with Zero, Double precision
ARM_fcmpezs,            // Floating point Compare (NaN Exceptions) with Zero, Single precision
ARM_fcmpzd,             // Floating point Compare with Zero, Double precision
ARM_fcmpzs,             // Floating point Compare with Zero, Single precision
ARM_fcpyd,              // Floating point Copy, Double precision
ARM_fcpys,              // Floating point Copy, Single precision
ARM_fcvtsd,             // Floating point Convert to Single precision from Double precision
ARM_fcvtds,             // Floating point Convert to Double precision from Single precision
ARM_fdivd,              // Floating point Divide, Double precision
ARM_fdivs,              // Floating point Divide, Single precision
ARM_fldd,               // Floating point Load, Double precision
ARM_flds,               // Floating point Load, Single precision
ARM_fldmd,              // Floating point Load Multiple, Double precision
ARM_fldms,              // Floating point Load Multiple, Single precision
ARM_fldmx,              // Floating point Load Multiple, Unknown precision
ARM_fmacd,              // Floating point Multiply and Accumulate, Double precision
ARM_fmacs,              // Floating point Multiply and Accumulate, Single precision
ARM_fmscd,              // Floating point Multiply and Subtract, Double precision
ARM_fmscs,              // Floating point Multiply and Subtract, Single precision
ARM_fmstat,             // Floating point Move Status
ARM_fmuld,              // Floating point Multiply, Double precision
ARM_fmuls,              // Floating point Multiply, Single precision
ARM_fnegd,              // Floating point Negate, Double precision
ARM_fnegs,              // Floating point Negate, Single precision
ARM_fnmacd,             // Floating point Negated Multiply and Accumulate, Double precision
ARM_fnmacs,             // Floating point Negated Multiply and Accumulate, Single precision
ARM_fnmscd,             // Floating point Negated Multiply and Subtract, Double precision
ARM_fnmscs,             // Floating point Negated Multiply and Subtract, Single precision
ARM_fnmuld,             // Floating point Negated Multiply, Double precision
ARM_fnmuls,             // Floating point Negated Multiply, Single precision
ARM_fsitod,             // Floating point Convert Signed Integer to Double precision
ARM_fsitos,             // Floating point Convert Signed Integer to Single precision
ARM_fsqrtd,             // Floating point Square Root, Double precision
ARM_fsqrts,             // Floating point Square Root, Single precision
ARM_fstd,               // Floating point Store, Double precision
ARM_fsts,               // Floating point Store, Single precision
ARM_fstmd,              // Floating point Store Multiple, Double precision
ARM_fstms,              // Floating point Store Multiple, Single precision
ARM_fstmx,              // Floating point Store Multiple, Unknown precision
ARM_fsubd,              // Floating point Subtract, Double precision
ARM_fsubs,              // Floating point Subtract, Single precision
ARM_ftosid,             // Floating point Convert to Signed Integer from Double precision
ARM_ftosis,             // Floating point Convert to Signed Integer from Single precision
ARM_ftosizd,            // Floating point Convert to Signed Integer from Double precision, RZ mode
ARM_ftosizs,            // Floating point Convert to Signed Integer from Single precision, RZ mode
ARM_ftouid,             // Floating point Convert to Unsigned Integer from Double precision
ARM_ftouis,             // Floating point Convert to Unsigned Integer from Single precision
ARM_ftouizd,            // Floating point Convert to Unsigned Integer from Double precision, RZ mode
ARM_ftouizs,            // Floating point Convert to Unsigned Integer from Single precision, RZ mode
ARM_fuitod,             // Floating point Convert Unsigned Integer to Double precision
ARM_fuitos,             // Floating point Convert Unsigned Integer to Single precision
ARM_fmdhr,              // Floating point Move to Double precision High from Register
ARM_fmrdh,              // Floating point Move to Register from Double precision High
ARM_fmdlr,              // Floating point Move to Double precision Low from Register
ARM_fmrdl,              // Floating point Move to Register from Double precision Low
ARM_fmxr,               // Floating point Move to System Register from Register
ARM_fmrx,               // Floating point Move to Register from System Register
ARM_fmsr,               // Floating point Move to Single precision from Register
ARM_fmrs,               // Floating point Move to Register from Single precision

// VFP ARMv5TE extensions

ARM_fmdrr,              // Floating point Move to Double precision from two Registers
ARM_fmrrd,              // Floating point Move to two Registers from Double precision
ARM_fmsrr,              // Floating point Move to two Single precision from two Registers
ARM_fmrrs,              // Floating point Move to two Registers from two Single precision

// ARM v5J instructions

ARM_bxj,                // Branch to Jazelle

// ARM v6 instructions

ARM_mcrr2,              // Move to Coprocessor from two ARM Registers
ARM_mrrc2,              // Move to two ARM Registers from Coprocessor
ARM_cps,                // Change Processor State
ARM_cpsid,              // Disable Interrupts
ARM_cpsie,              // Enable Interrupts
ARM_ldrex,              // Load Register Exclusive
ARM_pkhbt,              // Pack halfword bottom + top
ARM_pkhtb,              // Pack halfword top + bottom
ARM_qadd16,             // Signed saturating arithmetic hafword-wise addition
ARM_qadd8,              // Signed saturating arithmetic byte-wise addition
ARM_qaddsubx,           // Signed saturating arithmetic exchange, add, substract
ARM_qsub16,             // Signed saturating arithmetic hafword-wise substraction
ARM_qsub8,              // Signed saturating arithmetic byte-wise substraction
ARM_qsubaddx,           // Signed saturating arithmetic exchange, substract, add
ARM_rev,                // Byte Reverse Word
ARM_rev16,              // Byte Reverse Packed Halfword
ARM_revsh,              // Byte Reverse Signed Halfword
ARM_rfe,                // Return from exception
ARM_sadd16,             // Signed arithmetic modulo hafword-wise addition
ARM_sadd8,              // Signed arithmetic modulo byte-wise addition
ARM_saddsubx,           // Signed arithmetic modulo exchange, add, substract
ARM_sel,                // Select bytes
ARM_setend,             // Set Byte Endianness
ARM_shadd16,            // Signed arithmetic hafword-wise addition, halving results
ARM_shadd8,             // Signed arithmetic byte-wise addition, halving results
ARM_shaddsubx,          // Signed arithmetic exchange, add, substract, halving results
ARM_shsub16,            // Signed arithmetic hafword-wise substraction, halving results
ARM_shsub8,             // Signed arithmetic byte-wise substraction, halving results
ARM_shsubaddx,          // Signed arithmetic exchange, substract, add, halving results
ARM_smlad,              // Dual signed multiply, add and accumulate
ARM_smladx,             // Dual signed multiply, add and accumulate crossed
ARM_smuad,              // Dual signed multiply and add
ARM_smuadx,             // Dual signed multiply and add crossed
ARM_smlald,             // Dual signed multiply, add and accumulate long
ARM_smlaldx,            // Dual signed multiply, add and accumulate long crossed
ARM_smlsd,              // Dual signed multiply, substract and accumulate
ARM_smlsdx,             // Dual signed multiply, substract and accumulate crossed
ARM_smusd,              // Dual signed multiply and substract
ARM_smusdx,             // Dual signed multiply and substract crossed
ARM_smlsld,             // Dual signed multiply, substract and accumulate long
ARM_smlsldx,            // Dual signed multiply, substract and accumulate long crossed
ARM_smmla,              // Signed most significant word multiply and accumulate truncated
ARM_smmlar,             // Signed most significant word multiply and accumulate rounded
ARM_smmul,              // Signed most significant word multiply truncated
ARM_smmulr,             // Signed most significant word multiply rounded
ARM_smmls,              // Signed most significant word multiply and substract truncated
ARM_smmlsr,             // Signed most significant word multiply and substract rounded
ARM_srs,                // Store Return State
ARM_ssat,               // Signed Saturate
ARM_ssat16,             // Signed saturate two halfwords
ARM_ssub16,             // Signed arithmetic hafword-wise substraction
ARM_ssub8,              // Signed arithmetic byte-wise substraction
ARM_ssubaddx,           // Signed arithmetic exchange, substract, add
ARM_strex,              // Store Register Exclusive
ARM_sxtab,              // Signed extend byte to word, add
ARM_sxtb,               // Signed extend byte to word
ARM_sxtab16,            // Signed extend two bytes to halfwords, add
ARM_sxtb16,             // Signed extend two bytes to halfwords
ARM_sxtah,              // Signed extend halfword to word, add
ARM_sxth,               // Signed extend halfword to word
ARM_uadd16,             // Unsigned arithmetic modulo hafword-wise addition
ARM_uadd8,              // Unsigned arithmetic modulo byte-wise addition
ARM_uaddsubx,           // Unsigned arithmetic modulo exchange, add, substract
ARM_uhadd16,            // Unsigned arithmetic hafword-wise addition, halving results
ARM_uhadd8,             // Unsigned arithmetic byte-wise addition, halving results
ARM_uhaddsubx,          // Unsigned arithmetic exchange, add, substract, halving results
ARM_uhsub16,            // Unsigned arithmetic hafword-wise substraction, halving results
ARM_uhsub8,             // Unsigned arithmetic byte-wise substraction, halving results
ARM_uhsubaddx,          // Unsigned arithmetic exchange, substract, add, halving results
ARM_umaal,              // Multiply unsigned double accumulate long
ARM_uqadd16,            // Unsigned saturating arithmetic hafword-wise addition
ARM_uqadd8,             // Unsigned saturating arithmetic byte-wise addition
ARM_uqaddsubx,          // Unsigned saturating arithmetic exchange, add, substract
ARM_uqsub16,            // Unsigned saturating arithmetic hafword-wise substraction
ARM_uqsub8,             // Unsigned saturating arithmetic byte-wise substraction
ARM_uqsubaddx,          // Unsigned saturating arithmetic exchange, substract, add
ARM_usada8,             // Unsigned sum of absolute differences and accumulate
ARM_usad8,              // Unsigned sum of absolute differences
ARM_usat,               // Unsigned saturate word
ARM_usat16,             // Unsigned saturate two halfwords
ARM_usub16,             // Unsigned arithmetic hafword-wise substraction
ARM_usub8,              // Unsigned arithmetic byte-wise substraction
ARM_usubaddx,           // Unsigned arithmetic exchange, substract, add
ARM_uxtab,              // Unsigned extend byte to word, add
ARM_uxtb,               // Unsigned extend byte to word
ARM_uxtab16,            // Unsigned extend two bytes to halfwords, add
ARM_uxtb16,             // Unsigned extend two bytes to halfwords
ARM_uxtah,              // Unsigned extend halfword to word, add
ARM_uxth,               // Unsigned extend halfword to word

// ARM v6zk instructions

ARM_clrex,              // Clear Exclusive
ARM_ldrexb,             // Load Byte Exclusive
ARM_ldrexd,             // Load DoubleWord Exclusive
ARM_ldrexh,             // Load Halfword Exclusive
ARM_strexb,             // Store Byte Exclusive
ARM_strexd,             // Store DoubleWord Exclusive
ARM_strexh,             // Store Halfword Exclusive
ARM_yield,              // Yield (hint)
ARM_sev,                // Send Event (hint)
ARM_wfe,                // Wait For Event (hint)
ARM_wfi,                // Wait For Interrupt (hint)
ARM_smc,                // Secure Monitor Call

// ARM Thumb32 instructions

ARM_orn,                // Rd = Op1 | ~Op2
ARM_movt,               // Move Top
ARM_sbfx,               // Signed Bit Field Extract
ARM_ubfx,               // Unsigned Bit Field Extract
ARM_bfi,                // Bit Field Insert
ARM_bfc,                // Bit Field Clear
ARM_tbb,                // Table Branch Byte
ARM_tbh,                // Table Branch Halfword
ARM_pli,                // Prepare to load code
ARM_rbit,               // Reverse Bits
ARM_it,                 // If Then
ARM_mls,                // Multiply and Subtract
ARM_sdiv,               // Signed Divide
ARM_udiv,               // Unsigned Divide
ARM_cbz,                // Compare and Branch on Zero
ARM_cbnz,               // Compare and Branch on Non-Zero
ARM_dsb,                // Data Synchronization Barrier
ARM_dmb,                // Data Memory Barrier
ARM_isb,                // Instruction Synchronization Barrier
ARM_dbg,                // Debug Hint

ARM_und,                // Architecturally undefined instruction

// missing instructions (not yet decoded)

ARM_rrx,                // Rotate Right with Extend
ARM_enterx,             // Enter ThumbEE state
ARM_leavex,             // Leave ThumbEE state
ARM_chka,               // Check Array
ARM_hb,                 // Handler Branch
ARM_hbl,                // Handler Branch with Link
ARM_hblp,               // Handler Branch with Link and Parameter
ARM_hbp,                // Handler Branch with Parameter

// NEON (Advanced SIMD) and extra VFP instructions

ARM_vaba,               // Vector Absolute Difference and Accumulate
ARM_vabal,              // Vector Absolute Difference and Accumulate Long
ARM_vabd,               // Vector Absolute Difference
ARM_vabdl,              // Vector Absolute Difference Long
ARM_vabs,               // Vector Absolute
ARM_vacge,              // Vector Absolute Compare Greater Than or Equal
ARM_vacgt,              // Vector Absolute Compare Greater Than
ARM_vacle,              // Vector Absolute Compare Less Than or Equal
ARM_vaclt,              // Vector Absolute Compare Less Than
ARM_vadd,               // Vector Add
ARM_vaddhn,             // Vector Add and Narrow, returning High Half
ARM_vaddl,              // Vector Add Long
ARM_vaddw,              // Vector Add Wide
ARM_vand,               // Vector Bitwise AND
ARM_vbic,               // Vector Bitwise Bit Clear
ARM_vbif,               // Vector Bitwise Insert if False
ARM_vbit,               // Vector Bitwise Insert if True
ARM_vbsl,               // Vector Bitwise Select
ARM_vceq,               // Vector Compare Equal
ARM_vcge,               // Vector Compare Greater Than or Equal
ARM_vcgt,               // Vector Compare Greater Than
ARM_vcle,               // Vector Compare Less Than or Equal
ARM_vcls,               // Vector Count Leading Sign Bits
ARM_vclt,               // Vector Compare Less Than
ARM_vclz,               // Vector Count Leading Zeros
ARM_vcmp,               // Vector Compare
ARM_vcmpe,              // Vector Compare (Quiet NaNs trigger Exception)
ARM_vcnt,               // Vector Count Number of Bits
ARM_vcvt,               // Vector Convert
ARM_vcvtr,              // Vector Convert Rounding
ARM_vcvtb,              // Vector Convert Half-Precision Bottom
ARM_vcvtt,              // Vector Convert Half-Precision Top
ARM_vdiv,               // Vector Divide
ARM_vdup,               // Vector Duplicate
ARM_veor,               // Vector Bitwise Exclusive OR
ARM_vext,               // Vector Extract
ARM_vfma,               // Vector Fused Multiply Accumulate
ARM_vfms,               // Vector Fused Multiply Substract
ARM_vfnma,              // Vector Fused Negated Multiply Accumulate
ARM_vfnms,              // Vector Fused Negated Multiply Substract
ARM_vhadd,              // Vector Halving Add
ARM_vhsub,              // Vector Halving Sub
ARM_vld1,               // Vector Load Single Element
ARM_vld2,               // Vector Load Two-Element Structures
ARM_vld3,               // Vector Load Three-Element Structures
ARM_vld4,               // Vector Load Four-Element Structures
ARM_vldm,               // Vector Load Multiple
ARM_vldr,               // Vector Load Register
ARM_vmax,               // Vector Maximum
ARM_vmin,               // Vector Minimum
ARM_vmla,               // Vector Multiply Accumulate
ARM_vmlal,              // Vector Multiply Accumulate Long
ARM_vmls,               // Vector Multiply Subtract
ARM_vmlsl,              // Vector Multiply Subtract Long
ARM_vmov,               // Vector Move
ARM_vmovl,              // Vector Move Long
ARM_vmovn,              // Vector Move and Narrow
ARM_vmrs,               // Move FPSCR to ARM Register
ARM_vmsr,               // Move to FPSCR from ARM Register 
ARM_vmul,               // Vector Multiply
ARM_vmull,              // Vector Multiply Long
ARM_vmvn,               // Vector Bitwise NOT
ARM_vneg,               // Vector Negate
ARM_vnmla,              // Vector Multiply Add Negated
ARM_vnmls,              // Vector Multiply Substract Negated
ARM_vnmul,              // Vector Multiply Negated
ARM_vorn,               // Vector Bitwise OR NOT
ARM_vorr,               // Vector Bitwise OR
ARM_vpadal,             // Vector Pairwise Add and Accumulate Long
ARM_vpadd,              // Vector Pairwise Add
ARM_vpaddl,             // Vector Pairwise Add Long
ARM_vpmax,              // Vector Pairwise Maximum
ARM_vpmin,              // Vector Pairwise Minimum
ARM_vpop,               // Vector Pop
ARM_vpush,              // Vector Push
ARM_vqabs,              // Vector Saturating Absolute
ARM_vqadd,              // Vector Saturating Add
ARM_vqdmlal,            // Vector Saturating Doubling Multiply Accumulate Long
ARM_vqdmlsl,            // Vector Saturating Doubling Multiply Subtract Long
ARM_vqdmulh,            // Vector Saturating Doubling Multiply Returning High Half
ARM_vqdmull,            // Vector Saturating Doubling Multiply Long
ARM_vqmovn,             // Vector Saturating Move and Narrow
ARM_vqmovun,            // Vector Saturating Move and Narrow, Unsigned result
ARM_vqneg,              // Vector Saturating Negate
ARM_vqrdmulh,           // Vector Saturating Rounding Doubling Multiply Returning High Half
ARM_vqrshl,             // Vector Saturating Rounding Shift Left
ARM_vqrshrn,            // Vector Saturating Rounding Shift Right, Narrow
ARM_vqrshrun,           // Vector Saturating Rounding Shift Right, Narrow, Unsigned result
ARM_vqshl,              // Vector Saturating Shift Left
ARM_vqshlu,             // Vector Saturating Shift Left, Unsigned result
ARM_vqshrn,             // Vector Saturating Shift Right, Narrow
ARM_vqshrun,            // Vector Saturating Shift Right, Narrow, Unsigned result
ARM_vqsub,              // Vector Saturating Subtract
ARM_vraddhn,            // Vector Rounding Add and Narrow, returning High Half
ARM_vrecpe,             // Vector Reciprocal Estimate
ARM_vrecps,             // Vector Reciprocal Step
ARM_vrev16,             // Vector Reverse in halfwords
ARM_vrev32,             // Vector Reverse in words
ARM_vrev64,             // Vector Reverse in doublewords
ARM_vrhadd,             // Vector Rounding Halving Add
ARM_vrshl,              // Vector Rounding Shift Left
ARM_vrshr,              // Vector Rounding Shift Right
ARM_vrshrn,             // Vector Rounding Shift Right and Narrow
ARM_vrsqrte,            // Vector Reciprocal Square Root Estimate
ARM_vrsqrts,            // Vector Reciprocal Square Root Step
ARM_vrsra,              // Vector Rounding Shift Right and Accumulate
ARM_vrsubhn,            // Vector Rounding Subtract and Narrow, returning High Half
ARM_vshl,               // Vector Shift Left
ARM_vshll,              // Vector Shift Left Long
ARM_vshr,               // Vector Shift Right
ARM_vshrn,              // Vector Shift Right Narrow
ARM_vsli,               // Vector Shift Left and Insert
ARM_vsqrt,              // Vector Square Root
ARM_vsra,               // Vector Shift Right and Accumulate
ARM_vsri,               // Vector Shift Right and Insert
ARM_vst1,               // Vector Store Single Element
ARM_vst2,               // Vector Store Two-Element Structures
ARM_vst3,               // Vector Store Three-Element Structures
ARM_vst4,               // Vector Store Four-Element Structures
ARM_vstm,               // Vector Store Multiple
ARM_vstr,               // Vector Store Register
ARM_vsub,               // Vector Subtract
ARM_vsubhn,             // Vector Subtract and Narrow, returning High Half
ARM_vsubl,              // Vector Subtract Long
ARM_vsubw,              // Vector Subtract Wide
ARM_vswp,               // Vector Swap
ARM_vtbl,               // Vector Table Lookup
ARM_vtbx,               // Vector Table Extension
ARM_vtrn,               // Vector Transpose
ARM_vtst,               // Vector Test Bits
ARM_vuzp,               // Vector Unzip
ARM_vzip,               // Vector Zip

// ARMv7 Hypervisor extensions

ARM_eret,               // Exception Return
ARM_hvc,                // Hypervisor Call

// new ARMv8 instructions for A32 and T32 mode

ARM_lda,                // Load-Acquire Word/Byte/Halfword
ARM_stl,                // Store-Release Word/Byte/Halfword
ARM_ldaex,              // Load-Acquire Exclusive Word/Byte/Halfword
ARM_stlex,              // Store-Release Exclusive Word/Byte/Halfword
ARM_vsel,               // Floating-point Conditional Select

ARM_vmaxnm,             // Vector Maximum Numeric
ARM_vminnm,             // Vector Minimum Numeric
ARM_vcvta,              // Vector Convert Round to Nearest with Ties to Away
ARM_vcvtn,              // Vector Convert Round to Nearest with Ties to Even
ARM_vcvtp,              // Vector Convert Round towards +Infinity
ARM_vcvtm,              // Vector Convert Round towards -Infinity
ARM_vrintx,             // Vector Round to Integral, FPSCR rounding mode and signaling inexactness
ARM_vrintr,             // Vector Round to Integral, FPSCR rounding mode
ARM_vrintz,             // Vector Round to Integral, Round toward Zero
ARM_vrinta,             // Vector Round to Integral, Round to Nearest with Ties to Away
ARM_vrintn,             // Vector Round to Integral, Round to Nearest with Ties to Even
ARM_vrintp,             // Vector Round to Integral, Round towards +Infinity
ARM_vrintm,             // Vector Round to Integral, Round towards -Infinity

ARM_aesd,               // AES single round decryption
ARM_aese,               // AES single round encryption
ARM_aesimc,             // AES inverse mix columns
ARM_aesmc,              // AES mix columns
ARM_sha1c,              // SHA1 hash update accelerator, choose
ARM_sha1m,              // SHA1 hash update accelerator, majority
ARM_sha1p,              // SHA1 hash update accelerator, parity
ARM_sha1h,              // SHA1 hash update accelerator, rotate left by 30
ARM_sha1su0,            // SHA1 schedule update accelerator, first part
ARM_sha1su1,            // SHA1 schedule update accelerator, second part
ARM_sha256h,            // SHA256 hash update accelerator
ARM_sha256h2,           // SHA256 hash update accelerator upper part
ARM_sha256su0,          // SHA256 schedule update accelerator, first part
ARM_sha256su1,          // SHA256 schedule update accelerator, second part

ARM_dcps1,              // Debug Change Processor State to EL1
ARM_dcps2,              // Debug Change Processor State to EL2
ARM_dcps3,              // Debug Change Processor State to EL3
ARM_hlt,                // Halting mode software breakpoint
ARM_sevl,               // Send Event Locally

// ARMv8 AArch64 new instructions

ARM_tbz,                // Test and Branch Zero
ARM_tbnz,               // Test and Branch Non-Zero
ARM_br,                 // Branch To Register
ARM_blr,                // Branch and Link Register
ARM_ldur,               // Load Single Register (unscaled offset)
ARM_stur,               // Store Single Register (unscaled offset)
ARM_ldp,                // Load Pair
ARM_stp,                // Store Pair
ARM_ldnp,               // Load Non-temporal Pair
ARM_stnp,               // Store Non-temporal Pair
ARM_ldtr,               // Load Unprivileged Register
ARM_sttr,               // Store Unprivileged Register
ARM_ldxr,               // Load Exclusive Register
ARM_stxr,               // Store Exclusive Register
ARM_ldxp,               // Load Exclusive Pair
ARM_stxp,               // Store Exclusive Pair
ARM_ldar,               // Load-Acquire Register
ARM_stlr,               // Store-Release Register
ARM_ldaxr,              // Load-Acquire Exclusive Register
ARM_stlxr,              // Store-Release Exclusive Register
ARM_ldaxp,              // Load-Acquire Exclusive Pair
ARM_stlxp,              // Store Exclusive Pair
ARM_prfm,               // Prefetch Memory
ARM_prfum,              // Prefetch Memory (unscaled offset)
ARM_movi,               // Move Immediate
ARM_mvni,               // Move Inverted Immediate
ARM_movz,               // Move with Zero
ARM_movn,               // Move with Not
ARM_movk,               // Move with Keep
ARM_adrp,               // Address of Page
ARM_bfm,                // Bitfield Move
ARM_sbfm,               // Signed Bitfield Move
ARM_ubfm,               // Unsigned Bitfield Move
//ARM_bfi,                // Bitfield Insert
ARM_bfxil,              // Bitfield Extract and Insert Low
ARM_sbfiz,              // Signed Bitfield Insert in Zero
//ARM_sbfx,               // Signed Bitfield Extract
ARM_ubfiz,              // Signed Bitfield Insert in Zero
//ARM_ubfx,               // Unsigned Bitfield Extract
ARM_extr,               // Extract
ARM_sxtw,               // Signed Extend Word
ARM_uxtw,               // Unsigned Extend Word
ARM_eon,                // Bitwise exclusive OR NOT
ARM_not,                // Bitwise NOT
ARM_cls,                // Count Leading Sign Bits
ARM_rev32,              // Reverse Bytes in Words
ARM_csel,               // Conditional Select
ARM_csinc,              // Conditional Select Increment
ARM_csinv,              // Conditional Select Invert
ARM_csneg,              // Conditional Select Negate
ARM_cset,               // Conditional Set
ARM_csetm,              // Conditional Set Mask
ARM_cinc,               // Conditional Increment
ARM_cinv,               // Conditional Invert
ARM_cneg,               // Conditional Negate
ARM_ngc,                // Negate with Carry
ARM_ccmn,               // Conditional Compare Negative
ARM_ccmp,               // Conditional Compare
ARM_madd,               // Multiply-Add
ARM_msub,               // Multiply-Subtract
ARM_mneg,               // Multiply-Negate
ARM_smaddl,             // Signed Multiply-Add Long
ARM_smsubl,             // Signed Multiply-Subtract Long
ARM_smnegl,             // Signed Multiply-Negate Long
ARM_smulh,              // Signed Multiply High
ARM_umaddl,             // Unsigned Multiply-Add Long
ARM_umsubl,             // Unsigned Multiply-Subtract Long
ARM_umnegl,             // Unsigned Multiply-Negate Long
ARM_umulh,              // Unsigned Multiply High

ARM_drps,               // Debug Restore Processor State
ARM_sys,                // System Maintenance Operation
ARM_sysl,               // System Maintenance Operation With Result
ARM_ic,                 // Instruction Cache Maintenance
ARM_dc,                 // Data Cache Maintenance
ARM_at,                 // Address Translation
ARM_tlbi,               // TLB Invalidation
ARM_hint,               // Hint instruction
ARM_brk,                // Monitor debug-mode breakpoint

// AArch64 Advanced SIMD
ARM_uaba,               // Unsigned Integer Absolute Difference and Accumulate
ARM_saba,               // Signed Integer Absolute Difference and Accumulate
ARM_uabal,              // Unsigned Integer Absolute Difference and Accumulate Long
ARM_uabal2,             // Unsigned Integer Absolute Difference and Accumulate Long (Second Part)
ARM_sabal,              // Signed Integer Absolute Difference and Accumulate Long
ARM_sabal2,             // Signed Integer Absolute Difference and Accumulate Long (Second Part)
ARM_uabd,               // Unsigned Integer Absolute Difference
ARM_sabd,               // Signed Integer Absolute Difference
ARM_fabd,               // Floating-point Absolute Difference
ARM_uabdl,              // Unsigned Integer Absolute Difference Long
ARM_uabdl2,             // Unsigned Integer Absolute Difference Long (Second Part)
ARM_sabdl,              // Signed Integer Absolute Difference Long
ARM_sabdl2,             // Signed Integer Absolute Difference Long (Second Part)
ARM_abs,                // Integer Absolute Value
ARM_fabs,               // Floating-point Absolute Value
ARM_facge,              // Floating-point Absolute Compare Greater Than or Equal
ARM_facgt,              // Floating-point Absolute Compare Greater Than
ARM_facle,              // Floating-point Absolute Compare Less Than or Equal
ARM_faclt,              // Floating-point Absolute Compare Less Than
ARM_fadd,               // Floating-point Add
ARM_addhn,              // Integer Vector Add and Narrow High Half
ARM_addhn2,             // Integer Vector Add and Narrow High Half (second part)
ARM_uaddl,              // Unsigned Integer Add Long
ARM_uaddl2,             // Unsigned Integer Add Long (second part)
ARM_saddl,              // Signed Integer Add Long
ARM_saddl2,             // Signed Integer Add Long (second part)
ARM_uaddw,              // Unsigned Integer Add Wide
ARM_uaddw2,             // Unsigned Integer Add Wide (second part)
ARM_saddw,              // Signed Integer Add Wide
ARM_saddw2,             // Signed Integer Add Wide (second part)
ARM_bif,                // Bitwise Vector Insert if False
ARM_bit,                // Bitwise Vector Insert if True
ARM_bsl,                // Bitwise Vector Select
ARM_cmeq,               // Ingeger Vector Compare Equal
ARM_fcmeq,              // Floating-point Vector Compare Equal
ARM_cmhs,               // Unsigned Compare Greater Than or Equal
ARM_cmge,               // Signed Compare Greater Than or Equal
ARM_fcmge,              // Floating-point Compare Greater Than or Equal
ARM_cmhi,               // Unsigned Compare Greater Than
ARM_cmgt,               // Signed Compare Greater Than
ARM_fcmgt,              // Floating-point Compare Greater Than
ARM_cmls,               // Unsigned Compare Less Than or Equal
ARM_cmle,               // Signed Compare Less Than or Equal
ARM_fcmle,              // Floating-point Compare Less Than or Equal
ARM_cmlo,               // Unsigned Compare Less Than
ARM_cmlt,               // Signed Compare Less Than
ARM_fcmlt,              // Floating-point Compare Less Than
ARM_fcmp,               // Floating-point Compare
ARM_fcmpe,              // Floating-point Signaling Compare
ARM_fccmp,              // Floating-point Conditional Quiet Compare
ARM_fccmpe,             // Floating-point Conditional Signaling Compare
ARM_fcsel,              // Floating-point Conditional Select
ARM_cnt,                // Vector Count Non-zero Bits
ARM_fcvt,               // Floating-point convert precision (scalar)
ARM_fcvtzs,             // Convert Floating-point to Signed Integer (Round to Zero)
ARM_fcvtas,             // Convert Floating-point to Signed Integer (Round to Nearest, Ties to Away)
ARM_fcvtns,             // Convert Floating-point to Signed Integer (Round to Nearest, Ties to Even)
ARM_fcvtps,             // Convert Floating-point to Signed Integer (Round towards +Infinity)
ARM_fcvtms,             // Convert Floating-point to Signed Integer (Round towards -Infinity)
ARM_fcvtzu,             // Convert Floating-point to Unsigned Integer (Round to Zero)
ARM_fcvtau,             // Convert Floating-point to Unsigned Integer (Round to Nearest, Ties to Away)
ARM_fcvtnu,             // Convert Floating-point to Unsigned Integer (Round to Nearest, Ties to Even)
ARM_fcvtpu,             // Convert Floating-point to Unsigned Integer (Round towards +Infinity)
ARM_fcvtmu,             // Convert Floating-point to Unsigned Integer (Round towards -Infinity)
ARM_ucvtf,              // Convert Unsigned Integer to Floating-point 
ARM_scvtf,              // Convert Signed Integer to Floating-point 
ARM_fcvtn,              // Floating-point Convert Precision, Narrow
ARM_fcvtn2,             // Floating-point Convert Precision, Narrow (second part)
ARM_fcvtl,              // Floating-point Convert Precision, Long
ARM_fcvtl2,             // Floating-point Convert Precision, Long (second part)
ARM_fcvtxn,             // Floating-point Convert Precision, Narrow (Round to Odd)
ARM_fcvtxn2,            // Floating-point Convert Precision, Narrow (Round to Odd) (second part)
ARM_frinta,             // Floating-point Round to Integral (Round to Nearest, Ties to Away)
ARM_frinti,             // Floating-point Round to Integral (using FPCR rounding mode)
ARM_frintm,             // Floating-point Round to Integral (Round towards -Infinity)
ARM_frintn,             // Floating-point Round to Integral (Round to Nearest, Ties to Even)
ARM_frintp,             // Floating-point Round to Integral (Round towards +Infinity)
ARM_frintx,             // Floating-point Round to Integral (using FPCR rounding mode, with exactness test)
ARM_frintz,             // Floating-point Round to Integral (Round towards Zero)
ARM_fmadd,              // Floating-point scalar fused multiply-add
ARM_fmsub,              // Floating-point scalar fused multiply-subtract
ARM_fnmadd,             // Floating-point scalar negated fused multiply-add
ARM_fnmsub,             // Floating-point scalar negated fused multiply-subtract
ARM_fdiv,               // Floating-point Divide
ARM_dup,                // Duplicate Single Element to All Elements
ARM_ins,                // Insert Single Element in Another Element
ARM_ext,                // Bitwise Extract
ARM_uhadd,              // Unsigned Integer Halving Add
ARM_shadd,              // Signed Integer Halving Add
ARM_uhsub,              // Unsigned Integer Halving Subtract
ARM_shsub,              // Signed Integer Halving Subtract
ARM_ld1,                // Vector Load Single Element
ARM_ld2,                // Vector Load Two-Element Structures
ARM_ld3,                // Vector Load Three-Element Structures
ARM_ld4,                // Vector Load Four-Element Structures
ARM_ld1r,               // Vector Load Single Element and Replicate
ARM_ld2r,               // Vector Load Two-Element Structures and Replicate
ARM_ld3r,               // Vector Load Three-Element Structures and Replicate
ARM_ld4r,               // Vector Load Four-Element Structures and Replicate
ARM_umax,               // Unsigned Integer Maximum
ARM_smax,               // Signed Integer Maximum
ARM_fmax,               // Floating-point Maximum
ARM_fmaxnm,             // Floating-point maxNum()
ARM_umin,               // Unsigned Integer Minimum
ARM_smin,               // Signed Integer Minimum
ARM_fmin,               // Floating-point Minimum
ARM_fminnm,              // Floating-point minNum()
ARM_fmla,               // Fused Multiply-Add
ARM_umlal2,             // Unsigned Integer Multiply-Add Long (second part)
ARM_smlal2,             // Signed Integer Multiply-Add Long (second part)
ARM_fmls,               // Fused Multiply-Subtract
ARM_umlsl,              // Unsigned Integer Multiply-Subtract Long
ARM_umlsl2,             // Unsigned Integer Multiply-Subtract Long (second part)
ARM_smlsl,              // Signed Integer Multiply-Subtract Long
ARM_smlsl2,             // Signed Integer Multiply-Subtract Long (second part)
ARM_umov,               // Unsigned Integer Move
ARM_smov,               // Signed Integer Move
ARM_fmov,               // Floating-point Move
ARM_uxtl,               // Unsigned Integer Vector Lengthen
ARM_uxtl2,              // Unsigned Integer Vector Lengthen (second part)
ARM_sxtl,               // Signed Integer Vector Lengthen
ARM_sxtl2,              // Signed Integer Vector Lengthen (second part)
ARM_xtn,                // Integer Vector Narrow
ARM_xtn2,               // Integer Vector Narrow (second part)
ARM_fmul,               // Floating-point Vector Multiply
ARM_pmul,               // Polynomial Vector Multiply
ARM_fmulx,              // Floating-point Multiply Extended
ARM_fnmul,              // Floating-point Multiply-Negate
ARM_umull2,             // Unsigned Integer Vector Multiply Long (second part)
ARM_smull2,             // Signed Integer Vector Multiply Long (second part)
ARM_pmull,              // Polynomial Vector Multiply Long
ARM_pmull2,             // Polynomial Vector Multiply Long (second part)
ARM_fneg,               // Floating-point Negate
ARM_uadalp,             // Unsigned Integer Vector Add and Accumulate Long Pair
ARM_sadalp,             // Signed Integer Vector Add and Accumulate Long Pair
ARM_addp,               // Integer Vector Add Pair
ARM_faddp,              // Floating-point Vector Add Pair
ARM_uaddlp,             // Unsigned Integer Vector Add Long Pair
ARM_saddlp,             // Signed Integer Vector Add Long Pair
ARM_umaxp,              // Unsigned Integer Maximum Pair
ARM_smaxp,              // Signed Integer Maximum Pair
ARM_fmaxp,              // Floating-point Maximum Pair
ARM_fmaxnmp,            // Floating-point maxNum Pair
ARM_uminp,              // Unsigned Integer Minimum Pair
ARM_sminp,              // Signed Integer Minimum Pair
ARM_fminp,              // Floating-point Minimum Pair
ARM_fminnmp,            // Floating-point minNum Pair
ARM_sqabs,              // Signed Integer Saturating Absolute
ARM_uqadd,              // Unsigned Integer Saturating Add
ARM_sqadd,              // Signed Integer Saturating Add
ARM_suqadd,             // Signed Integer Saturating Accumulate of Unsigned Values
ARM_usqadd,             // Unsigned Integer Saturating Accumulate of Signed Values
ARM_sqdmlal,            // Signed Integer Saturating Doubling Multiply Add Long
ARM_sqdmlal2,           // Signed Integer Saturating Doubling Multiply Add Long (second part)
ARM_sqdmlsl,            // Signed Integer Saturating Doubling Multiply Subtract Long
ARM_sqdmlsl2,           // Signed Integer Saturating Doubling Multiply Subtract Long (second part)
ARM_sqdmulh,            // Signed Integer Saturating Doubling Multiply Returning High Half
ARM_sqdmull,            // Signed Integer Saturating Doubling Multiply Long
ARM_sqdmull2,           // Signed Integer Saturating Doubling Multiply Long (second part)
ARM_uqxtn,              // Unsigned Integer Saturating Vector Narrow
ARM_uqxtn2,             // Unsigned Integer Saturating Vector Narrow (second part)
ARM_sqxtn,              // Signed Integer Saturating Vector Narrow
ARM_sqxtn2,             // Signed Integer Saturating Vector Narrow (second part)
ARM_sqxtun,             // Signed Integer Saturating Vector and Unsigned Narrow
ARM_sqxtun2,            // Signed Integer Saturating Vector and Unsigned Narrow (second part)
ARM_sqneg,              // Signed Integer Saturating Negate
ARM_sqrdmulh,           // Signed Integer Saturating Rounding Doubling Multiply Returning High Half
ARM_uqrshl,             // Unsigned Integer Saturating Rounding Shift Left
ARM_sqrshl,             // Signed Integer Saturating Rounding Shift Left
ARM_uqrshrn,            // Unsigned Integer Saturating Rounding Shift Right, Narrow
ARM_uqrshrn2,           // Unsigned Integer Saturating Rounding Shift Right, Narrow (second part)
ARM_sqrshrn,            // Signed Integer Saturating Rounding Shift Right, Narrow
ARM_sqrshrn2,           // Signed Integer Saturating Rounding Shift Right, Narrow (second part)
ARM_sqrshrun,           // Signed Integer Saturating Rounding Shift Right, Narrow, Unsigned result
ARM_sqrshrun2,          // Signed Integer Saturating Rounding Shift Right, Narrow, Unsigned result (second part)
ARM_uqshl,              // Unsigned Integer Saturating Shift Left
ARM_sqshl,              // Signed Integer Saturating Shift Left
ARM_sqshlu,             // Signed Integer Saturating Shift Left, Unsigned result
ARM_uqshrn,             // Unsigned Integer Saturating Shift Right, Narrow
ARM_uqshrn2,            // Unsigned Integer Saturating Shift Right, Narrow (second part)
ARM_sqshrn,             // Signed Integer Saturating Shift Right, Narrow
ARM_sqshrn2,            // Signed Integer Saturating Shift Right, Narrow (second part)
ARM_sqshrun,            // Signed Integer Saturating Shift Right, Narrow, Unsigned result
ARM_sqshrun2,           // Signed Integer Saturating Shift Right, Narrow, Unsigned result (second part)
ARM_uqsub,              // Unsigned Integer Saturating Subtract
ARM_sqsub,              // Signed Integer Saturating Subtract
ARM_raddhn,             // Integer Rounding Add and Narrow, returning High Half
ARM_raddhn2,            // Integer Rounding Add and Narrow, returning High Half (second part)
ARM_urecpe,             // Unsigned Integer Reciprocal Estimate
ARM_frecpe,             // Floating-point Reciprocal Estimate
ARM_frecps,             // Floating-point Reciprocal Step
ARM_frecpx,             // Floating-point Reciprocal Exponent
ARM_rev64,              // Reverse doublewords
ARM_urhadd,             // Unsigned Integer Rounding Halving Add
ARM_srhadd,             // Signed Integer Rounding Halving Add
ARM_urshl,              // Unsigned Integer Rounding Shift Left
ARM_srshl,              // Signed Integer Rounding Shift Left
ARM_urshr,              // Unsigned Integer Rounding Shift Right
ARM_srshr,              // Signed Integer Rounding Shift Right
ARM_rshrn,              // Integer Rounding Shift Right and Narrow
ARM_rshrn2,             // Integer Rounding Shift Right and Narrow (second part)
ARM_ursqrte,            // Unsigned Integer Reciprocal Square Root Estimate
ARM_frsqrte,            // Floating-point Reciprocal Square Root Estimate
ARM_frsqrts,            // Floating-point Reciprocal Square Root Step
ARM_ursra,              // Unsigned Integer Rounding Shift Right and Accumulate
ARM_srsra,              // Signed Integer Rounding Shift Right and Accumulate
ARM_rsubhn,             // Integer Subtract and Narrow, returning High Half
ARM_rsubhn2,            // Integer Subtract and Narrow, returning High Half (second part)
ARM_ushl,               // Unsigned Integer Shift Left
ARM_sshl,               // Signed Integer Shift Left
ARM_ushll,              // Unsigned Integer Shift Left Long
ARM_ushll2,             // Unsigned Integer Shift Left Long (second part)
ARM_sshll,              // Signed Integer Shift Left Long
ARM_sshll2,             // Signed Integer Shift Left Long (second part)
ARM_ushr,               // Unsigned Integer Shift Right
ARM_sshr,               // Signed Integer Shift Right
ARM_shrn,               // Integer Shift Right Narrow
ARM_shrn2,              // Integer Shift Right Narrow (second part)
ARM_shl,                // Shift Left
ARM_shll,               // Shift Left Long
ARM_shll2,              // Shift Left Long (second part)
ARM_sli,                // Integer Vector Shift Left and Insert
ARM_fsqrt,              // Floating-point Square Root
ARM_usra,               // Unsigned Integer Shift Right and Accumulate
ARM_ssra,               // Signed Integer Shift Right and Accumulate
ARM_sri,                // Integer Shift Right and Insert
ARM_st1,                // Vector Store Single Element
ARM_st2,                // Vector Store Two-Element Structures
ARM_st3,                // Vector Store Three-Element Structures
ARM_st4,                // Vector Store Four-Element Structures
ARM_fsub,               // Floating-point Subtract
ARM_subhn,              // Integer Subtract and Narrow, returning High Half
ARM_subhn2,             // Integer Subtract and Narrow, returning High Half (second part)
ARM_usubl,              // Unsigned Integer Subtract Long
ARM_usubl2,             // Unsigned Integer Subtract Long (second part)
ARM_ssubl,              // Signed Integer Subtract Long
ARM_ssubl2,             // Signed Integer Subtract Long (second part)
ARM_usubw,              // Unsigned Integer Subtract Wide
ARM_usubw2,             // Unsigned Integer Subtract Wide (second part)
ARM_ssubw,              // Signed Integer Subtract Wide
ARM_ssubw2,             // Signed Integer Subtract Wide (second part)
ARM_tbl,                // Vector Table Lookup
ARM_tbx,                // Vector Table Extension
ARM_trn1,               // Vector Element Transpose (primary)
ARM_trn2,               // Vector Element Transpose (secondary)
ARM_cmtst,              // Integer Compare Mask Bitwise Test
ARM_uzp1,               // Vector Element Unzip (primary)
ARM_uzp2,               // Vector Element Unzip (secondary)
ARM_zip1,               // Vector Element Zip (primary)
ARM_zip2,               // Vector Element Zip (secondary)
ARM_addv,               // Integer Sum Elements in Vector
ARM_uaddlv,             // Unsigned Integer Sum Elements in Vector Long
ARM_saddlv,             // Signed Integer Sum Elements in Vector Long
ARM_umaxv,              // Unsigned Integer Max Element in Vector
ARM_smaxv,              // Signed Integer Max Element in Vector
ARM_fmaxv,              // Floating-point Max Element in Vector
ARM_fmaxnmv,            // Floating-point maxNum Element in Vector
ARM_uminv,              // Unsigned Integer Min Element in Vector
ARM_sminv,              // Signed Integer Min Element in Vector
ARM_fminv,              // Floating-point Min Element in Vector
ARM_fminnmv,            // Floating-point minNum Element in Vector

ARM_last

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      TMS320C6xx - VLIW (very long instruction word) architecture
 *
 */


extern instruc_t Instructions[];

enum 
{
// Original TMS320C62x instructions

TMS6_null = 0,  // Unknown Operation
TMS6_abs,       // Absolute value
TMS6_add,       // Integer addition without saturation (signed)
TMS6_addu,      // Integer addition without saturation (unsigned)
TMS6_addab,     // Integer addition using addressing mode (byte)
TMS6_addah,     // Integer addition using addressing mode (halfword)
TMS6_addaw,     // Integer addition using addressing mode (word)
TMS6_addk,      // Integer addition 16bit signed constant
TMS6_add2,      // Two 16bit Integer adds on register halves
TMS6_and,       // Logical AND
TMS6_b,         // Branch
TMS6_clr,       // Clear a bit field
TMS6_cmpeq,     // Compare for equality
TMS6_cmpgt,     // Compare for greater than (signed)
TMS6_cmpgtu,    // Compare for greater than (unsigned)
TMS6_cmplt,     // Compare for less than (signed)
TMS6_cmpltu,    // Compare for less than (unsigned)
TMS6_ext,       // Extract and sign-extend a bit filed
TMS6_extu,      // Extract an unsigned bit field
TMS6_idle,      // Multicycle NOP with no termination until interrupt
TMS6_ldb,       // Load from memory (signed 8bit)
TMS6_ldbu,      // Load from memory (unsigned 8bit)
TMS6_ldh,       // Load from memory (signed 16bit)
TMS6_ldhu,      // Load from memory (unsigned 16bit)
TMS6_ldw,       // Load from memory (32bit)
TMS6_lmbd,      // Leftmost bit detection
TMS6_mpy,       // Signed Integer Multiply (LSB16 x LSB16)
TMS6_mpyu,      // Unsigned Integer Multiply (LSB16 x LSB16)
TMS6_mpyus,     // Integer Multiply Signed*Unsigned (LSB16 x LSB16)
TMS6_mpysu,     // Integer Multiply Unsigned*Signed (LSB16 x LSB16)
TMS6_mpyh,      // Signed Integer Multiply (MSB16 x MSB16)
TMS6_mpyhu,     // Unsigned Integer Multiply (MSB16 x MSB16)
TMS6_mpyhus,    // Integer Multiply Unsigned*Signed (MSB16 x MSB16)
TMS6_mpyhsu,    // Integer Multiply Signed*Unsigned (MSB16 x MSB16)
TMS6_mpyhl,     // Signed Integer Multiply (MSB16 x LSB16)
TMS6_mpyhlu,    // Unsigned Integer Multiply (MSB16 x LSB16)
TMS6_mpyhuls,   // Integer Multiply Signed*Unsigned (MSB16 x LSB16)
TMS6_mpyhslu,   // Integer Multiply Unsigned*Signed (MSB16 x LSB16)
TMS6_mpylh,     // Signed Integer Multiply (LSB16 x MB16)
TMS6_mpylhu,    // Unsigned Integer Multiply (LSB16 x MSB16)
TMS6_mpyluhs,   // Integer Multiply Signed*Unsigned (LSB16 x MSB16)
TMS6_mpylshu,   // Integer Multiply Unsigned*Signed (LSB16 x MSB16)
TMS6_mv,        // Move from register to register
TMS6_mvc,       // Move between the control file & register file
TMS6_mvk,       // Move a 16bit signed constant into register
TMS6_mvkh,      // Move a 16bit constant into the upper bits of a register
TMS6_mvklh,     // Move a 16bit constant into the upper bits of a register
TMS6_neg,       // Negate
TMS6_nop,       // No operation
TMS6_norm,      // Normalize
TMS6_not,       // Bitwise NOT
TMS6_or,        // Logical or
TMS6_sadd,      // Integer addition with saturation
TMS6_sat,       // Saturate 40bit value to 32bits
TMS6_set,       // Set a bit field
TMS6_shl,       // Arithmetic shift left
TMS6_shr,       // Arithmetic shift right
TMS6_shru,      // Logical shift left
TMS6_smpy,      // Integer multiply with left shift & saturation (LSB16*LSB16)
TMS6_smpyhl,    // Integer multiply with left shift & saturation (MSB16*LSB16)
TMS6_smpylh,    // Integer multiply with left shift & saturation (LSB16*MSB16)
TMS6_smpyh,     // Integer multiply with left shift & saturation (MSB16*MSB16)
TMS6_sshl,      // Shift left with saturation
TMS6_ssub,      // Integer substraction with saturation
TMS6_stb,       // Store to memory (signed 8bit)
TMS6_stbu,      // Store to memory (unsigned 8bit)
TMS6_sth,       // Store to memory (signed 16bit)
TMS6_sthu,      // Store to memory (unsigned 16bit)
TMS6_stw,       // Store to memory (32bit)
TMS6_sub,       // Integer substaraction without saturation (signed)
TMS6_subu,      // Integer substaraction without saturation (unsigned)
TMS6_subab,     // Integer subtraction using addressing mode (byte)
TMS6_subah,     // Integer subtraction using addressing mode (halfword)
TMS6_subaw,     // Integer subtraction using addressing mode (word)
TMS6_subc,      // Conditional subtract & shift (for division)
TMS6_sub2,      // Two 16bit integer subtractions on register halves
TMS6_xor,       // Exclusive OR
TMS6_zero,      // Zero a register

// New TMS320C674x instructions

TMS6_abs2,      // Absolute Value With Saturation, Signed, Packed 16-bit
TMS6_absdp,     // Absolute Value, Double-Precision Floating-Point
TMS6_abssp,     // Absolute Value, Single-Precision Floating-Point
TMS6_add4,      // Add Without Saturation, Four 8-Bit Pairs for Four 8-Bit Results
TMS6_addad,     // Add Using Doubleword Addressing Mode
TMS6_adddp,     // Add Two Double-Precision Floating-Point Values
TMS6_addkpc,    // Add Signed 7-bit Constant to Program Counter
TMS6_addsp,     // Add Two Single-Precision Floating-Point Values
TMS6_addsub,    // Parallel ADD and SUB Operations On Common Inputs
TMS6_addsub2,   // Parallel ADD2 and SUB2 Operations On Common Inputs
TMS6_andn,      // Bitwise AND Invert
TMS6_avg2,      // Average, Signed, Packed 16-bit
TMS6_avgu4,     // Average, Unsigned, Packed 16-bit
TMS6_bdec,      // Branch and Decrement
TMS6_bitc4,     // Bit Count, Packed 8-bit
TMS6_bitr,      // Bit Reverse
TMS6_bnop,      // Branch With NOP
TMS6_bpos,      // Branch Positive
TMS6_callp,     // Call Using a Displacement
TMS6_cmpeq2,    // Compare for Equality, Packed 16-bit
TMS6_cmpeq4,    // Compare for Equality, Packed 8-bit
TMS6_cmpeqdp,   // Compare for Equality, Double-Precision Floating-Point Values
TMS6_cmpeqsp,   // Compare for Equality, Single-Precision Floating-Point Values
TMS6_cmpgt2,    // Compare for Greater Than, Packed 16-bit
TMS6_cmpgtdp,   // Compare for Greater Than, Double-Precision Floating-Point Values
TMS6_cmpgtsp,   // Compare for Greater Than, Single-Precision Floating-Point Values
TMS6_cmpgtu4,   // Compare for Greater Than, Unsigned, Packed 8-bit
TMS6_cmplt2,    // Compare for Less Than, Packed 16-bit
TMS6_cmpltdp,   // Compare for Less Than, Double-Precision Floating-Point Values
TMS6_cmpltsp,   // Compare for Less Than, Single-Precision Floating-Point Values
TMS6_cmpltu4,   // Compare for Less Than, Unsigned, Packed 8-bit
TMS6_cmpy,      // Complex Multiply Two Pairs, Signed, Packed 16-bit
TMS6_cmpyr,     // Complex Multiply Two Pairs, Signed, Packed 16-bit With Rounding
TMS6_cmpyr1,    // Complex Multiply Two Pairs, Signed, Packed 16-bit With Rounding
TMS6_ddotp4,    // Double Dot Product, Signed, Packed 16-Bit and Signed, Packed 8-Bit
TMS6_ddotph2,   // Double Dot Product, Two Pairs, Signed, Packed 16-Bit
TMS6_ddotph2r,  // Double Dot Product With Rounding, Two Pairs, Signed, Packed 16-Bit
TMS6_ddotpl2,   // Double Dot Product, Two Pairs, Signed, Packed 16-Bit
TMS6_ddotpl2r,  // Double Dot Product With Rounding, Two Pairs, Signed Packed 16-Bit
TMS6_deal,      // Deinterleave and Pack
TMS6_dint,      // Disable Interrupts and Save Previous Enable State
TMS6_dmv,       // Move Two Independent Registers to Register Pair
TMS6_dotp2,     // Dot Product, Signed, Packed 16-Bit
TMS6_dotpn2,    // Dot Product With Negate, Signed, Packed 16-Bit
TMS6_dotpnrsu2, // Dot Product With Negate, Shift and Round, Signed by Unsigned, Packed 16-Bit
TMS6_dotpnrus2, // Dot Product With Negate, Shift and Round, Unsigned by Signed, Packed 16-Bit
TMS6_dotprsu2,  // Dot Product With Shift and Round, Signed by Unsigned, Packed 16-Bit
TMS6_dotprus2,  // Dot Product With Shift and Round, Unsigned by Signed, Packed 16-Bit
TMS6_dotpsu4,   // Dot Product, Signed by Unsigned, Packed 8-Bit
TMS6_dotpu4,    // Dot Product, Unsigned, Packed 8-Bit
TMS6_dotpus4,   // Dot Product, Unsigned by Signed, Packed 8-Bit
TMS6_dpack2,    // Parallel PACK2 and PACKH2 Operations
TMS6_dpackx2,   // Parallel PACKLH2 Operations
TMS6_dpint,     // Convert Double-Precision Floating-Point Value to Integer
TMS6_dpsp,      // Convert Double-Precision Floating-Point Value to Single-Precision Floating-Point Value
TMS6_dptrunc,   // Convert Double-Precision Floating-Point Value to Integer With Truncation
TMS6_gmpy,      // Galois Field Multiply
TMS6_gmpy4,     // Galois Field Multiply, Packed 8-Bit
TMS6_intdp,     // Convert Signed Integer to Double-Precision Floating-Point Value
TMS6_intdpu,    // Convert Unsigned Integer to Double-Precision Floating-Point Value
TMS6_intsp,     // Convert Signed Integer to Single-Precision Floating-Point Value
TMS6_intspu,    // Convert Unsigned Integer to Single-Precision Floating-Point Value
TMS6_lddw,      // Load Doubleword From Memory With a 5-Bit Unsigned Constant Offset or Register Offset
TMS6_ldndw,     // Load Nonaligned Doubleword From Memory With Constant or Register Offset
TMS6_ldnw,      // Load Nonaligned Word From Memory With Constant or Register Offset
TMS6_max2,      // Maximum, Signed, Packed 16-Bit
TMS6_maxu4,     // Maximum, Unsigned, Packed 8-Bit
TMS6_min2,      // Minimum, Signed, Packed 16-Bit
TMS6_minu4,     // Minimum, Unsigned, Packed 8-Bit
TMS6_mpy2,      // Multiply Signed by Signed, 16 LSB x 16 LSB and 16 MSB x 16 MSB
TMS6_mpy2ir,    // Multiply Two 16-Bit x 32-Bit, Shifted by 15 to Produce a Rounded 32-Bit Result
TMS6_mpy32,     // Multiply Signed 32-Bit x Signed 32-Bit Into 32-Bit Result
TMS6_mpy32su,   // Multiply Signed 32-Bit x Unsigned 32-Bit Into Signed 64-Bit Result
TMS6_mpy32u,    // Multiply Unsigned 32-Bit x Unsigned 32-Bit Into Unsigned 64-Bit Result
TMS6_mpy32us,   // Multiply Unsigned 32-Bit x Signed 32-Bit Into Signed 64-Bit Result
TMS6_mpydp,     // Multiply Two Double-Precision Floating-Point Values
TMS6_mpyhi,     // Multiply 16 MSB x 32-Bit Into 64-Bit Result
TMS6_mpyhir,    // Multiply 16 MSB x 32-Bit, Shifted by 15 to Produce a Rounded 32-Bit Result
TMS6_mpyi,      // Multiply 32-Bit x 32-Bit Into 32-Bit Result
TMS6_mpyid,     // Multiply 32-Bit x 32-Bit Into 64-Bit Result
TMS6_mpyih,     // Multiply 32-Bit x 16-MSB Into 64-Bit Result
TMS6_mpyihr,    // Multiply 32-Bit x 16 MSB, Shifted by 15 to Produce a Rounded 32-Bit Result
TMS6_mpyil,     // Multiply 32-Bit x 16 LSB Into 64-Bit Result
TMS6_mpyilr,    // Multiply 32-Bit x 16 LSB, Shifted by 15 to Produce a Rounded 32-Bit Result
TMS6_mpyli,     // Multiply 16 LSB x 32-Bit Into 64-Bit Result
TMS6_mpylir,    // Multiply 16 LSB x 32-Bit, Shifted by 15 to Produce a Rounded 32-Bit Result
TMS6_mpysp,     // Multiply Two Single-Precision Floating-Point Values
TMS6_mpysp2dp,  // Multiply Two Single-Precision Floating-Point Values for Double-Precision Result
TMS6_mpyspdp,   // Multiply Single-Precision Floating-Point Value x Double-Precision Floating-Point Value
TMS6_mpysu4,    // Multiply Signed x Unsigned, Four 8-Bit Pairs for Four 8-Bit Results
TMS6_mpyu4,     // Multiply Unsigned x Unsigned, Four 8-Bit Pairs for Four 8-Bit Results
TMS6_mpyus4,    // Multiply Unsigned x Signed, Four 8-Bit Pairs for Four 8-Bit Results
TMS6_mvd,       // Move From Register to Register, Delayed
TMS6_mvkl,      // Move Signed Constant Into Register and Sign Extend
TMS6_pack2,     // Pack Two 16 LSBs Into Upper and Lower Register Halves
TMS6_packh2,    // Pack Two 16 MSBs Into Upper and Lower Register Halves
TMS6_packh4,    // Pack Four High Bytes Into Four 8-Bit Halfwords
TMS6_packhl2,   // Pack 16 MSB Into Upper and 16 LSB Into Lower Register Halves
TMS6_packl4,    // Pack Four Low Bytes Into Four 8-Bit Halfwords
TMS6_packlh2,   // Pack 16 LSB Into Upper and 16 MSB Into Lower Register Halves
TMS6_rcpdp,     // Double-Precision Floating-Point Reciprocal Approximation
TMS6_rcpsp,     // Single-Precision Floating-Point Reciprocal Approximation
TMS6_rint,      // Restore Previous Enable State
TMS6_rotl,      // Rotate Left
TMS6_rpack2,    // Shift With Saturation and Pack Two 16 MSBs Into Upper and Lower Register Halves
TMS6_rsqrdp,    // Double-Precision Floating-Point Square-Root Reciprocal Approximation
TMS6_rsqrsp,    // Single-Precision Floating-Point Square-Root Reciprocal Approximation
TMS6_sadd2,     // Add Two Signed 16-Bit Integers on Upper and Lower Register Halves With Saturation
TMS6_saddsu2,   // Add Two Signed and Unsigned 16-Bit Integers on Register Halves With Saturation
TMS6_saddsub,   // Parallel SADD and SSUB Operations On Common Inputs
TMS6_saddsub2,  // Parallel SADD2 and SSUB2 Operations On Common Inputs
TMS6_saddu4,    // Add With Saturation, Four Unsigned 8-Bit Pairs for Four 8-Bit Results
TMS6_saddus2,   // Add Two Unsigned and Signed 16-Bit Integers on Register Halves With Saturation
TMS6_shfl,      // Shuffle
TMS6_shfl3,     // 3-Way Bit Interleave On Three 16-Bit Values Into a 48-Bit Result
TMS6_shlmb,     // Shift Left and Merge Byte
TMS6_shr2,      // Arithmetic Shift Right, Signed, Packed 16-Bit
TMS6_shrmb,     // Shift Right and Merge Byte
TMS6_shru2,     // Arithmetic Shift Right, Unsigned, Packed 16-Bit
TMS6_smpy2,     // Multiply Signed by Signed, 16 LSB x 16 LSB and 16 MSB x 16 MSB With Left Shift and Saturation
TMS6_smpy32,    // Multiply Signed 32-Bit x Signed 32-Bit Into 64-Bit Result With Left Shift and Saturation
TMS6_spack2,    // Saturate and Pack Two 16 LSBs Into Upper and Lower Register Halves
TMS6_spacku4,   // Saturate and Pack Four Signed 16-Bit Integers Into Four Unsigned 8-Bit Halfwords
TMS6_spdp,      // Convert Single-Precision Floating-Point Value to Double-Precision Floating-Point Value
TMS6_spint,     // Convert Single-Precision Floating-Point Value to Integer
TMS6_spkernel,  // Software Pipelined Loop (SPLOOP) Buffer Operation Code Boundary
TMS6_spkernelr, // Software Pipelined Loop (SPLOOP) Buffer Operation Code Boundary
TMS6_sploop,    // Software Pipelined Loop (SPLOOP) Buffer Operation
TMS6_sploopd,   // Software Pipelined Loop (SPLOOP) Buffer Operation With Delayed Testing
TMS6_sploopw,   // Software Pipelined Loop (SPLOOP) Buffer Operation With Delayed Testing and No Epilog
TMS6_spmask,    // Software Pipelined Loop (SPLOOP) Buffer Operation Load/Execution Control
TMS6_spmaskr,   // Software Pipelined Loop (SPLOOP) Buffer Operation Load/Execution Control
TMS6_sptrunc,   // Convert Single-Precision Floating-Point Value to Integer With Truncation
TMS6_sshvl,     // Variable Shift Left
TMS6_sshvr,     // Variable Shift Right
TMS6_ssub2,     // Subtract Two Signed 16-Bit Integers on Upper and Lower Register Halves With Saturation
TMS6_stdw,      // Store Doubleword to Memory With a 5-Bit Unsigned Constant Offset or Register Offset
TMS6_stndw,     // Store Nonaligned Doubleword to Memory With a 5-Bit Unsigned Constant Offset or Register Offset
TMS6_stnw,      // Store Nonaligned Word to Memory With a 5-Bit Unsigned Constant Offset or Register Offset
TMS6_sub4,      // Subtract Without Saturation, Four 8-Bit Pairs for Four 8-Bit Results
TMS6_subabs4,   // Subtract With Absolute Value, Four 8-Bit Pairs for Four 8-Bit Results
TMS6_subdp,     // Subtract Two Double-Precision Floating-Point Values
TMS6_subsp,     // Subtract Two Single-Precision Floating-Point Values
TMS6_swap2,     // Swap Bytes in Upper and Lower Register Halves
TMS6_swap4,     // Swap Byte Pairs in Upper and Lower Register Halves
TMS6_swe,       // Software Exception
TMS6_swenr,     // Software Exception, no Return
TMS6_unpkhu4,   // Unpack 16 MSB Into Two Lower 8-Bit Halfwords of Upper and Lower Register Halves
TMS6_unpklu4,   // Unpack 16 LSB Into Two Lower 8-Bit Halfwords of Upper and Lower Register Halves
TMS6_xormpy,    // Galois Field Multiply With Zero Polynomial
TMS6_xpnd2,     // Expand Bits to Packed 16-Bit Masks
TMS6_xpnd4,     // Expand Bits to Packed 8-Bit Masks

TMS6_last,

    };

/*
 *  Interactive disassembler (IDA).
 *  Intel 80196 module
 *
 */


extern instruc_t Instructions[];

enum 
{
  I196_null = 0,    // Unknown Operation

  I196_add2,        // Add words (2 operands)
  I196_add3,        // Add words (3 operands)
  I196_addb2,       // Add bytes (2 operands)
  I196_addb3,       // Add bytes (3 operands)

  I196_addc,        // Add words with carry
  I196_addcb,       // Add bytes with carry

  I196_and2,        // Logical AND words (2 operands)
  I196_and3,        // Logical AND words (3 operands)
  I196_andb2,       // Logical AND bytes (2 operands)
  I196_andb3,       // Logical AND bytes (3 operands)

  I196_bmov,        // Block move
  I196_bmovi,       // Interruptable block move

  I196_br,          // Branch indirect

  I196_clr,         // Clear word
  I196_clrb,        // Clear byte
  I196_clrc,        // Clear carry flag
  I196_clrvt,       // Clear overflow-trap flag

  I196_cmp,         // Compare words
  I196_cmpb,        // Compare bytes
  I196_cmpl,        // Compare long

  I196_dec,         // Decrement word
  I196_decb,        // Decrement byte

  I196_di,          // Disable interrupts

  I196_div,         // Divide integers
  I196_divb,        // Divide short-integers
  I196_divu,        // Divide words, unsigned
  I196_divub,       // Divide bytes, unsigned

  I196_djnz,        // Decrement and jump if not zero
  I196_djnzw,       // Decrement and jump if not zero word

  I196_dpts,        // Disable peripheral transaction server

  I196_ei,          // Enable interrupts

  I196_epts,        // Enable peripheral transaction server

  I196_ext,         // Sign-extend integer into long-integer
  I196_extb,        // Sign-extend short-integer into integer

  I196_idlpd,       // Idle/powerdown

  I196_inc,         // Increment word
  I196_incb,        // Increment byte

  I196_jbc,         // Jump if bit is clear
  I196_jbs,         // Jump if bit is set
  I196_jc,          // Jump if carry flag is set
  I196_je,          // Jump if equal
  I196_jge,         // Jump if signed greater than or equal
  I196_jgt,         // Jump if signed greater than
  I196_jh,          // Jump if higher (unsigned)
  I196_jle,         // Jump if signed less than or equal
  I196_jlt,         // Jump if signed less than
  I196_jnc,         // Jump if carry flag is clear
  I196_jne,         // Jump if not equal
  I196_jnh,         // Jump if not higher (unsigned)
  I196_jnst,        // Jump if sticky bit flag is clear
  I196_jnv,         // Jump if overflow flag is clear
  I196_jnvt,        // Jump if overflow-trap flag is clear
  I196_jst,         // Jump if sticky bit flag is set
  I196_jv,          // Jump if overflow flag is set
  I196_jvt,         // Jump if overflow-trap flag is set

  I196_lcall,       // Long call

  I196_ld,          // Load word
  I196_ldb,         // Load byte
  I196_ldbse,       // Load byte sign-extended
  I196_ldbze,       // Load byte zero-extended

  I196_ljmp,        // Long jump

  I196_mul2,        // Multiply integers (2 operands)
  I196_mul3,        // Multiply integers (3 operands)
  I196_mulb2,       // Multiply short-integers (2 operands)
  I196_mulb3,       // Multiply short-integers (3 operands)
  I196_mulu2,       // Multiply words, unsigned (2 operands)
  I196_mulu3,       // Multiply words, unsigned (3 operands)
  I196_mulub2,      // Multiply bytes, unsigned (2 operands)
  I196_mulub3,      // Multiply bytes, unsigned (3 operands)

  I196_neg,         // Negate integer
  I196_negb,        // Negate short-integer

  I196_nop,         // No operation

  I196_norml,       // Normalize long-integer

  I196_not,         // Complement word
  I196_notb,        // Complement byte

  I196_or,          // Logical OR words
  I196_orb,         // Logical OR bytes

  I196_pop,         // Pop word
  I196_popa,        // Pop all
  I196_popf,        // Pop flags
  I196_push,        // Push word
  I196_pusha,       // Push all
  I196_pushf,       // Push flags

  I196_ret,         // Return from subroutine

  I196_rst,         // Reset system

  I196_scall,       // Short call

  I196_setc,        // Set carry flag

  I196_shl,         // Shift word left
  I196_shlb,        // Shift byte left
  I196_shll,        // Shift double-word left
  I196_shr,         // Logical right shift word
  I196_shra,        // Arithmetic right shift word
  I196_shrab,       // Arithmetic right shift byte
  I196_shral,       // Arithmetic right shift double-word
  I196_shrb,        // Logical right shift byte
  I196_shrl,        // Logical right shift double-word

  I196_sjmp,        // Short jump

  I196_skip,        // Two byte no-operation

  I196_st,          // Store word
  I196_stb,         // Store byte

  I196_sub2,        // Subtract words (2 operands)
  I196_sub3,        // Subtract words (3 operands)
  I196_subb2,       // Subtract bytes (2 operands)
  I196_subb3,       // subtract bytes (3 operands)

  I196_subc,        // Subtract words with borrow
  I196_subcb,       // Subtract bytes with borrow

  I196_tijmp,       // Table indirect jump

  I196_trap,        // Software trap

  I196_xch,         // Exchange word
  I196_xchb,        // Exchange byte

  I196_xor,         // Logical exclusive-or words
  I196_xorb,        // Logical exclusive-or bytes

// 8x196NU, NP instructions

  I196_ebmovi,      // Extended interruptable block move
  I196_ebr,         // Extended branch indirect
  I196_ecall,       // Extended call
  I196_ejmp,        // Extended jump
  I196_eld,         // Extended load word
  I196_eldb,        // Extended load byte
  I196_est,         // Extended store word
  I196_estb,        // Extended store byte

  I196_last
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      Renesas SuperH - RISC with 16bit instructions
 *
 */


extern instruc_t Instructions[];


enum 
{
SH3_null = 0,     // Unknown Operation

SH3_add,          // Add binary
SH3_addc,         // Add with Carry
SH3_addv,         // Add with V Flag Overflow Check
SH3_and,          // AND Logical
SH3_and_b,        // AND Byte Logical
SH3_bf,           // Branch if False
SH3_bf_s,         // Branch of False with Delay Slot
SH3_bra,          // Branch
SH3_braf,         // Branch Far
SH3_bsr,          // Branch to Subroutine
SH3_bsrf,         // Branch to Subroutine Far
SH3_bt,           // Branch if True
SH3_bt_s,         // Branch if True with Delay Slot
SH3_clrmac,       // Clear MAC register
SH3_clrs,         // Clear S Bit
SH3_clrt,         // Clear T Bit
SH3_cmp_eq,       // Compare: Equal
SH3_cmp_ge,       // Compare: Signed Greater or Equal
SH3_cmp_gt,       // Compare: Signed Greater
SH3_cmp_hi,       // Compare: Unsigned Greater
SH3_cmp_hs,       // Compare: Unsigned Greater or Equal
SH3_cmp_pl,       // Compare: Positive
SH3_cmp_pz,       // Compare: Positive or Zero
SH3_cmp_str,      // Compare: Equal Bytes
SH3_div0s,        // Divide Step 0 as Signed
SH3_div0u,        // Divide Step 0 as Unsigned
SH3_div1,         // Divide Step 1
SH3_dmuls_l,      // Double-Length Multiply as Signed
SH3_dmulu_l,      // Double-Length Multiply as Unsigned
SH3_dt,           // Decrement and Test
SH3_exts_b,       // Extend as Signed (Byte)
SH3_exts_w,       // Extend as Signed (Word)
SH3_extu_b,       // Extend as Unsigned (Byte)
SH3_extu_w,       // Extend as Unsigned (Word)
SH3_jmp,          // Jump
SH3_jsr,          // Jump to Subroutine
SH3_ldc,          // Load to Control Register
SH3_ldc_l,        // Load to Control Register Long
SH3_lds,          // Load to System Register
SH3_lds_l,        // Load to System Register Long
SH3_ldtlb,        // Load PTEH/PTEL/PTEA to TLB
SH3_mac_w,        // Multiply and Accumulate Word
SH3_mac_l,        // Multiply and Accumulate Long
SH3_mov,          // Move Data
SH3_mov_b,        // Move Byte Data
SH3_mov_w,        // Move Word Data
SH3_mov_l,        // Move Long Data
SH3_movi,         // Move Immediate Byte Data
SH3_movi_w,       // Move Immediate Word Data
SH3_movi_l,       // Move Immediate Long Data
SH3_movp_b,       // Move Peripherial Byte Data
SH3_movp_w,       // Move Peripherial Word Data
SH3_movp_l,       // Move Peripherial Long Data
SH3_movs_b,       // Move Structure Byte Data
SH3_movs_w,       // Move Structure Word Data
SH3_movs_l,       // Move Structure Long Data
SH3_mova,         // Move Effective Address
SH3_movt,         // Move T Bit
SH3_mul,          // Multiply Long
SH3_muls,         // Multiply as Signed Word
SH3_mulu,         // Multiply as Unsigned Word
SH3_neg,          // Negate
SH3_negc,         // Negate with Carry
SH3_nop,          // No Operation
SH3_not,          // NOT - Logical Complement
SH3_or,           // OR Logical
SH3_or_b,         // OR Byte Logical
SH3_pref,         // Prefetch Data to the Cache
SH3_rotcl,        // Rotate with Carry Left
SH3_rotcr,        // Rotate with Carry Right
SH3_rotl,         // Rotate Left
SH3_rotr,         // Rotate Right
SH3_rte,          // Return from Exception
SH3_rts,          // Return from Subroutine
SH3_sets,         // Set S Bit
SH3_sett,         // Set T Bit
SH3_shad,         // Shift Arithmetic Dynamically
SH3_shal,         // Shift Arithmetic Left
SH3_shar,         // Shift Arithmetic Right
SH3_shld,         // Shift Logical Dynamically
SH3_shll,         // Shift Logical Left
SH3_shll2,        // Shift Logical Left 2
SH3_shll8,        // Shift Logical Left 8
SH3_shll16,       // Shift Logical Left 16
SH3_shlr,         // Shift Logical Right
SH3_shlr2,        // Shift Logical Right 2
SH3_shlr8,        // Shift Logical Right 8
SH3_shlr16,       // Shift Logical Right 16
SH3_sleep,        // Sleep
SH3_stc,          // Store Control Register
SH3_stc_l,        // Store Control Register Long
SH3_sts,          // Store System Register
SH3_sts_l,        // Store System Register Long
SH3_sub,          // Subtract Binary
SH3_subc,         // Subtract with Carry
SH3_subv,         // Subtract with V Flag Underflow Check
SH3_swap_b,       // Swap Register Halves (Byte)
SH3_swap_w,       // Swap Register Halves (Word)
SH3_tas_b,        // Test and Set
SH3_trapa,        // Trap Always
SH3_tst,          // Test Logical
SH3_tst_b,        // Test Byte Logical
SH3_xor,          // Exclusive OR Logical
SH3_xor_b,        // Exclusive OR Byte Logical
SH3_xtrct,        // Extract

SH4_fabs,         // Floating-point absolute value
SH4_fadd,         // Floating-point add
SH4_fcmp_eq,      // Floating-point compare eqaul
SH4_fcmp_gt,      // Floating-point compare greater than
SH4_fcnvds,       // Floating-point convert double to single precision
SH4_fcnvsd,       // Floating-point convert single to double precision
SH4_fdiv,         // Floating-point divide
SH4_fipr,         // Floating-point inner product
SH4_fldi0,        // Floating-point Load Immediate 0.0
SH4_fldi1,        // Floating-point Load Immediate 1.0
SH4_flds,         // Floating-point Load to system register
SH4_float,        // Floating-point convert from integer
SH4_fmac,         // Floating-point multiply and accumulate
SH4_fmov,         // Floating-point move
SH4_fmov_s,       // Floating-point move single precision
SH4_fmovex,       // Floating-point move extension
SH4_fmul,         // Floating-point multiply
SH4_fneg,         // Floating-Point sign inversion
SH4_frchg,        // FR-bit change
SH4_fschg,        // SZ-bit change
SH4_fsqrt,        // Floating-point square root
SH4_fsts,         // Floating-point store system register
SH4_fsub,         // Floating-point subtract
SH4_ftrc,         // Floating-point truncate and convert to integer
SH4_ftrv,         // Floating-point transform vector
SH4_ftstn,        // Floating point square root reciprocal approximate
SH4_movca_l,      // Move with cache block allocation
SH4_ocbi,         // Operand Cache Block Invalidate
SH4_ocbp,         // Operand Cache Block Purge
SH4_ocbwb,        // Operand Cache Block Write Back
SH4_fsca,         // Floating point sine and cosine approximate

// SH-2a new instructions

SH2a_band_b,      // Bit And Byte Data
SH2a_bandnot_b,   // Bit And Not Byte Data
SH2a_bclr,        // Bit Clear
SH2a_bclr_b,      // Bit Clear Byte Data
SH2a_bld,         // Bit Load
SH2a_bld_b,       // Bit Load Byte Data
SH2a_bldnot_b,    // Bit Load Not Byte Data
SH2a_bor_b,       // Bit Or Byte Data
SH2a_bornot_b,    // Bit Or Not Byte Data
SH2a_bset,        // Bit Set
SH2a_bset_b,      // Bit Set Byte Data
SH2a_bst,         // Bit Store
SH2a_bst_b,       // Bit Store Byte Data
SH2a_bxor_b,      // Bit Exclusive Or Byte Data
SH2a_clips_b,     // Clip as Signed Byte
SH2a_clips_w,     // Clip as Signed Word
SH2a_clipu_b,     // Clip as Unsigned Byte
SH2a_clipu_w,     // Clip as Unsigned Word
SH2a_divs,        // Divide as Signed
SH2a_divu,        // Divide as Unsigned
SH2a_jsr_n,       // Jump to Subroutine with No delay slot
SH2a_ldbank,      // Load Register Bank
SH2a_movi20,      // 20-bit immediate data transfer
SH2a_movi20s,     // 20-bit immediate data transfer, 8-bit left-shift
SH2a_movml_l,     // Move Multi-register Lower part
SH2a_movmu_l,     // Move Multi-register Upper part
SH2a_movrt,       // Move Reverse T bit
SH2a_movu_b,      // Move Structure Byte Data as Unsigned
SH2a_movu_w,      // Move Structure Word Data as Unsigned
SH2a_mulr,        // Multiply to Register
SH2a_nott,        // Not T bit
SH2a_resbank,     // Restore From Register Bank
SH2a_rts_n,       // Return from Subroutine with No delay slot
SH2a_rtv_n,       // Return to Value and from Subroutine with No delay slot
SH2a_stbank,      // Store Register Bank

// SH-4a new instructions

SH4a_movco_l,     // Move Conditional
SH4a_movli_l,     // Move Linked
SH4a_movua_l,     // Move Unaligned
SH4a_icbi,        // Instruction Cache Block Invalidate
SH4a_prefi,       // Prefetch Instruction Cache Block
SH4a_synco,       // Synchronize Data Operation
SH4a_fsrra,       // Floating Point Square Reciprocal Approximate
SH4a_fpchg,       // PR-bit Change

SH4_last,

    };

/*
 *  Interactive disassembler (IDA).
 *  Zilog Z8 module
 *
 */


extern instruc_t Instructions[];

enum 
{
  Z8_null = 0,    // Unknown Operation

  Z8_adc,         // Add with carry
  Z8_add,         // Add
  Z8_and,         // Logical AND
  Z8_call,        // Call procedure
  Z8_ccf,         // Complement carry flag
  Z8_clr,         // Clear
  Z8_com,         // Complement
  Z8_cp,          // Compare
  Z8_da,          // Decimal adjust
  Z8_dec,         // Decrement
  Z8_decw,        // Decrement word
  Z8_di,          // Disable interrupts
  Z8_djnz,        // Decrement and jump if non-zero
  Z8_ei,          // Enable interrupts
  Z8_halt,        // Enter HALT mode
  Z8_inc,         // Increment
  Z8_incw,        // Increment word
  Z8_iret,        // Return from interrupt
  Z8_jp,          // Unconditional jump
  Z8_jpcond,      // Conditional jump
  Z8_jr,          // Relative jump
  Z8_jrcond,      // Conditional relative jump
  Z8_ld,          // Load data
  Z8_ldc,         // Load constant
  Z8_ldci,        // Load constant with auto-increment
  Z8_lde,         // Load external data
  Z8_ldei,        // Load external data with auto-increment
  Z8_nop,         // NOP
  Z8_or,          // Logical OR
  Z8_pop,         // Pop
  Z8_push,        // Push
  Z8_rcf,         // Reset carry flag
  Z8_ret,         // Return
  Z8_rl,          // Rotate left
  Z8_rlc,         // Rotate left through carry
  Z8_rr,          // Rotate right
  Z8_rrc,         // Rotate right through carry
  Z8_sbc,         // Subtract with carry
  Z8_scf,         // Set carry flag
  Z8_sra,         // Shift right arithmetic
  Z8_srp,         // Set register pointer
  Z8_stop,        // Enter STOP mode
  Z8_sub,         // Subtract
  Z8_swap,        // Swap nibbles
  Z8_tm,          // Test under mask
  Z8_tcm,         // Test complement under mask
  Z8_xor,         // Logical EXCLUSIVE OR
  Z8_wdh,         // Enable WATCH-DOG in HALT mode
  Z8_wdt,         // Clear WATCH-DOG timer

  Z8_last
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      Atmel AVR - 8-bit RISC processor
 *
 */


extern instruc_t Instructions[];

enum 
{

AVR_null = 0,     // Unknown Operation

// ARITHMETIC AND LOGIC INSTRUCTIONS
AVR_add,          // Add without Carry
AVR_adc,          // Add with Carry
AVR_adiw,         // Add Immediate to Word
AVR_sub,          // Subtract without Carry
AVR_subi,         // Subtract Immediate
AVR_sbc,          // Subtract with Carry
AVR_sbci,         // Subtract Immediate with Carry
AVR_sbiw,         // Subtract Immediate from Word
AVR_and,          // Logical AND
AVR_andi,         // Logical AND with Immediate
AVR_or,           // Logical OR
AVR_ori,          // Logical OR with Immediate
AVR_eor,          // Exclusive OR
AVR_com,          // One's Complement
AVR_neg,          // Two's Complement
AVR_sbr,          // Set Bit(s) in Register
AVR_cbr,          // Clear Bit(s) in Register
AVR_inc,          // Increment
AVR_dec,          // Decrement
AVR_tst,          // Test for Zero or Minus
AVR_clr,          // Clear Register
AVR_ser,          // Set Register
AVR_cp,           // Compare
AVR_cpc,          // Compare with Carry
AVR_cpi,          // Compare with Immediate
AVR_mul,          // Multiply

// BRANCH INSTRUCTIONS
AVR_rjmp,         // Relative Jump
AVR_ijmp,         // Indirect Jump to (Z)
AVR_jmp,          // Jump
AVR_rcall,        // Relative Call Subroutine
AVR_icall,        // Indirect Call to (Z)
AVR_call,         // Call Subroutine
AVR_ret,          // Subroutine Return
AVR_reti,         // Interrupt Return
AVR_cpse,         // Compare, Skip if Equal
AVR_sbrc,         // Skip if Bit in Register Cleared
AVR_sbrs,         // Skip if Bit in Register Set
AVR_sbic,         // Skip if Bit in I/O Register Cleared
AVR_sbis,         // Skip if Bit in I/O Register Set
AVR_brbs,         // Branch if Status Flag Set
AVR_brbc,         // Branch if Status Flag Cleared
AVR_breq,         // Branch if Equal
AVR_brne,         // Branch if Not Equal
AVR_brcs,         // Branch if Carry Set
AVR_brcc,         // Branch if Carry Cleared
AVR_brsh,         // Branch if Same or Higher
AVR_brlo,         // Branch if Lower
AVR_brmi,         // Branch if Minus
AVR_brpl,         // Branch if Plus
AVR_brge,         // Branch if Greater or Equal
AVR_brlt,         // Branch if Less Than
AVR_brhs,         // Branch if Half Carry Flag Set
AVR_brhc,         // Branch if Half Carry Flag Cleared
AVR_brts,         // Branch if T Flag Set
AVR_brtc,         // Branch if T Flag Cleared
AVR_brvs,         // Branch if Overflow Flag is Set
AVR_brvc,         // Branch if Overflow Flag is Cleared
AVR_brie,         // Branch if Interrupt Enabled
AVR_brid,         // Branch if Interrupt Disabled

// DATA TRANSFER INSTRUCTIONS
AVR_mov,          // Copy Register
AVR_ldi,          // Load Immediate
AVR_lds,          // Load Direct
AVR_ld,           // Load Indirect
AVR_ldd,          // Load Indirect with Displacement
AVR_sts,          // Store Direct to SRAM
AVR_st,           // Store Indirect
AVR_std,          // Store Indirect with Displacement
AVR_lpm,          // Load Program Memory
AVR_in,           // In Port
AVR_out,          // Out Port
AVR_push,         // Push Register on Stack
AVR_pop,          // Pop Register from Stack

// BIT AND BIT-TEST INSTRUCTIONS
AVR_lsl,          // Logical Shift Left
AVR_lsr,          // Logical Shift Right
AVR_rol,          // Rotate Left Through Carry
AVR_ror,          // Rotate Right Through Carry
AVR_asr,          // Arithmetic Shift Right
AVR_swap,         // Swap Nibbles
AVR_bset,         // Flag Set
AVR_bclr,         // Flag Clear
AVR_sbi,          // Set Bit in I/O Register
AVR_cbi,          // Clear Bit in I/O Register
AVR_bst,          // Bit Store from Register to T
AVR_bld,          // Bit load from T to Register
AVR_sec,          // Set Carry
AVR_clc,          // Clear Carry
AVR_sen,          // Set Negative Flag
AVR_cln,          // Clear Negative Flag
AVR_sez,          // Set Zero Flag
AVR_clz,          // Clear Zero Flag
AVR_sei,          // Global Interrupt Enable
AVR_cli,          // Global Interrupt Disable
AVR_ses,          // Set Signed Test Flag
AVR_cls,          // Clear Signed Test Flag
AVR_sev,          // Set Two's Complement Overflow
AVR_clv,          // Clear Two's Complement Overflow
AVR_set,          // Set T in SREG
AVR_clt,          // Clear T in SREG
AVR_seh,          // Set Half Carry Flag in SREG
AVR_clh,          // Clear Half Carry Flag in SREG
AVR_nop,          // No Operation
AVR_sleep,        // Sleep
AVR_wdr,          // Watchdog Reset

// New MegaAVR instructions

AVR_elpm,         // Extended Load Program Memory
AVR_espm,         // Extended Store Program Memory
AVR_fmul,         // Fractional Multiply Unsigned
AVR_fmuls,        // Fractional Multiply Signed
AVR_fmulsu,       // Fractional Multiply Signed with Unsigned
AVR_movw,         // Copy Register Word
AVR_muls,         // Multiply Signed
AVR_mulsu,        // Multiply Signed with Unsigned
AVR_spm,          // Store Program Memory
AVR_eicall,       // Extended Indirect Call to Subroutine
AVR_eijmp,        // Extended Indirect Jump

// New XMega instructions

AVR_des,          // Data Encryption Standard
AVR_lac,          // Load And Clear
AVR_las,          // Load And Set
AVR_lat,          // Load And Toggle
AVR_xch,          // Exchange

AVR_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
MIPS_null = 0,    // Unknown Operation

// The core processor instructions

MIPS_add,         // Add
MIPS_addu,        // Add Unsigned
MIPS_and,         // AND
MIPS_dadd,        // Doubleword Add
MIPS_daddu,       // Doubleword Add Unsigned
MIPS_dsub,        // Doubleword Subtract
MIPS_dsubu,       // Doubleword Subtract Unsigned
MIPS_nor,         // NOR
MIPS_or,          // OR
MIPS_slt,         // Set on Less Than
MIPS_sltu,        // Set on Less Than Unsigned
MIPS_sub,         // Subtract
MIPS_subu,        // Subtract Unsigned
MIPS_xor,         // Exclusive OR
MIPS_dsll,        // Doubleword Shift Left Logical
MIPS_dsll32,      // Doubleword Shift Left Logical + 32
MIPS_dsra,        // Doubleword Shift Right Arithmetic
MIPS_dsra32,      // Doubleword Shift Right Arithmetic + 32
MIPS_dsrl,        // Doubleword Shift Right Logical
MIPS_dsrl32,      // Doubleword Shift Right Logical + 32
MIPS_sll,         // Shift Left Logical
MIPS_sra,         // Shift Right Arithmetic
MIPS_srl,         // Shift Right Logical
MIPS_dsllv,       // Doubleword Shift Left Logical Variable
MIPS_dsrav,       // Doubleword Shift Right Arithmetic Variable
MIPS_dsrlv,       // Doubleword Shift Right Logical Variable
MIPS_sllv,        // Shift Left Logical Variable
MIPS_srav,        // Shift Right Arithmetic Variable
MIPS_srlv,        // Shift Right Logical Variable
MIPS_addi,        // Add Immediate
MIPS_addiu,       // Add Immediate Unsigned
MIPS_daddi,       // Doubleword Add Immediate
MIPS_daddiu,      // Doubleword Add Immediate Unsigned
MIPS_slti,        // Set on Less Than Immediate
MIPS_sltiu,       // Set on Less Than Immediate Unsigned
MIPS_andi,        // AND Immediate
MIPS_ori,         // OR Immediate
MIPS_xori,        // Exclusive OR Immediate
MIPS_teq,         // Trap if Equal
MIPS_tge,         // Trap if Greater Than or Equal
MIPS_tgeu,        // Trap if Greater Than or Equal Unsigned
MIPS_tlt,         // Trap if Less Than
MIPS_tltu,        // Trap if Less Than Unsigned
MIPS_tne,         // Trap if Not Equal
MIPS_cfc1,        // Move Control From FPU
MIPS_cfc2,        // Move Control From Coprocessor 2
MIPS_ctc1,        // Move Control to FPU
MIPS_ctc2,        // Move Control to Coprocessor 2
MIPS_dmfc0,       // Doubleword Move From CP0
MIPS_qmfc2,       // Quadword Move From CP2
MIPS_dmtc0,       // Doubleword Move To CP0
MIPS_qmtc2,       // Quadword Move To CP2
MIPS_mfc0,        // Move from CP0
MIPS_mfc1,        // Move from FPU
MIPS_mfc2,        // Move from CP2
MIPS_mtc0,        // Move to CP0
MIPS_mtc1,        // Move to FPU
MIPS_mtc2,        // Move to CP2
MIPS_teqi,        // Trap if Equal Immediate
MIPS_tgei,        // Trap if Greater Than or Equal Immediate
MIPS_tgeiu,       // Trap if Greater Than or Equal Immediate Unsigned
MIPS_tlti,        // Trap if Less Than Immediate
MIPS_tltiu,       // Trap if Less Than Immediate Unsigned
MIPS_tnei,        // Trap if Not Equal Immediate
MIPS_ddiv,        // Doubleword Divide
MIPS_ddivu,       // Doubleword Divide Unsigned
MIPS_div,         // Divide
MIPS_divu,        // Divide Unsigned
MIPS_dmult,       // Doubleword Multiply
MIPS_dmultu,      // Doubleword Multiply Unsigned
MIPS_mult,        // Multiply
MIPS_multu,       // Multiply Unsigned
MIPS_mthi,        // Move To HI
MIPS_mtlo,        // Move To LO
MIPS_mfhi,        // Move From HI
MIPS_mflo,        // Move From LO
MIPS_cop0,        // Coprocessor 0 Operation
MIPS_cop1,        // FPU Operation
MIPS_cop2,        // Coprocessor 2 Operation
MIPS_break,       // Break
MIPS_syscall,     // System Call
MIPS_bc0f,        // Branch on Coprocessor 0 False
MIPS_bc1f,        // Branch on FPU False
MIPS_bc2f,        // Branch on Coprocessor 2 False
MIPS_bc3f,        // Branch on Coprocessor 3 False
MIPS_bc0fl,       // Branch on Coprocessor 0 False Likely
MIPS_bc1fl,       // Branch on FPU False Likely
MIPS_bc2fl,       // Branch on Coprocessor 2 False Likely
MIPS_bc3fl,       // Branch on Coprocessor 3 False Likely
MIPS_bc0t,        // Branch on Coprocessor 0 True
MIPS_bc1t,        // Branch on FPU True
MIPS_bc2t,        // Branch on Coprocessor 2 True
MIPS_bc3t,        // Branch on Coprocessor 3 True
MIPS_bc0tl,       // Branch on Coprocessor 0 True Likely
MIPS_bc1tl,       // Branch on FPU True Likely
MIPS_bc2tl,       // Branch on Coprocessor 2 True Likely
MIPS_bc3tl,       // Branch on Coprocessor 3 True Likely
MIPS_bgez,        // Branch on Greater Than or Equal to Zero
MIPS_bgezal,      // Branch on Greater Than or Equal to Zero And Link
MIPS_bgezall,     // Branch on Greater Than or Equal to Zero And Link Likely
MIPS_bgezl,       // Branch on Greater Than or Equal to Zero Likely
MIPS_bgtz,        // Branch on Greater Than Zero
MIPS_bgtzl,       // Branch on Greater Than Zero Likely
MIPS_blez,        // Branch on Less Than or Equal to Zero
MIPS_blezl,       // Branch on Less Than or Equal to Zero Likely
MIPS_bltz,        // Branch on Less Than Zero
MIPS_bltzal,      // Branch on Less Than Zero And Link
MIPS_bltzall,     // Branch on Less Than Zero And Link Likely
MIPS_bltzl,       // Branch on Less Than Zero Likely
MIPS_beq,         // Branch on Equal
MIPS_beql,        // Branch on Equal Likely
MIPS_bne,         // Branch on Not Equal
MIPS_bnel,        // Branch on Not Equal Likely
MIPS_jalr,        // Jump And Link Register
MIPS_j,           // Jump
MIPS_jr,          // Jump Register
MIPS_jal,         // Jump And Link
MIPS_jalx,        // Jump And Link And Exchange
MIPS_cache,       // Cache Operation
MIPS_lb,          // Load Byte
MIPS_lbu,         // Load Byte Unsigned
MIPS_ldl,         // Load Doubleword Left
MIPS_ldr,         // Load Doubleword Right
MIPS_lwl,         // Load Word Left
MIPS_lwr,         // Load Word Right
MIPS_ld,          // Load Doubleword
MIPS_lld,         // Load Linked Doubleword
MIPS_ldc1,        // Load Double FPU
MIPS_ldc2,        // Load Double Coprocessor 2
MIPS_ll,          // Load Linked
MIPS_lw,          // Load Word
MIPS_lwu,         // Load Word Unsigned
MIPS_lh,          // Load Halfword
MIPS_lhu,         // Load Halfword Unsigned
MIPS_lui,         // Load Upper Immediate
MIPS_lwc1,        // Load Word to FPU
MIPS_lwc2,        // Load Word to Coprocessor 2
MIPS_sb,          // Store Byte
MIPS_sdl,         // Store Doubleword Left
MIPS_sdr,         // Store Doubleword Right
MIPS_swl,         // Store Word Left
MIPS_swr,         // Store Word Right
MIPS_scd,         // Store Conditional Doubleword
MIPS_sd,          // Store Doubleword
MIPS_sdc1,        // Store Double FPU
MIPS_sdc2,        // Store Double Coprocessor 2
MIPS_sc,          // Store Conditional
MIPS_sw,          // Store Word
MIPS_sh,          // Store Halfword
MIPS_swc1,        // Store Word from FPU
MIPS_swc2,        // Store Word from Coprocessor 2
MIPS_sync,        // Sync

// Coprocessor 0 instructions

MIPS_eret,        // Exception Return
MIPS_tlbp,        // Probe TLB for Matching Entry
MIPS_tlbr,        // Read Indexed TLB Entry
MIPS_tlbwi,       // Write Indexed TLB Entry
MIPS_tlbwr,       // Write Random TLB Entry


// Coprocessor 1 (FPU) instructions

MIPS_fadd,        // Floating-point Add
MIPS_fsub,        // Floating-point Subtract
MIPS_fmul,        // Floating-point Multiply
MIPS_fdiv,        // Floating-point Divide
MIPS_fabs,        // Floating-point Absolute Value
MIPS_fcvt_s,      // Floating-point Convert to Single Fixed-Point Format
MIPS_fcvt_d,      // Floating-point Convert to Double Floating-Point Format
MIPS_fcvt_w,      // Floating-point Convert to Fixed-Point Format
MIPS_fcvt_l,      // Floating-point Convert to Long Fixed-Point Format
MIPS_fround_l,    // Floating-point Round to Long Fixed-Point Format
MIPS_ftrunc_l,    // Floating-point Truncate to Long Fixed-Point Format
MIPS_fceil_l,     // Floating-point Ceiling to Long Fixed-Point Format
MIPS_ffloor_l,    // Floating-point Floor to Long Fixed-Point Format
MIPS_fround_w,    // Floating-point Round to Single Fixed-Point Format
MIPS_ftrunc_w,    // Floating-point Truncate to Single Fixed-Point Format
MIPS_fceil_w,     // Floating-point Ceiling to Single Fixed-Point Format
MIPS_ffloor_w,    // Floating-point Floor to Single Fixed-Point Format
MIPS_fmov,        // Floating-point Move
MIPS_fneg,        // Floating-point Negate
MIPS_fsqrt,       // Floating-point Square Root
MIPS_fc_f,        // Floating-point Compare
MIPS_fc_un,       // Floating-point Compare
MIPS_fc_eq,       // Floating-point Compare
MIPS_fc_ueq,      // Floating-point Compare
MIPS_fc_olt,      // Floating-point Compare
MIPS_fc_ult,      // Floating-point Compare
MIPS_fc_ole,      // Floating-point Compare
MIPS_fc_ule,      // Floating-point Compare
MIPS_fc_sf,       // Floating-point Compare
MIPS_fc_ngle,     // Floating-point Compare
MIPS_fc_seq,      // Floating-point Compare
MIPS_fc_ngl,      // Floating-point Compare
MIPS_fc_lt,       // Floating-point Compare
MIPS_fc_nge,      // Floating-point Compare
MIPS_fc_le,       // Floating-point Compare
MIPS_fc_ngt,      // Floating-point Compare

// Pseudo instructions

MIPS_nop,         // No operation
MIPS_mov,         // Move register
MIPS_neg,         // Negate
MIPS_negu,        // Negate Unsigned
MIPS_bnez,        // Branch on Not Zero
MIPS_bnezl,       // Branch on Not Zero Likely
MIPS_beqz,        // Branch on Zero
MIPS_beqzl,       // Branch on Zero Likely
MIPS_b,           // Branch Always
MIPS_bal,         // Branch Always and Link
MIPS_li,          // Load Immediate
MIPS_la,          // Load Address

// MIPS IV instructions

MIPS_pref,        // Prefetch
MIPS_ldxc1,       // Load Doubleword Indexed to Floating Point
MIPS_lwxc1,       // Load Word Indexed to Floating Point
MIPS_sdxc1,       // Store Doubleword Indexed from Floating Point
MIPS_swxc1,       // Store Word Indexed from Floating Point
MIPS_madd_s,      // Floating-Point Multiply Add
MIPS_madd_d,      // Floating-Point Multiply Add
MIPS_msub_s,      // Floating-Point Multiply Subtract
MIPS_msub_d,      // Floating-Point Multiply Subtract
MIPS_movf,        // Move Conditional on FP False
MIPS_movt,        // Move Conditional on FP True
MIPS_movn,        // Move Conditional on Not Zero
MIPS_movz,        // Move Conditional on Zero
MIPS_fmovf,       // Floating-Point Move Conditional on FP False
MIPS_fmovt,       // Floating-Point Move Conditional on FP True
MIPS_fmovn,       // Floating-Point Move Conditional on Not Zero
MIPS_fmovz,       // Floating-Point Move Conditional on Zero
MIPS_nmadd_s,     // Floating-Pont Negative Multiply Add
MIPS_nmadd_d,     // Floating-Pont Negative Multiply Add
MIPS_nmsub_s,     // Floating-Pont Negative Multiply Subtract
MIPS_nmsub_d,     // Floating-Pont Negative Multiply Subtract
MIPS_prefx,       // Prefetch Indexed
MIPS_frecip,      // Reciprocal Approximation
MIPS_frsqrt,      // Reciprocal Suare Root Approximation

// RSP instructions

MIPS_lbv,         // Load Byte into Vector
MIPS_lsv,         // Load Short into Vector
MIPS_llv,         // Load Word into Vector
MIPS_ldv,         // Load Doubleword into Vector
MIPS_lqv,         // Load Quadword into Vector
MIPS_lrv,         // Load Rest Vector
MIPS_lpv,         // Load Packed Vector
MIPS_luv,         // Load Unpack Vector
MIPS_lhv,         // Load Half Vector
MIPS_lfv,         // Load Fourth Vector
MIPS_lwv,         // Load Wrap Vector
MIPS_ltv,         // Load Transpose Vector
MIPS_sbv,         // Store Byte from Vector
MIPS_ssv,         // Store Short from Vector
MIPS_slv,         // Store Word from Vector
MIPS_sdv,         // Store Doubleword from Vector
MIPS_sqv,         // Store Quadword from Vector
MIPS_srv,         // Store Rest Vector
MIPS_spv,         // Store Packed Vector
MIPS_suv,         // Store Unpack Vector
MIPS_shv,         // Store Half Vector
MIPS_sfv,         // Store Fourth Vector
MIPS_swv,         // Store Wrap Vector
MIPS_stv,         // Store Transpose Vector
MIPS_vmulf,       // Vector (Frac) Multiply
MIPS_vmacf,       // Vector (Frac) Multiply Accumulate
MIPS_vmulu,       // Vector (Unsigned Frac) Multiply
MIPS_vmacu,       // Vector (Unsigned Frac) Multiply Accumulate
MIPS_vrndp,       // Vector DCT Round (+)
MIPS_vrndn,       // Vector DCT Round (-)
MIPS_vmulq,       // Vector (Integer) Multiply
MIPS_vmacq,       // Vector (Integer) Multiply Accumulate
MIPS_vmudh,       // Vector (High) Multiply
MIPS_vmadh,       // Vector (High) Multiply Accumulate
MIPS_vmudm,       // Vector (Mid-M) Multiply
MIPS_vmadm,       // Vector (Mid-M) Multiply Accumulate
MIPS_vmudn,       // Vector (Mid-N) Multiply
MIPS_vmadn,       // Vector (Mid-N) Multiply Accumulate
MIPS_vmudl,       // Vector (Low) Multiply
MIPS_vmadl,       // Vector (Low) Multiply Accumulate
MIPS_vadd,        // Vector Add
MIPS_vsub,        // Vector Subtract
MIPS_vsut,        // Vector SUT (vt - vs)
MIPS_vabs,        // Vector Absolute Value
MIPS_vaddc,       // Vector ADDC
MIPS_vsubc,       // Vector SUBC
MIPS_vaddb,       // Vector Add Byte
MIPS_vsubb,       // Vector Subtract Byte
MIPS_vaccb,       // Vector Add Byte/Add Accumulator
MIPS_vsucb,       // Vector Subtract Byte/Add Accumulator
MIPS_vsad,        // Vector SAD
MIPS_vsac,        // Vector SAC
MIPS_vsum,        // Vector SUM
MIPS_vsaw,        // Vector SAW
MIPS_vlt,         // Vector Less Than
MIPS_veq,         // Vector Equal To
MIPS_vne,         // Vector Not Equal To
MIPS_vge,         // Vector Greater Than or Equal To
MIPS_vcl,         // Vector Clip Low
MIPS_vch,         // Vector Clip High
MIPS_vcr,         // Vector 1's Complement Clip
MIPS_vmrg,        // Vector Merge
MIPS_vand,        // Vector Logical AND
MIPS_vnand,       // Vector Logical NAND
MIPS_vor,         // Vector Logical OR
MIPS_vnor,        // Vector Logical NOR
MIPS_vxor,        // Vector Logical Exclusive OR
MIPS_vnxor,       // Vector Logical NOT Exclusive OR
MIPS_vnoop,       // Vector No-Operation
MIPS_vmov,        // Vector Scalar-Element Move
MIPS_vrcp,        // Single Precision, Lookup Source, Write Result
MIPS_vrsq,        // Single Precision, Lookup Source, Write Result
MIPS_vrcph,       // Set Source, Write Previous Result
MIPS_vrsqh,       // Set Source, Write Previous Result
MIPS_vrcpl,       // Lookup Source and Previous, Write Result
MIPS_vrsql,       // Lookup Source and Previous, Write Result
MIPS_vinst,       // Vector Insert Triple (5/5/5/1)
MIPS_vextt,       // Vector Extract Triple (5/5/5/1)
MIPS_vinsq,       // Vector Insert Quad (4/4/4/4)
MIPS_vextq,       // Vector Extract Quad (4/4/4/4)
MIPS_vinsn,       // Vector Insert Nibble (4/4/4/4) Sign-Extended
MIPS_vextn,       // Vector Insert Nibble (4/4/4/4) Sign-Extended
MIPS_cfc0,        // Move Control From Coprocessor 0
MIPS_ctc0,        // Move Control to Coprocessor 0

// R5900 (PSX2 or PlayStation2) processor additional commands

MIPS_mtsa,              // Move To Shift Amount Register
MIPS_R5900_first = MIPS_mtsa,
MIPS_mfsa,              // Move From Shift Amount Register
MIPS_mtsab,             // Move Byte Count To Shift Amount Register
MIPS_mtsah,             // Move Halfword Count To Shift Amount Register
MIPS_fadda,             // Floating-point add to accumulator
MIPS_fsuba,             // Floating-point subtract to accumulator
MIPS_fmula,             // Floating-point multiply to accumulator
MIPS_fmadda,            // Floating-point multiply and add to accumulator
MIPS_fmsuba,            // Floating-point multiply and subtract from accumulator
MIPS_fmadd,             // Floating-point multiply and add
MIPS_fmsub,             // Floating-point multiply and subtract
MIPS_fmax,              // Floating-point maximum
MIPS_fmin,              // Floating-point minimum
MIPS_plzcw,             // Parallel Leading Zero or One Count Word
MIPS_mthi1,             // Move To HI1 Register
MIPS_mtlo1,             // Move To LO1 Register
MIPS_pmthl_lw,          // Parallel Move From HI/LO Register
MIPS_pmthi,             // Parallel Move To HI Register
MIPS_pmtlo,             // Parallel Move To LO Register
MIPS_div1,              // Divide Pipeline 1
MIPS_divu1,             // Divide Unsigned Pipeline 1
MIPS_pdivw,             // Parallel Divide Word
MIPS_pdivuw,            // Parallel Divide Unsigned Word
MIPS_pdivbw,            // Parallel Divide Broadcast Word
MIPS_paddw,             // Parallel Add Word
MIPS_pmaddw,            // Parallel Multiply-Add Word
MIPS_mult1,             // Multiply Pipeline 1
MIPS_multu1,            // Multiply Unsigned Pipeline 1
MIPS_madd1,             // Multiply-Add Pipeline 1
MIPS_maddu1,            // Multiply-Add Unsigned Pipeline 1
MIPS_pmadduw,           // Parallel Multiply-Add Unsigned Word
MIPS_psubw,             // Parallel Subtract HalfWord
MIPS_pcgtw,             // Parallel Compare for Greater Than Word
MIPS_psllvw,            // Parallel Shift Left Logical Variable Word
MIPS_pceqw,             // Parallel Compare for Equal Word
MIPS_pmaxw,             // Parallel Maximize Word
MIPS_psrlvw,            // Parallel Shift Right Logical Variable Word
MIPS_pminw,             // Parallel Minimize Word
MIPS_psravw,            // Parallel Shift Right Arithmetic Variable Word
MIPS_paddh,             // Parallel Add Halfword
MIPS_pmsubw,            // Parallel Multiply-Subtract Word
MIPS_padsbh,            // Parallel Add/Subtract Halfword
MIPS_psubh,             // Parallel Subtract Halfword
MIPS_pcgth,             // Parallel Compare for Greater Than Halfword
MIPS_pceqh,             // Parallel Compare for Equal Halfword
MIPS_pmaxh,             // Parallel Maximize Halfword
MIPS_pminh,             // Parallel Minimize Halfword
MIPS_paddb,             // Parallel Add Byte
MIPS_psubb,             // Parallel Subtract Byte
MIPS_pcgtb,             // Parallel Compare for Greater Than Byte
MIPS_pinth,             // Parallel Interleave Halfword
MIPS_pceqb,             // Parallel Compare for Equal Byte
MIPS_pintoh,            // Parallel Interleave Odd Halfword
MIPS_pmultw,            // Parallel Multiply Word
MIPS_pmultuw,           // Parallel Multiply Unsigned Word
MIPS_pcpyld,            // Parallel Copy Lower Doubleword
MIPS_pcpyud,            // Parallel Copy Upper Doubleword
MIPS_paddsw,            // Parallel Add with Signed Saturation Word
MIPS_pmaddh,            // Parallel Multiply-Add Halfword
MIPS_padduw,            // Parallel Add with Unsigned Saturation Word
MIPS_psubsw,            // Parallel Subtract with Signed Saturation Word
MIPS_phmadh,            // Parallel Horizontal Multiply-Add Halfword
MIPS_psubuw,            // Parallel Subtract with Unsigned Saturation Word
MIPS_pextlw,            // Parallel Extend Lower from Word
MIPS_pand,              // Parallel AND
MIPS_pextuw,            // Parallel Extend Upper from Word
MIPS_por,               // Parallel OR
MIPS_ppacw,             // Paralle Pack to Word
MIPS_pxor,              // Parallel XOR
MIPS_pnor,              // Parallel NOR
MIPS_paddsh,            // Parallel Add with Signed Saturation Halfword
MIPS_pmsubh,            // Parallel Multiply-Subtract Halfword
MIPS_padduh,            // Parallel Add with Unsigned Saturation Halfword
MIPS_psubsh,            // Parallel Subtract with Signed Saturation Halfword
MIPS_phmsbh,            // Parallel Horizontal Multiply-Subtract Halfword
MIPS_psubuh,            // Parallel Subtract with Unsigned Saturation Halfword
MIPS_pextlh,            // Parallel Extend Lower from Halfword
MIPS_pextuh,            // Parallel Extend Upper from Halfword
MIPS_ppach,             // Paralle Pack to Halfword
MIPS_paddsb,            // Parallel Add with Signed Saturation Byte
MIPS_paddub,            // Parallel Add with Unsigned Saturation Byte
MIPS_psubsb,            // Parallel Subtract with Signed Saturation Byte
MIPS_psubub,            // Parallel Subtract with Unsigned Saturation Byte
MIPS_pextlb,            // Parallel Extend Lower from Byte
MIPS_pextub,            // Parallel Extend Upper from Byte
MIPS_ppacb,             // Paralle Pack to Byte
MIPS_qfsrv,             // Quadword Funnel Shift Right Variable
MIPS_pmulth,            // Parallel Multiply Halfword
MIPS_pabsw,             // Parallel Absolute Word
MIPS_pabsh,             // Parallel Absolute Halfword
MIPS_pexoh,             // Parallel Exchange Odd Halfword
MIPS_pexch,             // Parallel Exchange Center Halfword
MIPS_prevh,             // Parallel Reverse Halfword
MIPS_pcpyh,             // Parallel Copy Halfword
MIPS_pext5,             // Parallel Extend Upper from 5 bits
MIPS_pexow,             // Parallel Exchange Odd Word
MIPS_pexcw,             // Parallel Exchange Center Word
MIPS_ppac5,             // Parallel Pack to 5 bits
MIPS_prot3w,            // Parallel Rotate 3 Words
MIPS_psllh,             // Parallel Shift Left Logical Halfword
MIPS_psrlh,             // Parallel Shift Right Logical Halfword
MIPS_psrah,             // Parallel Shift Right Arithmetic Halfword
MIPS_psllw,             // Parallel Shift Left Logical Word
MIPS_psrlw,             // Parallel Shift Right Logical Word
MIPS_psraw,             // Parallel Shift Right Arithmetic Word
MIPS_mfhi1,             // Move From HI1 Register
MIPS_mflo1,             // Move From LO1 Register
MIPS_pmfhi,             // Parallel Move From HI Register
MIPS_pmflo,             // Parallel Move From LO Register
MIPS_pmfhl,             // Parallel Move From HI/LO Register
MIPS_lq,                // Load Quadword
MIPS_sq,                // Store Quadword
MIPS_lqc2,              // Load Quadword Coprocessor 2
MIPS_sqc2,              // Store Quadword Coprocessor 2
MIPS_madd_r5900,        // Multiply/Add
MIPS_maddu_r5900,       // Multiply/Add Unsigned
MIPS_R5900_last = MIPS_maddu_r5900,
MIPS_mult3,             // Multiply (3-operand)
MIPS_multu3,            // Multiply Unsigned (3-operand)

// 16-bit instructions
// NOTE: in previous version of IDA, all mips16 instructions
//  had separate numbers, even for the instructions with the
//  same mnemonics. Now same mnemonics have the same numbers,
//  regardless of the encoding.

MIPS_bteqz=439,           // Branch on T Equal to Zero
MIPS_btnez,               // Branch on T Not Equal to Zero
MIPS_cmp,                 // Compare
MIPS_cmpi,                // Compare Immediate
MIPS_extend=458,          // Extend
MIPS_move=473,            // Move
MIPS_not=477,             // Not
MIPS_dla=495,             // Load 64-bit address

// Instructions which belong to MIPS32 but which were not decoded by IDA

MIPS_clo=498,           // Count Leading Ones in Word
MIPS_clz,               // Count Leading Zeros in Word
MIPS_madd,              // Multiply/Add
MIPS_maddu,             // Multiply/Add Unsigned
MIPS_msub,              // Multiply and Subtract Word to Hi,Lo
MIPS_msubu,             // Multiply and Subtract Word to Hi,Lo
MIPS_mul,               // Multiply Word to GPR
MIPS_sdbbp,             // Software Debug Breakpoint
MIPS_wait,              // Enter Standby Mode

// New instructions for MIPS32 Release 2.0

MIPS_alnv_ps,           // Floating Point Align Variable
MIPS_deret,             // Debug Exception Return
MIPS_di,                // Disable interrupts
MIPS_ehb,               // Execution Hazard Barrier
MIPS_ei,                // Enable interrupts
MIPS_ext,               // Extract Bit Field
MIPS_fcvt_ps,           // Floating Point Convert Pair to Paired Single
MIPS_fcvt_s_pl,         // Floating Point Convert Pair Lower to Single Floating Point
MIPS_fcvt_s_pu,         // Floating Point Convert Pair Upper to Single Floating Point
MIPS_ins,               // Insert Bit Field
MIPS_jalr_hb,           // Jump and Link Register with Hazard Barrier
MIPS_jr_hb,             // Jump Register with Hazard Barrier
MIPS_luxc1,             // Load Doubleword Indexed Unaligned to Floating Point
MIPS_madd_ps,           // Floating Point Multiply Add
MIPS_mfhc1,             // Move Word from High Half of Floating Point Register
MIPS_mfhc2,             // Move Word from High Half of Coprocessor 2 Register
MIPS_msub_ps,           // Floating Point Multiply Subtract
MIPS_mthc1,             // Move Word to High Half of Floating Point Register
MIPS_mthc2,             // Move Word to High Half of Coprocessor 2 Register
MIPS_nmadd_ps,          // Floating Point Negative Multiply Add
MIPS_nmsub_ps,          // Floating Point Negative Multiply Subtract
MIPS_pll,               // Pair Lower Lower
MIPS_plu,               // Pair Lower Upper
MIPS_pul,               // Pair Upper Lower
MIPS_puu,               // Pair Upper Upper
MIPS_rdhwr,             // Read Hardware Register
MIPS_rdpgpr,            // Read GPR from Previous Shadow Set
MIPS_rotr,              // Rotate Word Right
MIPS_rotrv,             // Rotate Word Right Variable
MIPS_seb,               // Sign-Extend Byte
MIPS_seh,               // Sign-Extend Halfword
MIPS_suxc1,             // Store Doubleword Indexed Unaligned from Floating Point
MIPS_synci,             // Synchronize Caches to Make Instruction Writes Effective
MIPS_wrpgpr,            // Write GPR to Previous Shadow Set
MIPS_wsbh,              // Word Swap Bytes Within Halfwords

// Missing instructions - for some reason they were not supported before

MIPS_dmfc1,             // Doubleword Move From CP1
MIPS_dmtc1,             // Doubleword Move To CP1

// additional MIPS16e instructions

MIPS_save,              // Save Registers and Set Up Stack Frame
MIPS_restore,           // Restore Registers and Deallocate Stack Frame
MIPS_jalrc,             // Jump and Link Register, Compact
MIPS_jrc,               // Jump Register, Compact
MIPS_sew,               // Sign-Extend Word
MIPS_zeb,               // Zero-Extend Byte
MIPS_zeh,               // Zero-Extend Halfword
MIPS_zew,               // Zero-Extend Word

// extra pseudoinstructions
MIPS_ssnop,             // Superscalar No operation
MIPS_li_s,              // Load floating-point immediate
MIPS_li_d,              // Load floating-point immediate
MIPS_dneg,              // Negate
MIPS_dnegu,             // Negate Unsigned
MIPS_pause,             // Wait for the LLBit to clear

// Missing instructions from MIPS64 Release 2

MIPS_dclo,              // Count Leading Ones in Doubleword
MIPS_dclz,              // Count Leading Zeros in Doubleword
MIPS_dext,              // Doubleword Extract Bit Field
MIPS_dextm,             // Doubleword Extract Bit Field Middle
MIPS_dextu,             // Doubleword Extract Bit Field Upper
MIPS_dins,              // Doubleword Insert Bit Field
MIPS_dinsm,             // Doubleword Insert Bit Field Middle
MIPS_dinsu,             // Doubleword Insert Bit Field Upper
MIPS_dmfc2,             // Doubleword Move From CP2
MIPS_dmtc2,             // Doubleword Move To CP2
MIPS_drotr,             // Doubleword Rotate Right
MIPS_drotr32,           // Doubleword Rotate Right Plus 32
MIPS_drotrv,            // Doubleword Rotate Right Variable
MIPS_dsbh,              // Doubleword Swap Bytes Within Halfwords
MIPS_dshd,              // Doubleword Swap Halfwords Within Doublewords

// cnMIPS (Cavium Networks Octeon) instructions

MIPS_baddu,             // Unsigned Byte Add
MIPS_bbit0,             // Branch on Bit Clear
MIPS_bbit032,           // Branch on Bit Clear Plus 32
MIPS_bbit1,             // Branch on Bit Set
MIPS_bbit132,           // Branch on Bit Set Plus 32
MIPS_cins,              // Clear and Insert a Bit Field
MIPS_cins32,            // Clear and Insert a Bit Field Plus 32
MIPS_dmul,              // Multiply Doubleword to GPR
MIPS_dpop,              // Count Ones in a Doubleword
MIPS_exts,              // Extract a Signed Bit Field
MIPS_exts32,            // Extract a Signed Bit Field Plus 32
MIPS_mtm0,              // Load Multiplier Register MPL0
MIPS_mtm1,              // Load Multiplier Register MPL1
MIPS_mtm2,              // Load Multiplier Register MPL2
MIPS_mtp0,              // Load Multiplier Register P0
MIPS_mtp1,              // Load Multiplier Register P1
MIPS_mtp2,              // Load Multiplier Register P2
MIPS_pop,               // Count Ones in a Word
MIPS_saa,               // Store Atomic Add Word
MIPS_saad,              // Store Atomic Add Double Word
MIPS_seq,               // Set on Equal
MIPS_seqi,              // Set on Equal Immediate
MIPS_sne,               // Set on Not Equal
MIPS_snei,              // Set on Not Equal Immediate
MIPS_synciobdma,        // Synchronize IOBDMAs
MIPS_syncs,             // Synchronize Special
MIPS_syncw,             // Synchronize Stores
MIPS_syncws,            // Synchronize Stores Special
MIPS_uld,               // Unaligned Load Doubleword
MIPS_ulw,               // Unaligned Load Word
MIPS_usd,               // Unaligned Store Doubleword
MIPS_usw,               // Unaligned Store Word
MIPS_v3mulu,            // 192-bit x 64-bit Unsigned Multiply and Add
MIPS_vmm0,              // 64-bit Unsigned Multiply and Add Move
MIPS_vmulu_cn,          // 64-bit Unsigned Multiply and Add

// NEC VR5432 and PSP instructions

MIPS_dbreak,            // Debug Break
MIPS_dret,              // Debug Return
MIPS_mfdr,              // Move from Debug Register
MIPS_mtdr,              // Move to Debug Register

// Allegrex (Sony PSP) instructions

PSP_bitrev,            // Bit reverse
PSP_max,               // Maximum
PSP_min,               // Minimum
PSP_mfic,              // Move from interrupt controller
PSP_mtic,              // Move to interrupt controller
PSP_wsbw,              // Word Swap Bytes Within Word
PSP_sleep,             // Sleep

// Allegrex VFPU instructions

PSP_lv,                // Load Vector
PSP_lvl,               // Load Vector Left
PSP_lvr,               // Load Vector Right
PSP_sv,                // Store Vector
PSP_svl,               // Store Vector Left
PSP_svr,               // Store Vector Right
PSP_mfv,               // Move from VFPU
PSP_mtv,               // Move to VFPU
PSP_mfvc,              // Move Control from VFPU
PSP_mtvc,              // Move Control to VFPU
PSP_bvf,               // Branch on VFPU False
PSP_bvt,               // Branch on VFPU True
PSP_bvfl,              // Branch on VFPU False Likely
PSP_bvtl,              // Branch on VFPU True Likely
PSP_vnop,              // VFPU no-op
PSP_vflush,            // VFPU flush
PSP_vsync,             // VFPU sync
PSP_vabs,              // Vector absolute value
PSP_vadd,              // Vector add
PSP_vasin,             // Vector arcsine
PSP_vavg,              // Vector average
PSP_vbfy1,             // IDCT butterfly 1
PSP_vbfy2,             // IDCT butterfly 2
PSP_vc2i,              // Vector convert signed char to integer
PSP_vcmovf,            // Vector move if condition field is true
PSP_vcmovt,            // Vector move if condition field is false
PSP_vcmp,              // Vector compare and set condition fields
PSP_vcos,              // Vector cosine
PSP_vcrs,              // Vector cross multiplication: vd = [y1*z2, z1*x2, x1*y2]
PSP_vcrsp,             // Vector cross product
PSP_vcst,              // Set constant
PSP_vdet,              // Determinant
PSP_vdiv,              // Vector divide
PSP_vdot,              // Vector dot product
PSP_vexp2,             // Vector exponent of 2 (2^x)
PSP_vf2h,              // Vector convert float single to half precision
PSP_vf2id,             // Vector convert float to integer, round down
PSP_vf2in,             // Vector convert float to integer, round to nearest
PSP_vf2iu,             // Vector convert float to integer, round up
PSP_vf2iz,             // Vector convert float to integer, round toward zero
PSP_vfad,              // Vector funnel add (sum components)
PSP_vfim,              // Set floating-point immediate
PSP_vh2f,              // Vector convert float half to single precision
PSP_vhdp,              // Vector homogenous dot product
PSP_vhtfm2,            // Homogenous transform vector by matrix
PSP_vhtfm3,            // Homogenous transform vector by matrix
PSP_vhtfm4,            // Homogenous transform vector by matrix
PSP_vi2c,              // Vector convert integer to signed char
PSP_vi2f,              // Vector convert integer to float
PSP_vi2s,              // Vector convert integer to signed short
PSP_vi2uc,             // Vector convert integer to unsigned char
PSP_vi2us,             // Vector convert integer to unsigned short
PSP_vidt,              // Set vector to identity
PSP_viim,              // Set integer immediate
PSP_vlgb,              // 
PSP_vlog2,             // Vector logarithm base 2
PSP_vmax,              // Vector maximum values
PSP_vmfvc,             // Vector move from control register
PSP_vmidt,             // Set matrix to identity
PSP_vmin,              // Vector minimum values
PSP_vmmov,             // Move matrix
PSP_vmmul,             // Matrix multiply
PSP_vmone,             // Set matrix to ones
PSP_vmov,              // Move vector
PSP_vmscl,             // Scale matrix by
PSP_vmtvc,             // Vector move to control register
PSP_vmul,              // Vector multiply
PSP_vmzero,            // Set matrix to zeroes
PSP_vneg,              // Vector negate
PSP_vnrcp,             // Vector negative reciprocal (-1/x)
PSP_vnsin,             // Vector negative sine
PSP_vocp,              // Vector one complement (1-x)
PSP_vone,              // Set vector to ones
PSP_vpfxd,             // Set prefix operation for vd
PSP_vpfxs,             // Set prefix operation for vs
PSP_vpfxt,             // Set prefix operation for vt
PSP_vqmul,             // Quaternion multiply
PSP_vrcp,              // Vector reciprocal (1/x)
PSP_vrexp2,            // Vector reciprocal exponent of 2 (1/(2^x))
PSP_vrndf1,            // Vector generate pseudorandom float (1.0 ~ 2.0)
PSP_vrndf2,            // Vector generate pseudorandom float (2.0 ~ 4.0)
PSP_vrndi,             // Vector generate pseudorandom integer
PSP_vrnds,             // Vector set pseudorandom seed
PSP_vrot,              // Rotate vector
PSP_vrsq,              // Vector reciprocal square root (1/sqrt(x))
PSP_vs2i,              // Vector convert signed short to integer
PSP_vsat0,             // Vector saturate to range 0..1
PSP_vsat1,             // Vector saturate to range -1..1
PSP_vsbn,              // Vector scale by 2^x, round to nearest
PSP_vsbz,              // Vector scale by 2^x, round towards zero
PSP_vscl,              // Vector scale by
PSP_vscmp,             // Vector set signed compare
PSP_vsge,              // Vector set results for greater than or equal
PSP_vsgn,              // Vector get sign
PSP_vsin,              // Vector sine
PSP_vslt,              // Vector set results for less
PSP_vsocp,             // Vector split and one complement
PSP_vsqrt,             // Vector square root
PSP_vsrt1,             // Vector sort 1: vd = min(x,y), max(x,y), min(z,w), max(z,w)
PSP_vsrt2,             // Vector sort 2: vd = min(x,w), max(y,z), min(y,z), max(x,w)
PSP_vsrt3,             // Vector sort 3: vd = max(x,y), min(x,y), max(z,w), min(z,w)
PSP_vsrt4,             // Vector sort 4: vd = max(x,w), max(y,z), min(y,z), max(x,w)
PSP_vsub,              // Vector subtract
PSP_vt4444,            // Transform color RGBA8888 to RGBA4444
PSP_vt5551,            // Transform color RGBA8888 to RGBA5551
PSP_vt5650,            // Transform color RGB888 to RGB565
PSP_vtfm2,             // Transform vector by matrix
PSP_vtfm3,             // Transform vector by matrix
PSP_vtfm4,             // Transform vector by matrix
PSP_vuc2i,             // Vector convert unsigned char to integer
PSP_vus2i,             // Vector convert unsigned short to integer
PSP_vwbn,              // 
PSP_vzero,             // VFPU set vector to zeroes

// PSP Media Engine instructions
PSP_mfvme,  // move from VME
PSP_mtvme,  // move to VME

// Toshiba TX19a instructions
MIPS_ac0iu,            // Add Coprocessor 0 Immediate Unsigned
MIPS_bs1f,             // Bit Search One Forward
MIPS_bfins,            // Bit field insert
MIPS_addmiu,           // Add Immediate to Memory Word
MIPS_sadd,             // Saturated Add
MIPS_ssub,             // Saturated Subtract
MIPS_btst,             // Bit Test
MIPS_bclr,             // Bit Clear
MIPS_bset,             // Bit Set
MIPS_bins,             // Bit Insert
MIPS_bext,             // Bit Extract
MIPS_dive,             // Divide, with Overflow Exception
MIPS_diveu,            // Divide unsigned, with Overflow Exception
MIPS_min,              // Minimum signed
MIPS_max,              // Maximum signed

MIPS_madd3,            // Multiply/Add (3-operand)
MIPS_maddu3,           // Multiply/Add Unsigned (3-operand)
MIPS_msub3,            // Multiply and Subtract Word to Hi,Lo (3-operand)
MIPS_msubu3,           // Multiply and Subtract Word to Hi,Lo (3-operand)

// MIPS-MT
MIPS_dvpe,             // Disable Virtual Processor Execution
MIPS_evpe,             // Enable Virtual Processor Execution
MIPS_dmt,              // Disable Multi-Threaded Execution
MIPS_emt,              // Enable Multi-Threaded Execution
MIPS_fork,             // Allocate and Schedule a New Thread
MIPS_yield,            // Conditionally Deschedule or Deallocate the Current Thread
MIPS_mftr,             // Move From Thread Context
MIPS_mftc0,
MIPS_mftlo,
MIPS_mfthi,
MIPS_mftacx,
MIPS_mftdsp,
MIPS_mfthc1,
MIPS_mftc1,
MIPS_cftc1,
MIPS_mfthc2,
MIPS_mftc2,
MIPS_cftc2,
MIPS_mftgpr,
MIPS_mttr,             // Move To Thread Context
MIPS_mttc0,
MIPS_mttlo,
MIPS_mtthi,
MIPS_mttacx,
MIPS_mttdsp,
MIPS_mtthc1,
MIPS_mttc1,
MIPS_cttc1,
MIPS_mtthc2,
MIPS_mttc2,
MIPS_cttc2,
MIPS_mttgpr,

// MIPS-3D
MIPS_faddr,          // Floating-point Reduction Addition
MIPS_bc1any2f,       // Branch on Any of Two Floating Point Condition Codes False
MIPS_bc1any2t,       // Branch on Any of Two Floating Point Condition Codes True
MIPS_bc1any4f,       // Branch on Any of Four Floating Point Condition Codes False
MIPS_bc1any4t,       // Branch on Any of Four Floating Point Condition Codes True
MIPS_fcabs_f,        // Floating-point Absolute Compare
MIPS_fcabs_un,       // Floating-point Absolute Compare
MIPS_fcabs_eq,       // Floating-point Absolute Compare
MIPS_fcabs_ueq,      // Floating-point Absolute Compare
MIPS_fcabs_olt,      // Floating-point Absolute Compare
MIPS_fcabs_ult,      // Floating-point Absolute Compare
MIPS_fcabs_ole,      // Floating-point Absolute Compare
MIPS_fcabs_ule,      // Floating-point Absolute Compare
MIPS_fcabs_sf,       // Floating-point Absolute Compare
MIPS_fcabs_ngle,     // Floating-point Absolute Compare
MIPS_fcabs_seq,      // Floating-point Absolute Compare
MIPS_fcabs_ngl,      // Floating-point Absolute Compare
MIPS_fcabs_lt,       // Floating-point Absolute Compare
MIPS_fcabs_nge,      // Floating-point Absolute Compare
MIPS_fcabs_le,       // Floating-point Absolute Compare
MIPS_fcabs_ngt,      // Floating-point Absolute Compare
MIPS_fcvt_pw_ps,     // Floating-point Convert Paired Single to Paired Word
MIPS_fcvt_ps_pw,     // Floating-point Convert Paired Word to Paired Single
MIPS_fmulr,          // Floating-point Reduction Multiply
MIPS_frecip1,        // Floating-point Reduced Precision Reciprocal (Step 1)
MIPS_frecip2,        // Floating-point Reduced Precision Reciprocal (Step 2)
MIPS_frsqrt1,        // Floating-point Reduced Precision Reciprocal Square Root (Step 1)
MIPS_frsqrt2,        // Floating-point Reduced Precision Reciprocal Square Root (Step 2)

// smartMIPS
MIPS_lwxs,
MIPS_maddp,
MIPS_mflhxu,
MIPS_mtlhx,
MIPS_multp,
MIPS_pperm,

// microMIPS
MIPS_jals,      // Jump and Link, Short Delay Slot
MIPS_lwp,       // Load Word Pair
MIPS_ldp,       // Load Doubleword Pair
MIPS_lwm,       // Load Word Multiple
MIPS_ldm,       // Load Doubleword Multiple
MIPS_swp,       // Store Word Pair
MIPS_sdp,       // Store Doubleword Pair
MIPS_swm,       // Store Word Multiple
MIPS_sdm,       // Store Doubleword Multiple
MIPS_bnezc,     // Branch on Not Equal to Zero, Compact
MIPS_bltzals,   // Branch on Less Than Zero and Link, Short Delay-Slot
MIPS_beqzc,     // Branch on Equal to Zero, Compact
MIPS_bgezals,   // Branch on Greater Than or Equal to Zero and Link, Short Delay-Slot
MIPS_jraddiusp, // Jump Register, Adjust Stack Pointer
MIPS_jalrs,     // Jump and Link Register, Short Delay Slot
MIPS_jalrs_hb,  // Jump and Link Register with Hazard Barrier, Short Delay-Slot
MIPS_movep,     // Move a Pair of Registers

// had been missed; 64-bit MIPS pseudoinstruction
MIPS_dli,       // Doubleword Load Immediate

// DSP ASE instructions
MIPS_insv,             // Insert Bit Field Variable
MIPS_dinsv,            // Doubleword Insert Variable Bit Field
MIPS_bposge32,         // Branch on Greater Than or Equal To Value 32 in DSPControl Pos Field
MIPS_bposge64,         // Branch on Greater Than or Equal To Value 64 in DSPControl Pos Field
MIPS_addu_qb,          // Unsigned Add Quad Byte Vectors
MIPS_addu_ph,          // Unsigned Add Integer Halfwords
MIPS_addsc,            // Add Signed Word and Set Carry Bit
MIPS_subu_qb,          // Subtract Unsigned Quad Byte Vector
MIPS_subu_ph,          // Subtract Unsigned Integer Halfwords
MIPS_addwc,            // Add Word with Carry Bit
MIPS_addq_ph,          // Add Fractional Halfword Vectors
MIPS_modsub,           // Modular Subtraction on an Index Value
MIPS_subq_ph,          // Subtract Fractional Halfword Vector
MIPS_addu_s_qb,        // Unsigned Add Quad Byte Vectors
MIPS_addu_s_ph,        // Unsigned Add Integer Halfwords
MIPS_raddu_w_qb,       // Unsigned Reduction Add Vector Quad Bytes
MIPS_muleq_s_w_phl,    // Multiply Vector Fractional Left Halfwords to Expanded Width Products
MIPS_subu_s_qb,        // Subtract Unsigned Quad Byte Vector
MIPS_subu_s_ph,        // Subtract Unsigned Integer Halfwords
MIPS_muleq_s_w_phr,    // Multiply Vector Fractional Right Halfwords to Expanded Width Products
MIPS_muleu_s_ph_qbl,   // Multiply Unsigned Vector Left Bytes by Halfwords to Halfword Products
MIPS_addq_s_ph,        // Add Fractional Halfword Vectors
MIPS_addq_s_w,         // Add Fractional Words
MIPS_mulq_s_ph,        // Multiply Vector Fractional Half-Words to Same Size Products
MIPS_muleu_s_ph_qbr,   // Multiply Unsigned Vector Right Bytes with halfwords to Half Word Products
MIPS_subq_s_ph,        // Subtract Fractional Halfword Vector
MIPS_subq_s_w,         // Subtract Fractional Word
MIPS_mulq_rs_ph,       // Multiply Vector Fractional Halfwords to Fractional Halfword Products
MIPS_addu_ob,          // Unsigned Add Octal Byte Vectors
MIPS_subu_ob,          // Subtract Unsigned Octal Byte Vector
MIPS_addq_qh,          // Add Fractional Halfword Vectors
MIPS_addq_pw,          // Add Fractional Word Vectors
MIPS_subq_qh,          // Subtract Fractional Halfword Vector
MIPS_subq_pw,          // Subtract Fractional Word Vector
MIPS_addu_s_ob,        // Unsigned Add Octal Byte Vectors
MIPS_raddu_l_ob,       // Unsigned Reduction Add Vector Octal Bytes
MIPS_muleq_s_pw_qhl,   // Multiply Vector Fractional Left Halfwords to Expanded Width Products
MIPS_subu_s_ob,        // Subtract Unsigned Octal Byte Vector
MIPS_muleq_s_pw_qhr,   // Multiply Vector Fractional Right Halfwords to Expanded Width Products
MIPS_muleu_s_qh_obl,   // Multiply Unsigned Vector Left Bytes by Halfwords to Halfword Products
MIPS_addq_s_qh,        // Add Fractional Halfword Vectors
MIPS_addq_s_pw,        // Add Fractional Word Vectors
MIPS_muleu_s_qh_obr,   // Multiply Unsigned Vector Right Bytes by Halfwords to Halfword Products
MIPS_subq_s_qh,        // Subtract Fractional Halfword Vector
MIPS_subq_s_pw,        // Subtract Fractional Word Vector
MIPS_mulq_rs_qh,       // Multiply Vector Fractional Halfwords to Fractional Halfword Products
MIPS_cmpu_eq_qb,       // Compare Vectors of Unsigned Byte Values
MIPS_cmp_eq_ph,        // Compare Vectors of Signed Integer Halfword Values
MIPS_cmpgdu_eq_qb,     // Compare Unsigned Vector of Four Bytes and Write Result to GPR and DSPControl
MIPS_cmpu_lt_qb,       // Compare Vectors of Unsigned Byte Values
MIPS_cmp_lt_ph,        // Compare Vectors of Signed Integer Halfword Values
MIPS_cmpgdu_lt_qb,     // Compare Unsigned Vector of Four Bytes and Write Result to GPR and DSPControl
MIPS_cmpu_le_qb,       // Compare Vectors of Unsigned Byte Values
MIPS_cmp_le_ph,        // Compare Vectors of Signed Integer Halfword Values
MIPS_cmpgdu_le_qb,     // Compare Unsigned Vector of Four Bytes and Write Result to GPR and DSPControl
MIPS_pick_qb,          // Pick a Vector of Byte Values Based on Condition Code Bits
MIPS_pick_ph,          // Pick a Vector of Halfword Values Based on Condition Code Bits
MIPS_cmpgu_eq_qb,      // Compare Vectors of Unsigned Byte Values and Write Results to a GPR
MIPS_precrq_qb_ph,     // Precision Reduce Four Fractional Halfwords to Four Bytes
MIPS_precrq_ph_w,      // Precision Reduce Fractional Words to Fractional Halfwords
MIPS_cmpgu_lt_qb,      // Compare Vectors of Unsigned Byte Values and Write Results to a GPR
MIPS_precr_qb_ph,      // Precision Reduce Four Integer Halfwords to Four Bytes
MIPS_precrq_rs_ph_w,   // Precision Reduce Fractional Words to Halfwords With Rounding and Saturation
MIPS_cmpgu_le_qb,      // Compare Vectors of Unsigned Byte Values and Write Results to a GPR
MIPS_packrl_ph,        // Pack a Vector of Halfwords from Vector Halfword Sources
MIPS_precr_sra_ph_w,   // Precision Reduce Two Integer Words to Halfwords after a Right Shift
MIPS_precrqu_s_qb_ph,  // Precision Reduce Fractional Halfwords to Unsigned Bytes With Saturation
MIPS_precr_sra_r_ph_w, // Precision Reduce Two Integer Words to Halfwords after a Right Shift
MIPS_cmpu_eq_ob,       // Compare Vectors of Unsigned Byte Values
MIPS_cmp_eq_qh,        // Compare Vectors of Signed Integer Halfword Values
MIPS_cmp_eq_pw,        // Compare Vectors of Signed Integer Word Values
MIPS_cmpu_lt_ob,       // Compare Vectors of Unsigned Byte Values
MIPS_cmp_lt_qh,        // Compare Vectors of Signed Integer Halfword Values
MIPS_cmp_lt_pw,        // Compare Vectors of Signed Integer Word Values
MIPS_cmpu_le_ob,       // Compare Vectors of Unsigned Byte Values
MIPS_cmp_le_qh,        // Compare Vectors of Signed Integer Halfword Values
MIPS_cmp_le_pw,        // Compare Vectors of Signed Integer Word Values
MIPS_pick_ob,          // Pick a Vector of Byte Values Based on Condition Code Bits
MIPS_pick_qh,          // Pick a Vector of Halfword Values Based on Condition Code Bits
MIPS_pick_pw,          // Pick a Vector of Word Values Based on Condition Code Bits
MIPS_cmpgu_eq_ob,      // Compare Vectors of Unsigned Byte Values and Write Results to a GPR
MIPS_precrq_ob_qh,     // Precision Reduce Fractional Halfwords to Fractional Bytes
MIPS_precrq_qh_pw,     // Precision Reduce Fractional Words to Fractional Halfwords
MIPS_precrq_pw_l,      // Precision Reduce Fractional Doublewords to Fractional Words
MIPS_cmpgu_lt_ob,      // Compare Vectors of Unsigned Byte Values and Write Results to a GPR
MIPS_precrq_rs_qh_pw,  // Precision Reduce Fractional Words to Halfwords With Rounding and Saturation
MIPS_cmpgu_le_ob,      // Compare Vectors of Unsigned Byte Values and Write Results to a GPR
MIPS_packrl_pw,        // Pack a Vector of Words from Vector Word Sources
MIPS_precrqu_s_ob_qh,  // Precision Reduce Fractional Halfwords to Unsigned Bytes With Saturation
MIPS_absq_s_qb,        // Find Absolute Value of Four Fractional Byte Values
MIPS_absq_s_ph,        // Find Absolute Value of Two Fractional Halfwords
MIPS_absq_s_w,         // Find Absolute Value of Fractional Word
MIPS_repl_qb,          // Replicate Immediate Integer into all Vector Element Positions
MIPS_repl_ph,          // Replicate Immediate Integer into all Vector Element Positions
MIPS_replv_qb,         // Replicate Byte into all Vector Element Positions
MIPS_replv_ph,         // Replicate a Halfword into all Vector Element Positions
MIPS_bitrev,           // Bit-Reverse Halfword
MIPS_precequ_ph_qbl,   // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceq_w_phl,     // Precision Expand Fractional Halfword to Fractional Word Value
MIPS_preceu_ph_qbl,    // Precision Expand Two Unsigned Bytes to Unsigned Halfword Values
MIPS_precequ_ph_qbr,   // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceq_w_phr,     // Precision Expand Fractional Halfword to Fractional Word Value
MIPS_preceu_ph_qbr,    // Precision Expand two Unsigned Bytes to Unsigned Halfword Values
MIPS_precequ_ph_qbla,  // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceu_ph_qbla,   // Precision Expand Two Unsigned Bytes to Unsigned Halfword Values
MIPS_precequ_ph_qbra,  // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceu_ph_qbra,   // Precision Expand Two Unsigned Bytes to Unsigned Halfword Values
MIPS_absq_s_qh,        // Find Absolute Value of Four Fractional Halfwords
MIPS_absq_s_pw,        // Find Absolute Value of Two Fractional Words
MIPS_repl_ob,          // Replicate Immediate Integer into all Vector Element Positions
MIPS_repl_qh,          // Replicate Immediate Integer into all Vector Element Positions
MIPS_repl_pw,          // Replicate Immediate Integer into all Vector Element Positions
MIPS_replv_ob,         // Replicate Byte into all Vector Element Positions
MIPS_replv_qh,         // Replicate a Halfword into all Vector Element Positions
MIPS_replv_pw,         // Replicate Word into all Vector Element Positions
MIPS_precequ_pw_qhl,   // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceq_pw_qhl,    // Precision Expand Two Fractional Halfwords to Fractional Word Values
MIPS_preceq_s_l_pwl,   // Precision Expand Fractional Word to Fractional Doubleword Value
MIPS_preceu_qh_obl,    // Precision Expand Four Unsigned Bytes to Unsigned Halfword Values
MIPS_precequ_pw_qhr,   // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceq_pw_qhr,    // Precision Expand Two Fractional Halfwords to Fractional Word Values
MIPS_preceq_s_l_pwr,   // Precision Expand Fractional Word to Fractional Doubleword Value
MIPS_preceu_qh_obr,    // Precision Expand Four Unsigned Bytes to Unsigned Halfword Values
MIPS_precequ_pw_qhla,  // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceq_pw_qhla,   // Precision Expand Two Fractional Halfwords to Fractional Word Values
MIPS_preceu_qh_obla,   // Precision Expand Four Unsigned Bytes to Unsigned Halfword Values
MIPS_precequ_pw_qhra,  // Precision Expand two Unsigned Bytes to Fractional Halfword Values
MIPS_preceq_pw_qhra,   // Precision Expand Two Fractional Halfwords to Fractional Word Values
MIPS_preceu_qh_obra,   // Precision Expand Four Unsigned Bytes to Unsigned Halfword Values
MIPS_shll_qb,          // Shift Left Logical Vector Quad Bytes
MIPS_shll_ph,          // Shift Left Logical Vector Pair Halfwords
MIPS_shrl_qb,          // Shift Right Logical Vector Quad Bytes
MIPS_shra_ph,          // Shift Right Arithmetic Vector Pair Halfwords
MIPS_shrl_ph,          // Shift Right Logical Two Halfwords
MIPS_shllv_qb,         // Shift Left Logical Variable Vector Quad Bytes
MIPS_shllv_ph,         // Shift Left Logical Variable Vector Pair Halfwords
MIPS_shrlv_qb,         // Shift Right Logical Variable Vector Quad Bytes
MIPS_shrav_ph,         // Shift Right Arithmetic Variable Vector Pair Halfwords
MIPS_shrlv_ph,         // Shift Variable Right Logical Pair of Halfwords
MIPS_shra_qb,          // Shift Right Arithmetic Vector of Four Bytes
MIPS_shll_s_ph,        // Shift Left Logical Vector Pair Halfwords
MIPS_shll_s_w,         // Shift Left Logical Word with Saturation
MIPS_shra_r_qb,        // Shift Right Arithmetic Vector of Four Bytes
MIPS_shra_r_ph,        // Shift Right Arithmetic Vector Pair Halfwords
MIPS_shra_r_w,         // Shift Right Arithmetic Word with Rounding
MIPS_shrav_qb,         // Shift Right Arithmetic Variable Vector of Four Bytes
MIPS_shllv_s_ph,       // Shift Left Logical Variable Vector Pair Halfwords
MIPS_shllv_s_w,        // Shift Left Logical Variable Vector Word
MIPS_shrav_r_qb,       // Shift Right Arithmetic Variable Vector of Four Bytes
MIPS_shrav_r_ph,       // Shift Right Arithmetic Variable Vector Pair Halfwords
MIPS_shrav_r_w,        // Shift Right Arithmetic Variable Word with Rounding
MIPS_shll_ob,          // Shift Left Logical Vector Octal Bytes
MIPS_shll_qh,          // Shift Left Logical Vector Quad Halfwords
MIPS_shll_pw,          // Shift Left Logical Vector Pair Words
MIPS_shrl_ob,          // Shift Right Logical Vector Octal Bytes
MIPS_shra_qh,          // Shift Right Arithmetic Vector Quad Halfwords
MIPS_shra_pw,          // Shift Right Arithmetic Vector Pair Words
MIPS_shllv_ob,         // Shift Left Logical Variable Vector Octal Bytes
MIPS_shllv_qh,         // Shift Left Logical Variable Vector Quad Halfwords
MIPS_shllv_pw,         // Shift Left Logical Variable Vector Pair Words
MIPS_shrlv_ob,         // Shift Right Logical Variable Vector Octal Bytes
MIPS_shrav_qh,         // Shift Right Arithmetic Variable Vector Quad Halfwords
MIPS_shrav_pw,         // Shift Right Arithmetic Variable Vector Pair Words
MIPS_shll_s_qh,        // Shift Left Logical Vector Quad Halfwords
MIPS_shll_s_pw,        // Shift Left Logical Vector Pair Words
MIPS_shra_r_qh,        // Shift Right Arithmetic Vector Quad Halfwords
MIPS_shra_r_pw,        // Shift Right Arithmetic Vector Pair Words
MIPS_shllv_s_qh,       // Shift Left Logical Variable Vector Quad Halfwords
MIPS_shllv_s_pw,       // Shift Left Logical Variable Vector Pair Words
MIPS_shrav_r_qh,       // Shift Right Arithmetic Variable Vector Quad Halfwords
MIPS_shrav_r_pw,       // Shift Right Arithmetic Variable Vector Pair Words
MIPS_lwx,              // Load Word Indexed
MIPS_ldx,              // Load Doubleword Indexed
MIPS_lhx,              // Load Halfword Indexed
MIPS_lbux,             // Load Unsigned Byte Indexed
MIPS_dpa_w_ph,         // Dot Product with Accumulate on Vector Integer Halfword Elements
MIPS_dpax_w_ph,        // Cross Dot Product with Accumulate on Vector Integer Halfword Elements
MIPS_maq_sa_w_phl,     // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dpaqx_s_w_ph,     // Cross Dot Product with Accumulation on Fractional Halfword Elements
MIPS_dps_w_ph,         // Dot Product with Subtract on Vector Integer Half-Word Elements
MIPS_dpsx_w_ph,        // Cross Dot Product with Subtract on Vector Integer Halfword Elements
MIPS_dpsqx_s_w_ph,     // Cross Dot Product with Subtraction on Fractional Halfword Elements
MIPS_mulsa_w_ph,       // Multiply and Subtract Vector Integer Halfword Elements and Accumulate
MIPS_maq_sa_w_phr,     // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dpaqx_sa_w_ph,    // Cross Dot Product with Accumulation on Fractional Halfword Elements
MIPS_dpau_h_qbl,       // Dot Product with Accumulate on Vector Unsigned Byte Elements
MIPS_dpsu_h_qbl,       // Dot Product with Subtraction on Vector Unsigned Byte Elements
MIPS_dpsqx_sa_w_ph,    // Cross Dot Product with Subtraction on Fractional Halfword Elements
MIPS_dpaq_s_w_ph,      // Dot Product with Accumulation on Fractional Halfword Elements
MIPS_dpaq_sa_l_w,      // Dot Product with Accumulate on Fractional Word Element
MIPS_maq_s_w_phl,      // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dpsq_s_w_ph,      // Dot Product with Subtraction on Fractional Halfword Elements
MIPS_dpsq_sa_l_w,      // Dot Product with Subtraction on Fractional Word Element
MIPS_mulsaq_s_w_ph,    // Multiply And Subtract Vector Fractional Halfwords And Accumulate
MIPS_maq_s_w_phr,      // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dpau_h_qbr,       // Dot Product with Accumulate on Vector Unsigned Byte Elements
MIPS_dpsu_h_qbr,       // Dot Product with Subtraction on Vector Unsigned Byte Elements
MIPS_maq_sa_w_qhll,    // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_maq_sa_w_qhlr,    // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dmadd,            // Multiply Vector Words And Accumulate
MIPS_dmsub,            // Multiply Vector Words And Subtract From Accumulator
MIPS_maq_sa_w_qhrl,    // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dpau_h_obl,       // Dot Product with Accumulate on Vector Unsigned Byte Elements
MIPS_dpsu_h_obl,       // Dot Product with Subtract on Vector Unsigned Byte Elements
MIPS_maq_sa_w_qhrr,    // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dpaq_s_w_qh,      // Dot Product with Accumulation on Fractional Halfword Elements
MIPS_dpaq_sa_l_pw,     // Dot Product with Accumulate on Fractional Word Elements
MIPS_maq_s_w_qhll,     // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_maq_s_l_pwl,      // Multiply with Accumulate Single Vector Fractional Word Element
MIPS_dpsq_s_w_qh,      // Dot Product with Subtraction on Fractional Halfword Elements
MIPS_dpsq_sa_l_pw,     // Dot Product with Subtraction on Fractional Word Elements
MIPS_maq_s_w_qhlr,     // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dmaddu,           // Multiply Vector Unsigned Words And Accumulate
MIPS_mulsaq_s_w_qh,    // Multiply And Subtract Vector Fractional Halfwords And Accumulate
MIPS_mulsaq_s_l_pw,    // Multiply And Subtract Vector Fractional Words And Accumulate
MIPS_maq_s_w_qhrl,     // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_maq_s_l_pwr,      // Multiply with Accumulate Single Vector Fractional Word Element
MIPS_dpau_h_obr,       // Dot Product with Accumulate on Vector Unsigned Byte Elements
MIPS_dpsu_h_obr,       // Dot Product with Subtract on Vector Unsigned Byte Elements
MIPS_maq_s_w_qhrr,     // Multiply with Accumulate Single Vector Fractional Halfword Element
MIPS_dmsubu,           // Multiply Vector Unsigned Words And Subtract From Accumulator
MIPS_extr_w,           // Extract Word Value With Right Shift From Accumulator to GPR
MIPS_extrv_w,          // Extract Word Value With Variable Right Shift From Accumulator to GPR
MIPS_extp,             // Extract Fixed Bitfield From Arbitrary Position in Accumulator to GPR
MIPS_extpdp,           // Extract Fixed Bitfield From Arbitrary Position in Accumulator to GPR and Decrement Pos
MIPS_rddsp,            // Read DSPControl Register Fields to a GPR
MIPS_shilo,            // Shift an Accumulator Value Leaving the Result in the Same Accumulator
MIPS_extpv,            // Extract Variable Bitfield From Arbitrary Position in Accumulator to GPR
MIPS_extpdpv,          // Extract Variable Bitfield From Arbitrary Position in Accumulator to GPR and Decrement Pos
MIPS_wrdsp,            // Write Fields to DSPControl Register from a GPR
MIPS_shilov,           // Variable Shift of Accumulator Value Leaving the Result in the Same Accumulator
MIPS_extr_r_w,         // Extract Word Value With Right Shift And Rounding From Accumulator to GPR
MIPS_extrv_r_w,        // Extract Word Value With Variable Right Shift And Rounding From Accumulator to GPR
MIPS_extr_rs_w,        // Extract Word Value With Right Shift From Accumulator to GPR
MIPS_extr_s_h,         // Extract Halfword Value From Accumulator to GPR With Right Shift and Saturate
MIPS_extrv_rs_w,       // Extract Word Value With Variable Right Shift From Accumulator to GPR
MIPS_extrv_s_h,        // Extract Halfword Value Variable From Accumulator to GPR With Right Shift and Saturate
MIPS_mthlip,           // Copy LO to HI and a GPR to LO and Increment Pos by 32
MIPS_dextr_w,          // Extract Word Value With Right Shift From Accumulator to GPR
MIPS_dextr_l,          // Extract Doubleword Value With Right Shift From Accumulator to GPR
MIPS_dextrv_w,         // Extract Word Value With Variable Right Shift From Accumulator to GPR
MIPS_dextrv_l,         // Extract Doubleword Value With Variable Right Shift From Accumulator to GPR
MIPS_dextp,            // Extract Fixed Bitfield From Arbitrary Position in Accumulator to GPR
MIPS_dextpdp,          // Extract Fixed Bitfield From Arbitrary Position in Accumulator to GPR and Decrement Pos
MIPS_dshilo,           // Shift an Accumulator Value Leaving the Result in the Same Accumulator
MIPS_dextpv,           // Extract Variable Bitfield From Arbitrary Position in Accumulator to GPR
MIPS_dextpdpv,         // Extract Variable Bitfield From Arbitrary Position in Accumulator to GPR and Decrement Pos
MIPS_dshilov,          // Variable Shift of Accumulator Value Leaving the Result in the Same Accumulator
MIPS_dextr_r_w,        // Extract Word Value With Right Shift And Rounding From Accumulator to GPR
MIPS_dextr_r_l,        // Extract Doubleword Value With Right Shift And Rounding From Accumulator to GPR
MIPS_dextrv_r_w,       // Extract Word Value With Variable Right Shift And Rounding From Accumulator to GPR
MIPS_dextrv_r_l,       // Extract Doubleword Value With Variable Right Shift And Rounding From Accumulator to GPR
MIPS_dextr_rs_w,       // Extract Word Value With Right Shift From Accumulator to GPR
MIPS_dextr_s_h,        // Extract Halfword Value From Accumulator to GPR With Right Shift and Saturate
MIPS_dextr_rs_l,       // Extract Doubleword Value With Right Shift From Accumulator to GPR
MIPS_dextrv_rs_w,      // Extract Word Value With Variable Right Shift From Accumulator to GPR
MIPS_dextrv_s_h,       // Extract Halfword Value Variable From Accumulator to GPR With Right Shift and Saturate
MIPS_dextrv_rs_l,      // Extract Doubleword Value With Variable Right Shift From Accumulator to GPR
MIPS_dmthlip,          // Copy LO to HI and a GPR to LO and Increment Pos by 64
MIPS_adduh_qb,         // Unsigned Add Vector Quad-Bytes And Right Shift to Halve Results
MIPS_addqh_ph,         // Add Fractional Halfword Vectors And Shift Right to Halve Results
MIPS_addqh_w,          // Add Fractional Words And Shift Right to Halve Results
MIPS_subuh_qb,         // Subtract Unsigned Bytes And Right Shift to Halve Results
MIPS_subqh_ph,         // Subtract Fractional Halfword Vectors And Shift Right to Halve Results
MIPS_subqh_w,          // Subtract Fractional Words And Shift Right to Halve Results
MIPS_adduh_r_qb,       // Unsigned Add Vector Quad-Bytes And Right Shift to Halve Results
MIPS_addqh_r_ph,       // Add Fractional Halfword Vectors And Shift Right to Halve Results
MIPS_addqh_r_w,        // Add Fractional Words And Shift Right to Halve Results
MIPS_subuh_r_qb,       // Subtract Unsigned Bytes And Right Shift to Halve Results
MIPS_subqh_r_ph,       // Subtract Fractional Halfword Vectors And Shift Right to Halve Results
MIPS_subqh_r_w,        // Subtract Fractional Words And Shift Right to Halve Results
MIPS_mul_ph,           // Multiply Vector Integer HalfWords to Same Size Products
MIPS_mul_s_ph,         // Multiply Vector Integer HalfWords to Same Size Products
MIPS_mulq_s_w,         // Multiply Fractional Words to Same Size Product with Saturation
MIPS_mulq_rs_w,        // Multiply Fractional Words to Same Size Product with Saturation and Rounding
MIPS_append,           // Left Shift and Append Bits to the LSB
MIPS_balign,           // Byte Align Contents from Two Registers
MIPS_prepend,          // Right Shift and Prepend Bits to the MSB

// Cavium Octeon II instructions
MIPS_laa,      // Load Atomic Add Word
MIPS_laad,     // Load Atomic Add Doubleword
MIPS_lac,      // Load Atomic Clear Word
MIPS_lacd,     // Load Atomic Clear Doubleword
MIPS_lad,      // Load Atomic Decrement Word
MIPS_ladd,     // Load Atomic Decrement Doubleword
MIPS_lai,      // Load Atomic Increment Word
MIPS_laid,     // Load Atomic Increment Doubleword
MIPS_las,      // Load Atomic Set Word
MIPS_lasd,     // Load Atomic Set Doubleword
MIPS_law,      // Load Atomic Swap Word
MIPS_lawd,     // Load Atomic Swap Doubleword
// we don't know the following mnemonics for sure
MIPS_lbx,      // Load Byte Indexed
MIPS_lhux,     // Load Halfword Unsigned Indexed
MIPS_lwux,     // Load Word Unsigned Indexed
MIPS_qmac_00,  // Q15 Multiply Accumulate
MIPS_qmac_01,  // Q15 Multiply Accumulate
MIPS_qmac_02,  // Q15 Multiply Accumulate
MIPS_qmac_03,  // Q15 Multiply Accumulate
MIPS_qmacs_00, // Q15 Multiply Accumulate
MIPS_qmacs_01, // Q15 Multiply Accumulate
MIPS_qmacs_02, // Q15 Multiply Accumulate
MIPS_qmacs_03, // Q15 Multiply Accumulate
MIPS_zcb,      // Zero Cache Block
MIPS_zcbt,     // Zero Cache Block

// MSA ASE
// some of these have the same name as existing mnemonics; to avoid ambiguity
// we use an msa_ prefix on all of them
MIPS_msa_sll_b,      // Vector Shift Left
MIPS_msa_sll_h,      // Vector Shift Left
MIPS_msa_sll_w,      // Vector Shift Left
MIPS_msa_sll_d,      // Vector Shift Left
MIPS_msa_slli_b,     // Immediate Shift Left
MIPS_msa_slli_h,     // Immediate Shift Left
MIPS_msa_slli_w,     // Immediate Shift Left
MIPS_msa_slli_d,     // Immediate Shift Left
MIPS_msa_sra_b,      // Vector Shift Right Arithmetic
MIPS_msa_sra_h,      // Vector Shift Right Arithmetic
MIPS_msa_sra_w,      // Vector Shift Right Arithmetic
MIPS_msa_sra_d,      // Vector Shift Right Arithmetic
MIPS_msa_srai_b,     // Immediate Shift Right Arithmetic
MIPS_msa_srai_h,     // Immediate Shift Right Arithmetic
MIPS_msa_srai_w,     // Immediate Shift Right Arithmetic
MIPS_msa_srai_d,     // Immediate Shift Right Arithmetic
MIPS_msa_srl_b,      // Vector Shift Right Logical
MIPS_msa_srl_h,      // Vector Shift Right Logical
MIPS_msa_srl_w,      // Vector Shift Right Logical
MIPS_msa_srl_d,      // Vector Shift Right Logical
MIPS_msa_srli_b,     // Immediate Shift Right Logical
MIPS_msa_srli_h,     // Immediate Shift Right Logical
MIPS_msa_srli_w,     // Immediate Shift Right Logical
MIPS_msa_srli_d,     // Immediate Shift Right Logical
MIPS_msa_bclr_b,     // Vector Bit Clear
MIPS_msa_bclr_h,     // Vector Bit Clear
MIPS_msa_bclr_w,     // Vector Bit Clear
MIPS_msa_bclr_d,     // Vector Bit Clear
MIPS_msa_bclri_b,    // Immediate Bit Clear
MIPS_msa_bclri_h,    // Immediate Bit Clear
MIPS_msa_bclri_w,    // Immediate Bit Clear
MIPS_msa_bclri_d,    // Immediate Bit Clear
MIPS_msa_bset_b,     // Vector Bit Set
MIPS_msa_bset_h,     // Vector Bit Set
MIPS_msa_bset_w,     // Vector Bit Set
MIPS_msa_bset_d,     // Vector Bit Set
MIPS_msa_bseti_b,    // Immediate Bit Set
MIPS_msa_bseti_h,    // Immediate Bit Set
MIPS_msa_bseti_w,    // Immediate Bit Set
MIPS_msa_bseti_d,    // Immediate Bit Set
MIPS_msa_bneg_b,     // Vector Bit Negate
MIPS_msa_bneg_h,     // Vector Bit Negate
MIPS_msa_bneg_w,     // Vector Bit Negate
MIPS_msa_bneg_d,     // Vector Bit Negate
MIPS_msa_bnegi_b,    // Immediate Bit Negate
MIPS_msa_bnegi_h,    // Immediate Bit Negate
MIPS_msa_bnegi_w,    // Immediate Bit Negate
MIPS_msa_bnegi_d,    // Immediate Bit Negate
MIPS_msa_binsl_b,    // Vector Bit Insert Left
MIPS_msa_binsl_h,    // Vector Bit Insert Left
MIPS_msa_binsl_w,    // Vector Bit Insert Left
MIPS_msa_binsl_d,    // Vector Bit Insert Left
MIPS_msa_binsli_b,   // Immediate Bit Insert Left
MIPS_msa_binsli_h,   // Immediate Bit Insert Left
MIPS_msa_binsli_w,   // Immediate Bit Insert Left
MIPS_msa_binsli_d,   // Immediate Bit Insert Left
MIPS_msa_binsr_b,    // Vector Bit Insert Right
MIPS_msa_binsr_h,    // Vector Bit Insert Right
MIPS_msa_binsr_w,    // Vector Bit Insert Right
MIPS_msa_binsr_d,    // Vector Bit Insert Right
MIPS_msa_binsri_b,   // Immediate Bit Insert Right
MIPS_msa_binsri_h,   // Immediate Bit Insert Right
MIPS_msa_binsri_w,   // Immediate Bit Insert Right
MIPS_msa_binsri_d,   // Immediate Bit Insert Right
MIPS_msa_addv_b,     // Vector Add
MIPS_msa_addv_h,     // Vector Add
MIPS_msa_addv_w,     // Vector Add
MIPS_msa_addv_d,     // Vector Add
MIPS_msa_addvi_b,    // Immediate Add
MIPS_msa_addvi_h,    // Immediate Add
MIPS_msa_addvi_w,    // Immediate Add
MIPS_msa_addvi_d,    // Immediate Add
MIPS_msa_subv_b,     // Vector Subtract
MIPS_msa_subv_h,     // Vector Subtract
MIPS_msa_subv_w,     // Vector Subtract
MIPS_msa_subv_d,     // Vector Subtract
MIPS_msa_subvi_b,    // Immediate Subtract
MIPS_msa_subvi_h,    // Immediate Subtract
MIPS_msa_subvi_w,    // Immediate Subtract
MIPS_msa_subvi_d,    // Immediate Subtract
MIPS_msa_max_s_b,    // Vector Signed Maximum
MIPS_msa_max_s_h,    // Vector Signed Maximum
MIPS_msa_max_s_w,    // Vector Signed Maximum
MIPS_msa_max_s_d,    // Vector Signed Maximum
MIPS_msa_maxi_s_b,   // Immediate Signed Maximum
MIPS_msa_maxi_s_h,   // Immediate Signed Maximum
MIPS_msa_maxi_s_w,   // Immediate Signed Maximum
MIPS_msa_maxi_s_d,   // Immediate Signed Maximum
MIPS_msa_max_u_b,    // Vector Unsigned Maximum
MIPS_msa_max_u_h,    // Vector Unsigned Maximum
MIPS_msa_max_u_w,    // Vector Unsigned Maximum
MIPS_msa_max_u_d,    // Vector Unsigned Maximum
MIPS_msa_maxi_u_b,   // Immediate Unsigned Maximum
MIPS_msa_maxi_u_h,   // Immediate Unsigned Maximum
MIPS_msa_maxi_u_w,   // Immediate Unsigned Maximum
MIPS_msa_maxi_u_d,   // Immediate Unsigned Maximum
MIPS_msa_min_s_b,    // Vector Signed Minimum
MIPS_msa_min_s_h,    // Vector Signed Minimum
MIPS_msa_min_s_w,    // Vector Signed Minimum
MIPS_msa_min_s_d,    // Vector Signed Minimum
MIPS_msa_mini_s_b,   // Immediate Signed Minimum
MIPS_msa_mini_s_h,   // Immediate Signed Minimum
MIPS_msa_mini_s_w,   // Immediate Signed Minimum
MIPS_msa_mini_s_d,   // Immediate Signed Minimum
MIPS_msa_min_u_b,    // Vector Unsigned Minimum
MIPS_msa_min_u_h,    // Vector Unsigned Minimum
MIPS_msa_min_u_w,    // Vector Unsigned Minimum
MIPS_msa_min_u_d,    // Vector Unsigned Minimum
MIPS_msa_mini_u_b,   // Immediate Unsigned Minimum
MIPS_msa_mini_u_h,   // Immediate Unsigned Minimum
MIPS_msa_mini_u_w,   // Immediate Unsigned Minimum
MIPS_msa_mini_u_d,   // Immediate Unsigned Minimum
MIPS_msa_max_a_b,    // Vector Maximum Based on Absolute Values
MIPS_msa_max_a_h,    // Vector Maximum Based on Absolute Values
MIPS_msa_max_a_w,    // Vector Maximum Based on Absolute Values
MIPS_msa_max_a_d,    // Vector Maximum Based on Absolute Values
MIPS_msa_min_a_b,    // Vector Minimum Based on Absolute Value
MIPS_msa_min_a_h,    // Vector Minimum Based on Absolute Value
MIPS_msa_min_a_w,    // Vector Minimum Based on Absolute Value
MIPS_msa_min_a_d,    // Vector Minimum Based on Absolute Value
MIPS_msa_ceq_b,      // Vector Compare Equal
MIPS_msa_ceq_h,      // Vector Compare Equal
MIPS_msa_ceq_w,      // Vector Compare Equal
MIPS_msa_ceq_d,      // Vector Compare Equal
MIPS_msa_ceqi_b,     // Immediate Compare Equal
MIPS_msa_ceqi_h,     // Immediate Compare Equal
MIPS_msa_ceqi_w,     // Immediate Compare Equal
MIPS_msa_ceqi_d,     // Immediate Compare Equal
MIPS_msa_clt_s_b,    // Vector Compare Signed Less Than
MIPS_msa_clt_s_h,    // Vector Compare Signed Less Than
MIPS_msa_clt_s_w,    // Vector Compare Signed Less Than
MIPS_msa_clt_s_d,    // Vector Compare Signed Less Than
MIPS_msa_clti_s_b,   // Immediate Compare Signed Less Than
MIPS_msa_clti_s_h,   // Immediate Compare Signed Less Than
MIPS_msa_clti_s_w,   // Immediate Compare Signed Less Than
MIPS_msa_clti_s_d,   // Immediate Compare Signed Less Than
MIPS_msa_clt_u_b,    // Vector Compare Unsigned Less Than
MIPS_msa_clt_u_h,    // Vector Compare Unsigned Less Than
MIPS_msa_clt_u_w,    // Vector Compare Unsigned Less Than
MIPS_msa_clt_u_d,    // Vector Compare Unsigned Less Than
MIPS_msa_clti_u_b,   // Immediate Compare Unsigned Less Than
MIPS_msa_clti_u_h,   // Immediate Compare Unsigned Less Than
MIPS_msa_clti_u_w,   // Immediate Compare Unsigned Less Than
MIPS_msa_clti_u_d,   // Immediate Compare Unsigned Less Than
MIPS_msa_cle_s_b,    // Vector Compare Signed Less Than or Equal
MIPS_msa_cle_s_h,    // Vector Compare Signed Less Than or Equal
MIPS_msa_cle_s_w,    // Vector Compare Signed Less Than or Equal
MIPS_msa_cle_s_d,    // Vector Compare Signed Less Than or Equal
MIPS_msa_clei_s_b,   // Immediate Compare Signed Less Than or Equal
MIPS_msa_clei_s_h,   // Immediate Compare Signed Less Than or Equal
MIPS_msa_clei_s_w,   // Immediate Compare Signed Less Than or Equal
MIPS_msa_clei_s_d,   // Immediate Compare Signed Less Than or Equal
MIPS_msa_cle_u_b,    // Vector Compare Unsigned Less Than or Equal
MIPS_msa_cle_u_h,    // Vector Compare Unsigned Less Than or Equal
MIPS_msa_cle_u_w,    // Vector Compare Unsigned Less Than or Equal
MIPS_msa_cle_u_d,    // Vector Compare Unsigned Less Than or Equal
MIPS_msa_clei_u_b,   // Immediate Compare Unsigned Less Than or Equal
MIPS_msa_clei_u_h,   // Immediate Compare Unsigned Less Than or Equal
MIPS_msa_clei_u_w,   // Immediate Compare Unsigned Less Than or Equal
MIPS_msa_clei_u_d,   // Immediate Compare Unsigned Less Than or Equal
MIPS_msa_ld_b,       // Vector Load
MIPS_msa_ld_h,       // Vector Load
MIPS_msa_ld_w,       // Vector Load
MIPS_msa_ld_d,       // Vector Load
MIPS_msa_st_b,       // Vector Store
MIPS_msa_st_h,       // Vector Store
MIPS_msa_st_w,       // Vector Store
MIPS_msa_st_d,       // Vector Store
MIPS_msa_sat_s_b,    // Immediate Signed Saturate
MIPS_msa_sat_s_h,    // Immediate Signed Saturate
MIPS_msa_sat_s_w,    // Immediate Signed Saturate
MIPS_msa_sat_s_d,    // Immediate Signed Saturate
MIPS_msa_sat_u_b,    // Immediate Unsigned Saturate
MIPS_msa_sat_u_h,    // Immediate Unsigned Saturate
MIPS_msa_sat_u_w,    // Immediate Unsigned Saturate
MIPS_msa_sat_u_d,    // Immediate Unsigned Saturate
MIPS_msa_add_a_b,    // Vector Add Absolute Values
MIPS_msa_add_a_h,    // Vector Add Absolute Values
MIPS_msa_add_a_w,    // Vector Add Absolute Values
MIPS_msa_add_a_d,    // Vector Add Absolute Values
MIPS_msa_adds_a_b,   // Vector Saturated Add of Absolute Values
MIPS_msa_adds_a_h,   // Vector Saturated Add of Absolute Values
MIPS_msa_adds_a_w,   // Vector Saturated Add of Absolute Values
MIPS_msa_adds_a_d,   // Vector Saturated Add of Absolute Values
MIPS_msa_adds_s_b,   // Vector Signed Saturated Add of Signed Values
MIPS_msa_adds_s_h,   // Vector Signed Saturated Add of Signed Values
MIPS_msa_adds_s_w,   // Vector Signed Saturated Add of Signed Values
MIPS_msa_adds_s_d,   // Vector Signed Saturated Add of Signed Values
MIPS_msa_adds_u_b,   // Vector Unsigned Saturated Add of Unsigned Values
MIPS_msa_adds_u_h,   // Vector Unsigned Saturated Add of Unsigned Values
MIPS_msa_adds_u_w,   // Vector Unsigned Saturated Add of Unsigned Values
MIPS_msa_adds_u_d,   // Vector Unsigned Saturated Add of Unsigned Values
MIPS_msa_ave_s_b,    // Vector Signed Average
MIPS_msa_ave_s_h,    // Vector Signed Average
MIPS_msa_ave_s_w,    // Vector Signed Average
MIPS_msa_ave_s_d,    // Vector Signed Average
MIPS_msa_ave_u_b,    // Vector Unsigned Average
MIPS_msa_ave_u_h,    // Vector Unsigned Average
MIPS_msa_ave_u_w,    // Vector Unsigned Average
MIPS_msa_ave_u_d,    // Vector Unsigned Average
MIPS_msa_aver_s_b,   // Vector Signed Average Rounded
MIPS_msa_aver_s_h,   // Vector Signed Average Rounded
MIPS_msa_aver_s_w,   // Vector Signed Average Rounded
MIPS_msa_aver_s_d,   // Vector Signed Average Rounded
MIPS_msa_aver_u_b,   // Vector Unsigned Average Rounded
MIPS_msa_aver_u_h,   // Vector Unsigned Average Rounded
MIPS_msa_aver_u_w,   // Vector Unsigned Average Rounded
MIPS_msa_aver_u_d,   // Vector Unsigned Average Rounded
MIPS_msa_subs_s_b,   // Vector Signed Saturated Subtract of Signed Values
MIPS_msa_subs_s_h,   // Vector Signed Saturated Subtract of Signed Values
MIPS_msa_subs_s_w,   // Vector Signed Saturated Subtract of Signed Values
MIPS_msa_subs_s_d,   // Vector Signed Saturated Subtract of Signed Values
MIPS_msa_subs_u_b,   // Vector Unsigned Saturated Subtract of Unsigned Values
MIPS_msa_subs_u_h,   // Vector Unsigned Saturated Subtract of Unsigned Values
MIPS_msa_subs_u_w,   // Vector Unsigned Saturated Subtract of Unsigned Values
MIPS_msa_subs_u_d,   // Vector Unsigned Saturated Subtract of Unsigned Values
MIPS_msa_subsus_u_b, // Vector Unsigned Saturated Subtract of Signed from Unsigned
MIPS_msa_subsus_u_h, // Vector Unsigned Saturated Subtract of Signed from Unsigned
MIPS_msa_subsus_u_w, // Vector Unsigned Saturated Subtract of Signed from Unsigned
MIPS_msa_subsus_u_d, // Vector Unsigned Saturated Subtract of Signed from Unsigned
MIPS_msa_subsuu_s_b, // Vector Signed Saturated Subtract of Unsigned Values
MIPS_msa_subsuu_s_h, // Vector Signed Saturated Subtract of Unsigned Values
MIPS_msa_subsuu_s_w, // Vector Signed Saturated Subtract of Unsigned Values
MIPS_msa_subsuu_s_d, // Vector Signed Saturated Subtract of Unsigned Values
MIPS_msa_asub_s_b,   // Vector Absolute Values of Signed Subtract
MIPS_msa_asub_s_h,   // Vector Absolute Values of Signed Subtract
MIPS_msa_asub_s_w,   // Vector Absolute Values of Signed Subtract
MIPS_msa_asub_s_d,   // Vector Absolute Values of Signed Subtract
MIPS_msa_asub_u_b,   // Vector Absolute Values of Unsigned Subtract
MIPS_msa_asub_u_h,   // Vector Absolute Values of Unsigned Subtract
MIPS_msa_asub_u_w,   // Vector Absolute Values of Unsigned Subtract
MIPS_msa_asub_u_d,   // Vector Absolute Values of Unsigned Subtract
MIPS_msa_mulv_b,     // Vector Multiply
MIPS_msa_mulv_h,     // Vector Multiply
MIPS_msa_mulv_w,     // Vector Multiply
MIPS_msa_mulv_d,     // Vector Multiply
MIPS_msa_maddv_b,    // Vector Multiply and Add
MIPS_msa_maddv_h,    // Vector Multiply and Add
MIPS_msa_maddv_w,    // Vector Multiply and Add
MIPS_msa_maddv_d,    // Vector Multiply and Add
MIPS_msa_msubv_b,    // Vector Multiply and Subtract
MIPS_msa_msubv_h,    // Vector Multiply and Subtract
MIPS_msa_msubv_w,    // Vector Multiply and Subtract
MIPS_msa_msubv_d,    // Vector Multiply and Subtract
MIPS_msa_div_s_b,    // Vector Signed Divide
MIPS_msa_div_s_h,    // Vector Signed Divide
MIPS_msa_div_s_w,    // Vector Signed Divide
MIPS_msa_div_s_d,    // Vector Signed Divide
MIPS_msa_div_u_b,    // Vector Unsigned Divide
MIPS_msa_div_u_h,    // Vector Unsigned Divide
MIPS_msa_div_u_w,    // Vector Unsigned Divide
MIPS_msa_div_u_d,    // Vector Unsigned Divide
MIPS_msa_mod_s_b,    // Vector Signed Modulo
MIPS_msa_mod_s_h,    // Vector Signed Modulo
MIPS_msa_mod_s_w,    // Vector Signed Modulo
MIPS_msa_mod_s_d,    // Vector Signed Modulo
MIPS_msa_mod_u_b,    // Vector Unsigned Modulo
MIPS_msa_mod_u_h,    // Vector Unsigned Modulo
MIPS_msa_mod_u_w,    // Vector Unsigned Modulo
MIPS_msa_mod_u_d,    // Vector Unsigned Modulo
MIPS_msa_dotp_s_h,   // Vector Signed Dot Product
MIPS_msa_dotp_s_w,   // Vector Signed Dot Product
MIPS_msa_dotp_s_d,   // Vector Signed Dot Product
MIPS_msa_dotp_u_h,   // Vector Unsigned Dot Product
MIPS_msa_dotp_u_w,   // Vector Unsigned Dot Product
MIPS_msa_dotp_u_d,   // Vector Unsigned Dot Product
MIPS_msa_dpadd_s_h,  // Vector Signed Dot Product and Add
MIPS_msa_dpadd_s_w,  // Vector Signed Dot Product and Add
MIPS_msa_dpadd_s_d,  // Vector Signed Dot Product and Add
MIPS_msa_dpadd_u_h,  // Vector Unsigned Dot Product and Add
MIPS_msa_dpadd_u_w,  // Vector Unsigned Dot Product and Add
MIPS_msa_dpadd_u_d,  // Vector Unsigned Dot Product and Add
MIPS_msa_dpsub_s_h,  // Vector Signed Dot Product and Subtract
MIPS_msa_dpsub_s_w,  // Vector Signed Dot Product and Subtract
MIPS_msa_dpsub_s_d,  // Vector Signed Dot Product and Subtract
MIPS_msa_dpsub_u_h,  // Vector Unsigned Dot Product and Subtract
MIPS_msa_dpsub_u_w,  // Vector Unsigned Dot Product and Subtract
MIPS_msa_dpsub_u_d,  // Vector Unsigned Dot Product and Subtract
MIPS_msa_sld_b,      // GPR Columns Slide
MIPS_msa_sld_h,      // GPR Columns Slide
MIPS_msa_sld_w,      // GPR Columns Slide
MIPS_msa_sld_d,      // GPR Columns Slide
MIPS_msa_sldi_b,     // Immediate Columns Slide
MIPS_msa_sldi_h,     // Immediate Columns Slide
MIPS_msa_sldi_w,     // Immediate Columns Slide
MIPS_msa_sldi_d,     // Immediate Columns Slide
MIPS_msa_splat_b,    // GPR Element Splat
MIPS_msa_splat_h,    // GPR Element Splat
MIPS_msa_splat_w,    // GPR Element Splat
MIPS_msa_splat_d,    // GPR Element Splat
MIPS_msa_splati_b,   // Immediate Element Splat
MIPS_msa_splati_h,   // Immediate Element Splat
MIPS_msa_splati_w,   // Immediate Element Splat
MIPS_msa_splati_d,   // Immediate Element Splat
MIPS_msa_pckev_b,    // Vector Pack Even
MIPS_msa_pckev_h,    // Vector Pack Even
MIPS_msa_pckev_w,    // Vector Pack Even
MIPS_msa_pckev_d,    // Vector Pack Even
MIPS_msa_pckod_b,    // Vector Pack Odd
MIPS_msa_pckod_h,    // Vector Pack Odd
MIPS_msa_pckod_w,    // Vector Pack Odd
MIPS_msa_pckod_d,    // Vector Pack Odd
MIPS_msa_ilvl_b,     // Vector Interleave Left
MIPS_msa_ilvl_h,     // Vector Interleave Left
MIPS_msa_ilvl_w,     // Vector Interleave Left
MIPS_msa_ilvl_d,     // Vector Interleave Left
MIPS_msa_ilvr_b,     // Vector Interleave Right
MIPS_msa_ilvr_h,     // Vector Interleave Right
MIPS_msa_ilvr_w,     // Vector Interleave Right
MIPS_msa_ilvr_d,     // Vector Interleave Right
MIPS_msa_ilvev_b,    // Vector Interleave Even
MIPS_msa_ilvev_h,    // Vector Interleave Even
MIPS_msa_ilvev_w,    // Vector Interleave Even
MIPS_msa_ilvev_d,    // Vector Interleave Even
MIPS_msa_ilvod_b,    // Vector Interleave Odd
MIPS_msa_ilvod_h,    // Vector Interleave Odd
MIPS_msa_ilvod_w,    // Vector Interleave Odd
MIPS_msa_ilvod_d,    // Vector Interleave Odd
MIPS_msa_vshf_b,     // Vector Data Preserving Shuffle
MIPS_msa_vshf_h,     // Vector Data Preserving Shuffle
MIPS_msa_vshf_w,     // Vector Data Preserving Shuffle
MIPS_msa_vshf_d,     // Vector Data Preserving Shuffle
MIPS_msa_srar_b,     // Vector Shift Right Arithmetic Rounded
MIPS_msa_srar_h,     // Vector Shift Right Arithmetic Rounded
MIPS_msa_srar_w,     // Vector Shift Right Arithmetic Rounded
MIPS_msa_srar_d,     // Vector Shift Right Arithmetic Rounded
MIPS_msa_srari_b,    // Immediate Shift Right Arithmetic Rounded
MIPS_msa_srari_h,    // Immediate Shift Right Arithmetic Rounded
MIPS_msa_srari_w,    // Immediate Shift Right Arithmetic Rounded
MIPS_msa_srari_d,    // Immediate Shift Right Arithmetic Rounded
MIPS_msa_srlr_b,     // Vector Shift Right Logical Rounded
MIPS_msa_srlr_h,     // Vector Shift Right Logical Rounded
MIPS_msa_srlr_w,     // Vector Shift Right Logical Rounded
MIPS_msa_srlr_d,     // Vector Shift Right Logical Rounded
MIPS_msa_srlri_b,    // Immediate Shift Right Logical Rounded
MIPS_msa_srlri_h,    // Immediate Shift Right Logical Rounded
MIPS_msa_srlri_w,    // Immediate Shift Right Logical Rounded
MIPS_msa_srlri_d,    // Immediate Shift Right Logical Rounded
MIPS_msa_hadd_s_h,   // Vector Signed Horizontal Add
MIPS_msa_hadd_s_w,   // Vector Signed Horizontal Add
MIPS_msa_hadd_s_d,   // Vector Signed Horizontal Add
MIPS_msa_hadd_u_h,   // Vector Unsigned Horizontal Add
MIPS_msa_hadd_u_w,   // Vector Unsigned Horizontal Add
MIPS_msa_hadd_u_d,   // Vector Unsigned Horizontal Add
MIPS_msa_hsub_s_h,   // Vector Signed Horizontal Subtract
MIPS_msa_hsub_s_w,   // Vector Signed Horizontal Subtract
MIPS_msa_hsub_s_d,   // Vector Signed Horizontal Subtract
MIPS_msa_hsub_u_h,   // Vector Unsigned Horizontal Subtract
MIPS_msa_hsub_u_w,   // Vector Unsigned Horizontal Subtract
MIPS_msa_hsub_u_d,   // Vector Unsigned Horizontal Subtract
MIPS_msa_and_v,      // Vector Logical And
MIPS_msa_andi_b,     // Immediate Logical And
MIPS_msa_or_v,       // Vector Logical Or
MIPS_msa_ori_b,      // Immediate Logical Or
MIPS_msa_nor_v,      // Vector Logical Negated Or
MIPS_msa_nori_b,     // Immediate Logical Negated Or
MIPS_msa_xor_v,      // Vector Logical Exclusive Or
MIPS_msa_xori_b,     // Immediate Logical Exclusive Or
MIPS_msa_bmnz_v,     // Vector Bit Move If Not Zero
MIPS_msa_bmnzi_b,    // Immediate Bit Move If Not Zero
MIPS_msa_bmz_v,      // Vector Bit Move If Zero
MIPS_msa_bmzi_b,     // Immediate Bit Move If Zero
MIPS_msa_bsel_v,     // Vector Bit Select
MIPS_msa_bseli_b,    // Immediate Bit Select
MIPS_msa_shf_b,      // Immediate Set Shuffle Elements
MIPS_msa_shf_h,      // Immediate Set Shuffle Elements
MIPS_msa_shf_w,      // Immediate Set Shuffle Elements
MIPS_msa_bnz_v,      // Immediate Branch If Not Zero (At Least One Element of Any Format Is Not Zero)
MIPS_msa_bz_v,       // Immediate Branch If Zero (All Elements of Any Format Are Zero)
MIPS_msa_fill_b,     // Vector Fill from GPR
MIPS_msa_fill_h,     // Vector Fill from GPR
MIPS_msa_fill_w,     // Vector Fill from GPR
MIPS_msa_fill_d,     // Vector Fill from GPR
MIPS_msa_pcnt_b,     // Vector Population Count
MIPS_msa_pcnt_h,     // Vector Population Count
MIPS_msa_pcnt_w,     // Vector Population Count
MIPS_msa_pcnt_d,     // Vector Population Count
MIPS_msa_nloc_b,     // Vector Leading Ones Count
MIPS_msa_nloc_h,     // Vector Leading Ones Count
MIPS_msa_nloc_w,     // Vector Leading Ones Count
MIPS_msa_nloc_d,     // Vector Leading Ones Count
MIPS_msa_nlzc_b,     // Vector Leading Zeros Count
MIPS_msa_nlzc_h,     // Vector Leading Zeros Count
MIPS_msa_nlzc_w,     // Vector Leading Zeros Count
MIPS_msa_nlzc_d,     // Vector Leading Zeros Count
MIPS_msa_copy_s_b,   // Element Copy to GPR Signed
MIPS_msa_copy_s_h,   // Element Copy to GPR Signed
MIPS_msa_copy_s_w,   // Element Copy to GPR Signed
MIPS_msa_copy_s_d,   // Element Copy to GPR Signed
MIPS_msa_copy_u_b,   // Element Copy to GPR Unsigned
MIPS_msa_copy_u_h,   // Element Copy to GPR Unsigned
MIPS_msa_copy_u_w,   // Element Copy to GPR Unsigned
MIPS_msa_copy_u_d,   // Element Copy to GPR Unsigned
MIPS_msa_insert_b,   // GPR Insert Element
MIPS_msa_insert_h,   // GPR Insert Element
MIPS_msa_insert_w,   // GPR Insert Element
MIPS_msa_insert_d,   // GPR Insert Element
MIPS_msa_insve_b,    // Element Insert Element
MIPS_msa_insve_h,    // Element Insert Element
MIPS_msa_insve_w,    // Element Insert Element
MIPS_msa_insve_d,    // Element Insert Element
MIPS_msa_bnz_b,      // Immediate Branch If All Elements Are Not Zero
MIPS_msa_bnz_h,      // Immediate Branch If All Elements Are Not Zero
MIPS_msa_bnz_w,      // Immediate Branch If All Elements Are Not Zero
MIPS_msa_bnz_d,      // Immediate Branch If All Elements Are Not Zero
MIPS_msa_bz_b,       // Immediate Branch If At Least One Element Is Zero
MIPS_msa_bz_h,       // Immediate Branch If At Least One Element Is Zero
MIPS_msa_bz_w,       // Immediate Branch If At Least One Element Is Zero
MIPS_msa_bz_d,       // Immediate Branch If At Least One Element Is Zero
MIPS_msa_ldi_b,      // Immediate Load
MIPS_msa_ldi_h,      // Immediate Load
MIPS_msa_ldi_w,      // Immediate Load
MIPS_msa_ldi_d,      // Immediate Load
MIPS_msa_fcaf_w,     // Vector Floating-Point Quiet Compare Always False
MIPS_msa_fcaf_d,     // Vector Floating-Point Quiet Compare Always False
MIPS_msa_fcun_w,     // Vector Floating-Point Quiet Compare Unordered
MIPS_msa_fcun_d,     // Vector Floating-Point Quiet Compare Unordered
MIPS_msa_fceq_w,     // Vector Floating-Point Quiet Compare Equal
MIPS_msa_fceq_d,     // Vector Floating-Point Quiet Compare Equal
MIPS_msa_fcueq_w,    // Vector Floating-Point Quiet Compare Unordered or Equal
MIPS_msa_fcueq_d,    // Vector Floating-Point Quiet Compare Unordered or Equal
MIPS_msa_fclt_w,     // Vector Floating-Point Quiet Compare Less Than
MIPS_msa_fclt_d,     // Vector Floating-Point Quiet Compare Less Than
MIPS_msa_fcult_w,    // Vector Floating-Point Quiet Compare Unordered or Less Than
MIPS_msa_fcult_d,    // Vector Floating-Point Quiet Compare Unordered or Less Than
MIPS_msa_fcle_w,     // Vector Floating-Point Quiet Compare Less or Equal
MIPS_msa_fcle_d,     // Vector Floating-Point Quiet Compare Less or Equal
MIPS_msa_fcule_w,    // Vector Floating-Point Quiet Compare Unordered or Less or Equal
MIPS_msa_fcule_d,    // Vector Floating-Point Quiet Compare Unordered or Less or Equal
MIPS_msa_fsaf_w,     // Vector Floating-Point Signaling Compare Always False
MIPS_msa_fsaf_d,     // Vector Floating-Point Signaling Compare Always False
MIPS_msa_fsun_w,     // Vector Floating-Point Signaling Compare Unordered
MIPS_msa_fsun_d,     // Vector Floating-Point Signaling Compare Unordered
MIPS_msa_fseq_w,     // Vector Floating-Point Signaling Compare Equal
MIPS_msa_fseq_d,     // Vector Floating-Point Signaling Compare Equal
MIPS_msa_fsueq_w,    // Vector Floating-Point Signaling Compare Unordered or Equal
MIPS_msa_fsueq_d,    // Vector Floating-Point Signaling Compare Unordered or Equal
MIPS_msa_fslt_w,     // Vector Floating-Point Signaling Compare Less Than
MIPS_msa_fslt_d,     // Vector Floating-Point Signaling Compare Less Than
MIPS_msa_fsult_w,    // Vector Floating-Point Signaling Compare Unordered or Less Than
MIPS_msa_fsult_d,    // Vector Floating-Point Signaling Compare Unordered or Less Than
MIPS_msa_fsle_w,     // Vector Floating-Point Signaling Compare Less or Equal
MIPS_msa_fsle_d,     // Vector Floating-Point Signaling Compare Less or Equal
MIPS_msa_fsule_w,    // Vector Floating-Point Signaling Compare Unordered or Less or Equal
MIPS_msa_fsule_d,    // Vector Floating-Point Signaling Compare Unordered or Less or Equal
MIPS_msa_fadd_w,     // Vector Floating-Point Addition
MIPS_msa_fadd_d,     // Vector Floating-Point Addition
MIPS_msa_fsub_w,     // Vector Floating-Point Subtraction
MIPS_msa_fsub_d,     // Vector Floating-Point Subtraction
MIPS_msa_fmul_w,     // Vector Floating-Point Multiplication
MIPS_msa_fmul_d,     // Vector Floating-Point Multiplication
MIPS_msa_fdiv_w,     // Vector Floating-Point Division
MIPS_msa_fdiv_d,     // Vector Floating-Point Division
MIPS_msa_fmadd_w,    // Vector Floating-Point Multiply-Add
MIPS_msa_fmadd_d,    // Vector Floating-Point Multiply-Add
MIPS_msa_fmsub_w,    // Vector Floating-Point Multiply-Sub
MIPS_msa_fmsub_d,    // Vector Floating-Point Multiply-Sub
MIPS_msa_fexp2_w,    // Vector Floating-Point Base 2 Exponentiation
MIPS_msa_fexp2_d,    // Vector Floating-Point Base 2 Exponentiation
MIPS_msa_fexdo_h,    // Vector Floating-Point Down-Convert Interchange Format
MIPS_msa_fexdo_w,    // Vector Floating-Point Down-Convert Interchange Format
MIPS_msa_ftq_h,      // Vector Floating-Point Convert to Fixed-Point
MIPS_msa_ftq_w,      // Vector Floating-Point Convert to Fixed-Point
MIPS_msa_fmin_w,     // Vector Floating-Point Minimum
MIPS_msa_fmin_d,     // Vector Floating-Point Minimum
MIPS_msa_fmin_a_w,   // Vector Floating-Point Minimum Based on Absolute Values
MIPS_msa_fmin_a_d,   // Vector Floating-Point Minimum Based on Absolute Values
MIPS_msa_fmax_w,     // Vector Floating-Point Maximum
MIPS_msa_fmax_d,     // Vector Floating-Point Maximum
MIPS_msa_fmax_a_w,   // Vector Floating-Point Maximum Based on Absolute Values
MIPS_msa_fmax_a_d,   // Vector Floating-Point Maximum Based on Absolute Values
MIPS_msa_fcor_w,     // Vector Floating-Point Quiet Compare Ordered
MIPS_msa_fcor_d,     // Vector Floating-Point Quiet Compare Ordered
MIPS_msa_fcune_w,    // Vector Floating-Point Quiet Compare Unordered or Not Equal
MIPS_msa_fcune_d,    // Vector Floating-Point Quiet Compare Unordered or Not Equal
MIPS_msa_fcne_w,     // Vector Floating-Point Quiet Compare Not Equal
MIPS_msa_fcne_d,     // Vector Floating-Point Quiet Compare Not Equal
MIPS_msa_mul_q_h,    // Vector Fixed-Point Multiply
MIPS_msa_mul_q_w,    // Vector Fixed-Point Multiply
MIPS_msa_madd_q_h,   // Vector Fixed-Point Multiply and Add
MIPS_msa_madd_q_w,   // Vector Fixed-Point Multiply and Add
MIPS_msa_msub_q_h,   // Vector Fixed-Point Multiply and Subtract
MIPS_msa_msub_q_w,   // Vector Fixed-Point Multiply and Subtract
MIPS_msa_fsor_w,     // Vector Floating-Point Signaling Compare Ordered
MIPS_msa_fsor_d,     // Vector Floating-Point Signaling Compare Ordered
MIPS_msa_fsune_w,    // Vector Floating-Point Signaling Compare Unordered or Not Equal
MIPS_msa_fsune_d,    // Vector Floating-Point Signaling Compare Unordered or Not Equal
MIPS_msa_fsne_w,     // Vector Floating-Point Signaling Compare Not Equal
MIPS_msa_fsne_d,     // Vector Floating-Point Signaling Compare Not Equal
MIPS_msa_mulr_q_h,   // Vector Fixed-Point Multiply Rounded
MIPS_msa_mulr_q_w,   // Vector Fixed-Point Multiply Rounded
MIPS_msa_maddr_q_h,  // Vector Fixed-Point Multiply and Add Rounded
MIPS_msa_maddr_q_w,  // Vector Fixed-Point Multiply and Add Rounded
MIPS_msa_msubr_q_h,  // Vector Fixed-Point Multiply and Subtract Rounded
MIPS_msa_msubr_q_w,  // Vector Fixed-Point Multiply and Subtract Rounded
MIPS_msa_fclass_w,   // Vector Floating-Point Class Mask
MIPS_msa_fclass_d,   // Vector Floating-Point Class Mask
MIPS_msa_ftrunc_s_w, // Vector Floating-Point Truncate and Convert to Signed Integer
MIPS_msa_ftrunc_s_d, // Vector Floating-Point Truncate and Convert to Signed Integer
MIPS_msa_ftrunc_u_w, // Vector Floating-Point Truncate and Convert to Unsigned Integer
MIPS_msa_ftrunc_u_d, // Vector Floating-Point Truncate and Convert to Unsigned Integer
MIPS_msa_fsqrt_w,    // Vector Floating-Point Square Root
MIPS_msa_fsqrt_d,    // Vector Floating-Point Square Root
MIPS_msa_frsqrt_w,   // Vector Approximate Floating-Point Reciprocal of Square Root
MIPS_msa_frsqrt_d,   // Vector Approximate Floating-Point Reciprocal of Square Root
MIPS_msa_frcp_w,     // Vector Approximate Floating-Point Reciprocal
MIPS_msa_frcp_d,     // Vector Approximate Floating-Point Reciprocal
MIPS_msa_frint_w,    // Vector Floating-Point Round to Integer
MIPS_msa_frint_d,    // Vector Floating-Point Round to Integer
MIPS_msa_flog2_w,    // Vector Floating-Point Base 2 Logarithm
MIPS_msa_flog2_d,    // Vector Floating-Point Base 2 Logarithm
MIPS_msa_fexupl_w,   // Vector Floating-Point Up-Convert Interchange Format Left
MIPS_msa_fexupl_d,   // Vector Floating-Point Up-Convert Interchange Format Left
MIPS_msa_fexupr_w,   // Vector Floating-Point Up-Convert Interchange Format Right
MIPS_msa_fexupr_d,   // Vector Floating-Point Up-Convert Interchange Format Right
MIPS_msa_ffql_w,     // Vector Floating-Point Convert from Fixed-Point Left
MIPS_msa_ffql_d,     // Vector Floating-Point Convert from Fixed-Point Left
MIPS_msa_ffqr_w,     // Vector Floating-Point Convert from Fixed-Point Right
MIPS_msa_ffqr_d,     // Vector Floating-Point Convert from Fixed-Point Right
MIPS_msa_ftint_s_w,  // Vector Floating-Point Convert to Signed Integer
MIPS_msa_ftint_s_d,  // Vector Floating-Point Convert to Signed Integer
MIPS_msa_ftint_u_w,  // Vector Floating-Point Round and Convert to Unsigned Integer
MIPS_msa_ftint_u_d,  // Vector Floating-Point Round and Convert to Unsigned Integer
MIPS_msa_ffint_s_w,  // Vector Floating-Point Round and Convert from Signed Integer
MIPS_msa_ffint_s_d,  // Vector Floating-Point Round and Convert from Signed Integer
MIPS_msa_ffint_u_w,  // Vector Floating-Point Convert from Unsigned Integer
MIPS_msa_ffint_u_d,  // Vector Floating-Point Convert from Unsigned Integer
MIPS_msa_ctcmsa,     // GPR Copy to MSA Control Register
MIPS_msa_cfcmsa,     // GPR Copy from MSA Control Register
MIPS_msa_move_v,     // Vector Move

// MIPS R6 (LSA is also part of MSA)
MIPS_lsa,            // Left Shift Add
MIPS_dlsa,           // Doubleword Left Shift Add

MIPS_last,

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

H8_null = 0,      // Unknown Operation

H8_add,            // Add binary
H8_adds,           // Add with sign extension
H8_addx,           // Add with extend carry
H8_and,            // Logical AND
H8_andc,           // Logical AND with control register
H8_band,           // Bit AND
H8_bra,            // Branch always
H8_brn,            // Branch never
H8_bhi,            // Branch if higher
H8_bls,            // Branch if lower or same
H8_bcc,            // Branch if carry clear (higher or same)
H8_bcs,            // Branch if carry set (lower)
H8_bne,            // Branch if not equal
H8_beq,            // Branch if equal
H8_bvc,            // Branch if overflow clear
H8_bvs,            // Branch if overflow set
H8_bpl,            // Branch if plus
H8_bmi,            // Branch if minus
H8_bge,            // Branch if greates or equal
H8_blt,            // Branch if less
H8_bgt,            // Branch if greater
H8_ble,            // Branch if less or equal
H8_bclr,           // Bit clear
H8_biand,          // Bit invert AND
H8_bild,           // Bit invert load
H8_bior,           // Bit invert OR
H8_bist,           // Bit invert store
H8_bixor,          // Bit invert XOR
H8_bld,            // Bit load
H8_bnot,           // Bit NOT
H8_bor,            // Bit OR
H8_bset,           // Bit set
H8_bsr,            // Branch to subroutine
H8_bst,            // Bit store
H8_btst,           // Bit test
H8_bxor,           // Bit XOR
H8_clrmac,         // Clear MAC register
H8_cmp,            // Compare
H8_daa,            // Decimal adjust add
H8_das,            // Decimal adjust subtract
H8_dec,            // Decrement
H8_divxs,          // Divide extended as signed
H8_divxu,          // Divide extended as unsigned
H8_eepmov,         // Move data to EEPROM
H8_exts,           // Extend as signed
H8_extu,           // Extend as unsigned
H8_inc,            // Increment
H8_jmp,            // Jump
H8_jsr,            // Jump to subroutine
H8_ldc,            // Load to control register
H8_ldm,            // Load to multiple registers
H8_ldmac,          // Load to MAC register
H8_mac,            // Multiply and accumulate
H8_mov,            // Move data
H8_movfpe,         // Move from peripheral with E clock
H8_movtpe,         // Move to peripheral with E clock
H8_mulxs,          // Multiply extend as signed
H8_mulxu,          // Multiply extend as unsigned
H8_neg,            // Negate
H8_nop,            // No operation
H8_not,            // Logical complement
H8_or,             // Logical OR
H8_orc,            // Logical OR with control register
H8_pop,            // Pop data from stack
H8_push,           // Push data on stack
H8_rotl,           // Rotate left
H8_rotr,           // Rotate right
H8_rotxl,          // Rotate with extend carry left
H8_rotxr,          // Rotate with extend carry right
H8_rte,            // Return from exception
H8_rts,            // Return from subroutine
H8_shal,           // Shift arithmetic left
H8_shar,           // Shift arithmetic right
H8_shll,           // Shift logical left
H8_shlr,           // Shift logical right
H8_sleep,          // Power down mode
H8_stc,            // Store from control register
H8_stm,            // Store from multiple registers
H8_stmac,          // Store from MAC register
H8_sub,            // Subtract binary
H8_subs,           // Subtract with sign extension
H8_subx,           // Subtract with extend carry
H8_tas,            // Test and set
H8_trapa,          // Trap always
H8_xor,            // Logical XOR
H8_xorc,           // Logical XOR with control register

// H8SX
H8_rtel,           // Returns from an exception, restoring data to multiple general registers
H8_rtsl,           // Returns from a subroutine, restoring data to multiple general registers 
H8_movmd,          // Transfers a data block
H8_movsd,          // Transfers a data block with zero detection
H8_bras,           // Branch always after the next instruction (delay slot)
H8_movab,          // MOVe effective Address/B
H8_movaw,          // MOVe effective Address/W
H8_moval,          // MOVe effective Address/L
H8_bsetne,         // Bit SET if Not Equal
H8_bseteq,         // Bit SET if EQual
H8_bclrne,         // Bit CLeaR if Not Equal
H8_bclreq,         // Bit CLeaR if Equal
H8_bstz,           // Bit STore Zero flag
H8_bistz,          // Bit Invert STore Zero flag
H8_bfld,           // Bit Field LoaD
H8_bfst,           // Bit Field STore
H8_muls,           // MULtiply as Signed 
H8_divs,           // DIVide as Signed
H8_mulu,           // MULtiply as Unsigned
H8_divu,           // DIVide as Unsigned
H8_mulsu,          // MULtiply as Signed
H8_muluu,          // MULtiply as Unsigned
H8_brabc,          // BRAnch if Bit Cleared
H8_brabs,          // BRAnch if Bit Set
H8_bsrbc,          // Branch to SubRoutine if Bit Cleared 
H8_bsrbs,          // Branch to SubRoutine if Bit Set

H8_last,

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

PIC_null = 0,      // Unknown Operation

// BYTE-ORIENTED FILE REGISTER OPERATIONS

PIC_addwf,         // Add W and f
PIC_andwf,         // AND W with f
PIC_clrf,          // Clear f
PIC_clrw,          // Clear W
PIC_comf,          // Complement f
PIC_decf,          // Decrement f
PIC_decfsz,        // Decrement f, Skip if 0
PIC_incf,          // Increment f
PIC_incfsz,        // Increment f, Skip if 0
PIC_iorwf,         // Inclusive OR W with f
PIC_movf,          // Move f
PIC_movwf,         // Move W to f
PIC_nop,           // No Operation
PIC_rlf,           // Rotate Left f through Carry
PIC_rrf,           // Rotate Right f through Carry
PIC_subwf,         // Subtract W from f
PIC_swapf,         // Swap nibbles in f
PIC_xorwf,         // Exclusive OR W with f

// BIT-ORIENTED FILE REGISTER OPERATIONS

PIC_bcf,           // Bit Clear f
PIC_bsf,           // Bit Set f
PIC_btfsc,         // Bit Test f, Skip if Clear
PIC_btfss,         // Bit Test f, Skip if Set


// LITERAL AND CONTROL OPERATIONS

PIC_addlw,         // Add literal and W
PIC_andlw,         // AND literal with W
PIC_call,          // Call subroutine
PIC_clrwdt,        // Clear Watchdog Timer
PIC_goto,          // Go to address
PIC_iorlw,         // Inclusive OR literal with W
PIC_movlw,         // Move literal to W
PIC_retfie,        // Return from interrupt
PIC_retlw,         // Return with literal in W
PIC_return,        // Return from Subroutine
PIC_sleep,         // Go into standby mode
PIC_sublw,         // Subtract W from literal
PIC_xorlw,         // Exclusive OR literal with W

// ADDITIONAL INSTRUCTIONS TO MAINTAIN COMPITIBILITY WITH 12C5xx,16C5x

PIC_option,        // Load OPTION register
PIC_tris,          // Load TRIS Register

// MACROS

PIC_movfw,         // Move Contents of File Reg to W
PIC_tstf,          // Test Contents of File Register
PIC_negf,          // Negate File Register Contents
PIC_b,             // Branch to Address
PIC_clrc,          // Clear Carry
PIC_clrdc,         // Clear Digit Carry
PIC_clrz,          // Clear Zero
PIC_setc,          // Set Carry
PIC_setdc,         // Set Digit Carry
PIC_setz,          // Set Zero
PIC_skpc,          // Skip on Carry
PIC_skpdc,         // Skip on Digit Carry
PIC_skpnc,         // Skip on No Carry
PIC_skpndc,        // Skip on No Digit Carry
PIC_skpnz,         // Skip on No Zero
PIC_skpz,          // Skip on Zero
PIC_bc,            // Branch on Carry to Address k
PIC_bdc,           // Branch on Digit Carry to k
PIC_bnc,           // Branch on No Carry to k
PIC_bndc,          // Branch on No Digit Carry to k
PIC_bnz,           // Branch on No Zero to Address
PIC_bz,            // Branch on Zero to Address k
PIC_addcf,         // Add Carry to File Register
PIC_adddcf,        // Add Digit to File Register
PIC_subcf,         // Subtract Carry from File Reg

// ADDITIONAL INSTRUCTIONS FOR 18Cxx

// BYTE-ORIENTED FILE REGISTER OPERATIONS

PIC_addwf3,        // Add W and f
PIC_addwfc3,       // Add W and Carry to f
PIC_andwf3,        // AND W with f
PIC_clrf2,         // Clear f
PIC_comf3,         // Complement f
PIC_cpfseq2,       // Compare f with W, Skip if ==
PIC_cpfsgt2,       // Compare f with W, Skip if >
PIC_cpfslt2,       // Compare f with W, Skip if <
PIC_decf3,         // Decrement f
PIC_decfsz3,       // Decrement f, Skip if 0
PIC_dcfsnz3,       // Decrement f, Skip if not 0
PIC_incf3,         // Increment f
PIC_incfsz3,       // Increment f, Skip if 0
PIC_infsnz3,       // Increment f, Skip if not 0
PIC_iorwf3,        // Inclusive OR W with f
PIC_movf3,         // Move f
PIC_movff2,        // Move fs to fd
PIC_movwf2,        // Move W to f
PIC_mulwf2,        // Multiply W with f
PIC_negf2,         // Negate f
PIC_rlcf3,         // Rotate Left f through Carry
PIC_rlncf3,        // Rotate Left f
PIC_rrcf3,         // Rotate Right f through Carry
PIC_rrncf3,        // Rotate Right f
PIC_setf2,         // Set f
PIC_subfwb3,       // Substract f from W with borrow
PIC_subwf3,        // Substract W from f
PIC_subwfb3,       // Substract W from f with borrow
PIC_swapf3,        // Swap nibbles in f
PIC_tstfsz2,       // Test f, Skip if 0
PIC_xorwf3,        // Exclusive OR W with f

// BIT-ORIENTED FILE REGISTER OPERATIONS

PIC_bcf3,          // Bit Clear f
PIC_bsf3,          // Bit Set f
PIC_btfsc3,        // Bit Test f, Skip if Clear
PIC_btfss3,        // Bit Test f, Skip if Set
PIC_btg3,          // Bit Toggle f

// CONTROL OPERATIONS

PIC_bc1,           // Branch if Carry
PIC_bn1,           // Branch if Negative
PIC_bnc1,          // Branch if not Carry
PIC_bnn1,          // Branch if not Negative
PIC_bnov1,         // Branch if not Overflow
PIC_bnz1,          // Branch if not Zero
PIC_bov1,          // Branch if Overflow
PIC_bra1,          // Branch unconditionally
PIC_bz1,           // Branch if Zero
PIC_call2,         // Call subroutine
// PIC_clrwdt
PIC_daw0,          // Decimal Adjust W
// PIC_goto
// PIC_nop
// PIC_nop
PIC_pop0,          // Pop top of return stack
PIC_push0,         // Push top of return stack
PIC_rcall1,        // Relative Call subroutine
PIC_reset0,        // Software device Reset
PIC_retfie1,       // Return from interrupt enable
// PIC_retlw
PIC_return1,       // Return from Subroutine
// PIC_sleep

// LITERAL OPERATIONS

// PIC_addlw
// PIC_andlw
// PIC_iorlw
PIC_lfsr2,         // Move literal to FSR
PIC_movlb1,        // Move literal to BSR
// PIC_movlw
PIC_mullw1,        // Multiply literal with W
// PIC_retlw
// PIC_sublw
// PIC_xorlw

// DATA MEMORY <-> PROGRAM MEMORY OPERATIONS

PIC_tblrd0,        // Table Read
PIC_tblrd0p,       // Table Read with post-increment
PIC_tblrd0m,       // Table Read with post-decrement
PIC_tblrdp0,       // Table Read with pre-increment
PIC_tblwt0,        // Table Write
PIC_tblwt0p,       // Table Write with post-increment
PIC_tblwt0m,       // Table Write with post-decrement
PIC_tblwtp0,       // Table Write with pre-increment

PIC_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
SPARC_null = 0,   // Unknown Operation

// SPARC V9 instructions

SPARC_add,           // Add
SPARC_addcc,         // Add and modify condition codes
SPARC_addc,          // Add with carry
SPARC_addccc,        // Add with carry and modify condition codes
SPARC_and,           // And
SPARC_andcc,         // And and modify condition codes
SPARC_andn,          // And not
SPARC_andncc,        // And not (and modify condition codes)
SPARC_b,             // Branch on integer condition codes
SPARC_bp,            // Branch on integer condition codes with prediction
SPARC_bpr,           // Branch on contents of integer register with prediction
SPARC_call,          // Call and link
SPARC_casa,          // Compare and swap word in alternate space
SPARC_casxa,         // Compare and swap doubleword in alternate space
SPARC_done,          // Return from trap
SPARC_fabs,          // Floating-point absolute value
SPARC_fadd,          // Floating-point add
SPARC_fbp,           // Branch on floating-point condition codes with prediction
SPARC_fb,            // Branch on floating-point condition codes
SPARC_fcmp,          // Floating-point compare
SPARC_fcmpe,         // Floating-point compare (exception if unordered)
SPARC_fdiv,          // Floating-point divide
SPARC_fdmulq,        // Floating-point multiply double to quad
SPARC_flush,         // Flush instruction memory
SPARC_flushw,        // Flush register windows
SPARC_fmov,          // Floating-point move
SPARC_fmovcc,        // Conditional floating-point move
SPARC_fmovr,         // Move floating-point register if integer register contents satisfy condition
SPARC_fmul,          // Floating-point multiply
SPARC_fneg,          // Floating-point negate
SPARC_fsmuld,        // Floating-point multiply single to double
SPARC_fsqrt,         // Floating-point square root
SPARC_fsub,          // Floating-point subtract
SPARC_fstox,         // Convert single floating point to 64-bit integer
SPARC_fdtox,         // Convert doublee floating point to 64-bit integer
SPARC_fqtox,         // Convert quad floating point to 64-bit integer
SPARC_fxtos,         // Convert 64-bit integer to single floating point
SPARC_fxtod,         // Convert 64-bit integer to double floating point
SPARC_fxtoq,         // Convert 64-bit integer to quad floating point
SPARC_fitos,         // Convert 32-bit integer to single floating point
SPARC_fdtos,         // Convert single floating point to single floating point
SPARC_fqtos,         // Convert quade floating point to single floating point
SPARC_fitod,         // Convert 32-bit integer to double floating point
SPARC_fstod,         // Convert single floating point to double floating point
SPARC_fqtod,         // Convert quade floating point to double floating point
SPARC_fitoq,         // Convert 32-bit integer to quad floating point
SPARC_fstoq,         // Convert single floating point to quad floating point
SPARC_fdtoq,         // Convert quade floating point to quad floating point
SPARC_fstoi,         // Convert single floating point to 32-bit integer
SPARC_fdtoi,         // Convert doublee floating point to 32-bit integer
SPARC_fqtoi,         // Convert quad floating point to 32-bit integer
SPARC_illtrap,       // Illegal instruction
SPARC_impdep1,       // Implementation-dependent instruction
SPARC_impdep2,       // Implementation-dependent instruction
SPARC_jmpl,          // Jump and link
SPARC_ldd,           // Load doubleword
SPARC_ldda,          // Load doubleword from alternate space
SPARC_lddf,          // Load double floating-point
SPARC_lddfa,         // Load double floating-point from alternate space
SPARC_ldf,           // Load floating-point
SPARC_ldfa,          // Load floating-point from alternate space
SPARC_ldfsr,         // Load floating-point state register lower
SPARC_ldqf,          // Load quad floating-point
SPARC_ldqfa,         // Load quad floating-point from alternate space
SPARC_ldsb,          // Load signed byte
SPARC_ldsba,         // Load signed byte from alternate space
SPARC_ldsh,          // Load signed halfword
SPARC_ldsha,         // Load signed halfword from alternate space
SPARC_ldstub,        // Load-store unsigned byte
SPARC_ldstuba,       // Load-store unsigned byte in alternate space
SPARC_ldsw,          // Load signed word
SPARC_ldswa,         // Load signed word from alternate space
SPARC_ldub,          // Load unsigned byte
SPARC_lduba,         // Load unsigned byte from alternate space
SPARC_lduh,          // Load unsigned halfword
SPARC_lduha,         // Load unsigned halfword from alternate space
SPARC_lduw,          // Load unsigned word
SPARC_lduwa,         // Load unsigned word from alternate space
SPARC_ldx,           // Load extended
SPARC_ldxa,          // Load extended from alternate space
SPARC_ldxfsr,        // Load extended floating-point state register
SPARC_membar,        // Memory barrier
SPARC_mov,           // Move integer register if condition is satisfied
SPARC_movr,          // Move integer register on contents of integer register
SPARC_mulscc,        // Multiply step and modify condition codes
SPARC_mulx,          // Multiply 64-bit integers
SPARC_nop,           // No operation
SPARC_or,            // Inclusive-or
SPARC_orcc,          // Inclusive-or (and modify condition codes)
SPARC_orn,           // Inclusive-or not
SPARC_orncc,         // Inclusive-or not (and modify condition codes)
SPARC_popc,          // Population count
SPARC_prefetch,      // Prefetch data
SPARC_prefetcha,     // Prefetch data from alternate space
SPARC_rd,            // Read state register
SPARC_rdpr,          // Read privileged register
SPARC_restore,       // Restore caller's window
SPARC_restored,      // Window has been restored
SPARC_retry,         // Return from trap and retry
SPARC_return,        // Return
SPARC_save,          // Save caller's window
SPARC_saved,         // Window has been saved
SPARC_sdiv,          // 32-bit signed integer divide
SPARC_sdivcc,        // 32-bit signed integer divide and modify condition codes
SPARC_sdivx,         // 64-bit signed integer divide
SPARC_sethi,         // Set high 22 bits of low word of integer register
SPARC_sir,           // Software-initiated reset
SPARC_sll,           // Shift left logical
SPARC_sllx,          // Shift left logical, extended
SPARC_smul,          // Signed integer multiply
SPARC_smulcc,        // Signed integer multiply (and modify condition codes)
SPARC_sra,           // Shift right arithmetic
SPARC_srax,          // Shift right arithmetic, extended
SPARC_srl,           // Shift right logical
SPARC_srlx,          // Shift right logical, extended
SPARC_stb,           // Store byte
SPARC_stba,          // Store byte into alternate space
SPARC_stbar,         // Store barrier
SPARC_std,           // Store doubleword
SPARC_stda,          // Store doubleword into alternate space
SPARC_stdf,          // Store double floating-point
SPARC_stdfa,         // Store double floating-point into alternate space
SPARC_stf,           // Store floating-point
SPARC_stfa,          // Store floating-point into alternate space
SPARC_stfsr,         // Store floating-point state register
SPARC_sth,           // Store halfword
SPARC_stha,          // Store halfword into alternate space
SPARC_stqf,          // Store quad floating-point
SPARC_stqfa,         // Store quad floating-point into alternate space
SPARC_stw,           // Store word
SPARC_stwa,          // Store word into alternate space
SPARC_stx,           // Store extended
SPARC_stxa,          // Store extended into alternate space
SPARC_stxfsr,        // Store extended floating-point state register
SPARC_sub,           // Subtract
SPARC_subcc,         // Subtract and modify condition codes
SPARC_subc,          // Subtract with carry
SPARC_subccc,        // Subtract with carry and modify condition codes
SPARC_swap,          // Swap integer register with memory
SPARC_swapa,         // Swap integer register with memory in alternate space
SPARC_taddcc,        // Tagged add and modify condition codes
SPARC_taddcctv,      // Tagged add and modify condition codes (trap on overflow)
SPARC_tsubcc,        // Tagged subtract and modify condition codes
SPARC_tsubcctv,      // Tagged subtract and modify condition codes (trap on overflow)
SPARC_t,             // Trap on integer condition codes
SPARC_udiv,          // Unsigned integer divide
SPARC_udivcc,        // Unsigned integer divide and modify condition codes
SPARC_udivx,         // 64-bit unsigned integer divide
SPARC_umul,          // Unsigned integer multiply
SPARC_umulcc,        // Unsigned integer multiply and modify condition codes
SPARC_wr,            // Write Y register
SPARC_wrpr,          // Write privileged register
SPARC_xnor,          // Exclusive-nor
SPARC_xnorcc,        // Exclusive-nor (and modify condition codes)
SPARC_xor,           // Exclusive-or
SPARC_xorcc,         // Exclusive-or (and modify condition codes)

// Synthetic instructions

SPARC_cmp,           // Compare
SPARC_jmp,           // Jump
SPARC_iprefetch,     // Instruction prefetch
SPARC_tst,           // Test
SPARC_ret,           // Return from subroutine
SPARC_retl,          // Return from leaf subroutine
SPARC_setuw,         // Load unsigned constant
SPARC_setsw,         // Load signed constant
SPARC_setx,          // Load 64-bit constant
SPARC_signx,         // Sign-extend 32-bit value to 64 bits
SPARC_not,           // One's complement
SPARC_neg,           // Two's complement
SPARC_cas,           // Compare and swap
SPARC_casl,          // Compare and swap, little endian
SPARC_casx,          // Compare and swap extended
SPARC_casxl,         // Compare and swap extended, little endian
SPARC_inc,           // Increment
SPARC_inccc,         // Increment and modify condition codes
SPARC_dec,           // Deccrement
SPARC_deccc,         // Decrement and modify condition codes
SPARC_btst,          // Bit test
SPARC_bset,          // Bit set
SPARC_bclr,          // Bit clear
SPARC_btog,          // Bit toggle
SPARC_clr,           // Clear
SPARC_clrb,          // Clear byte
SPARC_clrh,          // Clear halfword
SPARC_clrx,          // Clear extended word
SPARC_clruw,         // Clear upper word
SPARC_pseudo_mov,    // Move register

// UtlraSPARC II extensions

SPARC_alignaddress,  // Calculate address for misaligned data access
SPARC_array,         // 3-D address to blocked byte address conversion
SPARC_edge,          // Edge boundary processing {little-endian}
SPARC_faligndata,    // Perform data alignment for misaligned data
SPARC_fandnot1,      // Negated src1 AND src2 (single precision)
SPARC_fandnot2,      // src1 AND negated src2 (single precision)
SPARC_fand,          // Logical AND (single precision)
SPARC_fcmpeq,        // Four 16-bit/two 32-bit compare; set integer dest if src1 = src2
SPARC_fcmpgt,        // Four 16-bit/two 32-bit compare; set integer dest if src1 > src2
SPARC_fcmple,        // Four 16-bit/two 32-bit compare; set integer dest if src1 <= src2
SPARC_fcmpne,        // Four 16-bit/two 32-bit compare; set integer dest if src1 != src2
SPARC_fexpand,       // Four 8-bit to 16-bit expands
SPARC_fmul8sux16,    // Signed upper 8/16-bit partitioned product of corresponding components
SPARC_fmul8ulx16,    // Unsigned lower 8/16-bit partitioned product of corresponding components
SPARC_fmul8x16,      // 8/16-bit partitioned product of corresponding components
SPARC_fmul8x16al,    // 8/16-bit lower a partitioned product of 4 components
SPARC_fmul8x16au,    // 8/16-bit upper a partitioned product of 4 components
SPARC_fmuld8sux16,   // Signed upper 8/16-bit multiply 32-bit partitioned product of components
SPARC_fmuld8ulx16,   // Unsigned lower 8/16-bit multiply 32-bit partitioned product of components
SPARC_fnand,         // Logical NAND (single precision)
SPARC_fnor,          // Logical NOR (single precision)
SPARC_fnot1,         // Negate (1's complement) src1 (single precision)
SPARC_fnot2,         // Negate (1's complement) src2 (single precision)
SPARC_fone,          // One fill (single precision)
SPARC_fornot1,       // Negated src1 OR src2 (single precision)
SPARC_fornot2,       // src1 OR negated src2 (single precision)
SPARC_for,           // Logical OR (single precision)
SPARC_fpackfix,      // Two 32-bit to 16-bit fixed pack
SPARC_fpack,         // Four 16-bit/two 32-bit pixel pack
SPARC_fpadd,         // Four 16-bit/two 32-bit partitioned add (single precision)
SPARC_fpmerge,       // Two 32-bit pixel to 64-bit pixel merge
SPARC_fpsub,         // Four 16-bit/two 32-bit partitioned subtract (single precision)
SPARC_fsrc1,         // Copy src1 (single precision)
SPARC_fsrc2,         // Copy src2 (single precision)
SPARC_fxnor,         // Logical XNOR (single precision)
SPARC_fxor,          // Logical XOR (single precision)
SPARC_fzero,         // Zero fill (single precision)
SPARC_pdist,         // Distance between 8 8-bit components
SPARC_shutdown,      // Power-down support
//SPARC_stdfa,         // 8/16-bit store from a double precision FP register

// V8 only instructions
SPARC_rett,          // Return from trap

SPARC_last,

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
HPPA_null = 0,     // Unknown Operation

HPPA_add,          // Add
HPPA_addb,         // Add and Branch
HPPA_addi,         // Add to Immediate
HPPA_addib,        // Add Immediate and Branch
HPPA_addil,        // Add to Immediate Left
HPPA_and,          // AND
HPPA_andcm,        // AND complement
HPPA_b,            // Branch
HPPA_bb,           // Branch on Bit
HPPA_be,           // Branch External
HPPA_blr,          // Branch and Link Register
HPPA_break,        // Break
HPPA_bv,           // Branch Vectored
HPPA_bve,          // Branch Vectored External
HPPA_cldd,         // Coprocessor Load Doubleword
HPPA_cldw,         // Coprocessor Load Word
HPPA_clrbts,       // Clear Branch Target Stack
HPPA_cmpb,         // Compare and Branch
HPPA_cmpclr,       // Compare and Clear
HPPA_cmpib,        // Compare Immediate and Branch
HPPA_cmpiclr,      // Compare Immediate and Clear
HPPA_copr,         // Coprocessor Operation
HPPA_cstd,         // Coprocessor Store Doubleword
HPPA_cstw,         // Coprocessor Store Word
HPPA_dcor,         // Decimal Correct
HPPA_depd,         // Deposit Doubleword
HPPA_depdi,        // Deposit Doubleword Immediate
HPPA_depw,         // Deposit Word
HPPA_depwi,        // Deposit Word Immediate
HPPA_diag,         // Diagnose
HPPA_ds,           // Divide Step
HPPA_extrd,        // Extract Doubleword
HPPA_extrw,        // Extract Word
HPPA_fdc,          // Flush Data Cache
HPPA_fdce,         // Flush Data Cache Entry
HPPA_fic,          // Flush Instruction Cache
HPPA_fice,         // Flush Instruction Cache Entry
HPPA_hadd,         // Halfword Parallel Add
HPPA_havg,         // Halfword Parallel Average
HPPA_hshl,         // Halfword Parallel Shift Left
HPPA_hshladd,      // Halfword Parallel Shift Left and Add
HPPA_hshr,         // Halfword Parallel Shift Right
HPPA_hshradd,      // Halfword Parallel Shift Right and Add
HPPA_hsub,         // Halfword Parallel Subtract
HPPA_idtlbt,       // Insert Data TLB Translation
HPPA_iitlbt,       // Insert Instruction TLB Translation
HPPA_lci,          // Load Coherence Index
HPPA_ldb,          // Load Byte
HPPA_ldcd,         // Load and Clear Doubleword
HPPA_ldcw,         // Load and Clear Word
HPPA_ldd,          // Load Doubleword
HPPA_ldda,         // Load Doubleword Absolute
HPPA_ldh,          // Load Halfword
HPPA_ldil,         // Load Immediate Left
HPPA_ldo,          // Load Offset
HPPA_ldsid,        // Load Space Identifier
HPPA_ldw,          // Load Word
HPPA_ldwa,         // Load Word Absolute
HPPA_lpa,          // Load Physical Address
HPPA_mfctl,        // Move From Control Register
HPPA_mfia,         // Move From Instruction Address
HPPA_mfsp,         // Move From Space Register
HPPA_mixh,         // Mix Halfwords
HPPA_mixw,         // Mix Words
HPPA_movb,         // Move and Branch
HPPA_movib,        // Move Immediate and Branch
HPPA_mtctl,        // Move To Control Register
HPPA_mtsarcm,      // Move To Shift Amount Register Complement
HPPA_mtsm,         // Move To System Mask
HPPA_mtsp,         // Move To Space Register
HPPA_or,           // Inclusive OR
HPPA_pdc,          // Purge Data Cache
HPPA_pdtlb,        // Purge Data TLB
HPPA_pdtlbe,       // Purge Data TLB Entry
HPPA_permh,        // Permute Halfwords
HPPA_pitlb,        // Purge Instruction TLB
HPPA_pitlbe,       // Purge Instruction TLB Entry
HPPA_popbts,       // Pop Branch Target Stack
HPPA_probe,        // Probe Access
HPPA_probei,       // Probe Access Immediate
HPPA_pushbts,      // Push Branch Target Stack
HPPA_pushnom,      // Push Nominated
HPPA_rfi,          // Return From Interruption
HPPA_rsm,          // Reset System Mask
HPPA_shladd,       // Shift Left and Add
HPPA_shrpd,        // Sihft Right Pair Doubleword
HPPA_shrpw,        // Sihft Right Pair Word
HPPA_spop0,        // Special Operation Zero
HPPA_spop1,        // Special Operation One
HPPA_spop2,        // Special Operation Two
HPPA_spop3,        // Special Operation Three
HPPA_ssm,          // Set System Mask
HPPA_stb,          // Store Byte
HPPA_stby,         // Store Bytes
HPPA_std,          // Store Doubleword
HPPA_stda,         // Store Doubleword Absolute
HPPA_stdby,        // Store Doubleword Bytes
HPPA_sth,          // Store Halfword
HPPA_stw,          // Store Word
HPPA_stwa,         // Store Word Absolute
HPPA_sub,          // Subtract
HPPA_subi,         // Subtract from Immediate
HPPA_sync,         // Synchronize Caches
HPPA_syncdma,      // Synchronize DMA
HPPA_uaddcm,       // Unit Add Complement
HPPA_uxor,         // Unit XOR
HPPA_xor,          // Exclusive OR

// Floating point instructions

HPPA_fabs,         // Floating-Point Absolute Value
HPPA_fadd,         // Floating-Point Add
HPPA_fcmp,         // Floating-Point Compare
HPPA_fcnv,         // Floating-Point Convert
HPPA_fcpy,         // Floating-Point Copy
HPPA_fdiv,         // Floating-Point Divide
HPPA_fid,          // Floating-Point Identity
HPPA_fldd,         // Floating-Point Load Doubleword
HPPA_fldw,         // Floating-Point Load Word
HPPA_fmpy,         // Floating-Point Multiply
HPPA_fmpyadd,      // Floating-Point Multiply/Add
HPPA_fmpyfadd,     // Floating-Point Multiply Fused Add
HPPA_fmpynfadd,    // Floating-Point Multiply Negate Fused Add
HPPA_fmpysub,      // Floating-Point Multiply/Subtract
HPPA_fneg,         // Floating-Point Negate
HPPA_fnegabs,      // Floating-Point Negate Absolute Value
HPPA_frem,         // Floating-Point Remainder
HPPA_frnd,         // Floating-Point Round to Integer
HPPA_fsqrt,        // Floating-Point Square Root
HPPA_fstd,         // Floating-Point Store Doubleword
HPPA_fstw,         // Floating-Point Store Word
HPPA_fsub,         // Floating-Point Subtract
HPPA_ftest,        // Floating-Point Test
HPPA_xmpyu,        // Fixed-Point Multiply Unsigned

// Performance Monitor Coprocessor

HPPA_pmdis,        // Performance Monitor Disable
HPPA_pmenb,        // Performance Monitor Enable

// Macros

HPPA_call,         // Call Subroutine
HPPA_ret,          // Return From Subroutine
HPPA_shld,         // Shift Left Doubleword
HPPA_shlw,         // Shift Left Word
HPPA_shrd,         // Shift Right Doubleword
HPPA_shrw,         // Shift Right Word
HPPA_ldi,          // Load Immediate
HPPA_copy,         // Copy Register
HPPA_mtsar,        // Move To %SAR
HPPA_nop,          // No Operation

HPPA_last,

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

H8500_null = 0,      // Unknown Operation

// Data transfer

H8500_mov_g,         // B/W Move data
H8500_mov_e,         // B   Move data
H8500_mov_i,         // W   Move data
H8500_mov_f,         // B/W Move data
H8500_mov_l,         // B/W Move data
H8500_mov_s,         // B/W Move data
H8500_ldm,           // W   Pop data from the stack to one or more registers
H8500_stm,           // W   Push data from one or more registers onto the stack
H8500_xch,           // W   Exchange data between two general registers
H8500_swap,          // B   Exchange the upper and lower bytes in a general register
H8500_movtpe,        // B   Transfer data from a general register to memory
H8500_movfpe,        // B   Transfer data from memory to a general register

// Arithmetic operations

H8500_add_g,         // B/W Addition
H8500_add_q,         // B/W Addition
H8500_sub,           // B/W Subtraction
H8500_adds,          // B/W Addition
H8500_subs,          // B/W Subtraction
H8500_addx,          // B/W Addition with carry
H8500_subx,          // B/W Subtraction with borrow
H8500_dadd,          // B   Decimal addition
H8500_dsub,          // B   Decimal subtraction
H8500_mulxu,         // B/W Unsigned multiplication
H8500_divxu,         // B/W Unsigned division
H8500_cmp_g,         // B/W Compare data
H8500_cmp_e,         // B   Compare data
H8500_cmp_i,         // W   Compare data
H8500_exts,          // B   Convert byte to word by extending the sign bit
H8500_extu,          // B   Convert byte to word data by padding with zero bits
H8500_tst,           // B/W Compare with 0
H8500_neg,           // B/W Negate
H8500_clr,           // B/W Make zero
H8500_tas,           // B   Test and set

// Logic Operations

H8500_and,           // B/W Logical AND
H8500_or,            // B/W Logical OR
H8500_xor,           // B/W Exclusive OR
H8500_not,           // B/W Bitwise NOT

// Shift Operations

H8500_shal,          // B/W Arithmetic shift left
H8500_shar,          // B/W Arithmetic shift right
H8500_shll,          // B/W Logical shift left
H8500_shlr,          // B/W Logical shift right
H8500_rotl,          // B/W Rotate left
H8500_rotr,          // B/W Rotate right
H8500_rotxl,         // B/W Rotate through carry left
H8500_rotxr,         // B/W Rotate through carry right

// Bit Manipulations

H8500_bset,          // B/W Test bit and set
H8500_bclr,          // B/W Test bit and clear
H8500_bnot,          // B/W Test bit and invert
H8500_btst,          // B/W Test bit

// Branching Instructions

H8500_bra,           //     Branch Always
H8500_brn,           //     Branch Never
H8500_bhi,           //     Branch if High (C|Z = 0)
H8500_bls,           //     Branch if Low or Same (C|Z = 1)
H8500_bcc,           //     Branch if Carry Clear (C = 0)
H8500_bcs,           //     Branch if Carry Set (C = 1)
H8500_bne,           //     Branch if Not Equal (Z = 0)
H8500_beq,           //     Branch if Equal (Z = 1)
H8500_bvc,           //     Branch if Overflow Clear (V = 0)
H8500_bvs,           //     Branch if Overflow Set (V = 1)
H8500_bpl,           //     Branch if Plus (N = 0)
H8500_bmi,           //     Branch if Minus (N = 1)
H8500_bge,           //     Branch if Greater or Equal (N^V = 0)
H8500_blt,           //     Branch if Less Than (N^V = 1)
H8500_bgt,           //     Branch if Greater Than (Z|(N^V) = 0)
H8500_ble,           //     Branch if Less or Equal (Z|(N^V) = 1)
H8500_jmp,           //     Branch unconditionally (same page)
H8500_pjmp,          //     Branch unconditionally (specified page)
H8500_bsr,           //     Branch to subroutine (same page)
H8500_jsr,           //     Branch to subroutine (same page)
H8500_pjsr,          //     Branch to subroutine (specified page)
H8500_rts,           //     Return from subroutine (same page)
H8500_prts,          //     Return from subroutine (different page)
H8500_rtd,           //     Return from subroutine (same page) and adjust SP
H8500_prtd,          //     Return from subroutine (different page) and adjust SP
H8500_scb,           //     Control loop

// System Control Instructions

H8500_trapa,         //     Generate trap exception
H8500_trap_vs,       //     Generate trap exception if the V bit is set
H8500_rte,           //     Return from exception-handling routine
H8500_link,          //     Create stack frame
H8500_unlk,          //     Deallocate stack frame
H8500_sleep,         //     Go to power-down state
H8500_ldc,           // B/W Move to control register
H8500_stc,           // B/W Move from control register
H8500_andc,          // B/W Logically AND control register
H8500_orc,           // B/W Logically OR control register
H8500_xorc,          // B/W Logically exclusive-OR control register
H8500_nop,           //     No operation
H8500_bpt,           //

H8500_last,

    };



extern instruc_t Instructions[];

enum 
{
DSP56_null = 0,     // Unknown Operation

DSP56_abs,          // Absolute Value
DSP56_adc,          // Add Long with Carry
DSP56_add,          // Addition
DSP56_addl,         // Shift Left and Add
DSP56_addr,         // Shift Right and Add
DSP56_and,          // Logical AND
DSP56_andi,         // AND Immediate to Control Register
DSP56_asl,          // Arithmetic Shift Left
DSP56_asl4,         // Arithmetic Shift Left 4
DSP56_asr,          // Arithmetic Shift Right
DSP56_asr4,         // Arithmetic Shift Right 4
DSP56_asr16,        // Arithmetic Shift Right 16
DSP56_bfchg,        // Test Bit Field and Change
DSP56_bfclr,        // Clear Bit Field
DSP56_bfset,        // Set Bit Field
DSP56_bftsth,       // Test Bit Field High
DSP56_bftstl,       // Test Bit Field Low
DSP56_bcc,          // Branch Conditionaly
DSP56_bchg,         // Bit Test and Change
DSP56_bclr,         // Bit Test and Clear
DSP56_bra,          // Branch Always
DSP56_brclr,        // Branch if Bit Clear
DSP56_brkcc,        // Exit Current DO Loop Conditionally
DSP56_brset,        // Branch if Bit Set
DSP56_bscc,         // Branch to Subroutine Conditionaly
DSP56_bsclr,        // Branch to Subroutine if Bit Clear
DSP56_bset,         // Bit Test and Set
DSP56_bsr,          // Branch to Subroutine
DSP56_bsset,        // Branch to Subroutine if Bit Set
DSP56_btst,         // Bit Test on Memory and Registers
DSP56_chkaau,           // Check address ALU result
DSP56_clb,          // Count Leading Bits
DSP56_clr,          // Clear an Operand
DSP56_clr24,        // Clear 24 MS-bits of Accumulator
DSP56_cmp,          // Compare
DSP56_cmpm,         // Compare Magnitude
DSP56_cmpu,         // Compare Unsigned
DSP56_debug,        // Enter Debug Mode
DSP56_debugcc,      // Enter Debug Mode Conditionally
DSP56_dec,          // Decrement by One
DSP56_dec24,        // Decrement 24 MS-bit of Accumulator
DSP56_div,          // Divide Iteration
DSP56_dmac,         // Double-Precision Multiply-Accumulate With Right Shift
DSP56_do,           // Start Hardware Loop
DSP56_do_f,         // Start Infinite Loop
DSP56_dor,          // Start PC-Relative Hardware Loop
DSP56_dor_f,        // Start PC-Relative Infinite Loop
DSP56_enddo,        // Exit from Hardware Loop
DSP56_eor,          // Logical Exclusive OR
DSP56_extract,      // Extract Bit Field
DSP56_extractu,     // Extract Unsigned Bit Field
DSP56_ext,          // Sign Extend Accumulator
DSP56_ill,          // Illegal Instruction
DSP56_imac,         // Integer Multiply-Accumulate
DSP56_impy,         // Integer Multiply
DSP56_inc,          // Increment by One
DSP56_inc24,        // Increment 24 MS-bit of Accumulator
DSP56_insert,       // Insert Bit Field
DSP56_jcc,          // Jump Conditionally
DSP56_jclr,         // Jump if Bit Clear
DSP56_jmp,          // Jump
DSP56_jscc,         // Jump to Subroutine Conditionally
DSP56_jsclr,        // Jump to Subroutine if Bit Clear
DSP56_jset,         // Jump if Bit Set
DSP56_jsr,          // Jump to Subroutine
DSP56_jsset,        // Jump to Subroutine if Bit Set
DSP56_lra,          // Load PC-Reliative Address
DSP56_lsl,          // Logical Shift Left
DSP56_lsr,          // Logical Shift Right
DSP56_lua,          // Load Updated Address
DSP56_lea,          // Load Updated Address
DSP56_mac,          // Signed Multiply-Accumulate
DSP56_maci,         // Signed Multiply-Accumulate With Immediate Operand
DSP56_mac_s_u,      // Mixed Multiply-Accumulate
DSP56_macr,         // Signed Multiply-Accumulate and Round
DSP56_macri,        // Signed Multiply-Accumulate and Round With Immediate Operand
DSP56_max,          // Transfer by Signed Value
DSP56_maxm,         // Transfer by Magnitude
DSP56_merge,        // Merge Two Half Words
DSP56_move,         // Move Data
DSP56_movec,        // Move Control Register
DSP56_movei,        // Move Immediate Short
DSP56_movem,        // Move Program Memory
DSP56_movep,        // Move Peripheral Data
DSP56_moves,        // Move Absolute Short
DSP56_mpy,          // Signed Multiply
DSP56_mpyi,         // Signed Multiply With Immediate Operand
DSP56_mpy_s_u,      // Mixed Multiply
DSP56_mpyr,         // Signed Multiply and Round
DSP56_mpyri,        // Signed Multiply and Round With Immediate Operand
DSP56_neg,          // Negate Accumulator
DSP56_negc,         // Negate Accumulator
DSP56_nop,          // No Operation
DSP56_norm,         // Norm Accumulator Iteration
DSP56_normf,        // Fast Accumulator Normalization
DSP56_not,          // Logical Complement
DSP56_or,           // Logical Inclusive OR
DSP56_ori,          // OR Immediate to Control Register
DSP56_pflush,       // Program Cache Flush
DSP56_pflushun,     // Program Cache Flush Unlocked Sectors
DSP56_pfree,        // Program Cache Global Unlock
DSP56_plock,        // Lock Instruction Cache Sector
DSP56_plockr,       // Lock Instruction Cache Relative Sector
DSP56_punlock,      // Unlock Instruction Cache Sector
DSP56_punlockr,     // Unlock Instruction Cache Relative Sector
DSP56_rep,          // Repeat Next Instruction
DSP56_repcc,        // Repeat Next Instruction
DSP56_reset,        // Reset On-Chip Peripheral Devices
DSP56_rnd,          // Round Accumulator
DSP56_rol,          // Rotate Left
DSP56_ror,          // Rotate Right
DSP56_rti,          // Return from Interrupt
DSP56_rts,          // Return from Subroutine
DSP56_sbc,          // Subtract Long with Carry
DSP56_stop,         // Stop Processing (Low-Power Standby)
DSP56_sub,          // Subtract
DSP56_subl,         // Shift Left and Subtract
DSP56_subr,         // Shift Right and Subtract
DSP56_swap,         // Swap Accumulator Words
DSP56_tcc,          // Transfer Conditionally
DSP56_tfr,          // Transfer Data ALU Register
DSP56_tfr2,         // Transfer Data ALU Register
DSP56_tfr3,         // Transfer Data ALU Register
DSP56_trap,         // Software Interrupt
DSP56_trapcc,       // Software Interrupt Conditionally
DSP56_tst,          // Test an Operand
DSP56_tst2,         // Test an Operand
DSP56_vsl,          // Viterbi Shift Left
DSP56_wait,         // Wait for Interrupt or DMA Request (Low-Power Standby)
DSP56_zero,         // Zero Extend Accumulator
DSP56_swi,          // Software Interrupt (only for 56000)
DSP56_pmov,         // Pseudo cmd
DSP56_last,

};



extern instruc_t Instructions[];

enum 
{
DSP96_null = 0,     // Unknown Operation

DSP96_abs,          // Absolute Value
DSP96_add,          // Addition
DSP96_addc,         // Add with Carry
DSP96_and,          // Logical AND
DSP96_andc,         // AND Immediate with Complement
DSP96_andi,         // AND Immediate to Control Register
DSP96_asl,          // Arithmetic Shift Left
DSP96_asr,          // Arithmetic Shift Right
DSP96_bcc,          // Branch Conditionally (indirect)
DSP96_bccd,         // Branch Conditionally (direct)
DSP96_bchg,         // Bit Test and Change
DSP96_bclr,         // Bit Test and Clear
DSP96_bfind,        // Find Leading One
DSP96_bra,          // Branch Always
DSP96_brclr,        // Branch if Bit Clear
DSP96_brset,        // Branch if Bit Set
DSP96_bscc,         // Branch to Subroutine Conditionally (indirect)
DSP96_bsccd,        // Branch to Subroutine Conditionally (direct)
DSP96_bsclr,        // Branch to Subroutine if Bit Clear
DSP96_bset,         // Bit Test and Set
DSP96_bsr,          // Branch to Subroutine (indirect)
DSP96_bsrd,         // Branch to Subroutine (direct)
DSP96_bsset,        // Branch to Subroutine if Bit Set
DSP96_btst,         // Bit Test on Memory and Registers
DSP96_clr,          // Clear an Operand
DSP96_cmp,          // Compare
DSP96_cmpg,         // Graphics Compare with Trivial Accept.Reject Flags
DSP96_debugcc,      // Enter Debug Mode Conditionally
DSP96_dec,          // Decrement by One
DSP96_do,           // Start Hardware Loop
DSP96_dor,          // Start PC-Relative Hardware Loop
DSP96_enddo,        // Exit from Hardware Loop
DSP96_eor,          // Logical Exclusive OR
DSP96_ext,          // Sign Extend 16-Bit To 32-Bit
DSP96_extb,         // Sign Extend 8-Bit to 32-Bit
DSP96_fabs,         // Absolute Value
DSP96_fadd,         // Add
DSP96_faddsub,      // Add and Subtract
DSP96_fbcc,         // Branch Conditionally (Floating-Point Condition) (indirect)
DSP96_fbccd,        // Branch Conditionally (Floating-Point Condition) (direct)
DSP96_fbscc,        // Branch to Subroutine Conditionally (Floating-Point Condition) (indirect)
DSP96_fbsccd,       // Branch to Subroutine Conditionally (Floating-Point Condition) (direct)
DSP96_fclr,         // Clear a Floating-Point Operand
DSP96_fcmp,         // Compare
DSP96_fcmpg,        // Graphics Compare with Trivial Accept.Reject Flags
DSP96_fcmpm,        // Compare Magnitude
DSP96_fcopys,       // Copy Sign
DSP96_fdebugcc,     // Enter Debug Mode Conditionally
DSP96_fgetman,      // Get Mantissa
DSP96_fint,         // Convert to Floating-Point Integer
DSP96_fjcc,         // Jump Conditionally (indirect)
DSP96_fjccd,        // Jump Conditionally (direct)
DSP96_fjscc,        // Jump to Subroutine Conditionally (indirect)
DSP96_fjsccd,        // Jump to Subroutine Conditionally (direct)
DSP96_float,        // Integer to Floating-Point Conversion
DSP96_floatu,       // Unsigned Integer to SP Floating-Point Conversion
DSP96_floor,        // Convert to Floating-Point Integer round to -Infinity
DSP96_fmove,        // Move FP Data Register(s)
DSP96_fmpyfadd,     // Multiply and Add
//DSP96_fmpyfaddx,    // Multiply and Add (Single Extended Precision) (Handled with DSP96_fmpyfadd)
DSP96_fmpyfaddsub,  // Multiply, Add and Subtract
//DSP96_fmpyfaddsubx, // Multiply, Add and Subtract (Single Extended Precision) (Handled with DSP96_fmpyfaddsub)
DSP96_fmpyfsub,     // Multiply and Subtract
//DSP96_fmpyfsubx,    // Multiply and Subtract (Single Extended Precision) (Handled with DSP96_fmpyfsub)
DSP96_fmpy,         // Multiply
DSP96_fneg,         // Change sign
DSP96_fscale,       // Scale a Floating-Point Operand
DSP96_fseedd,       // Reciprocal Approximation
DSP96_fseedr,       // Square-Root Reciprocal Approximation
DSP96_fsub,         // Subtract
DSP96_ftfr,         // Transfer Floating-Point Register
DSP96_ftrapcc,      // Conditional Software Interrupt
DSP96_ftst,         // Test a floating-Point Operand
DSP96_getexp,       // Get Exponent
DSP96_illegal,      // Illegal Instruction
DSP96_inc,          // Increment by One
DSP96_int,          // Floating-Point to Integer Conversion
DSP96_intrz,        // Floating-Point to Integer Conversion Round to Zero
DSP96_intu,         // Floating-Point to Unsigned Integer Conversion
DSP96_inturz,       // Floating-Point to Unsigned Integer Conversion Round to Zero
DSP96_jcc,          // Jump Conditionally (indirect)
DSP96_jccd,         // Jump Conditionally (direct)
DSP96_jclr,         // Jump if Bit Clear
// DSP96_jmp,          // Jump (The same as jcc with condition code = 11111)
DSP96_join,         // Join two 16-Bit Integers
DSP96_joinb,        // Join two 8-Bit Integers
DSP96_jscc,         // Jump to Subroutine Conditionally
DSP96_jsccd,         // Jump to Subroutine Conditionally
DSP96_jsclr,        // Jump to Subroutine if Bit Clear
DSP96_jset,         // Jump if Bit Set
// DSP96_jsr,          // Jump to Subroutine ( The same as jscc with condition code = 11111)
DSP96_jsset,        // Jump to Subroutine if Bit Set
DSP96_lea,          // Load Effective Address
DSP96_lra,          // Load PC Relative Address
DSP96_lsl,          // Logical Shift Left
DSP96_lsr,          // Logical Shift Right
DSP96_move,         // Move Data Register(s)
DSP96_movec,        // Move Control Register
DSP96_movei,        // Move Immediate
DSP96_movem,        // Move Program Memory
DSP96_movep,        // Move Peripheral Data
DSP96_moves,        // Move Absolute Short
DSP96_moveta,       // Move Data Register(s) and Test Address
DSP96_mpys,         // Signed Multiply
DSP96_mpyu,         // Unsigned Multiply
DSP96_neg,          // Negate Accumulator
DSP96_negc,         // Negate with Carry
DSP96_nop,          // No Operation
DSP96_not,          // Logical Complement
DSP96_or,           // Logical Inclusive OR
DSP96_orc,          // OR With Compliment
DSP96_ori,          // OR Immediate to Control Register
DSP96_rep,          // Repeat Next Instruction
DSP96_reset,        // Reset On-Chip Peripheral Devices
DSP96_rol,          // Rotate Left
DSP96_ror,          // Rotate Right
DSP96_rti,          // Return from Interrupt
DSP96_rtr,          // Return from Subroutine and Restore Status Register
DSP96_rts,          // Return from Subroutine
DSP96_setw,         // Set an Operand
DSP96_split,        // Extract 16-Bit Integer
DSP96_splitb,       // Extract 8-Bit Integer
DSP96_stop,         // Stop Processing (Low Power Stand-by)
DSP96_sub,          // Subtract
DSP96_subc,         // Subtract with Carry
DSP96_tfr,          // Transfer Data ALU Register
DSP96_trapcc,       // Software Interrupt Conditionally
DSP96_tst,          // Test an Operand
DSP96_wait,         // Wait for Interrupt or DMA Request (Low Power Stand-by)
DSP96_last,

};

enum PMNum // Parallel Move enumeration
{
  PM96_NoMove,
  PM96_R2R,           // Register to register parallel move
  PM96_Update,        // Move update
  PM96_XYMem,         // X or Y memory move
  PM96_XYmemR,        // X or Y memory and Registry move
  PM96_Long,          // Long Memory move
  PM96_XY,            // XY Memory move

  PM96_IFcc,          // Integer if conditional Instruction without CCR update
  // All the following moves are handled by IFcc
  // PM96_FFcc,          // Floating-Point if conditional Instruction without CCR, ER, IER update
  // PM96_FFccU,         // Floating-Point if conditional Instruction with CCR, ER, IER update
  // PM96_IFccU,         // Integer if conditional Instruction with CCR update
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

C166_null = 0,      // Unknown Operation

C166_add,           // Integer Addition
C166_addb,          // Integer Addition Byte
C166_addc,          // Integer Addition with Carry
C166_addcb,         // Integer Addition with Carry Byte
C166_and,           // Logical AND
C166_andb,          // Logical AND Byte
C166_ashr,          // Arithmetic Shift Right
C166_atomic,        // Begin ATOMIC Sequence
C166_band,          // Bit Logical AND
C166_bclr,          // Bit Clear
C166_bcmp,          // Bit to Bit Compare
C166_bfldh,         // Bit Field High Byte
C166_bfldl,         // Bit Field Low Byte
C166_bmov,          // Bit to Bit Move
C166_bmovn,         // Bit to Bit Move and Negate
C166_bor,           // Bit Logical OR
C166_bset,          // Bit Set
C166_bxor,          // Bit Logical XOR
C166_calla,         // Call Subroutine Absolute
C166_calli,         // Call Subroutine Indirect
C166_callr,         // Call Subroutine Relative
C166_calls,         // Call Inter-Segment Subroutine
C166_cmp,           // Integer Compare
C166_cmpb,          // Integer Compare Byte
C166_cmpd1,         // Integer Compare and Decrement by 1
C166_cmpd2,         // Integer Compare and Decrement by 2
C166_cmpi1,         // Integer Compare and Increment by 1
C166_cmpi2,         // Integer Compare and Increment by 2
C166_cpl,           // Integer One's Complement
C166_cplb,          // Integer One's Complement Byte
C166_diswdt,        // Disable Watchdog Timer
C166_div,           // 16-by-16 Signed Division
C166_divl,          // 32-by-16 Signed Division
C166_divlu,         // 32-by-16 Unsigned Division
C166_divu,          // 16-by-16 Unsigned Division
C166_einit,         // End of Intialization
C166_extr,          // Begin Extended Register Sequence
C166_extp,          // Begin Extended Page Sequence
C166_extpr,         // Begin Extended Page and Register Sequence
C166_exts,          // Begin Extended Segment Sequence
C166_extsr,         // Begin Extended Segment and Register Sequence
C166_idle,          // Enter Idle Mode
C166_jb,            // Relative Jump if Bit Set
C166_jbc,           // Relative Jump if Bit Set and Clear Bit
C166_jmpa,          // Absolute Conditional Jump
C166_jmpi,          // Indirect Conditional Jump
C166_jmpr,          // Relative Conditional Jump
C166_jmps,          // Absolute Inter-Segment Jump
C166_jnb,           // Relative Jump if Bit Clear
C166_jnbs,          // Relative Jump if Bit Clear and Set Bit
C166_mov,           // Move Word
C166_movb,          // Move Byte
C166_movbs,         // Move Byte Sign Extend
C166_movbz,         // Move Byte Zero Extend
C166_mul,           // Signed Multiplication
C166_mulu,          // Unsigned Multiplication
C166_neg,           // Integer Two's Complement
C166_negb,          // Integer Two's Complement Byte
C166_nop,           // No Operation
C166_or,            // Logical OR
C166_orb,           // Logical OR Byte
C166_pcall,         // Push Word and Call Subroutine Absolute
C166_pop,           // Pop Word from System Stack
C166_prior,         // Prioritize Register
C166_push,          // Push Word on System Stack
C166_pwrdn,         // Enter Power Down Mode
C166_ret,           // Return from Subroutine
C166_reti,          // Return from Interrupt Routine
C166_retp,          // Return from Subroutine and Pop Word
C166_rets,          // Return from Inter-Segment Subroutine
C166_rol,           // Rotate Left
C166_ror,           // Rotate Right
C166_scxt,          // Switch Context
C166_shl,           // Shift Left
C166_shr,           // Shift Rightt
C166_srst,          // Software Reset
C166_srvwdt,        // Service Watchdog Timer
C166_sub,           // Integer Subtraction
C166_subb,          // Integer Subtraction Byte
C166_subc,          // Integer Subtraction with Carry
C166_subcb,         // Integer Subtraction with Carry Byte
C166_trap,          // Software Trap
C166_xor,           // Logical Exclusive OR
C166_xorb,          // Logical Exclusive OR Byte

// ST10 MAC instructions

ST10_CoABS,         // Absolute Value
ST10_CoADD,         // Add
ST10_CoASHR,        // Accumulator Arithmetic Shift Right with Optional Round
ST10_CoCMP,         // Compare
ST10_CoLOAD,        // Load Accumulator
ST10_CoMAC,         // Multiply-Accumulate & Optional Round
ST10_CoMACM,        // Multiply-Accumulate Parallel Data Move & Optional Round
ST10_CoMAX,         // Maximum
ST10_CoMIN,         // Minimum
ST10_CoMOV,         // Memory to Memory Move
ST10_CoMUL,         // Multiply & Optional Round
ST10_CoNEG,         // Negate Accumulator with Optional Rounding
ST10_CoNOP,         // No-Operation
ST10_CoRND,         // Round Accumulator
ST10_CoSHL,         // Accumulator Logical Shift Left
ST10_CoSHR,         // Accumulator Logical Shift Right
ST10_CoSTORE,       // Store a MAC-Unit Register
ST10_CoSUB,         // Subtract

// missing C166v2/ST10 instructions

C166_enwdt,         // Enable Watchdog Timer
C166_sbrk,          // Software Break

C166_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
ST20_null = 0,      // Unknown Operation

// C1 instructions

ST20_adc,           // add constant
ST20_add,           // add
ST20_addc,          // add with carry
ST20_ajw,           // adjust work space
ST20_and,           // and
ST20_arot,          // anti-rotate stack
ST20_ashr,          // arithmetic shift right
ST20_biquad,        // biquad IIR filter step
ST20_bitld,         // load bit
ST20_bitmask,       // create bit mask
ST20_bitst,         // store bit
ST20_breakpoint,    // breakpoint
ST20_cj,            // conditional jump
ST20_dequeue,       // dequeue a process
ST20_divstep,       // divide step
ST20_dup,           // duplicate
ST20_ecall,         // exception call
ST20_enqueue,       // enqueue a process
ST20_eqc,           // equals constant
ST20_eret,          // exception return
ST20_fcall,         // function call
ST20_gajw,          // general adjust workspace
ST20_gt,            // greater than
ST20_gtu,           // greater than unsigned
ST20_io,            // input/output
ST20_j,             // jump
ST20_jab,           // jump absolute
ST20_lbinc,         // load byte and increment
ST20_ldc,           // load constant
ST20_ldl,           // load local
ST20_ldlp,          // load local pointer
ST20_ldnl,          // load non-local
ST20_ldnlp,         // load non-local pointer
ST20_ldpi,          // load pointer to instruction
ST20_ldprodid,      // load product identity
ST20_ldtdesc,       // load task descriptor
ST20_lsinc,         // load sixteen and increment
ST20_lsxinc,        // load sixteen sign extended and increment
ST20_lwinc,         // load word and increment
ST20_mac,           // multiply accumulate
ST20_mul,           // multiply
ST20_nfix,          // negative prefix
ST20_nop,           // no operation
ST20_not,           // bitwise not
ST20_opr,           // operate
ST20_or,            // or
ST20_order,         // order
ST20_orderu,        // unsigned order
ST20_pfix,          // prefix
ST20_rev,           // reverse
ST20_rmw,           // read modify write
ST20_rot,           // rotate stack
ST20_run,           // run process
ST20_saturate,      // saturate
ST20_sbinc,         // store byte and increment
ST20_shl,           // shift left
ST20_shr,           // shift right
ST20_signal,        // signal
ST20_smacinit,      // initialize short multiply accumulate loop
ST20_smacloop,      // short multiply accumulate loop
ST20_smul,          // short multiply
ST20_ssinc,         // store sixteen and increment
ST20_statusclr,     // clear bits in status register
ST20_statusset,     // set bits in status register
ST20_statustst,     // test status register
ST20_stl,           // store local
ST20_stnl,          // store non-local
ST20_stop,          // stop process
ST20_sub,           // subtract
ST20_subc,          // subtract with carry
ST20_swap32,        // byte swap 32
ST20_swinc,         // store word and increment
ST20_timeslice,     // timeslice
ST20_umac,          // unsigned multiply accumulate
ST20_unsign,        // unsign argument
ST20_wait,          // wait
ST20_wsub,          // word subscript
ST20_xbword,        // sign extend byte to word
ST20_xor,           // exclusive or
ST20_xsword,        // sign extend sixteen to word

// C2-C4 instructions

ST20_alt,           // alt start
ST20_altend,        // alt end
ST20_altwt,         // alt wait
ST20_bcnt,          // byte count
ST20_bitcnt,        // count bits set in word
ST20_bitrevnbits,   // reverse bottom n bits in word
ST20_bitrevword,    // reverse bits in word
ST20_bsub,          // byte subscript
ST20_call,          // call
ST20_causeerror,    // cause error
ST20_cb,            // check byte
ST20_cbu,           // check byte unsigned
ST20_ccnt1,         // check count from 1
ST20_cflerr,        // check floating point error
ST20_cir,           // check in range
ST20_ciru,          // check in range unsigned
ST20_clockdis,      // clock disable
ST20_clockenb,      // clock enable
ST20_clrhalterr,    // clear halt-on error flag
ST20_crcbyte,       // calculate CRC on byte
ST20_crcword,       // calculate CRC on word
ST20_cs,            // check sixteen
ST20_csngl,         // check single
ST20_csu,           // check sixteen unsigned
ST20_csub0,         // check subscript from 0
ST20_cword,         // check word
ST20_devlb,         // device load byte
ST20_devls,         // device load sixteen
ST20_devlw,         // device load word
ST20_devmove,       // device move
ST20_devsb,         // device store byte
ST20_devss,         // device store sixteen
ST20_devsw,         // device store word
ST20_diff,          // difference
ST20_disc,          // disable channel
ST20_diss,          // disable skip
ST20_dist,          // disable timer
ST20_div,           // divide
ST20_enbc,          // enable channel
ST20_enbs,          // enable skip
ST20_enbt,          // enable timer
ST20_endp,          // end process
ST20_fmul,          // fractional multiply
ST20_fptesterr,     // test for FPU error
ST20_gcall,         // general call
ST20_gintdis,       // general interrupt disable
ST20_gintenb,       // general interrupt enable
ST20_in,            // input message
ST20_insertqueue,   // insert at front of scheduler queue
ST20_intdis,        // (localised) interrupt disable
ST20_intenb,        // (localised) interrupt enable
ST20_iret,          // interrupt return
ST20_ladd,          // long add
ST20_lb,            // load byte
ST20_lbx,           // load byte and sign extend
ST20_ldclock,       // load clock
ST20_lddevid,       // load device identity
ST20_ldiff,         // long diff
ST20_ldinf,         // load infinity
ST20_ldiv,          // long divide
ST20_ldmemstartval, // load value of MemStart address
ST20_ldpri,         // load current priority
ST20_ldshadow,      // load shadow registers
ST20_ldtimer,       // load timer
ST20_ldtraph,       // load trap handler
ST20_ldtrapped,     // load trapped process status
ST20_lend,          // loop end
ST20_lmul,          // long multiply
ST20_ls,            // load sixteen
ST20_lshl,          // long shift left
ST20_lshr,          // long shift right
ST20_lsub,          // long subtract
ST20_lsum,          // long sum
ST20_lsx,           // load sixteen and sign extend
ST20_mint,          // minimum integer
ST20_move,          // move message
ST20_move2dall,     // 2D block copy
ST20_move2dinit,    // initialize data for 2D block move
ST20_move2dnonzero, // 2D block copy non-zero bytes
ST20_move2dzero,    // 2D block copy zero bytes
ST20_norm,          // normalize
ST20_out,           // output message
ST20_outbyte,       // output byte
ST20_outword,       // output word
ST20_pop,           // pop processor stack
ST20_postnormsn,    // post-normalize correction of single length fp number
ST20_prod,          // product
ST20_reboot,        // reboot
ST20_rem,           // remainder
ST20_resetch,       // reset channel
ST20_restart,       // restart
ST20_ret,           // return
ST20_roundsn,       // round single length floating point number
ST20_runp,          // run process
ST20_satadd,        // saturating add
ST20_satmul,        // saturating multiply
ST20_satsub,        // saturating subtract
ST20_saveh,         // save high priority queue registers
ST20_savel,         // save low priority queue registers
ST20_sb,            // store byte
ST20_seterr,        // set error flags
ST20_sethalterr,    // set halt-on error flag
ST20_settimeslice,  // set timeslicing status
ST20_slmul,         // signed long multiply
ST20_ss,            // store sixteen
ST20_ssub,          // sixteen subscript
ST20_startp,        // start process
ST20_stclock,       // store clock register
ST20_sthb,          // store high priority back pointer
ST20_sthf,          // store high priority front pointer
ST20_stlb,          // store low priority back pointer
ST20_stlf,          // store low priority front pointer
ST20_stoperr,       // stop on error
ST20_stopp,         // stop process
ST20_stshadow,      // store shadow registers
ST20_sttimer,       // store timer
ST20_sttraph,       // store trap handler
ST20_sttrapped,     // store trapped process
ST20_sulmul,        // signed timer unsigned long multiply
ST20_sum,           // sum
ST20_swapqueue,     // swap scheduler queue
ST20_swaptimer,     // swap timer queue
ST20_talt,          // timer alt start
ST20_taltwt,        // timer alt wait
ST20_testerr,       // test error flag
ST20_testhalterr,   // test halt-on error flag
ST20_testpranal,    // test processor analysing
ST20_tin,           // timer input
ST20_trapdis,       // trap disable
ST20_trapenb,       // trap enable
ST20_tret,          // trap return
ST20_unpacksn,      // unpack single length fp number
ST20_wcnt,          // word count
ST20_wsubdb,        // form double word subscript
ST20_xdble,         // extend to double
ST20_xword,         // extend word

ST20_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
ST7_null = 0,           // Unknown Operation

ST7_adc,                // Add with Carry
ST7_add,                // Addition
ST7_and,                // Logical And
ST7_bcp,                // Bit compare
ST7_bres,               // Bit Reset
ST7_bset,               // Bit Set
ST7_btjf,               // Jump if bit is false
ST7_btjt,               // Jump if bit is true
ST7_call,               // Call subroutine
ST7_callr,              // Call subroutine relative
ST7_clr,                // Clear
ST7_cp,                 // Arithmetic Compare
ST7_cpl,                // One Complement
ST7_dec,                // Decrement
ST7_halt,               // Halt
ST7_iret,               // Interrupt routine return
ST7_inc,                // Increment
ST7_jp,                 // Absolute Jump
ST7_jra,                // Jump relative always
ST7_jrt,                // Jump relative
ST7_jrf,                // Never jump
ST7_jrih,               // Jump if Port INT pin = 1
ST7_jril,               // Jump if Port INT pin = 0
ST7_jrh,                // Jump if H = 1
ST7_jrnh,               // Jump if H = 0
ST7_jrm,                // Jump if I = 1
ST7_jrnm,               // Jump if I = 0
ST7_jrmi,               // Jump if N = 1 (minus)
ST7_jrpl,               // Jump if N = 0 (plus)
ST7_jreq,               // Jump if Z = 1 (equal)
ST7_jrne,               // Jump if Z = 0 (not equal)
ST7_jrc,                // Jump if C = 1
ST7_jrnc,               // Jump if C = 0
ST7_jrult,              // Jump if C = 1
ST7_jruge,              // Jump if C = 0
ST7_jrugt,              // Jump if (C + Z = 0)
ST7_jrule,              // Jump if (C + Z = 1)
ST7_ld,                 // Load
ST7_mul,                // Multiply
ST7_neg,                // Negate
ST7_nop,                // No Operation
ST7_or,                 // OR Operation
ST7_pop,                // Pop from the Stack
ST7_push,               // Push onto the Stack
ST7_rcf,                // Reset carry flag
ST7_ret,                // Subroutine Return
ST7_rim,                // Enable Interrupts
ST7_rlc,                // Rotate left true
ST7_rrc,                // Rotate right true
ST7_rsp,                // Reset Stack Pointer
ST7_sbc,                // Subtract with Carry
ST7_scf,                // Set carry flag
ST7_sim,                // Disable Interrupts
ST7_sla,                // Shift left Arithmetic
ST7_sll,                // Shift left Logic
ST7_srl,                // Shift right Logic
ST7_sra,                // Shift right Arithmetic
ST7_sub,                // Substraction
ST7_swap,               // SWAP nibbles
ST7_tnz,                // Test for Neg & Zero
ST7_trap,               // S/W trap
ST7_wfi,                // Wait for Interrupt
ST7_xor,                // Exclusive OR

ST7_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
IA64_null,       //   0
IA64_0,          //   1
IA64_1,          //   2
IA64_a,          //   3
IA64_acq,        //   4
IA64_add,        //   5 Add
IA64_addl,       //   6
IA64_addp4,      //   7 Add pointer
IA64_adds,       //   8
IA64_alloc,      //   9 Allocate stack frame
IA64_and,        //  10 Logical AND
IA64_andcm,      //  11 And complement
IA64_b,          //  12
IA64_bias,       //  13
IA64_br,         //  14 Branch
IA64_break,      //  15 Break
IA64_brl,        //  16 Branch long
IA64_brp,        //  17 Branch predict
IA64_bsw,        //  18 Bank switch
IA64_c,          //  19
IA64_call,       //  20
IA64_cexit,      //  21
IA64_chk,        //  22 Speculation check
IA64_cloop,      //  23
IA64_clr,        //  24
IA64_clrrrb,     //  25 Clear RRB
IA64_cmp,        //  26 Compare
IA64_cmp4,       //  27 Compare word
IA64_cmpxchg1,   //  28 Compare and exchange byte
IA64_cmpxchg2,   //  29 Compare and exchange halfword
IA64_cmpxchg4,   //  30 Compare and exchange word
IA64_cmpxchg8,   //  31 Compare and exchange double word
IA64_cond,       //  32
IA64_cover,      //  33 Cover stack frame
IA64_ctop,       //  34
IA64_czx1,       //  35 Compute zero index
IA64_czx2,       //  36 Compute zero index
IA64_d,          //  37
IA64_dep,        //  38 Deposit
IA64_dpnt,       //  39
IA64_dptk,       //  40
IA64_e,          //  41
IA64_epc,        //  42 Enter privileged code
IA64_eq,         //  43
IA64_excl,       //  44
IA64_exit,       //  45
IA64_exp,        //  46
IA64_extr,       //  47 Extract
IA64_f,          //  48
IA64_fabs,       //  49 Floating point absolute value
IA64_fadd,       //  50 Floating point add
IA64_famax,      //  51 Floating point absolute maximum
IA64_famin,      //  52 Floating point absolute minimum
IA64_fand,       //  53 Floating point logical and
IA64_fandcm,     //  54 Floating point and complement
IA64_fault,      //  55
IA64_fc,         //  56 Flush cache
IA64_fchkf,      //  57 Floating point check flags
IA64_fclass,     //  58 Floating point class
IA64_fclrf,      //  59 Floating point clear flags
IA64_fcmp,       //  60 Floating point compare
IA64_fcvt,       //  61 Convert floating point to/from integer
IA64_fetchadd4,  //  62 Fetch word and add immediate
IA64_fetchadd8,  //  63 Fetch double word and add immediate
IA64_few,        //  64
IA64_fill,       //  65
IA64_flushrs,    //  66 Flush register stack
IA64_fma,        //  67 Floating point multiply add
IA64_fmax,       //  68 Floating point maximum
IA64_fmerge,     //  69 Floating point merge
IA64_fmin,       //  70 Floating point minimum
IA64_fmix,       //  71 Floating point mix
IA64_fmpy,       //  72 Floating point multiply
IA64_fms,        //  73 Floating point multiply subtract
IA64_fneg,       //  74 Floating point negate
IA64_fnegabs,    //  75 Floating point negate absolute value
IA64_fnma,       //  76 Floating point negative multiply add
IA64_fnmpy,      //  77 Floating point negative multiply
IA64_fnorm,      //  78 Floating point normalize
IA64_for,        //  79 Floating point logical or
IA64_fpabs,      //  80 Floating point parallel absolute value
IA64_fpack,      //  81 Floating point pack
IA64_fpamax,     //  82 Floating point parallel absolute maximum
IA64_fpamin,     //  83 Floating point parallel absolute minimum
IA64_fpcmp,      //  84 Floating point parallel compare
IA64_fpcvt,      //  85 Floating point parallel conversion
IA64_fpma,       //  86 Floating point parallel multiply add
IA64_fpmax,      //  87 Floating point parallel maximum
IA64_fpmerge,    //  88 Floating point parallel merge
IA64_fpmin,      //  89 Floating point parallel minimum
IA64_fpmpy,      //  90 Floating point parallel multiply
IA64_fpms,       //  91 Floating point parallel multiply subtract
IA64_fpneg,      //  92 Floating point parallel negate
IA64_fpnegabs,   //  93 Floating point parallel negate absolute value
IA64_fpnma,      //  94 Floating point parallel negative multiply add
IA64_fpnmpy,     //  95 Floating point parallel negative multiply
IA64_fprcpa,     //  96 Floating point parallel reciprocal approximation
IA64_fprsqrta,   //  97 Floating point parallel reciprocal square root approximation
IA64_frcpa,      //  98 Floating point reciprocal approximation
IA64_frsqrta,    //  99 Floating point reciprocal square root approximation
IA64_fselect,    // 100 Floating point select
IA64_fsetc,      // 101 Floating point set controls
IA64_fsub,       // 102 Floating point subtract
IA64_fswap,      // 103 Floating point swap
IA64_fsxt,       // 104 Floating point sign extend
IA64_fwb,        // 105 Flush write buffers
IA64_fx,         // 106
IA64_fxor,       // 107 Floating point exclusive or
IA64_fxu,        // 108
IA64_g,          // 109
IA64_ga,         // 110
IA64_ge,         // 111
IA64_getf,       // 112 Get floating point value or exponent or significand
IA64_geu,        // 113
IA64_gt,         // 114
IA64_gtu,        // 115
IA64_h,          // 116
IA64_hu,         // 117
IA64_i,          // 118
IA64_ia,         // 119
IA64_imp,        // 120
IA64_invala,     // 121 Invalidate ALAT
IA64_itc,        // 122 Insert translation cache
IA64_itr,        // 123 Insert translation register
IA64_l,          // 124
IA64_ld1,        // 125 Load byte
IA64_ld2,        // 126 Load halfword
IA64_ld4,        // 127 Load word
IA64_ld8,        // 128 Load double word
IA64_ldf,        // 129 Floating point load
IA64_ldf8,       // 130 Floating point load
IA64_ldfd,       // 131 Floating point load
IA64_ldfe,       // 132 Floating point load
IA64_ldfp8,      // 133 Floating point load pair
IA64_ldfpd,      // 134 Floating point load pair
IA64_ldfps,      // 135 Floating point load pair
IA64_ldfs,       // 136 Floating point load
IA64_le,         // 137
IA64_leu,        // 138
IA64_lfetch,     // 139 Line prefetch
IA64_loadrs,     // 140 Load register stack
IA64_loop,       // 141
IA64_lr,         // 142
IA64_lt,         // 143
IA64_ltu,        // 144
IA64_lu,         // 145
IA64_m,          // 146
IA64_many,       // 147
IA64_mf,         // 148 Memory fence
IA64_mix1,       // 149 Mix bytes
IA64_mix2,       // 150 Mix halfwords
IA64_mix4,       // 151 Mix words
IA64_mov,        // 152 Move register
IA64_movl,       // 153 Move long immediate
IA64_mux1,       // 154 Mux byte
IA64_mux2,       // 155 Mux halfword
IA64_nc,         // 156
IA64_ne,         // 157
IA64_neq,        // 158
IA64_nge,        // 159
IA64_ngt,        // 160
IA64_nl,         // 161
IA64_nle,        // 162
IA64_nlt,        // 163
IA64_nm,         // 164
IA64_nop,        // 165 No operation
IA64_nr,         // 166
IA64_ns,         // 167
IA64_nt1,        // 168
IA64_nt2,        // 169
IA64_nta,        // 170
IA64_nz,         // 171
IA64_or,         // 172 Logical or
IA64_orcm,       // 173
IA64_ord,        // 174
IA64_pack2,      // 175 Pack
IA64_pack4,      // 176 Pack
IA64_padd1,      // 177 Parallel add bytes
IA64_padd2,      // 178 Parallel add halfwords
IA64_padd4,      // 179 Parallel add words
IA64_pavg1,      // 180 Parallel average bytes
IA64_pavg2,      // 181 Parallel average halfwords
IA64_pavgsub1,   // 182 Parallel average subtract bytes
IA64_pavgsub2,   // 183 Parallel average subtract halfwords
IA64_pcmp1,      // 184 Parallel compare bytes
IA64_pcmp2,      // 185 Parallel compare halfwords
IA64_pcmp4,      // 186 Parallel compare words
IA64_pmax1,      // 187 Parallel maxmimum bytes
IA64_pmax2,      // 188 Parallel maxmimum halfwords
IA64_pmin1,      // 189 Parallel minimum bytes
IA64_pmin2,      // 190 Parallel minimum halfwords
IA64_pmpy2,      // 191 Parallel multiply
IA64_pmpyshr2,   // 192 Parallel multiply and shift right
IA64_popcnt,     // 193 Population count
IA64_pr,         // 194
IA64_probe,      // 195 Probe address
IA64_psad1,      // 196 Parallel sum of absolute difference
IA64_pshl2,      // 197 Parallel shift left halfwords
IA64_pshl4,      // 198 Parallel shift left words
IA64_pshladd2,   // 199 Parallel shift left and Add
IA64_pshr2,      // 200 Parallel shift right halfwords
IA64_pshr4,      // 201 Parallel shift right words
IA64_pshradd2,   // 202 Parallel shift right and Add
IA64_psub1,      // 203 Parallel subtract bytes
IA64_psub2,      // 204 Parallel subtract halfwords
IA64_psub4,      // 205 Parallel subtract words
IA64_ptc,        // 206 Purge translation cache
IA64_ptr,        // 207 Purge translation register
IA64_r,          // 208
IA64_raz,        // 209
IA64_rel,        // 210
IA64_ret,        // 211
IA64_rfi,        // 212 Return from interruption
IA64_rsm,        // 213 Reset system mask
IA64_rum,        // 214 Reset user mask
IA64_rw,         // 215
IA64_s,          // 216
IA64_s0,         // 217
IA64_s1,         // 218
IA64_s2,         // 219
IA64_s3,         // 220
IA64_sa,         // 221
IA64_se,         // 222
IA64_setf,       // 223 Set floating point value, exponent, or significand
IA64_shl,        // 224 Shift left
IA64_shladd,     // 225 Shift left and Add
IA64_shladdp4,   // 226 Shift left and Add pointer
IA64_shr,        // 227 Shift right
IA64_shrp,       // 228 Shift right pair
IA64_sig,        // 229
IA64_spill,      // 230
IA64_spnt,       // 231
IA64_sptk,       // 232
IA64_srlz,       // 233 Serialize
IA64_ssm,        // 234 Set system mask
IA64_sss,        // 235
IA64_st1,        // 236 Store byte
IA64_st2,        // 237 Store halfword
IA64_st4,        // 238 Store word
IA64_st8,        // 239 Store double word
IA64_stf,        // 240 Store floating point
IA64_stf8,       // 241 Store floating point
IA64_stfd,       // 242 Store floating point
IA64_stfe,       // 243 Store floating point
IA64_stfs,       // 244 Store floating point
IA64_sub,        // 245 Subtract
IA64_sum,        // 246 Set user mask
IA64_sxt1,       // 247 Sign extend byte
IA64_sxt2,       // 248 Sign extend halfword
IA64_sxt4,       // 249 Sign extend word
IA64_sync,       // 250 Memory synchronization
IA64_tak,        // 251 Translation access key
IA64_tbit,       // 252 Test bit
IA64_thash,      // 253 Translation hashed entry address
IA64_tnat,       // 254 Test NaT
IA64_tpa,        // 255 Translate to physical address
IA64_trunc,      // 256
IA64_ttag,       // 257 Translation hashed entry tag
IA64_u,          // 258
IA64_unc,        // 259
IA64_unord,      // 260
IA64_unpack1,    // 261 Unpack byte
IA64_unpack2,    // 262 Unpack halfword
IA64_unpack4,    // 263 Unpack word
IA64_uss,        // 264
IA64_uus,        // 265
IA64_uuu,        // 266
IA64_w,          // 267
IA64_wexit,      // 268
IA64_wtop,       // 269
IA64_x,          // 270
IA64_xchg1,      // 271 Exchange bytes
IA64_xchg2,      // 272 Exchange halfwords
IA64_xchg4,      // 273 Exchange words
IA64_xchg8,      // 274 Exchange double words
IA64_xf,         // 275
IA64_xma,        // 276 Fixed point multiply add
IA64_xmpy,       // 277 Fixed point multiply
IA64_xor,        // 278 Exclusive or
IA64_xuf,        // 279
IA64_z,          // 280
IA64_zxt1,       // 281 Zero extend byte
IA64_zxt2,       // 282 Zero extend halfword
IA64_zxt4,       // 283 Zero extend word
IA64_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
NET_null = 0,           // Unknown Operation

NET_add,                // Add two values, returning a new value
NET_add_ovf,            // Add signed integer values with overflow check
NET_add_ovf_un,         // Add unsigned integer values with overflow check
NET_and,                // Bitwise AND of two integral values, returns an integral value
NET_ann_arg,            //
NET_ann_call,           // Start of simple calling sequence
NET_ann_catch,          // Start an exception filter or handler
NET_ann_data,           // Multi-byte no operation
NET_ann_data_s,         // Multi-byte no operation, short form
NET_ann_dead,           // Stack location is no longer live
NET_ann_def,            // SSA definition node
NET_ann_hoisted,        // Start of the simple portion of a hoisted calling sequence
NET_ann_hoisted_call,   // Start of complex argument evaluation
NET_ann_lab,            //
NET_ann_live,           // Mark a stack location as live
NET_ann_phi,            // SSA definition node
NET_ann_ref,            // SSA reference node
NET_ann_ref_s,          // SSA reference node, short form
NET_arglist,            // Return argument list handle for the current method
NET_beq,                // Branch to target if equal
NET_beq_s,              // Branch to target if equal, short form
NET_bge,                // Branch to target if greater than or equal to
NET_bge_s,              // Branch to target if greater than or equal to, short form
NET_bge_un,             // Branch to target if greater than or equal to (unsigned or unordered)
NET_bge_un_s,           // Branch to target if greater than or equal to (unsigned or unordered), short form
NET_bgt,                // Branch to target if greater than
NET_bgt_s,              // Branch to target if greater than, short form
NET_bgt_un,             // Branch to target if greater than (unsigned or unordered)
NET_bgt_un_s,           // Branch to target if greater than (unsigned or unordered), short form
NET_ble,                // Branch to target if less than or equal to
NET_ble_s,              // Branch to target if less than or equal to, short form
NET_ble_un,             // Branch to target if less than or equal to (unsigned or unordered)
NET_ble_un_s,           // Branch to target if less than or equal to (unsigned or unordered), short form
NET_blt,                // Branch to target if less than
NET_blt_s,              // Branch to target if less than, short form
NET_blt_un,             // Branch to target if less than (unsigned or unordered)
NET_blt_un_s,           // Branch to target if less than (unsigned or unordered), short form
NET_bne_un,             // Branch to target if unequal or unordered
NET_bne_un_s,           // Branch to target if unequal or unordered, short form
NET_box,                // Convert value type to object reference
NET_br,                 // Branch to target
NET_br_s,               // Branch to target, short form
NET_break,              // Inform a debugger that a breakpoint has been reached
NET_brfalse,            // Branch to target if value is zero (false)
NET_brfalse_s,          // Branch to target if value is zero (false), short form
NET_brtrue,             // Branch to target if value is non-zero (true)
NET_brtrue_s,           // Branch to target if value is non-zero (true), short form
NET_call,               // Call a method
NET_calli,              // Call an indirect method
NET_callvirt,           // Call a method associated with obj
NET_castclass,          // Cast object to class
NET_ceq,                // Push 1 (of type I4) if value1 equals value2, else 0
NET_cgt,                // Push 1 (of type I4) if value1 > value2, else 0
NET_cgt_un,             // Push 1 (of type I4) if value1 > value2, unsigned or unordered, else 0
NET_ckfinite,           // Throw ArithmeticException if value is not a finite number
NET_clt,                // Push 1 (of type I4) if value1 < value2, else 0
NET_clt_un,             // Push 1 (of type I4) if value1 < value2, unsigned or unordered, else 0
NET_conv_i,             // Convert to I, pushing I on stack
NET_conv_i1,            // Convert to I1, pushing I4 on stack
NET_conv_i2,            // Convert to I2, pushing I4 on stack
NET_conv_i4,            // Convert to I4, pushing I4 on stack
NET_conv_i8,            // Convert to I8, pushing I8 on stack
NET_conv_ovf_i,         // Convert to an I (on the stack as I) and throw an exception on overflow
NET_conv_ovf_i1,        // Convert to an I1 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_i1_un,     // Convert unsigned to an I1 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_i2,        // Convert to an I2 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_i2_un,     // Convert unsigned to an I2 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_i4,        // Convert to an I4 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_i4_un,     // Convert unsigned to an I4 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_i8,        // Convert to an I8 (on the stack as I8) and throw an exception on overflow
NET_conv_ovf_i8_un,     // Convert unsigned to an I8 (on the stack as I8) and throw an exception on overflow
NET_conv_ovf_i_un,      // Convert to unsigned an I (on the stack as I) and throw an exception on overflow
NET_conv_ovf_u,         // Convert to a U (on the stack as I) and throw an exception on overflow
NET_conv_ovf_u1,        // Convert to a U1 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_u1_un,     // Convert to unsigned a U1 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_u2,        // Convert to a U2 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_u2_un,     // Convert to unsigned a U2 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_u4,        // Convert to a U4 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_u4_un,     // Convert to unsigned a U3 (on the stack as I4) and throw an exception on overflow
NET_conv_ovf_u8,        // Convert to a U8 (on the stack as I8) and throw an exception on overflow
NET_conv_ovf_u8_un,     // Convert to unsigned a U8 (on the stack as I8) and throw an exception on overflow
NET_conv_ovf_u_un,      // Convert to unsigned a U (on the stack as I) and throw an exception on overflow
NET_conv_r4,            // Convert to R4, pushing F on stack
NET_conv_r8,            // Convert to R8, pushing F on stack
NET_conv_r_un,          // Convert unsigned integer to floating point, pushing F on stack
NET_conv_u,             // Convert to U, pushing I on stack
NET_conv_u1,            // Convert to U1, pushing I4 on stack
NET_conv_u2,            // Convert to U2, pushing I4 on stack
NET_conv_u4,            // Convert to U4, pushing I4 on stack
NET_conv_u8,            // Convert to U8, pushing I8 on stack
NET_cpblk,              // Copy data from memory to memory
NET_cpobj,              // Copy a value type
NET_div,                // Divide two values to return a quotient or floating point result
NET_div_un,             // Divide two values, unsigned, returning a quotient
NET_dup,                // Duplicate value on the top of the stack
NET_endfilter,          // End filter clause of SEH exception handling
NET_endfinally,         // End fault clause of an exception block
NET_initblk,            // Set a block of memory to a given byte
NET_initobj,            // Initialize a value type
NET_isinst,             // Test if an object is an instance of a class or interface, returning NULL or an instance of that class or interface
NET_jmp,                // Exit current method and jump to specified method
NET_ldarg,              // Load argument numbered num onto stack
NET_ldarg_0,            // Load argument 0 onto stack
NET_ldarg_1,            // Load argument 1 onto stack
NET_ldarg_2,            // Load argument 2 onto stack
NET_ldarg_3,            // Load argument 3 onto stack
NET_ldarg_s,            // Load argument numbered num onto stack, short form
NET_ldarga,             // Fetch the address of argument
NET_ldarga_s,           // Fetch the address of argument, short form
NET_ldc_i4,             // Push num of type I4 onto the stack as I4
NET_ldc_i4_0,           // Push 0 onto the stack as I4
NET_ldc_i4_1,           // Push 1 onto the stack as I4
NET_ldc_i4_2,           // Push 2 onto the stack as I4
NET_ldc_i4_3,           // Push 3 onto the stack as I4
NET_ldc_i4_4,           // Push 4 onto the stack as I4
NET_ldc_i4_5,           // Push 5 onto the stack as I4
NET_ldc_i4_6,           // Push 6 onto the stack as I4
NET_ldc_i4_7,           // Push 7 onto the stack as I4
NET_ldc_i4_8,           // Push 8 onto the stack as I4
NET_ldc_i4_m1,          // Push -1 onto the stack as I4
NET_ldc_i4_s,           // Push num onto the stack as I4, short form
NET_ldc_i8,             // Push num of type I8 onto the stack as I8
NET_ldc_r4,             // Push num of type R4 onto the stack as F
NET_ldc_r8,             // Push num of type R8 onto the stack as F
NET_ldelem_i,           // Load the element at index with type I onto the top of the stack as an I
NET_ldelem_i1,          // Load the element at index with type I1 onto the top of the stack as an I4
NET_ldelem_i2,          // Load the element at index with type I2 onto the top of the stack as an I4
NET_ldelem_i4,          // Load the element at index with type I4 onto the top of the stack as an I4
NET_ldelem_i8,          // Load the element at index with type I8 onto the top of the stack as an I8
NET_ldelem_r4,          // Load the element at index with type R4 onto the top of the stack as an F
NET_ldelem_r8,          // Load the element at index with type R8 onto the top of the stack as an F
NET_ldelem_ref,         // Load the element at index, an object, onto the top of the stack as an O
NET_ldelem_u1,          // Load the element at index with type U1 onto the top of the stack as an I4
NET_ldelem_u2,          // Load the element at index with type U2 onto the top of the stack as an I4
NET_ldelem_u4,          // Load the element at index with type U4 onto the top of the stack as an I4
NET_ldelema,            // Load the address of element at index onto the top of the stack
NET_ldfld,              // Push the value of field of object on the stack
NET_ldflda,             // Push the address of field of object on the stack
NET_ldftn,              // Push a pointer to a method referenced by method on the stack
NET_ldind_i,            // Indirect load value of type I as I stack
NET_ldind_i1,           // Indirect load value of type I1 as I4 on stack
NET_ldind_i2,           // Indirect load value of type I2 as I4 on stack
NET_ldind_i4,           // Indirect load value of type I4 as I4 on stack
NET_ldind_i8,           // Indirect load value of type I8 as I8 on stack
NET_ldind_r4,           // Indirect load value of type R4 as F stack
NET_ldind_r8,           // Indirect load value of type R8 as F stack
NET_ldind_ref,          // Indirect load value of type object ref as O on stack
NET_ldind_u1,           // Indirect load value of type U1 as I4 stack
NET_ldind_u2,           // Indirect load value of type U2 as I4 stack
NET_ldind_u4,           // Indirect load value of type U4 as I4 stack
NET_ldlen,              // Push the length (of type U) of array on the stack
NET_ldloc,              // Load local variable of index indx onto stack
NET_ldloc_0,            // Load local variable 0 onto stack
NET_ldloc_1,            // Load local variable 1 onto stack
NET_ldloc_2,            // Load local variable 2 onto stack
NET_ldloc_3,            // Load local variable 3 onto stack
NET_ldloc_s,            // Load local variable of index indx onto stack, short form
NET_ldloca,             // Load address of local variable
NET_ldloca_s,           // Load address of local variable, short form
NET_ldnull,             // Push null GC reference on the stack
NET_ldobj,              // Copy instance of value type to the stack.
NET_ldsfld,             // Push the value of field on the stack
NET_ldsflda,            // Push the address of the static field on the stack
NET_ldstr,              // Push a string object for the literal string
NET_ldtoken,            // Convert metadata token to its runtime representation
NET_ldvirtftn,          // Push address of virtual method on the stack
NET_leave,              // Exit a protected region of code
NET_leave_s,            // Exit a protected region of code, short form
NET_localloc,           // Allocate space from the local memory pool
NET_mkrefany,           // Push a typed reference to pointer of type class onto the stack
NET_mul,                // Multiply values
NET_mul_ovf,            // Multiply signed integer values. Signed result must fit in same size
NET_mul_ovf_un,         // Multiply unsigned integer values. Unsigned result must fit in same size
NET_neg,                // Negate value
NET_newarr,             // Create a zero-based, one-dimensional array
NET_newobj,             // Create a new object
NET_nop,                // Do nothing
NET_not,                // Bitwise complement
NET_or,                 // Bitwise OR of two integer values, returns an integer
NET_pop,                // Pop a value from the stack
NET_refanytype,         // Push the type token stored in a typed reference
NET_refanyval,          // Push the address stored in a typed reference
NET_rem,                // Remainder of dividing value1 by value2
NET_rem_un,             // Remainder of unsigned dividing value1 by value2
NET_ret,                // Return from method, possibly returning a value
NET_rethrow,            // Rethrow the current exception
NET_shl,                // Shift an integer to the left (shifting in zeros)
NET_shr,                // Shift an integer right, (shift in sign), return an integer
NET_shr_un,             // Shift an integer right, (shift in zero), return an integer
NET_sizeof,             // Push the size, in bytes, of a value type as a U4
NET_starg,              // Store a value to argument
NET_starg_s,            // Store a value to argument, short form
NET_stelem_i,           // Replace array element at index with the I value on the stack
NET_stelem_i1,          // Replace array element at index with the I1 value on the stack
NET_stelem_i2,          // Replace array element at index with the I2 value on the stack
NET_stelem_i4,          // Replace array element at index with the I4 value on the stack
NET_stelem_i8,          // Replace array element at index with the I8 value on the stack
NET_stelem_r4,          // Replace array element at index with the R4 value on the stack
NET_stelem_r8,          // Replace array element at index with the R8 value on the stack
NET_stelem_ref,         // Replace array element at index with the ref value on the stack
NET_stfld,              // Store into a field of an object
NET_stind_i,            // Store value of type I into memory at address
NET_stind_i1,           // Store value of type I1 into memory at address
NET_stind_i2,           // Store value of type I2 into memory at address
NET_stind_i4,           // Store value of type I4 into memory at address
NET_stind_i8,           // Store value of type I8 into memory at address
NET_stind_r4,           // Store value of type R4 into memory at address
NET_stind_r8,           // Store value of type R8 into memory at address
NET_stind_ref,          // Store value of type object ref (type O) into memory at address
NET_stloc,              // Pop value from stack into local variable
NET_stloc_0,            // Pop value from stack into local variable 0
NET_stloc_1,            // Pop value from stack into local variable 1
NET_stloc_2,            // Pop value from stack into local variable 2
NET_stloc_3,            // Pop value from stack into local variable 3
NET_stloc_s,            // Pop value from stack into local variable, short form
NET_stobj,              // Store a value type from the stack into memory
NET_stsfld,             // Store a static field of a class
NET_sub,                // Subtract value2 from value1, returning a new value
NET_sub_ovf,            // Subtract I from an I. Signed result must fit in same size
NET_sub_ovf_un,         // Subtract U from a U. Unsigned result must fit in same size
NET_switch,             // Jump to one of N values
NET_tail_,              // (prefix) subsequent call terminates current method
NET_throw,              // Throw an exception
NET_unaligned_,         // (prefix) subsequent pointer instruction may be unaligned
NET_unbox,              // Convert boxed value type to its raw form
NET_volatile_,          // (prefix) subsequent pointer reference is volatile
NET_xor,                // Bitwise XOR of integer values, returns an integer
NET_ldelem,             // Load the element at index onto the top of the stack
NET_stelem,             // Replace array element at index with the ref value on the stack

// 2.0 instructions
NET_unbox_any,          // Extract a value-type from obj, its boxed representation
NET_constrained_,       // (prefix) invoke a member on a value of a variable type
NET_no_,                // (prefix) possibly skip a fault check
NET_readonly_,          // (prefix) following instruction returns a controlled-mutability managed pointer

NET_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      HCS12X opcodes added by alexbratovic@bratspeed.com 12/12/2007
 *      XGATE opcodes added by alexbratovic@bratspeed.com 12/09/2009
 */


extern instruc_t Instructions[];


enum 
{
MC12_null = 0,          // Unknown Operation

MC12_aba,               // Add B to A
MC12_abx,               // Add B to X
MC12_aby,               // Add B to Y
MC12_adca,              // Add with carry to A
MC12_adcb,              // Add with carry to B
MC12_adda,              // Add memory to A
MC12_addb,              // Add memory to B
MC12_addd,              // Add to D
MC12_anda,              // AND A with memory
MC12_andb,              // AND B with memory
MC12_andcc,             // Logical AND CCR with memory
MC12_asl,               // Arithmetic shift left memory
MC12_asla,              // Arithmetic shift left A
MC12_aslb,              // Arithmetic shift left B
MC12_asld,              // Arithmetic shift left D
MC12_asr,               // Arithmetic shift right memory
MC12_asra,              // Arithmetic shift right A
MC12_asrb,              // Arithmetic shift right B
MC12_bcc,               // Branch if carry clear
MC12_bclr,              // Clear bits in memory
MC12_bcs,               // Branch if carry set
MC12_beq,               // Branch if equal
MC12_bge,               // Branch if greater than or equal
MC12_bgnd,              // Enter background debug mode
MC12_bgt,               // Branch if greater than
MC12_bhi,               // Branch if higher
MC12_bhs,               // Branch if higher or same
MC12_bita,              // Bit test A
MC12_bitb,              // Bit test B
MC12_ble,               // Branch if less than or equal
MC12_blo,               // Branch if lower
MC12_bls,               // Branch if lower or same
MC12_blt,               // Branch if less than
MC12_bmi,               // Branch if minus
MC12_bne,               // Branch if not equal
MC12_bpl,               // Branch if plus
MC12_bra,               // Branch always
MC12_brclr,             // Branch if selected bits clear
MC12_brn,               // Branch never
MC12_brset,             // Branch if selected bits set
MC12_bset,              // Set bits in memory
MC12_bsr,               // Branch to subroutine
MC12_bvc,               // Branch if overflow clear
MC12_bvs,               // Branch if overflow set
MC12_call,              // Call subroutine in expanded memory
MC12_cba,               // Compare A to B
MC12_clc,               // Clear C bit
MC12_cli,               // Clear I bit
MC12_clr,               // Clear memory
MC12_clra,              // Clear A
MC12_clrb,              // Clear B
MC12_clv,               // Clear V bit
MC12_cmpa,              // Compare A to memory
MC12_cmpb,              // Compare B to memory
MC12_com,               // One's complement memory
MC12_coma,              // One's complement A
MC12_comb,              // One's complement B
MC12_cpd,               // Compare D to memory (16-bit)
MC12_cps,               // Compare SP to memory (16-bit)
MC12_cpx,               // Compare X to memory (16-bit)
MC12_cpy,               // Compare Y to memory (16-bit)
MC12_daa,               // Decimal adjust A
MC12_dbeq,              // Decrement counter and branch if = 0
MC12_dbne,              // Decrement counter and branch if != 0
MC12_dec,               // Decrement memory
MC12_deca,              // Decrement A
MC12_decb,              // Decrement B
MC12_des,               // Decrement SP
MC12_dex,               // Decrement X
MC12_dey,               // Decrement Y
MC12_ediv,              // 32 by 16 divide (unsigned) Quotient->Y Remainder->D
MC12_edivs,             // 32 by 16 divide (signed)  Quotient->Y Remainder->D
MC12_emacs,             // Multiply and accumulate (signed)
MC12_emaxd,             // MAX of two unsigned 16-bit values result to accumulator
MC12_emaxm,             // MAX of two unsigned 16-bit values result to memory
MC12_emind,             // MIN of two unsigned 16-bit values result to accumulator
MC12_eminm,             // MIN of two unsigned 16-bit values result to memory
MC12_emul,              // 16 by 16 multiply (unsigned)
MC12_emuls,             // 16 by 16 multiply (signed)
MC12_eora,              // Exclusive OR A with memory
MC12_eorb,              // Exclusive OR B with memory
MC12_etbl,              // 16-bit table lookup and interpolate
MC12_exg,               // Exchange register to register
MC12_fdiv,              // 16 by 16 fractional divide Remainder->D
MC12_ibeq,              // Increment counter and branch if = 0
MC12_ibne,              // Increment counter and branch if != 0
MC12_idiv,              // 16 by 16 integer divide (unsigned) Remainder->D
MC12_idivs,             // 16 by 16 integer divide (signed) Remainder->D
MC12_inc,               // Increment memory
MC12_inca,              // Increment A
MC12_incb,              // Increment B
MC12_ins,               // Increment SP
MC12_inx,               // Increment X
MC12_iny,               // Increment Y
MC12_jmp,               // Jump Address
MC12_jsr,               // Jump to subroutine
MC12_lbcc,              // Long branch if carry clear
MC12_lbcs,              // Long branch if carry set
MC12_lbeq,              // Long branch if equal
MC12_lbge,              // Long branch if greater than or equal
MC12_lbgt,              // Long branch if greater than
MC12_lbhi,              // Long branch if higher
MC12_lbhs,              // Long branch if higher or same
MC12_lble,              // Long branch if less than or equal
MC12_lblo,              // Long branch if lower
MC12_lbls,              // Long branch if lower or same
MC12_lblt,              // Long branch if less than
MC12_lbmi,              // Long branch if minus
MC12_lbne,              // Long branch if not equal
MC12_lbpl,              // Long branch if plus
MC12_lbra,              // Long branch always
MC12_lbrn,              // Long branch never
MC12_lbvc,              // Long branch if overflow clear
MC12_lbvs,              // Long branch if overflow set
MC12_ldaa,              // Load A
MC12_ldab,              // Load B
MC12_ldd,               // Load D
MC12_lds,               // Load SP
MC12_ldx,               // Load X
MC12_ldy,               // Load Y
MC12_leas,              // Load effective address into SP
MC12_leax,              // Load effective address into X
MC12_leay,              // Load effective address into Y
MC12_lsl,               // Logic shift left memory
MC12_lsla,              // Logic shift left A
MC12_lslb,              // Logic shift left B
MC12_lsld,              // Logic shift left D
MC12_lsr,               // Logic shift right memory
MC12_lsra,              // Logic shift right A
MC12_lsrb,              // Logic shift right B
MC12_lsrd,              // Logic shift right D
MC12_maxa,              // MAX of two unsigned 8-bit values result to accumulator
MC12_maxm,              // MAX of two unsigned 8-bit values result to memory
MC12_mem,               // Membership function
MC12_mina,              // MIN of two unsigned 8-bit values result to accumulator
MC12_minm,              // MIN of two unsigned 8-bit values result to memory
MC12_movb,              // Move byte (8-bit)
MC12_movw,              // Move word (16-bit)
MC12_mul,               // 8 by 8 multiply (unsigned)
MC12_neg,               // Two's complement memory
MC12_nega,              // Two's complement A
MC12_negb,              // Two's complement B
MC12_nop,               // Null operation
MC12_oraa,              // OR A with memory
MC12_orab,              // OR B with memory
MC12_orcc,              // Logical OR CCR with memory
MC12_psha,              // Push A
MC12_pshb,              // Push B
MC12_pshc,              // Push CCR
MC12_pshd,              // Push D
MC12_pshx,              // Push X
MC12_pshy,              // Push Y
MC12_pula,              // Pull A
MC12_pulb,              // Pull B
MC12_pulc,              // Pull CCR
MC12_puld,              // Pull D
MC12_pulx,              // Pull X
MC12_puly,              // Pull Y
MC12_rev,               // MIN-MAX rule evaluation
MC12_revw,              // MIN-MAX rule evaluation
MC12_rol,               // Rotate left memory through carry
MC12_rola,              // Rotate left A through carry
MC12_rolb,              // Rotate left B through carry
MC12_ror,               // Rotate right memory through carry
MC12_rora,              // Rotate right A through carry
MC12_rorb,              // Rotate right B through carry
MC12_rtc,               // Return from call
MC12_rti,               // Return from interrupt
MC12_rts,               // Return from subroutine
MC12_sba,               // Subtract B from A
MC12_sbca,              // Subtract with borrow from A
MC12_sbcb,              // Subtract with borrow from B
MC12_sec,               // Set C bit
MC12_sei,               // Set I bit
MC12_sev,               // Set V bit
MC12_sex,               // Sign extend 8-Bit operand
MC12_staa,              // Store A
MC12_stab,              // Store B
MC12_std,               // Store D
MC12_stop,              // Stop
MC12_sts,               // Store SP
MC12_stx,               // Store X
MC12_sty,               // Store Y
MC12_suba,              // Subtract memory from A
MC12_subb,              // Subtract memory from B
MC12_subd,              // Subtract memory from D
MC12_swi,               // Software interrupt
MC12_tab,               // Transfer A to B
MC12_tap,               // Transfer A to CCR
MC12_tba,               // Transfer B to A
MC12_tbeq,              // Test counter and branch if = 0
MC12_tbl,               // 8-bit table lookup and interpolate
MC12_tbne,              // Test counter and branch if != 0
MC12_tfr,               // Transfer register to register
MC12_tpa,               // Transfer CCR to A
MC12_trap,              // Unimplemented opcode interrupt
MC12_tst,               // Test memory for zero or minus
MC12_tsta,              // Test A for zero or minus
MC12_tstb,              // Test B for zero or minus
MC12_tsx,               // Transfer SP to X
MC12_tsy,               // Transfer SP to Y
MC12_txs,               // Transfer X to SP
MC12_tys,               // Transfer Y to SP
MC12_wai,               // Wait for interrupt
MC12_wav,               // Calculates numerator and denominator for weighted average calculation
MC12_wavr,              // Resumes execution of interrupted WAV instruction
MC12_xgdx,              // Exchange D with X
MC12_xgdy,              // Exchange D with Y

// Pseudoinstructions
MC12_skip1,             // Skip one byte
MC12_skip2,             // Skip two bytes

//hcs12x only instructions, 16-bit unless indicated
MC12X_addx,             // Add without Carry to X
MC12X_addy,             // Add without Carry to Y
MC12X_aded,             // Add with Carry to D(A:B)
MC12X_adex,             // Add with Carry to X
MC12X_adey,             // Add with Carry to Y
MC12X_andx,             // Logical AND X with Memory
MC12X_andy,             // Logical AND Y with Memory
MC12X_aslw,             // Arithmetic Shift Left
MC12X_aslx,             // Arithmetic Shift Left X
MC12X_asly,             // Arithmetic Shift Left Y
MC12X_asrw,             // Arithmetic Shift Right
MC12X_asrx,             // Arithmetic Shift Right X
MC12X_asry,             // Arithmetic Shift Right Y
MC12X_bitx,             // Logical AND X with Memory, does not change X or Memory
MC12X_bity,             // Logical AND Y with Memory, does not change Y or Memory
MC12X_btas,             // Bit(s) test and Set in Memory, Set CCR flags
MC12X_clrw,             // Clear Memory Location
MC12X_clrx,             // Clear X
MC12X_clry,             // Clear Y
MC12X_comw,             // One's Complement Memory
MC12X_comx,             // One's Complement X
MC12X_comy,             // One's Complement Y
MC12X_cped,             // Compare D to Memory with Borrow
MC12X_cpes,             // Compare SP to Memory with Borrow
MC12X_cpex,             // Compare X to Memory with Borrow
MC12X_cpey,             // Compare Y to Memory with Borrow
MC12X_decw,             // Decrement Memory
MC12X_decx,             // Decrement X
MC12X_decy,             // Decrement Y
MC12X_eorx,             // Exclusive OR X with Memory
MC12X_eory,             // Exclusive OR Y with Memory
MC12X_gldaa,            // Load A from Global Memory (8-bit)
MC12X_gldab,            // Load B from Global Memory (8-bit)
MC12X_gldd,             // Load D(A:B) from Global Memory
MC12X_glds,             // Load SP from Global Memory
MC12X_gldx,             // Load X from Global Memory
MC12X_gldy,             // Load Y from Global Memory
MC12X_gstaa,            // Store A to Global Memory (8-bit)
MC12X_gstab,            // Store B to Global Memory (8-bit)
MC12X_gstd,             // Store D(A:B) to Global Memory
MC12X_gsts,             // Store SP to Global Memory
MC12X_gstx,             // Store X to Global Memory
MC12X_gsty,             // Store Y to Global Memory
MC12X_incw,             // Increment Memory
MC12X_incx,             // Increment X
MC12X_incy,             // Increment Y
MC12X_lsrw,             // Logical Shift Memory to Right
MC12X_lsrx,             // Logical Shift X to Right
MC12X_lsry,             // Logical Shift Y to Right
MC12X_negw,             // Two's Complement Memory
MC12X_negx,             // Two's Complement X
MC12X_negy,             // Two's Complement Y
MC12X_orx,              // Logical OR X with memory
MC12X_ory,              // Logical OR Y with memory
MC12X_pshcw,            // Push CCR onto Stack (16-bit)
MC12X_pulcw,            // Pull CCR from Stack (16-bit)
MC12X_rolw,             // Rotate Memory Left through Carry
MC12X_rolx,             // Rotate X Left through Carry
MC12X_roly,             // Rotate Y Left through Carry
MC12X_rorw,             // Rotate Memory Right through Carry
MC12X_rorx,             // Rotate X Right through Carry
MC12X_rory,             // Rotate Y Right through Carry
MC12X_sbed,             // Subtract with Borrow from D
MC12X_sbex,             // Subtract with Borrow from X
MC12X_sbey,             // Subtract with Borrow from Y
MC12X_subx,             // Subtract Memory from X
MC12X_suby,             // Subtract Memory from Y
MC12X_tstw,             // Test Memory for Zero or Minus
MC12X_tstx,             // Test X for Zero or Minus
MC12X_tsty,             // Test Y for Zero or Minus

//hcs12x V2 cpu only instructions
MC12X_sys,              // System call interrupt

//hcs12x XGATE co-processor instructions
MC12XGATE_adc,          // Add with Carry
MC12XGATE_add,          // Add without Carry
MC12XGATE_addh,         // Add Immediate 8 bit Constant (High Byte)
MC12XGATE_addl,         // Add Immediate 8 bit Constant (Low Byte)
MC12XGATE_and,          // Logical AND
MC12XGATE_andh,         // Logical AND Immediate 8 bit Constant (High Byte)
MC12XGATE_andl,         // Logical AND Immediate 8 bit Constant (Low Byte)
MC12XGATE_asr,          // Arithmetic Shift Right
MC12XGATE_bcc,          // Branch if Carry Cleared (Same as BHS)
MC12XGATE_bcs,          // Branch if Carry Set (Same as BLO)
MC12XGATE_beq,          // Branch if Equal
MC12XGATE_bfext,        // Bit Field Extract
MC12XGATE_bffo,         // Bit Field Find First One
MC12XGATE_bfins,        // Bit Field Insert
MC12XGATE_bfinsi,       // Bit Field Insert and Invert
MC12XGATE_bfinsx,       // Bit Field Insert and XNOR
MC12XGATE_bge,          // Branch if Greater than or Equal to Zero
MC12XGATE_bgt,          // Branch if Greater than Zero
MC12XGATE_bhi,          // Branch if Higher
MC12XGATE_bhs,          // Branch if Higher or Same (Same as BCC)
MC12XGATE_bith,         // Bit Test Immediate 8 bit Constant (High Byte)
MC12XGATE_bitl,         // Bit Test Immediate 8 bit Constant (Low Byte)
MC12XGATE_ble,          // Branch if Less or Equal to Zero
MC12XGATE_blo,          // Branch if Carry Set (Same as BCS)
MC12XGATE_bls,          // Branch if Lower or Same
MC12XGATE_blt,          // Branch if Lower than Zero
MC12XGATE_bmi,          // Branch if Minus
MC12XGATE_bne,          // Branch if Not Equal
MC12XGATE_bpl,          // Branch if Plus
MC12XGATE_bra,          // Branch Always 
MC12XGATE_brk,          // Break
MC12XGATE_bvc,          // Branch if Overflow Cleared
MC12XGATE_bvs,          // Branch if Overflow Set
MC12XGATE_cmp,          // Compare
MC12XGATE_cmpl,         // Compare Immediate 8 bit Constant (Low Byte)
MC12XGATE_com,          // One's Complement
MC12XGATE_cpc,          // Compare with Carry
MC12XGATE_cpch,         // Compare Immediate 8 bit Constant with Carry(High Byte)
MC12XGATE_csem,         // Clear Semaphore
MC12XGATE_csl,          // Logical Shift Left with Carry
MC12XGATE_csr,          // Logical Shift Right with Carry
MC12XGATE_jal,          // Jump and Link
MC12XGATE_ldb,          // Load Byte from Memory (Low Byte)
MC12XGATE_ldh,          // Load Immediate 8 bit Constant (High Byte)
MC12XGATE_ldl,          // Load Immediate 8 bit Constant (Low Byte)
MC12XGATE_ldw,          // Load Word from Memory
MC12XGATE_lsl,          // Logical Shift Left
MC12XGATE_lsr,          // Logical Shift Right
MC12XGATE_mov,          // Move Register Content
MC12XGATE_neg,          // Two's Complement
MC12XGATE_nop,          // No Operation
MC12XGATE_or,           // Logical OR
MC12XGATE_orh,          // Logical OR Immediate 8 bit Constant (High Byte)
MC12XGATE_orl,          // Logical OR Immediate 8 bit Constant (Low Byte)
MC12XGATE_par,          // Calculate Parity
MC12XGATE_rol,          // Rotate Left
MC12XGATE_ror,          // Rotate Right
MC12XGATE_rts,          // Return to Scheduler
MC12XGATE_sbc,          // Subtract with Carry
MC12XGATE_sex,          // Sign Extend Byte to Word
MC12XGATE_sif,          // Set Interrupt Flag
MC12XGATE_ssem,         // Set Semaphore
MC12XGATE_stb,          // Store Byte to Memory (Low Byte)
MC12XGATE_stw,          // Store Word to Memory
MC12XGATE_sub,          // Subtract without Carry
MC12XGATE_subh,         // Subtract Immediate 8 bit Constant (High Byte)
MC12XGATE_subl,         // Subtract Immediate 8 bit Constant (Low Byte)
MC12XGATE_tfr,          // Transfer from and to Special Registers
MC12XGATE_tst,          // Test Register
MC12XGATE_xnor,         // Logical Exclusive NOR
MC12XGATE_xnorh,        // Logical Exclusive NOR Immediate 8 bit Constant (High Byte)
MC12XGATE_xnorl,        // Logical Exclusive NOR Immediate 8 bit Constant (Low Byte)

//Pseudo instructions that combine two IMM8 into IMM16
MC12XGATE_add16,         // Add Immediate 16 bit Constant
MC12XGATE_and16,         // Logical AND Immediate 16 bit Constant
MC12XGATE_cmp16,         // Compare Immediate 16 bit Constant
MC12XGATE_ldw16,         // Load Immediate 16 bit Constant
MC12XGATE_or16,          // Logical OR Immediate 16 bit Constant
MC12XGATE_sub16,         // Subtract Immediate 16 bit Constant
MC12XGATE_xnor16,        // Logical Exclusive NOR Immediate 16 bit Constant

MC12_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

  MC6816_null = 0, // Unknown Operation

  // DATA MOVEMENT INSTRUCTIONS
  // --------------------------

  // LOAD INSTRUCTIONS
  MC6816_ldaa,   // Load A
  MC6816_ldab,   // Load B
  MC6816_ldd,    // Load D
  MC6816_lde,    // Load E
  MC6816_lded,   // Load Concatenated D and E

  // MOVE INSTRUCTIONS
  MC6816_movb,   // Move Byte
  MC6816_movw,   // Move Word

  // STORE INSTRUCTIONS
  MC6816_staa,   // Store A
  MC6816_stab,   // Store B
  MC6816_std,    // Store D
  MC6816_ste,    // Store E
  MC6816_sted,   // Store Concatenated D and E

  // TRANSFER INSTRUCTIONS
  MC6816_tab,    // Transfer A to B
  MC6816_tba,    // Transfer B to A
  MC6816_tde,    // Transfer D to E
  MC6816_ted,    // Transfer E to D

  // EXCHANGE INSTRUCTIONS
  MC6816_xgab,   // Exchange A with B
  MC6816_xgde,   // Exchange D with E

  // MATHEMATIC INSTRUCTIONS
  // -----------------------

  // ADDITION AND SUBSTRACTION INSTRUCTIONS
  MC6816_aba,    // Add B to A
  MC6816_adca,   // Add with Carry to A
  MC6816_adcb,   // Add with Carry to B
  MC6816_adcd,   // Add with Carry to D
  MC6816_adce,   // Add with Carry to E
  MC6816_adda,   // Add to A
  MC6816_addb,   // Add to B
  MC6816_addd,   // Add to D
  MC6816_adde,   // Add to E
  MC6816_ade,    // Add D to E
  MC6816_sba,    // Subtract B from A
  MC6816_sbca,   // Subtract with Carry from A
  MC6816_sbcb,   // Subtract with Carry from B
  MC6816_sbcd,   // Subtract with Carry from D
  MC6816_sbce,   // Subtract with Carry from E
  MC6816_sde,    // Subtract D from E
  MC6816_suba,   // Subtract from A
  MC6816_subb,   // Subtract from B
  MC6816_subd,   // Subtract from D
  MC6816_sube,   // Subtract from E

  // BINARY CODED DECIMAL INSTRUCTIONS
  MC6816_daa,    // Decimal Adjust A
  MC6816_sxt,    // Sign Extend B into A

  // COMPARE AND TEST INSTRUCTIONS
  MC6816_cba,    // Compare A to B
  MC6816_cmpa,   // Compare A to Memory
  MC6816_cmpb,   // Compare B to Memory
  MC6816_cpd,    // Compare D to Memory
  MC6816_cpe,    // Compare E to Memory
  MC6816_tst,    // Test for Zero or Minus
  MC6816_tsta,   // Test A for Zero or Minus
  MC6816_tstb,   // Test B for Zero or Minus
  MC6816_tstd,   // Test D for Zero or Minus
  MC6816_tste,   // Test E for Zero or Minus
  MC6816_tstw,   // Test for Zero or Minus Word

  // MULTIPLICATION AND DIVISION INSTRUCTIONS
  MC6816_ediv,   // Extended Unsigned Divide
  MC6816_edivs,  // Extended Signed Divide
  MC6816_emul,   // Extended Unsigned Multiply
  MC6816_emuls,  // Extended Signed Multiply
  MC6816_fdiv,   // Unsigned Fractional Divide
  MC6816_fmuls,  // Signed Fractional Multiply
  MC6816_idiv,   // Integer Divide
  MC6816_mul,    // Multiply

  // DECREMENT AND INCREMENT INSTRUCTIONS
  MC6816_dec,    // Decrement Memory
  MC6816_deca,   // Decrement A
  MC6816_decb,   // Decrement B
  MC6816_decw,   // Decrement Memory Word
  MC6816_inc,    // Increment Memory
  MC6816_inca,   // Increment A
  MC6816_incb,   // Increment B
  MC6816_incw,   // Increment Memory Word

  // CLEAR, COMPLEMENT, AND NEGATE INSTRUCTIONS
  MC6816_clr,    // Clear Memory
  MC6816_clra,   // Clear A
  MC6816_clrb,   // Clear B
  MC6816_clrd,   // Clear D
  MC6816_clre,   // Clear E
  MC6816_clrw,   // Clear Memory Word
  MC6816_com,    // One's Complement Byte
  MC6816_coma,   // One's Complement A
  MC6816_comb,   // One's Complement B
  MC6816_comd,   // One's Complement D
  MC6816_come,   // One's Complement E
  MC6816_comw,   // One's Complement Word
  MC6816_neg,    // Two's Complement Byte
  MC6816_nega,   // Two's Complement A
  MC6816_negb,   // Two's Complement B
  MC6816_negd,   // Two's Complement D
  MC6816_nege,   // Two's Complement E
  MC6816_negw,   // Two's Complement Word

  // BOOLEAN LOGIC INSTRUCTIONS
  MC6816_anda,   // AND A
  MC6816_andb,   // AND B
  MC6816_andd,   // AND D
  MC6816_ande,   // AND E
  MC6816_eora,   // Exclusive OR A
  MC6816_eorb,   // Exclusive OR B
  MC6816_eord,   // Exclusive OR D
  MC6816_eore,   // Exclusive OR E
  MC6816_oraa,   // OR A
  MC6816_orab,   // OR B
  MC6816_ord,    // OR D
  MC6816_ore,    // OR E

  // BIT TEST AND MANIPULATION INSTRUCTIONS
  // --------------------------------------
  MC6816_bita,   // Bit Test A
  MC6816_bitb,   // Bit Test B
  MC6816_bclr,   // Clear Bit(s)
  MC6816_bclrw,  // Clear Bit(s) Word
  MC6816_bset,   // Set Bit(s)
  MC6816_bsetw,  // Set Bit(s) Word

  // SHIFT AND ROTATE INSTRUCTIONS
  // -----------------------------
  MC6816_lsr,    // Logic Shift Right
  MC6816_lsra,   // Logic Shift Right A
  MC6816_lsrb,   // Logic Shift Right B
  MC6816_lsrd,   // Logic Shift Right D
  MC6816_lsre,   // Logic Shift Right E
  MC6816_lsrw,   // Logic Shift Right Word
  MC6816_asl,    // Arithmetic Shift Left
  MC6816_asla,   // Arithmetic Shift Left A
  MC6816_aslb,   // Arithmetic Shift Left B
  MC6816_asld,   // Arithmetic Shift Left D
  MC6816_asle,   // Arithmetic Shift Left E
  MC6816_aslw,   // Arithmetic Shift Left Word
  MC6816_asr,    // Arithmetic Shift Right
  MC6816_asra,   // Arithmetic Shift Right A
  MC6816_asrb,   // Arithmetic Shift Right B
  MC6816_asrd,   // Arithmetic Shift Right D
  MC6816_asre,   // Arithmetic Shift Right E
  MC6816_asrw,   // Arithmetic Shift Right Word
  MC6816_rol,    // Rotate Left
  MC6816_rola,   // Rotate Left A
  MC6816_rolb,   // Rotate Left B
  MC6816_rold,   // Rotate Left D
  MC6816_role,   // Rotate Left E
  MC6816_rolw,   // Rotate Left Word
  MC6816_ror,    // Rotate Right
  MC6816_rora,   // Rotate Right A
  MC6816_rorb,   // Rotate Right B
  MC6816_rord,   // Rotate Right D
  MC6816_rore,   // Rotate Right E
  MC6816_rorw,   // Rotate Right Word

  // PROGRAM CONTROL INSTRUCTIONS
  // ----------------------------

  // SHORT BRANCH INSTRUCTIONS
  MC6816_bra,    // Branch Always
  MC6816_brn,    // Branch Never
  MC6816_bcc,    // Branch If Carry Clear
  MC6816_bcs,    // Branch If Carry Set
  MC6816_beq,    // Branch If Equal to Zero
  MC6816_bmi,    // Branch If Minus
  MC6816_bne,    // Branch If Not Equal to Zero
  MC6816_bpl,    // Branch If Plus
  MC6816_bvc,    // Branch If Overflow Clear
  MC6816_bvs,    // Branch If Overflow Set
  MC6816_bhi,    // Branch If Higher
  MC6816_bls,    // Branch If Lower or Same
  MC6816_bge,    // Branch If Greater than or Equal to Zero
  MC6816_bgt,    // Branch If Greater than Zero
  MC6816_ble,    // Branch If Less than or Equal to Zero
  MC6816_blt,    // Branch If Less than Zero

  // LONG BRANCH INSTRUCTIONS
  MC6816_lbra,   // Long Branch Always
  MC6816_lbrn,   // Long Branch Never
  MC6816_lbcc,   // Long Branch If Carry Clear
  MC6816_lbcs,   // Long Branch If Carry Set
  MC6816_lbeq,   // Long Branch If Equal to Zero
  MC6816_lbev,   // Long Branch If EV Set
  MC6816_lbmi,   // Long Branch If Minus
  MC6816_lbmv,   // Long Branch If MV Set
  MC6816_lbne,   // Long Branch If Not Equal to Zero
  MC6816_lbpl,   // Long Branch If Plus
  MC6816_lbvc,   // Long Branch If Overflow Clear
  MC6816_lbvs,   // Long Branch If Overflow Set
  MC6816_lbhi,   // Long Branch If Higher
  MC6816_lbls,   // Long Branch If Lower or Same
  MC6816_lbge,   // Long Branch If Greater than or Equal to Zero
  MC6816_lbgt,   // Long Branch If Greater than Zero
  MC6816_lble,   // Long Branch If Less than or Equal to Zero
  MC6816_lblt,   // Long Branch If Less than Zero

  // BIT CONDITION BRANCH INSTRUCTIONS
  MC6816_brclr,  // Branch if Bits Clear
  MC6816_brset,  // Branch if Bits Set

  // JUMP INSTRUCTION
  MC6816_jmp,    // Jump

  // SUBROUTINE INSTRUCTIONS
  MC6816_bsr,    // Branch to Subroutine
  MC6816_jsr,    // Jump to Subroutine
  MC6816_lbsr,   // Long Branch to Subroutine
  MC6816_rts,    // Return From Subroutine

  // INTERRUPT INSTRUCTIONS
  MC6816_rti,    // Return from Interrupt
  MC6816_swi,    // Software Interrupt

  // INDEXING AND ADDRESS EXTENSION INSTRUCTIONS
  // -------------------------------------------

  // INDEXING INSTRUCTIONS
  MC6816_abx,    // Add B to IX
  MC6816_aby,    // Add B to IY
  MC6816_abz,    // Add B to IZ
  MC6816_adx,    // Add D to IX
  MC6816_ady,    // Add D to IY
  MC6816_adz,    // Add D to IZ
  MC6816_aex,    // Add E to IX
  MC6816_aey,    // Add E to IY
  MC6816_aez,    // Add E to IZ
  MC6816_aix,    // Add Immediate Value to IX
  MC6816_aiy,    // Add Immediate Value to IY
  MC6816_aiz,    // Add Immediate Value to IZ
  MC6816_cpx,    // Compare IX to Memory
  MC6816_cpy,    // Compare IY to Memory
  MC6816_cpz,    // Compare IZ to Memory
  MC6816_ldx,    // Load IX
  MC6816_ldy,    // Load IY
  MC6816_ldz,    // Load IZ
  MC6816_stx,    // Store IX
  MC6816_sty,    // Store IY
  MC6816_stz,    // Store IZ
  MC6816_tsx,    // Transfer SP to IX
  MC6816_tsy,    // Transfer SP to IY
  MC6816_tsz,    // Transfer SP to IZ
  MC6816_txs,    // Transfer IX to SP
  MC6816_txy,    // Transfer IX to IY
  MC6816_txz,    // Transfer IX to IZ
  MC6816_tys,    // Transfer IY to SP
  MC6816_tyx,    // Transfer IY to IX
  MC6816_tyz,    // Transfer IY to IZ
  MC6816_tzs,    // Transfer IZ to SP
  MC6816_tzx,    // Transfer IZ to IX
  MC6816_tzy,    // Transfer IZ to IY
  MC6816_xgdx,   // Exchange D with IX
  MC6816_xgdy,   // Exchange D with IY
  MC6816_xgdz,   // Exchange D with IZ
  MC6816_xgex,   // Exchange E with IX
  MC6816_xgey,   // Exchange E with IY
  MC6816_xgez,   // Exchange E with IZ

  // ADDRESS EXTENSION INSTRUCTIONS
  MC6816_tbek,   // Transfer B to EK
  MC6816_tbsk,   // Transfer B to SK
  MC6816_tbxk,   // Transfer B to XK
  MC6816_tbyk,   // Transfer B to YK
  MC6816_tbzk,   // Transfer B to ZK
  MC6816_tekb,   // Transfer EK to B
  MC6816_tskb,   // Transfer SK to B
  MC6816_txkb,   // Transfer XK to B
  MC6816_tykb,   // Transfer YK to B
  MC6816_tzkb,   // Transfer ZK to B

  // STACKING INSTRUCTIONS
  // ---------------------
  MC6816_ais,    // Add Immediate Data to SP
  MC6816_cps,    // Compare SP to Memory
  MC6816_lds,    // Load SP
  MC6816_sts,    // Store SP
  MC6816_psha,   // Push A
  MC6816_pshb,   // Push B
  MC6816_pshm,   // Push Multiple Registers
  MC6816_pula,   // Pull A
  MC6816_pulb,   // Pull B
  MC6816_pulm,   // Pull Multiple Registers

  // CONDITION CODE INSTRUCTIONS
  // ---------------------------
  MC6816_andp,   // AND CCR
  MC6816_orp,    // OR CCR
  MC6816_tap,    // Transfer A to CCR
  MC6816_tdp,    // Transfer D to CCR
  MC6816_tpa,    // Transfer CCR MSB to A
  MC6816_tpd,    // Transfer CCR to D

  // DIGITAL SIGNAL PROCESSING INSTRUCTIONS
  // --------------------------------------
  MC6816_ace,    // Add E to AM[31:15]
  MC6816_aced,   // Add concatenated E and D to AM
  MC6816_aslm,   // Arithmetic Shift Left AM
  MC6816_asrm,   // Arithmetic Shift Right AM
  MC6816_clrm,   // Clear AM
  MC6816_ldhi,   // Initialize HR and IR
  MC6816_mac,    // Multiply and Accumulate
  MC6816_pshmac, // Push MAC State
  MC6816_pulmac, // Pull MAC State
  MC6816_rmac,   // Repeating Multiply and Accumulate
  MC6816_tdmsk,  // Transfer D to XMSK:YMSK
  MC6816_tedm,   // Transfer E and T to AM[31:0]
  MC6816_tem,    // Transfer E to AM[31:16]
  MC6816_tmer,   // Transfer AM to E Rounded
  MC6816_tmet,   // Transfer AM to E Truncated
  MC6816_tmxed,  // Transfer AM t IX:E:D

  // STOP AND WAIT INSTRUCTIONS
  // --------------------------
  MC6816_lpstop, // Low Power Stop
  MC6816_wai,    // Wait for Interrupt

  // BACKGROUND MODE AND NULL OPERATIONS
  // -----------------------------------
  MC6816_bgnd,   // Enter Background Debugging Mode
  MC6816_nop,    // Null operation

  MC6816_last
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
I960_null = 0,          // Unknown Operation

I960_addc,              // Add ordinal with carry
I960_addi,              // Add integer
I960_addo,              // Add ordinal
I960_alterbit,          // Alter bit
I960_and,               // Src2 AND src1
I960_andnot,            // Src2 AND (NOT src1)
I960_atadd,             // Atomic add
I960_atmod,             // Atomic modify
I960_b,                 // Branch
I960_bal,               // Branch and Link
I960_balx,              // Branch and Link Extended
I960_bbc,               // Check bit and branch if clear
I960_bbs,               // Check bit and branch if set
I960_bno,               // Branch if unordered/false
I960_bg,                // Branch if greater
I960_be,                // Branch if equal/true
I960_bge,               // Branch if greater or equal
I960_bl,                // Branch if less
I960_bne,               // Branch if not equal
I960_ble,               // Branch if less or equal
I960_bo,                // Branch if ordered
I960_bx,                // Branch Extended
I960_call,              // Call
I960_calls,             // Call system
I960_callx,             // Call extended
I960_chkbit,            // Check bit
I960_clrbit,            // Clear bit
I960_cmpdeci,           // Compare and decrement integer
I960_cmpdeco,           // Compare and decrement ordinal
I960_cmpi,              // Compare integer
I960_cmpibno,           // Compare integer and branch if unordered
I960_cmpibg,            // Compare integer and branch if greater
I960_cmpibe,            // Compare integer and branch if equal
I960_cmpibge,           // Compare integer and branch if greater or equal
I960_cmpibl,            // Compare integer and branch if less
I960_cmpibne,           // Compare integer and branch if not equal
I960_cmpible,           // Compare integer and branch if less or equal
I960_cmpibo,            // Compare integer and branch if ordered
I960_cmpinci,           // Compare and increment integer
I960_cmpinco,           // Compare and increment ordinal
I960_cmpo,              // Compare ordinal
I960_cmpobg,            // Compare ordinal and branch if greater
I960_cmpobe,            // Compare ordinal and branch if equal
I960_cmpobge,           // Compare ordinal and branch if greater or equal
I960_cmpobl,            // Compare ordinal and branch if less
I960_cmpobne,           // Compare ordinal and branch if not equal
I960_cmpoble,           // Compare ordinal and branch if less or equal
I960_concmpi,           // Conditional compare integer
I960_concmpo,           // Conditional compare ordinal
I960_divi,              // Divide integer
I960_divo,              // Divide ordinal
I960_ediv,              // Extended divide
I960_emul,              // Extended multiply
I960_eshro,             // Extended shift right ordinal
I960_extract,           // Extract
I960_faultno,           // Fault if unordered
I960_faultg,            // Fault if greater
I960_faulte,            // Fault if equal
I960_faultge,           // Fault if greater or equal
I960_faultl,            // Fault if less
I960_faultne,           // Fault if not equal
I960_faultle,           // Fault if less or equal
I960_faulto,            // Fault if ordered
I960_flushreg,          // Flush cached local register sets to memory
I960_fmark,             // Force mark
I960_ld,                // Load word
I960_lda,               // Load address
I960_ldib,              // Load integer byte
I960_ldis,              // Load integer short
I960_ldl,               // Load long
I960_ldob,              // Load ordinal byte
I960_ldos,              // Load ordinal short
I960_ldq,               // Load quad
I960_ldt,               // Load triple
I960_mark,              // Mark
I960_modac,             // Modify the AC register
I960_modi,              // Modulo integer
I960_modify,            // Modify
I960_modpc,             // Modify the process controls register
I960_modtc,             // Modify trace controls
I960_mov,               // Move word
I960_movl,              // Move long word
I960_movq,              // Move quad word
I960_movt,              // Move triple word
I960_muli,              // Multiply integer
I960_mulo,              // Multiply ordinal
I960_nand,              // NOT (src2 AND src1)
I960_nor,               // NOT (src2 OR src1)
I960_not,               // NOT src1
I960_notand,            // (NOT src2) AND src1
I960_notbit,            // Not bit
I960_notor,             // (NOT src2) or src1
I960_or,                // Src2 OR src1
I960_ornot,             // Src2 or (NOT src1)
I960_remi,              // Remainder integer
I960_remo,              // Remainder ordinal
I960_ret,               // Return
I960_rotate,            // Rotate left
I960_scanbit,           // Scan for bit
I960_scanbyte,          // Scan byte equal
I960_setbit,            // Set bit
I960_shli,              // Shift left integer
I960_shlo,              // Shift left ordinal
I960_shrdi,             // Shift right dividing integer
I960_shri,              // Shift right integer
I960_shro,              // Shift right ordinal
I960_spanbit,           // Span over bit
I960_st,                // Store word
I960_stib,              // Store integer byte
I960_stis,              // Store integer short
I960_stl,               // Store long
I960_stob,              // Store ordinal byte
I960_stos,              // Store ordinal short
I960_stq,               // Store quad
I960_stt,               // Store triple
I960_subc,              // Subtract ordinal with carry
I960_subi,              // Subtract integer
I960_subo,              // Subtract ordinal
I960_syncf,             // Synchronize faults
I960_testno,            // Test for unordered
I960_testg,             // Test for greater
I960_teste,             // Test for equal
I960_testge,            // Test for greater or equal
I960_testl,             // Test for less
I960_testne,            // Test for not equal
I960_testle,            // Test for less or equal
I960_testo,             // Test for ordered
I960_xnor,              // Src2 XNOR src1
I960_xor,               // Src2 XOR src1

// Cx instructions

I960_sdma,              // Set up a DMA controller channel
I960_sysctl,            // Perform system control function
I960_udma,              // Copy current DMA pointers to internal data RAM

// Unknown instructions

I960_dcinva,
I960_cmpob,
I960_cmpib,
I960_cmpos,
I960_cmpis,
I960_bswap,
I960_intdis,
I960_inten,
I960_synmov,
I960_synmovl,
I960_synmovq,
I960_cmpstr,
I960_movqstr,
I960_movstr,
I960_inspacc,
I960_ldphy,
I960_synld,
I960_fill,
I960_daddc,
I960_dsubc,
I960_dmovt,
I960_condrec,
I960_receive,
I960_intctl,
I960_icctl,
I960_dcctl,
I960_halt,
I960_send,
I960_sendserv,
I960_resumprcs,
I960_schedprcs,
I960_saveprcs,
I960_condwait,
I960_wait,
I960_signal,
I960_ldtime,
I960_addono,
I960_addino,
I960_subono,
I960_subino,
I960_selno,
I960_addog,
I960_addig,
I960_subog,
I960_subig,
I960_selg,
I960_addoe,
I960_addie,
I960_suboe,
I960_subie,
I960_sele,
I960_addoge,
I960_addige,
I960_suboge,
I960_subige,
I960_selge,
I960_addol,
I960_addil,
I960_subol,
I960_subil,
I960_sell,
I960_addone,
I960_addine,
I960_subone,
I960_subine,
I960_selne,
I960_addole,
I960_addile,
I960_subole,
I960_subile,
I960_selle,
I960_addoo,
I960_addio,
I960_suboo,
I960_subio,
I960_selo,

// Floating point instructions

I960_faddr, I960_fp_first = I960_faddr,
I960_faddrl,
I960_fatanr,
I960_fatanrl,
I960_fclassr,
I960_fclassrl,
I960_fcmpor,
I960_fcmporl,
I960_fcmpr,
I960_fcmprl,
I960_fcosr,
I960_fcosrl,
I960_fcpyrsre,
I960_fcpysre,
I960_fcvtilr,
I960_fcvtir,
I960_fcvtri,
I960_fcvtril,
I960_fcvtzri,
I960_fcvtzril,
I960_fdivr,
I960_fdivrl,
I960_fexpr,
I960_fexprl,
I960_flogbnr,
I960_flogbnrl,
I960_flogepr,
I960_flogeprl,
I960_flogr,
I960_flogrl,
I960_fmovr,
I960_fmovre,
I960_fmovrl,
I960_fmulr,
I960_fmulrl,
I960_fremr,
I960_fremrl,
I960_froundr,
I960_froundrl,
I960_fscaler,
I960_fscalerl,
I960_fsinr,
I960_fsinrl,
I960_fsqrtr,
I960_fsqrtrl,
I960_fsubr,
I960_fsubrl,
I960_ftanr,
I960_ftanrl, I960_fp_last = I960_ftanrl,

I960_last,

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

F2MC_null = 0,     // Unknown Operation

// TRANSFER INSTRUCTIONS

F2MC_mov,          // Move  byte data from source to destination
F2MC_movn,         // Move immediate nibble data to A
F2MC_movx,         // Move byte data with sign extension from source to A
F2MC_xch,          // Exchange byte data of source to destination
F2MC_movw,         // Move word data from source to destination
F2MC_xchw,         // Exchange word data of source to destination
F2MC_movl,         // Move long word data from source to destination

// NUMERIC DATA OPERATIONS INSTRUCTIONS

F2MC_add,          // Add byte data of destination and source to destination
F2MC_addc1,        // Add byte data of AL and AH with Carry to AL
F2MC_addc2,        // Add byte data of A and effective address with Carry to A
F2MC_adddc,        // Add decimal data of AL and AH with Carry to AL
F2MC_sub,          // Subtract byte data of source from festination to destination
F2MC_subc1,        // Subtract byte data of AL from AH with Carry to AL
F2MC_subc2,        // Subtract byte data of effective address from A with Carry to A
F2MC_subdc,        // Subtract decimal data of AL from AH with Carry to AL
F2MC_addw1,        // Add word data of AH and AL to AL
F2MC_addw2,        // Add word data of destination and source to destination
F2MC_addcw,        // Add word data of A and effective address from A with Carry to A
F2MC_subw1,        // Subtract word data of AL from AH to AL
F2MC_subw2,        // Subtract word data of source from festination to destination
F2MC_subcw,        // Subtract word data of A and effective address from A with carry to A
F2MC_addl,         // Add long word data of destination and source to destination
F2MC_subl,         // Subtract long word data of source from festination to destination
F2MC_inc,          // Increment byte data
F2MC_dec,          // Decrement byte data
F2MC_incw,         // Increment word data
F2MC_decw,         // Decrement word data
F2MC_incl,         // Increment long word data
F2MC_decl,         // Decrement long word data
F2MC_cmp1,         // Compare byte data of AH and AL
F2MC_cmp2,         // Compare byte data of destination and source
F2MC_cmpw1,        // Compare word data of AH and AL
F2MC_cmpw2,        // Compare word data of destination and source
F2MC_cmpl,         // Compare long word data of destination and source
F2MC_divu1,        // Divide unsigned AH by AL
F2MC_divu2,        // Divide unsigned word data by unsigned byte data
F2MC_divuw,        // Divide unsigned long word data by unsigned word data
F2MC_mulu1,        // Multiply unsigned byte AH by AL
F2MC_mulu2,        // Multiply unsigned byte data
F2MC_muluw1,       // Multiply unsigned word AH by AL
F2MC_muluw2,       // Multiply unsigned word data
F2MC_div1,         // Divide AH by AL
F2MC_div2,         // Divide word data by byte data
F2MC_divw,         // Divide long word data by word data
F2MC_mul1,         // Multiply byte AH by AL
F2MC_mul2,         // Multiply byte data
F2MC_mulw1,        // Multiply word AH by AL
F2MC_mulw2,        // Multiply word data

// LOGICAL DATA OPERATION INSTRUCTIONS

F2MC_and,          // And byte data of destination and source to destination
F2MC_or,           // Or byte data of destination and source to destination
F2MC_xor,          // Exclusive or byte data of destination and source to destination
F2MC_not,          // Not byte data of destination
F2MC_andw1,        // And word data of AH and AL to AL
F2MC_andw2,        // And word data of destination and source to destination
F2MC_orw1,         // Or word data of AH and AL to AL
F2MC_orw2,         // Or word data of destination and source to destination
F2MC_xorw1,        // Exclusive or word data of AH and AL to AL
F2MC_xorw2,        // Exclusive or word data of destination and source to destination
F2MC_notw,         // Not word data of destination
F2MC_andl,         // And long word data of destination and source to destination
F2MC_orl,          // Or long word data of destination and source to destination
F2MC_xorl,         // Exclusive or long word data of destination and source to destination
F2MC_neg,          // Negate byte data of destination
F2MC_negw,         // Negate word data of destination
F2MC_nrml,         // Normalize long word

// SHIFT INSTRUCTIONS

F2MC_rorc,         // Rotate byte data of A with Carry to right
F2MC_rolc,         // Rotate byte data of A with Carry to left
F2MC_asr,          // Arithmetic shift byte data of A to right
F2MC_lsr,          // Logical shift byte data of A to right
F2MC_lsl,          // Logical shift byte data of A to left
F2MC_asrw1,        // Arithmetic shift word data of A to right
F2MC_asrw2,        // Arithmetic shift word data of A to right
F2MC_lsrw1,        // Logical shift word data of A to right
F2MC_lsrw2,        // Logical shift word data of A to right
F2MC_lslw1,        // Logical shift word data of A to left
F2MC_lslw2,        // Logical shift word data of A to left
F2MC_asrl,         // Arithmetic shift long word data of A to right
F2MC_lsrl,         // Logical shift long word data of A to right
F2MC_lsll,         // Logical shift long word data of A to left

// BRANCH INSTRUCTIONS

F2MC_bz,           // Branch if Zero
F2MC_bnz,          // Branch if Not Zero
F2MC_bc,           // Branch if Carry
F2MC_bnc,          // Branch if Not Carry
F2MC_bn,           // Branch if Negative
F2MC_bp,           // Branch if Not Negative
F2MC_bv,           // Branch if Overflow
F2MC_bnv,          // Branch if Not Overflow
F2MC_bt,           // Branch if Sticky
F2MC_bnt,          // Branch if Not Sticky
F2MC_blt,          // Branch if Overflow or Negative
F2MC_bge,          // Branch if Not (Overflow or Negative)
F2MC_ble,          // Branch if (Overflow xor Negative) or Zero
F2MC_bgt,          // Branch if Not ((Overflow xor Negative) or Zero)
F2MC_bls,          // Branch if Carry or Zero
F2MC_bhi,          // Branch if Not (Carry or Zero)
F2MC_bra,          // Branch unconditionally
F2MC_jmp,          // Jump destination address
F2MC_jmpp,         // Jump destination physical address
F2MC_call,         // Call subroutine
F2MC_callv,        // Call vectored subroutine
F2MC_callp,        // Call physical address
F2MC_cbne,         // Compare byte data and branch if not Equal
F2MC_cwbne,        // Compare word data and branch if not Equal
F2MC_dbnz,         // Decrement byte data and branch if not Zero
F2MC_dwbnz,        // Decrement word data and branch if not Zero
F2MC_int,          // Software interrupt
F2MC_intp,         // Software interrupt
F2MC_int9,         // Software interrupt
F2MC_reti,         // Return from interrupt
F2MC_link,         // Link and create new stack frame
F2MC_unlink,       // Unlink and create new stack frame
F2MC_ret,          // Return from subroutine
F2MC_retp,         // Return from physical address

// OTHER INSTRUCTIONS

F2MC_pushw,        // Push to stack memory
F2MC_popw,         // Pop from stack memory
F2MC_jctx,         // Jump context
// F2MC_and,
// F2MC_or,
// F2MC_mov,
F2MC_movea,        // Move effective address to destination
F2MC_addsp,        // Add word data of SP and immediate data to SP
// F2MC_mov,
F2MC_nop,          // No operation
F2MC_adb,          // ADB register
F2MC_dtb,          // DTB register
F2MC_pcb,          // PCB register
F2MC_spb,          // SPB register
F2MC_ncc,          // Flag change inhibit
F2MC_cmr,          // Common register bank
F2MC_movb,         // Move bit data
F2MC_setb,         // Set bit
F2MC_clrb,         // Clear bit
F2MC_bbc,          // Branch if bit condition satisfied
F2MC_bbs,          // Branch if bit condition satisfied
F2MC_sbbs,         // Set bit and branch if bit set
F2MC_wbts,         // Wait until bit condition satisfied
F2MC_wbtc,         // Wait until bit condition satisfied
F2MC_swap,         // Swap byte data of A
F2MC_swapw,        // Swap word data of A
F2MC_ext,          // Sign extend from byte data to word data
F2MC_extw,         // Sign extend from word data to long word data
F2MC_zext,         // Zero extendfrom byte data to word data
F2MC_zextw,        // Zero extendfrom word data to long word data
F2MC_movsi,        // Move string byte with addresses incremented
F2MC_movsd,        // Move string byte with addresses decremented
F2MC_sceqi,        // Scan string byte until Equal with address incremented
F2MC_sceqd,        // Scan string byte until Equal with address decremented
F2MC_filsi,        // Fill string byte
F2MC_movswi,       // Move string word with address incremented
F2MC_movswd,       // Move string word with address decremented
F2MC_scweqi,       // Scan string word until Equal with address incremented
F2MC_scweqd,       // Scan string word until Equal with address decremented
F2MC_filswi,       // Fill string word

// MACROS

F2MC_bz16,         // Branch if Zero
F2MC_bnz16,        // Branch if Not Zero
F2MC_bc16,         // Branch if Carry
F2MC_bnc16,        // Branch if Not Carry
F2MC_bn16,         // Branch if Negative
F2MC_bp16,         // Branch if Not Negative
F2MC_bv16,         // Branch if Overflow
F2MC_bnv16,        // Branch if Not Overflow
F2MC_bt16,         // Branch if Sticky
F2MC_bnt16,        // Branch if Not Sticky
F2MC_blt16,        // Branch if Overflow or Negative
F2MC_bge16,        // Branch if Not (Overflow or Negative)
F2MC_ble16,        // Branch if (Overflow xor Negative) or Zero
F2MC_bgt16,        // Branch if Not ((Overflow xor Negative) or Zero)
F2MC_bls16,        // Branch if Carry or Zero
F2MC_bhi16,        // Branch if Not (Carry or Zero)

F2MC_cbne16,       // Compare byte data and branch if not Equal
F2MC_cwbne16,      // Compare word data and branch if not Equal

F2MC_dbnz16,       // Decrement byte data and branch if not Zero
F2MC_dwbnz16,      // Decrement word data and branch if not Zero

F2MC_bbc16,        // Branch if bit condition satisfied
F2MC_bbs16,        // Branch if bit condition satisfied
F2MC_sbbs16,       // Set bit and branch if bit set

F2MC_last,

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
TMS320C3X_null = 0,     // Unknown Operation

TMS320C3X_ABSF,                 // Absolute value of a floating-point number
TMS320C3X_ABSI,                 // Absolute value of an integer
TMS320C3X_ADDC,                 // Add integers with carry
TMS320C3X_ADDF,                 // Add Floating-Point Values
TMS320C3X_ADDI,                 // Add Integer
TMS320C3X_AND,                  // Bitwise-Logical AND
TMS320C3X_ANDN,                 // Bitwise-Logical AND With Complement
TMS320C3X_ASH,                  // Arithmetic Shift
TMS320C3X_CMPF,                 // Compare Floating-Point Value
TMS320C3X_CMPI,                 // Compare Integer
TMS320C3X_FIX,                  // Floating-Point-to-Integer Conversion
TMS320C3X_FLOAT,                // Integer-to-Floating-Point Conversion
TMS320C3X_IDLE,                 // Idle Until Interrupt
TMS320C3X_IDLE2,                // Low-Power Idle
TMS320C3X_LDE,                  // Load Floating-Point Exponent
TMS320C3X_LDF,                  // Load Floating-Point Value
TMS320C3X_LDFI,                 // Load Floating-Point Value, Interlocked
TMS320C3X_LDI,                  // Load Integer
TMS320C3X_LDII,                 // Load Integer, Interlocked
TMS320C3X_LDM,                  // Load Floating-Point Mantissa
TMS320C3X_LSH,                  // Logical Shift
TMS320C3X_MPYF,                 // Multiply Floating-Point Value
TMS320C3X_MPYI,                 // Multiply Integer
TMS320C3X_NEGB,                 // Negative Integer With Borrow
TMS320C3X_NEGF,                 // Negate Floating-Point Value
TMS320C3X_NEGI,                 // Negate Integer
TMS320C3X_NOP,                  // No Operation
TMS320C3X_NORM,                 // Normalize
TMS320C3X_NOT,                  // Bitwise-Logical Complement
TMS320C3X_POP,                  // Pop Integer
TMS320C3X_POPF,                 // Pop Floating-Point Value
TMS320C3X_PUSH,                 // PUSH Integer
TMS320C3X_PUSHF,                // PUSH Floating-Point Value
TMS320C3X_OR,                   // Bitwise-Logical OR
TMS320C3X_LOPOWER,              // Divide Clock by 16
TMS320C3X_MAXSPEED,             // Restore Clock to Regular Speed
TMS320C3X_RND,                  // Round Floating-Point Value
TMS320C3X_ROL,                  // Rotate Left
TMS320C3X_ROLC,                 // Rotate Left Through Carry
TMS320C3X_ROR,                  // Rotate Right
TMS320C3X_RORC,                 // Rotate Right Through Carry
TMS320C3X_RPTS,                 // Repeat Single Instruction
TMS320C3X_STF,                  // Store Floating-Point Value
TMS320C3X_STFI,                 // Store Floating-Point Value, Interlocked
TMS320C3X_STI,                  // Store Integer
TMS320C3X_STII,                 // Store Integer, Interlocked
TMS320C3X_SIGI,                 // Signal, Interlocked
TMS320C3X_SUBB,                 // Subtract Integer With Borrow
TMS320C3X_SUBC,                 // Subtract Integer Conditionally
TMS320C3X_SUBF,                 // Subtract Floating-Point Value
TMS320C3X_SUBI,                 // Subtract Integer
TMS320C3X_SUBRB,                // Subtract Reverse Integer With Borrow
TMS320C3X_SUBRF,                // Subtract Reverse Floating-Point Value
TMS320C3X_SUBRI,                // Subtract Reverse Integer
TMS320C3X_TSTB,                 // Test Bit Fields
TMS320C3X_XOR,                  // Bitwise-Exclusive OR
TMS320C3X_IACK,                 // Interrupt acknowledge

TMS320C3X_ADDC3,                // Add integers with carry (3-operand)
TMS320C3X_ADDF3,                // Add floating-point values (3-operand)
TMS320C3X_ADDI3,                // Add integers (3 operand)
TMS320C3X_AND3,                 // Bitwise-logical AND (3-operand)
TMS320C3X_ANDN3,                // Bitwise-logical ANDN (3-operand)
TMS320C3X_ASH3,                 // Arithmetic shift (3-operand)
TMS320C3X_CMPF3,                // Compare floating-point values (3-operand)
TMS320C3X_CMPI3,                // Compare integers (3-operand)
TMS320C3X_LSH3,                 // Logical shift (3-operand)
TMS320C3X_MPYF3,                // Multiply floating-point value (3-operand)
TMS320C3X_MPYI3,                // Multiply integers (3-operand)
TMS320C3X_OR3,                  // Bitwise-logical OR (3-operand)
TMS320C3X_SUBB3,                // Subtract integers with borrow (3-operand)
TMS320C3X_SUBF3,                // Subtract floating-point values (3-operand)
TMS320C3X_SUBI3,                // Subtract integers (3-operand)
TMS320C3X_TSTB3,                // Test Bit Fields, 3-Operand
TMS320C3X_XOR3,                 // Bitwise-Exclusive OR, 3-Operand

TMS320C3X_LDFcond,              // Load floating-point value conditionally
TMS320C3X_LDIcond,              // Load integer conditionally
TMS320C3X_BR,                   // Branch unconditionally (standard)
TMS320C3X_BRD,                  // Branch unconditionally (delayed)
TMS320C3X_CALL,                 // Call subroutine
TMS320C3X_RPTB,                 // Repeat block of instructions
TMS320C3X_SWI,                  // Software Interrupt
TMS320C3X_Bcond,                // Branch conditionally
TMS320C3X_DBcond,               // Decrement and branch conditionally
TMS320C3X_CALLcond,             // Call subroutine conditionally
TMS320C3X_TRAPcond,             // Trap Conditionally
TMS320C3X_RETIcond,             // Return from interrupt conditionally
TMS320C3X_RETScond,             // Return from subroutine conditionally
TMS320C3X_RETIU,                // Return from interrupt unconditionally
TMS320C3X_RETSU,                // Return from subroutine unconditionally

TMS320C3X_NONE,                 // Pseudo cmd (more accurate definition need)
TMS320C3X_MV_IDX,               // Pseudo cmd (move to next index need)
TMS320C3X_last,                 // last ID

};


/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

TMS320C54_null = 0, // Unknown Operation

// ARITHMETIC OPERATIONS

// ADD INSTRUCTIONS

TMS320C54_add1,     // Add to Accumulator
TMS320C54_add2,     // Add to Accumulator
TMS320C54_add3,     // Add to Accumulator
TMS320C54_addc,    // Add to Accumulator With Carry
TMS320C54_addm,    // Add Long-Immediate Value to Memory
TMS320C54_adds,    // Add to Accumulator With Sign-Extension Suppressed

// SUBTRACT INSTRUCTIONS

TMS320C54_sub1,     // Sub From Accumulator
TMS320C54_sub2,     // Sub From Accumulator
TMS320C54_sub3,     // Sub From Accumulator
TMS320C54_subb,    // Sub From Accumulator With Borrow
TMS320C54_subc,    // Subtract Conditionally
TMS320C54_subs,    // Subtract From Accumulator With Sign Extension Suppressed

// MULTIPLY INSTRUCTIONS

TMS320C54_mpy2,    // Multiply Without Rounding
TMS320C54_mpy3,    // Multiply Without Rounding
TMS320C54_mpyr2,   // Multiply With Rounding
TMS320C54_mpya,    // Multiply by Accumulator A
TMS320C54_mpyu,    // Multiply Unsigned
TMS320C54_squr,    // Square

// MULTIPLY-ACCUMULATE AND MULTIPLY-SUBTRACT INSTRUCTIONS

TMS320C54_mac2,    // Multiply Accumulate Without Rounding
TMS320C54_mac3,    // Multiply Accumulate Without Rounding
TMS320C54_macr2,   // Multiply Accumulate With Rounding
TMS320C54_macr3,   // Multiply Accumulate With Rounding
TMS320C54_maca1,   // Multiply by Accumulator A and Accumulate Without Rounding
TMS320C54_maca2,   // Multiply by Accumulator A and Accumulate Without Rounding
TMS320C54_maca3,   // Multiply by Accumulator A and Accumulate Without Rounding
TMS320C54_macar1,  // Multiply by Accumulator A and Accumulate With Rounding
TMS320C54_macar2,  // Multiply by Accumulator A and Accumulate With Rounding
TMS320C54_macar3,  // Multiply by Accumulator A and Accumulate With Rounding
TMS320C54_macd,    // Multiply by Program Memory and Accumulate With Delay
TMS320C54_macp,    // Multiply by Program Memory and Accumulate
TMS320C54_macsu,   // Multiply Signed by Unsigned and Accumulate
TMS320C54_mas2,    // Multiply and Subtract Without Rounding
TMS320C54_mas3,    // Multiply and Subtract Without Rounding
TMS320C54_masr2,   // Multiply and Subtract With Rounding
TMS320C54_masr3,   // Multiply and Subtract With Rounding
//TMS320C54_mas,
TMS320C54_masa1,   // Multiply by Accumulator A and Subtract Without Rounding
TMS320C54_masa2,   // Multiply by Accumulator A and Subtract Without Rounding
TMS320C54_masa3,   // Multiply by Accumulator A and Subtract Without Rounding
TMS320C54_masar1,  // Multiply by Accumulator A and Subtract With Rounding
TMS320C54_masar2,  // Multiply by Accumulator A and Subtract With Rounding
TMS320C54_masar3,  // Multiply by Accumulator A and Subtract With Rounding
TMS320C54_squra,   // Square and Accumulate
TMS320C54_squrs,   // Square and Subtract

// DOUBLE INSTRUCTIONS

TMS320C54_dadd2,   // Double-Precision/Dual 16-Bit Add to Accumulator
TMS320C54_dadd3,   // Double-Precision/Dual 16-Bit Add to Accumulator
TMS320C54_dadst,   // Double-Precision Load With T Add/Dual 16-Bit Load With T Add/Subtract
TMS320C54_drsub,   // Double-Precision/Dual 16-Bit Subtract From Long Word
TMS320C54_dsadt,   // Long-Word Load With T Add/Dual 16-Bit Load With T Subtract/Add
TMS320C54_dsub,    // Double-Precision/Dual 16-Bit Subtract From Accumulator
TMS320C54_dsubt,   // Long-Word Load With T Subtract/Dual 16-Bit Load With T Subtract

// APPLICATION-SPECIFIC INSTRUCTIONS

TMS320C54_abdst,   // Absolute distance
TMS320C54_abs1,    // Absolute Value of Accumulator
TMS320C54_abs2,    // Absolute Value of Accumulator
TMS320C54_cmpl1,   // Complement Accumulator
TMS320C54_cmpl2,   // Complement Accumulator
TMS320C54_delay,   // Memory Delay
TMS320C54_exp,     // Accumulator Exponent
TMS320C54_firs,    // Symmetrical Finite Impulse Response Filter
TMS320C54_lms,     // Least Mean Square
TMS320C54_max,     // Accumulator Maximum
TMS320C54_min,     // Accumulator Minimum
TMS320C54_neg1,    // Negate Accumulator
TMS320C54_neg2,    // Negate Accumulator
TMS320C54_norm1,   // Normalization
TMS320C54_norm2,   // Normalization
TMS320C54_poly,    // Polynominal Evaluation
TMS320C54_rnd1,    // Round Accumulator
TMS320C54_rnd2,    // Round Accumulator
TMS320C54_sat,     // Saturate Accumulator
TMS320C54_sqdst,   // Square Distance

// LOGICAL OPERATIONS

// AND INSTRUCTIONS

TMS320C54_and1,    // AND With Accumulator
TMS320C54_and2,    // AND With Accumulator
TMS320C54_and3,    // AND With Accumulator
TMS320C54_andm,    // AND Memory With Long Immediate

// OR INSTRUCTIONS

TMS320C54_or1,     // OR With Accumulator
TMS320C54_or2,     // OR With Accumulator
TMS320C54_or3,     // OR With Accumulator
TMS320C54_orm,     // OR Memory With Constant

// XOR INSTRUCTIONS

TMS320C54_xor1,    // Exclusive OR With Accumulator
TMS320C54_xor2,    // Exclusive OR With Accumulator
TMS320C54_xor3,    // Exclusive OR With Accumulator
TMS320C54_xorm,    // Exclusive OR Memory With Constant

// SHIFT INSTRUCTIONS

TMS320C54_rol,     // Rotate Accumulator
TMS320C54_roltc,   // Rotate Accumulator Left Using TC
TMS320C54_ror,     // Rotate Accumulator Right
TMS320C54_sfta2,   // Shift Accumulator Arithmetically
TMS320C54_sfta3,   // Shift Accumulator Arithmetically
TMS320C54_sftc,    // Shift Accumulator Conditionally
TMS320C54_sftl2,   // Shift Accumulator Logically
TMS320C54_sftl3,   // Shift Accumulator Logically

// TEST INSTRUCTIONS

TMS320C54_bit,     // Test Bit
TMS320C54_bitf,    // Test Bit Field Specified by Immediate Value
TMS320C54_bitt,    // Test Bit Specified by T
TMS320C54_cmpm,    // Compare Memory With Long Immediate
TMS320C54_cmpr,    // Compare Auxiliary Register with AR0

// PROGRAM CONTROL OPERATIONS

// BRANCH INSTRUCTIONS

TMS320C54_b,       // Branch Unconditionally
TMS320C54_bd,      // Branch Unconditionally
TMS320C54_bacc,    // Branch to Location Specified by Accumulator
TMS320C54_baccd,   // Branch to Location Specified by Accumulator
TMS320C54_banz,    // Branch on Auxiliary Register Not Zero
TMS320C54_banzd,   // Branch on Auxiliary Register Not Zero
TMS320C54_bc2,     // Branch Conditionally
TMS320C54_bc3,     // Branch Conditionally
TMS320C54_bcd2,    // Branch Conditionally
TMS320C54_bcd3,    // Branch Conditionally
TMS320C54_fb,      // Far Branch Unconditionally
TMS320C54_fbd,     // Far Branch Unconditionally
TMS320C54_fbacc,   // Far Branch to Location Specified by Accumulator
TMS320C54_fbaccd,  // Far Branch to Location Specified by Accumulator

// CALL INSTRUCTIONS

TMS320C54_cala,    // Call Subroutine at Location Specified by Accumulator
TMS320C54_calad,   // Call Subroutine at Location Specified by Accumulator
TMS320C54_call,    // Call Unconditionally
TMS320C54_calld,   // Call Unconditionally
TMS320C54_cc2,     // Call Conditionally
TMS320C54_cc3,     // Call Conditionally
TMS320C54_ccd2,    // Call Conditionally
TMS320C54_ccd3,    // Call Conditionally
TMS320C54_fcala,   // Far Call Subroutine at Location Specified by Accumulator
TMS320C54_fcalad,  // Far Call Subroutine at Location Specified by Accumulator
TMS320C54_fcall,   // Far Call Unconditionally
TMS320C54_fcalld,  // Far Call Unconditionally

// INTERRUPT INSTRUCTIONS

TMS320C54_intr,    // Software Interrupt
TMS320C54_trap,    // Software Interrupt

// RETURN INSTRUCTIONS

TMS320C54_fret,    // Far Return
TMS320C54_fretd,   // Far Return
TMS320C54_frete,   // Enable Interrupts and Far Return From Interrupt
TMS320C54_freted,  // Enable Interrupts and Far Return From Interrupt
TMS320C54_rc1,     // Return Conditionally
TMS320C54_rc2,     // Return Conditionally
TMS320C54_rc3,     // Return Conditionally
TMS320C54_rcd1,    // Return Conditionally
TMS320C54_rcd2,    // Return Conditionally
TMS320C54_rcd3,    // Return Conditionally
TMS320C54_ret,     // Return
TMS320C54_retd,    // Return
TMS320C54_rete,    // Enable Interrupts and Return From Interrupt
TMS320C54_reted,   // Enable Interrupts and Return From Interrupt
TMS320C54_retf,    // Enable Interrupts and Fast Return From Interrupt
TMS320C54_retfd,   // Enable Interrupts and Fast Return From Interrupt

// REPEAT INSTRUCTIONS

TMS320C54_rpt,     // Repeat Next Instruction
TMS320C54_rptb,    // Block Repeat
TMS320C54_rptbd,   // Block Repeat
TMS320C54_rptz,    // Repeat Next Instruction And Clear Accumulator

// STACK MANIPULATING INSTRUCTIONS

TMS320C54_frame,   // Stack Pointer Immediate Offset
TMS320C54_popd,    // Pop Top of Stack to Data Memory
TMS320C54_popm,    // Pop Top of Stack to Memory-Mapped Register
TMS320C54_pshd,    // Push Data-Memory Value Onto Stack
TMS320C54_pshm,    // Push Memory-Mapped Register Onto Stack

// MISCELLANEOUS PROGRAM-CONTROL INSTRUCTIONS

TMS320C54_idle,    // Idle Until Interrupt
TMS320C54_mar,     // Modify Auxiliary Register
TMS320C54_nop,     // No Operation
TMS320C54_reset,   // Software Reset
TMS320C54_rsbx1,   // Reset Status Register Bit
TMS320C54_rsbx2,   // Reset Status Register Bit
TMS320C54_ssbx1,   // Set Status Register Bit
TMS320C54_ssbx2,   // Set Status Register Bit
TMS320C54_xc2,     // Execute Conditionally
TMS320C54_xc3,     // Execute Conditionally

// LOAD AND STORE OPERATIONS

// LOAD INSTRUCTIONS

TMS320C54_dld,     // Double-Precision/Dual 16-Bit Long-Word Load to Accumulator
TMS320C54_ld1,     // Load Accumulator With Shift
TMS320C54_ld2,     // Load Accumulator With Shift
TMS320C54_ld3,     // Load Accumulator With Shift
TMS320C54_ldm,     // Load Memory-Mapped Register
TMS320C54_ldr,     // Load Memory Value in Accumulator High With Rounding
TMS320C54_ldu,     // Load Unsigned Memory Value
TMS320C54_ltd,     // Load T and insert Delay

// STORE INSTRUCTIONS

TMS320C54_dst,     // Store Accumulator in Long Word
TMS320C54_st,      // Store T, TRN, or Immediate Value into Memory
TMS320C54_sth2,    // Store Accumulator High Into Memory
TMS320C54_sth3,    // Store Accumulator High Into Memory
TMS320C54_stl2,    // Store Accumulator Low Into Memory
TMS320C54_stl3,    // Store Accumulator Low Into Memory
TMS320C54_stlm,    // Store Accumulator Low Into Memory-Mapped Register
TMS320C54_stm,     // Store Immediate Value Into Memory-Mapped Register

// CONDITIONAL STORE INSTRUCTIONS

TMS320C54_cmps,    // Compare, Select and Store Maximum
TMS320C54_saccd,   // Store Accumulator Conditionally
TMS320C54_srccd,   // Store Block Repeat Counter Conditionally
TMS320C54_strcd,   // Store T Conditionally

// PARALLEL LOAD AND STORE INSTRUCTIONS

TMS320C54_st_ld,   // Store Accumulator With Parallel Load

// PARALLEL LOAD AND MULTIPLY INSTRUCTIONS

TMS320C54_ld_mac,  // Load Accumulator With Parallel Multiply Accumulate Without Rounding
TMS320C54_ld_macr, // Load Accumulator With Parallel Multiply Accumulate With Rounding
TMS320C54_ld_mas,  // Load Accumulator With Parallel Multiply Subtract Without Rounding
TMS320C54_ld_masr, // Load Accumulator With Parallel Multiply Subtract With Rounding

// PARALLEL STORE AND ADD/SUBSTRACT INSTRUCTIONS

TMS320C54_st_add,  // Store Accumulator With Parallel Add
TMS320C54_st_sub,  // Store Accumulator With Parallel Subtract

// PARALLEL STORE AND MULTIPLY INSTRUCTIONS

TMS320C54_st_mac,  // Store Accumulator With Parallel Multiply Accumulate Without Rounding
TMS320C54_st_macr, // Store Accumulator With Parallel Multiply Accumulate With Rounding
TMS320C54_st_mas,  // Store Accumulator With Parallel Multiply Subtract Without Rounding
TMS320C54_st_masr, // Store Accumulator With Parallel Multiply Subtract With Rounding
TMS320C54_st_mpy,  // Store Accumulator With Parallel Multiply

// MISCELLANEOUS LOAD-TYPE AND STORE-TYPE INSTRUCTIONS

TMS320C54_mvdd,    // Move Data From Data Memory to Data Memory With X,Y Addressing
TMS320C54_mvdk,    // Move Data From Data Memory to Data Memory With Destination Addressing
TMS320C54_mvdm,    // Move Data From Data Memory to Memory-Mapped Register
TMS320C54_mvdp,    // Move Data From Data Memory to Program Memory
TMS320C54_mvkd,    // Move Data From Data Memory to Data Memory With Source Addressing
TMS320C54_mvmd,    // Move Data From Memory-Mapped Register to Data Memory
TMS320C54_mvmm,    // Move Data From Memory-Mapped Register to Memory-Mapped Register
TMS320C54_mvpd,    // Move Data From Program Memory to Data Memory
TMS320C54_portr,   // Read Data From Port
TMS320C54_portw,   // Write Data to Port
TMS320C54_reada,   // Read Program Memory Addressed by Accumulator A and Store in Data Memory
TMS320C54_writa,   // Write Data to Program Memory Addressed by Accumulator A

TMS320C54_last

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

TMS320C55_null = 0,      // Unknown Operation

// ARITHMETICAL OPERATIONS

TMS320C55_abdst,         // Absolute Distance
TMS320C55_abs1,          // Absolute Value
TMS320C55_abs2,          // Absolute Value

TMS320C55_add1,          // Addition
TMS320C55_add2,          // Addition
TMS320C55_add3,          // Addition
TMS320C55_add4,          // Addition
TMS320C55_addv1,         // Addition
TMS320C55_addv2,         // Addition
TMS320C55_addrv1,        // Addition and Round
TMS320C55_addrv2,        // Addition and Round

TMS320C55_maxdiff,       // Compare and Select Maximum
TMS320C55_dmaxdiff,      // Compare and Select 40-bit Maximum
TMS320C55_mindiff,       // Compare and Select Minimum
TMS320C55_dmindiff,      // Compare and Select 40-bit Minimum

TMS320C55_addsubcc4,     // Conditional Add or Subtract
TMS320C55_addsubcc5,     // Conditional Add or Subtract
TMS320C55_addsub2cc,     // Conditional Add or Subtract

TMS320C55_sftcc,         // Conditional Shift

TMS320C55_subc2,         // Conditional Subtract
TMS320C55_subc3,         // Conditional Subtract

TMS320C55_addsub,        // Paralleled Add - Subtract
TMS320C55_subadd,        // Parallel Subtract - Add

TMS320C55_mpy_mpy,       // Two Parallel Multiply
TMS320C55_mpy_mpyr,      // Two Parallel Multiply, and Round
TMS320C55_mpy_mpy40,     // Two Parallel Multiply, on 40 bits
TMS320C55_mpy_mpyr40,    // Two Parallel Multiply, and Round on 40 bits
TMS320C55_mac_mpy,       // Parallel Multiply - Accumulate
TMS320C55_macr_mpyr,     // Parallel Multiply - Accumulate, and Round
TMS320C55_mac40_mpy40,   // Parallel Multiply - Accumulate, on 40 bits
TMS320C55_macr40_mpyr40, // Parallel Multiply - Accumulate, and Round on 40 bits
TMS320C55_mas_mpy,       // Parallel Multiply - Subtract
TMS320C55_masr_mpyr,     // Parallel Multiply - Subtract, and Round
TMS320C55_mas40_mpy40,   // Parallel Multiply - Subtract, on 40 bits
TMS320C55_masr40_mpyr40, // Parallel Multiply - Subtract, and Round on 40 bits
TMS320C55_amar_mpy,      // Parallel Modify Auxiliary Register - Multiply
TMS320C55_amar_mpyr,     // Parallel Modify Auxiliary Register - Multiply, and Round
TMS320C55_amar_mpy40,    // Parallel Modify Auxiliary Register - Multiply, on 40 bits
TMS320C55_amar_mpyr40,   // Parallel Modify Auxiliary Register - Multiply, and Round on 40 bits
TMS320C55_mac_mac,       // Two Parallel Multiply and Accumulate
TMS320C55_macr_macr,     // Two Parallel Multiply and Accumulate, and Round
TMS320C55_mac40_mac40,   // Two Parallel Multiply and Accumulate, on 40 bits
TMS320C55_macr40_macr40, // Two Parallel Multiply and Accumulate, and Round on 40 bits
TMS320C55_mas_mac,       // Parallel Multiply and Subtract - Multiply and Accumulate
TMS320C55_masr_macr,     // Parallel Multiply and Subtract - Multiply and Accumulate, and Round
TMS320C55_mas40_mac40,   // Parallel Multiply and Subtract - Multiply and Accumulate, on 40 bits
TMS320C55_masr40_macr40, // Parallel Multiply and Subtract - Multiply and Accumulate, and Round on 40 bits
TMS320C55_amar_mac,      // Parallel Modify Auxiliary Register - Multiply and Accumulate
TMS320C55_amar_macr,     // Parallel Modify Auxiliary Register - Multiply and Accumulate, and Round
TMS320C55_amar_mac40,    // Parallel Modify Auxiliary Register - Multiply and Accumulate, on 40 bits
TMS320C55_amar_macr40,   // Parallel Modify Auxiliary Register - Multiply and Accumulate, and Round on 40 bits
TMS320C55_mas_mas,       // Two Parallel Multiply and Subtract
TMS320C55_masr_masr,     // Two Parallel Multiply and Subtract, and Round
TMS320C55_mas40_mas40,   // Two Parallel Multiply and Subtract, on 40 bits
TMS320C55_masr40_masr40, // Two Parallel Multiply and Subtract, and Round on 40 bits
TMS320C55_amar_mas,      // Parallel Modify Auxiliary Register - Multiply and Subtract
TMS320C55_amar_masr,     // Parallel Modify Auxiliary Register - Multiply and Subtract, and Round
TMS320C55_amar_mas40,    // Parallel Modify Auxiliary Register - Multiply and Subtract, on 40 bits
TMS320C55_amar_masr40,   // Parallel Modify Auxiliary Register - Multiply and Subtract, and Round on 40 bits
TMS320C55_mpy_mac,       // Parallel Multiply - Multiply and Accumulate
TMS320C55_mpyr_macr,     // Parallel Multiply - Multiply and Accumulate, and Round
TMS320C55_mpy40_mac40,   // Parallel Multiply - Multiply and Accumulate, on 40 bits
TMS320C55_mpyr40_macr40, // Parallel Multiply - Multiply and Accumulate, and Round on 40 bits
TMS320C55_amar3,         // Three Parallel Modify Auxiliary Registers

TMS320C55_firsadd,       // Parallel Multiply and Accumulate - Add
TMS320C55_firssub,       // Parallel Multiply and Accumulate - Subtract

TMS320C55_mpym_mov,      // Parallel Multiply - Store
TMS320C55_mpymr_mov,     // Parallel Multiply - Store, and Round
TMS320C55_macm_mov,      // Parallel Multiply and Accumulate - Store
TMS320C55_macmr_mov,     // Parallel Multiply and Accumulate - Store, and Round
TMS320C55_masm_mov,      // Parallel Multiply and Subtract - Store
TMS320C55_masmr_mov,     // Parallel Multiply and Subtract - Store, and Round
TMS320C55_add_mov,       // Parallel Add - Store
TMS320C55_sub_mov,       // Parallel Subtract - Store
TMS320C55_mov_mov,       // Parallel Load - Store
TMS320C55_mov_aadd,      // Parallel Store - aadd
TMS320C55_mov_add,       // Parallel Store - Add
TMS320C55_amar_amar,     // Parallel Modify Auxiliary Register - Modify Auxiliary Register
TMS320C55_add_asub,      // Parallel Add - asub
TMS320C55_btst_mov,      // Parallel Bit Test - Store
TMS320C55_mov_asub,      // Parallel Store - asub

TMS320C55_lms,           // Least Mean Square

TMS320C55_max1,          // Maximum Comparison
TMS320C55_max2,          // Maximum Comparison
TMS320C55_min1,          // Minimum Comparison
TMS320C55_min2,          // Minimum Comparison

TMS320C55_cmp,           // Memory Comparison
TMS320C55_cmpu,          // Unsigned memory Comparison

TMS320C55_aadd,          // Add Two Registers
TMS320C55_asub,          // Subtract Two Registers
TMS320C55_amov,          // Move From Register to Register
TMS320C55_amar1,         // Auxiliary Register Modification

TMS320C55_sqr1,          // Square
TMS320C55_sqr2,          // Square
TMS320C55_sqrr1,         // Square and Round
TMS320C55_sqrr2,         // Square and Round
TMS320C55_mpy1,          // Multiply
TMS320C55_mpy2,          // Multiply
TMS320C55_mpy3,          // Multiply
TMS320C55_mpyr1,         // Multiply and Round
TMS320C55_mpyr2,         // Multiply and Round
TMS320C55_mpyr3,         // Multiply and Round
TMS320C55_mpyk2,         // Multiply by Constant
TMS320C55_mpyk3,         // Multiply by Constant
TMS320C55_mpykr2,        // Multiply by Constant and Round
TMS320C55_mpykr3,        // Multiply by Constant and Round
TMS320C55_mpym2,         // Multiply Memory Value
TMS320C55_mpym3,         // Multiply Memory Values
TMS320C55_mpymr2,        // Multiply Memory Value and Round
TMS320C55_mpymr3,        // Multiply Memory Values and Round
TMS320C55_mpym403,       // Multiply Memory Values on 40 bits
TMS320C55_mpymr403,      // Multiply Memory Values and Round on 40 bits
TMS320C55_mpymu3,        // Unsigned multiply Memory Values
TMS320C55_mpymru3,       // Unsigned multiply Memory Values and Round
TMS320C55_sqrm,          // Square Memory Value
TMS320C55_sqrmr,         // Square Memory Value, and Round
TMS320C55_mpymk,         // Multiply Memory Value by Constant
TMS320C55_mpymkr,        // Multiply Memory Value by Constant and Round

TMS320C55_sqa1,          // Square and Accumulate
TMS320C55_sqa2,          // Square and Accumulate
TMS320C55_sqar1,         // Square, Accumulate and Round
TMS320C55_sqar2,         // Square, Accumulate and Round
TMS320C55_mac3,          // Multiply and Accumulate
TMS320C55_mac4,          // Multiply and Accumulate
TMS320C55_macr3,         // Multiply, Accumulate and Round
TMS320C55_macr4,         // Multiply, Accumulate and Round
TMS320C55_mack3,         // Multiply by Constant and Accumulate
TMS320C55_mack4,         // Multiply by Constant and Accumulate
TMS320C55_mackr3,        // Multiply by Constant, Round and Accumulate
TMS320C55_mackr4,        // Multiply by Constant, Round and Accumulate
TMS320C55_macm2,         // Multiply and Accumulate Memory Values
TMS320C55_macm3,         // Multiply and Accumulate Memory Values
TMS320C55_macm4,         // Multiply and Accumulate Memory Values
TMS320C55_macmr2,        // Multiply and Accumulate Memory Values, and Round
TMS320C55_macmr3,        // Multiply and Accumulate Memory Values, and Round
TMS320C55_macmr4,        // Multiply and Accumulate Memory Values, and Round
TMS320C55_macm403,       // Multiply and Accumulate Memory Values, on 40 bits
TMS320C55_macm404,       // Multiply and Accumulate Memory Values, on 40 bits
TMS320C55_macmr403,      // Multiply and Accumulate Memory Values, and Round on 40 bits
TMS320C55_macmr404,      // Multiply and Accumulate Memory Values, and Round on 40 bits
TMS320C55_macmz,         // Multiply and Accumulate Memory Values
TMS320C55_macmrz,        // Multiply and Accumulate Memory Values, and Round
TMS320C55_sqam2,         // Square and Accumulate Memory Value
TMS320C55_sqam3,         // Square and Accumulate Memory Values
TMS320C55_sqamr2,        // Square and Accumulate Memory Value, and Round
TMS320C55_sqamr3,        // Square and Accumulate Memory Values, and Round
TMS320C55_macmk3,        // Multiply Memory Value by Constant and Accumulate
TMS320C55_macmk4,        // Multiply Memory Value by Constant and Accumulate
TMS320C55_macmkr3,       // Multiply Memory Value by Constant - Accumulate, and Round
TMS320C55_macmkr4,       // Multiply Memory Value by Constant - Accumulate, and Round

TMS320C55_sqs1,          // Square and Subtract
TMS320C55_sqs2,          // Square and Subtract
TMS320C55_sqsr1,         // Square, Subtract and Round
TMS320C55_sqsr2,         // Square, Subtract and Round

TMS320C55_mas2,          // Multiply and Subtract
TMS320C55_mas3,          // Multiply and Subtract
TMS320C55_masr2,         // Multiply, Subtract and Round
TMS320C55_masr3,         // Multiply, Subtract and Round
TMS320C55_masm2,         // Multiply and Subtract Memory Value
TMS320C55_masm3,         // Multiply and Subtract Memory Values
TMS320C55_masm4,         // Multiply and Subtract Memory Values
TMS320C55_masmr2,        // Multiply and Subtract Memory Values, and Round
TMS320C55_masmr3,        // Multiply and Subtract Memory Values, and Round
TMS320C55_masmr4,        // Multiply and Subtract Memory Values, and Round
TMS320C55_masm403,       // Multiply and Subtract Memory Values, on 40 bits
TMS320C55_masm404,       // Multiply and Subtract Memory Values, on 40 bits
TMS320C55_masmr403,      // Multiply and Subtract Memory Values, and Round on 40 bits
TMS320C55_masmr404,      // Multiply and Subtract Memory Values, and Round on 40 bits
TMS320C55_sqsm2,         // Square and Subtract Memory Values
TMS320C55_sqsm3,         // Square and Subtract Memory Values
TMS320C55_sqsmr2,        // Square and Subtract Memory Values, and Round
TMS320C55_sqsmr3,        // Square and Subtract Memory Values, and Round

TMS320C55_neg1,          // Negation
TMS320C55_neg2,          // Negation

TMS320C55_mant_nexp,     // Exponent and Mantissa
TMS320C55_exp,           // Exponent

TMS320C55_cmpand,        // Compare and AND
TMS320C55_cmpandu,       // Unsigned compare and AND
TMS320C55_cmpor,         // Compare and OR
TMS320C55_cmporu,        // Unsigned compare and OR

TMS320C55_round1,        // Round
TMS320C55_round2,        // Round

TMS320C55_sat1,          // Saturate
TMS320C55_sat2,          // Saturate
TMS320C55_satr1,         // Saturate and Round
TMS320C55_satr2,         // Saturate and Round

TMS320C55_sfts2,         // Signed Shift
TMS320C55_sfts3,         // Signed Shift
TMS320C55_sftsc2,        // Signed Shift with Carry
TMS320C55_sftsc3,        // Signed Shift with Carry

TMS320C55_sqdst,         // Square distance

TMS320C55_sub1,          // Subtract
TMS320C55_sub2,          // Subtract
TMS320C55_sub3,          // Subtract
TMS320C55_sub4,          // Subtract

TMS320C55_band,          // Bit Field Comparison

TMS320C55_bfxpa,         // Bit Field Expand

TMS320C55_bfxtr,         // Bit Field Extract

TMS320C55_btst,          // Bit Test
TMS320C55_bnot,          // Bit NOT
TMS320C55_bclr2,         // Bit Clear
TMS320C55_bset2,         // Bit Set
TMS320C55_btstset,       // Bit Test and Set
TMS320C55_btstclr,       // Bit Test and Clear
TMS320C55_btstnot,       // Bit Test and NOT
TMS320C55_btstp,         // Bit Pair Test
TMS320C55_bclr1,         // Bit Clear
TMS320C55_bset1,         // Bit Set

TMS320C55_amar2,         // Load Effective Address to Extended Auxiliary Register
TMS320C55_popboth,       // Pop Extended Auxiliary Register from Stack Pointers
TMS320C55_pshboth,       // Push Extended Auxiliary Register to Stack Pointers

// LOGICAL OPERATIONS

TMS320C55_bcnt,          // Count Bit Field

TMS320C55_not1,          // NOT
TMS320C55_not2,          // NOT

TMS320C55_and1,          // AND
TMS320C55_and2,          // AND
TMS320C55_and3,          // AND

TMS320C55_or1,           // OR
TMS320C55_or2,           // OR
TMS320C55_or3,           // OR

TMS320C55_xor1,          // XOR
TMS320C55_xor2,          // XOR
TMS320C55_xor3,          // XOR

TMS320C55_sftl2,         // Logical Shift
TMS320C55_sftl3,         // Logical Shift

TMS320C55_rol,           // Rotate Left

TMS320C55_ror,           // Rotate Right

// MISCELLANEOUS OPERATIONS

// MOVE OPERATIONS

TMS320C55_swap,          // Swap Registers
TMS320C55_swapp,         // Swap Pair Registers
TMS320C55_swap4,         // Swap 4 Registers

TMS320C55_mov2,          // Move Data
TMS320C55_mov3,          // Move 2 Data
TMS320C55_mov402,        // Move Data on 40 bits

TMS320C55_delay,         // Memory Delay

TMS320C55_pop1,          // Pop Top of Stack1
TMS320C55_pop2,          // Pop Top of Stack2

TMS320C55_psh1,          // Pop Top of Stack3
TMS320C55_psh2,          // Pop Top of Stack4

// PROGRAM CONTROL OPERATIONS

TMS320C55_bcc,           // Branch Conditionally
TMS320C55_bccu,          // Branch Conditionally

TMS320C55_b,             // Branch Unconditionally

TMS320C55_callcc,        // Call Conditionally

TMS320C55_call,          // Call Unconditionally

TMS320C55_xcc,           // Execute Conditionally
TMS320C55_xccpart,       // Execute Conditionally

TMS320C55_idle,          // Idle

TMS320C55_nop,           // No Operation
TMS320C55_nop_16,        // No Operation

TMS320C55_rptblocal,     // Repeat Block of Instructions Unconditionally
TMS320C55_rptb,          // Repeat Block of Instructions Unconditionally

TMS320C55_rptcc,         // Repeat Single Instruction Conditionally

TMS320C55_rpt,           // Repeat Single Instruction Unconditionally
TMS320C55_rptadd,        // Repeat Single Instruction Unconditionally and Add to Register
TMS320C55_rptsub,        // Repeat Single Instruction Unconditionally and Subtract to Register

TMS320C55_retcc,         // Return Conditionally
TMS320C55_ret,           // Return Unconditionally
TMS320C55_reti,          // Return from Interrupt

TMS320C55_intr,          // Software Interrupt

TMS320C55_reset,         // Software Reset

TMS320C55_trap,          // Software Trap

TMS320C55_last

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
TRIMEDIA_null,

TRIMEDIA_igtri,         // signed compare greater with immediate
TRIMEDIA_igeqi,         // signed compare greater or equal with immediate
TRIMEDIA_ilesi,         // signed compare less with immediate
TRIMEDIA_ineqi,         // signed compare not equal with immediate
TRIMEDIA_ieqli,         // signed compare equal with immediate
TRIMEDIA_iaddi,         // add with immediate
TRIMEDIA_ild16d,        // signed 16-bit load with displacement
TRIMEDIA_ld32d,         // 32-bit load with displacement
TRIMEDIA_uld8d,         // unsigned 8-bit load with displacement
TRIMEDIA_lsri,          // logical shift right immediate
TRIMEDIA_asri,          // arithmetic shift right immediate
TRIMEDIA_asli,          // arithmetic shift left immediate
TRIMEDIA_iadd,          // signed add
TRIMEDIA_isub,          // signed subtract
TRIMEDIA_igeq,          // signed compare greater or equal
TRIMEDIA_igtr,          // signed compare greater
TRIMEDIA_bitand,        // bitwise logical AND
TRIMEDIA_bitor,         // bitwise logical OR
TRIMEDIA_asr,           // arithmetic shift right
TRIMEDIA_asl,           // arithmetic shift left
TRIMEDIA_ifloat,        // convert signed integer to floating-point
TRIMEDIA_ifixrz,        // convert floating-point to integer with round toward zero
TRIMEDIA_fadd,          // floating-point add
TRIMEDIA_imin,          // signed minimum
TRIMEDIA_imax,          // signed maximum
TRIMEDIA_iavgonep,      // signed average
TRIMEDIA_ume8uu,        // sum of absolute values of unsigned 8-bit differences
TRIMEDIA_imul,          // signed multiply
TRIMEDIA_fmul,          // floating-point multiply
TRIMEDIA_h_st8d,        // hardware 8-bit store with displacement
TRIMEDIA_h_st16d,       // hardware 16-bit store with displacement
TRIMEDIA_h_st32d,       // hardware 32-bit store with displacement
TRIMEDIA_isubi,         // subtract with immediate
TRIMEDIA_ugtr,          // unsigned compare greater
TRIMEDIA_ugtri,         // unsigned compare greater with immediate
TRIMEDIA_ugeq,          // unsigned compare greater or equal
TRIMEDIA_ugeqi,         // unsigned compare greater or equal with immediate
TRIMEDIA_ieql,          // signed compare equal
TRIMEDIA_ueqli,         // unsigned compare equal with immediate
TRIMEDIA_ineq,          // signed compare not equal
TRIMEDIA_uneqi,         // unsigned compare not equal
TRIMEDIA_ulesi,         // unsigned compare less with immediate
TRIMEDIA_ileqi,         // signed compare less or equal with immediate
TRIMEDIA_uleqi,         // unsigned compare less or equal with immediate
TRIMEDIA_h_iabs,        // hardware absolute value
TRIMEDIA_carry,         // compute carry bit from unsigned add
TRIMEDIA_izero,         // if zero select zero
TRIMEDIA_inonzero,      // if nonzero select zero
TRIMEDIA_bitxor,        // bitwise logical exclusive OR
TRIMEDIA_bitandinv,     // bitwise logical AND NOT
TRIMEDIA_bitinv,        // bitwise logical NOT
TRIMEDIA_sex16,         // sign extend 16 bits
TRIMEDIA_packbytes,     // pack least-significant bytes
TRIMEDIA_pack16lsb,     // pack least-significant 16-bit halfwords
TRIMEDIA_pack16msb,     // pack most-significant 16-bit halfwords
TRIMEDIA_ubytesel,      // select unsigned byte
TRIMEDIA_ibytesel,      // signed select byte
TRIMEDIA_mergelsb,      // merge least-significant byte
TRIMEDIA_mergemsb,      // merge most-significant byte
TRIMEDIA_ume8ii,        // unsigned sum of absolute values of signed 8-bit differences
TRIMEDIA_h_dspiabs,     // clipped signed absolute value
TRIMEDIA_dspiadd,       // clipped signed add
TRIMEDIA_dspuadd,       // clipped unsigned add
TRIMEDIA_dspisub,       // clipped signed subtract
TRIMEDIA_dspusub,       // clipped unsigned subtract
TRIMEDIA_dspidualadd,   // dual clipped add of signed 16-bit halfwords
TRIMEDIA_dspidualsub,   // dual clipped subtract of signed 16-bit halfwords
TRIMEDIA_h_dspidualabs, // dual clipped absolute value of signed 16-bit halfwords
TRIMEDIA_quadavg,       // unsigned byte-wise quad average
TRIMEDIA_iclipi,        // clip signed to signed
TRIMEDIA_uclipi,        // clip signed to unsigned
TRIMEDIA_uclipu,        // clip unsigned to unsigned
TRIMEDIA_iflip,         // if non-zero negate
TRIMEDIA_dspuquadaddui, // quad clipped add of unsigned/signed bytes
TRIMEDIA_quadumin,      // unsigned byte-wise quad minimum
TRIMEDIA_quadumax,      // unsigned byte-wise quad maximum
TRIMEDIA_dualiclipi,    // dual-16 clip signed to signed
TRIMEDIA_dualuclipi,    // dual-16 clip signed to unsigned
TRIMEDIA_quadumulmsb,   // unsigned quad 8-bit multiply most significant
TRIMEDIA_ufir8uu,       // unsigned sum of products of unsigned bytes
TRIMEDIA_ifir8ui,       // signed sum of products of unsigned/signed bytes
TRIMEDIA_ifir8ii,       // signed sum of products of signed bytes
TRIMEDIA_ifir16,        // sum of products of signed 16-bit halfwords
TRIMEDIA_ufir16,        // sum of products of unsigned 16-bit halfwords
TRIMEDIA_dspidualmul,   // dual clipped multiply of signed 16-bit halfwords
TRIMEDIA_lsr,           // logical shift right
TRIMEDIA_rol,           // rotate left
TRIMEDIA_roli,          // rotate left by immediate
TRIMEDIA_funshift1,     // funnel-shift 1 byte
TRIMEDIA_funshift2,     // funnel-shift 2 bytes
TRIMEDIA_funshift3,     // funnel-shift 3 bytes
TRIMEDIA_dualasr,       // dual-16 arithmetic shift right
TRIMEDIA_mergedual16lsb,// merge dual 16-bit lsb bytes
TRIMEDIA_fdiv,          // floating-point divide
TRIMEDIA_fdivflags,     // IEEE status flags from floating-point divide
TRIMEDIA_fsqrt,         // floating-point square root
TRIMEDIA_fsqrtflags,    // IEEE status flags from floating-point square root
TRIMEDIA_faddflags,     // IEEE status flags from floating-point add
TRIMEDIA_fsub,          // floating-point subtract
TRIMEDIA_fsubflags,     // IEEE status flags from floating-point subtract
TRIMEDIA_fabsval,       // floating-point absolute value
TRIMEDIA_fabsvalflags,  // IEEE status flags from floating-point absolute value
TRIMEDIA_ifloatrz,      // convert signed integer to floating-point with rounding toward zero
TRIMEDIA_ifloatrzflags, // IEEE status flags from convert signed integer to floating-point with rounding toward zero
TRIMEDIA_ufloatrz,      // convert unsigned integer to floating-point with round toward zero
TRIMEDIA_ufloatrzflags, // IEEE status flags from convert unsigned integer to floating-point with round toward zero
TRIMEDIA_ifixieee,      // convert floating-point to integer using PCSW rounding mode
TRIMEDIA_ifixieeeflags, // IEEE status flags from convert floating-point to integer using PCSW rounding mode
TRIMEDIA_ufixieee,      // convert floating-point to unsigned integer using PCSW rounding mode
TRIMEDIA_ufixieeeflags, // IEEE status flags from convert floating-point to unsigned integer using PCSW rounding mode
TRIMEDIA_ufixrz,        // convert floating-point to unsigned integer with round toward zero
TRIMEDIA_ufixrzflags,   // IEEE status flags from convert floating-point to unsigned integer with round toward zero
TRIMEDIA_ufloat,        // convert unsigned integer to floating-point
TRIMEDIA_ufloatflags,   // IEEE status flags from convert unsigned integer to floating-point
TRIMEDIA_ifixrzflags,   // IEEE status flags from convert floating-point to integer with round toward zero
TRIMEDIA_ifloatflags,   // IEEE status flags from convert signed integer to floating-point
TRIMEDIA_umul,          // unsigned multiply
TRIMEDIA_imulm,         // signed multiply, return most-significant 32 bits
TRIMEDIA_umulm,         // unsigned multiply, return most-significant 32 bits
TRIMEDIA_dspimul,       // clipped signed multiply
TRIMEDIA_dspumul,       // clipped unsigned multiply
TRIMEDIA_fmulflags,     // IEEE status flags from floating-point multiply
TRIMEDIA_fgtr,          // floating-point compare greater
TRIMEDIA_fgtrflags,     // IEEE status flags from floating-point compare greater
TRIMEDIA_fgeq,          // floating-point compare greater or equal
TRIMEDIA_fgeqflags,     // IEEE status flags from floating-point compare greater or equal
TRIMEDIA_feql,          // floating-point compare equal
TRIMEDIA_feqlflags,     // IEEE status flags from floating-point compare equal
TRIMEDIA_fneq,          // floating-point compare not equal
TRIMEDIA_fneqflags,     // IEEE status flags from floating-point compare not equal
TRIMEDIA_fsign,         // sign of floating-point value
TRIMEDIA_fsignflags,    // IEEE status flags from floating-point sign
TRIMEDIA_cycles,        // read clock cycle counter, least-significant word
TRIMEDIA_hicycles,      // read clock cycle counter, most-significant word
TRIMEDIA_readdpc,       // read destination program counter
TRIMEDIA_readspc,       // read source program counter
TRIMEDIA_readpcsw,      // read program control and status word
TRIMEDIA_writespc,      // write source program counter
TRIMEDIA_writedpc,      // write destination program counter
TRIMEDIA_writepcsw,     // write program control and status word
TRIMEDIA_curcycles,     // read current clock cycle, least-significant word
TRIMEDIA_jmpt,          // indirect jump on true
TRIMEDIA_ijmpt,         // interruptible indirect jump on true
TRIMEDIA_jmpi,          // jump immediate
TRIMEDIA_ijmpi,         // interruptible jump immediate
TRIMEDIA_jmpf,          // indirect jump on false
TRIMEDIA_ijmpf,         // interruptible indirect jump on false
TRIMEDIA_iclr,          // invalidate all instruction cache blocks
TRIMEDIA_uimm,          // unsigned immediate
TRIMEDIA_ild8d,         // signed 8-bit load with displacement
TRIMEDIA_ild8r,         // signed 8-bit load with index
TRIMEDIA_uld8r,         // unsigned 8-bit load with index
TRIMEDIA_ild16r,        // signed 16-bit load with index
TRIMEDIA_ild16x,        // signed 16-bit load with scaled index
TRIMEDIA_uld16d,        // unsigned 16-bit load with displacement
TRIMEDIA_uld16r,        // unsigned 16-bit load with index
TRIMEDIA_uld16x,        // unsigned 16-bit load with scaled index
TRIMEDIA_ld32r,         // 32-bit load with index
TRIMEDIA_ld32x,         // 32-bit load with scaled index
TRIMEDIA_rdtag,         // read data cache address tag
TRIMEDIA_rdstatus,      // read data cache status bits
TRIMEDIA_dcb,           // data cache copy back
TRIMEDIA_dinvalid,      // invalidate data cache block
TRIMEDIA_prefd,         // prefetch with displacement
TRIMEDIA_prefr,         // prefetch with index
TRIMEDIA_pref16x,       // prefetch with 16-bit scaled index
TRIMEDIA_pref32x,       // prefetch with 32-bit scaled index
TRIMEDIA_allocd,        // allocate a cache block with displacement
TRIMEDIA_allocr,        // allocate a cache block with index
TRIMEDIA_allocx,        // allocate a cache block with scaled index
TRIMEDIA_nop,

// Pseudo-operations

TRIMEDIA_alloc,         // allocate a cache block
TRIMEDIA_dspiabs,       // clipped signed absolute value
TRIMEDIA_dspidualabs,   // dual clipped absolute value of signed 16-bit halfwords
TRIMEDIA_iabs,          // absolute value
TRIMEDIA_ild16,         // signed 16-bit load
TRIMEDIA_ild8,          // signed 8-bit load
TRIMEDIA_ineg,          // signed negate
TRIMEDIA_ld32,          // 32-bit load
TRIMEDIA_pref,          // prefetch
TRIMEDIA_sex8,          // sign extend 8 bits
TRIMEDIA_st16,          // 16-bit store
TRIMEDIA_st16d,         // 16-bit store with displacement
TRIMEDIA_st32,          // 32-bit store
TRIMEDIA_st32d,         // 32-bit store with displacement
TRIMEDIA_st8,           // 8-bit store
TRIMEDIA_st8d,          // 8-bit store with displacement
TRIMEDIA_uld16,         // unsigned 16-bit load
TRIMEDIA_uld8,          // unsigned 8-bit load
TRIMEDIA_zex16,         // zero extend 16 bits
TRIMEDIA_zex8,          // zero extend 8 bits
TRIMEDIA_ident,         // identity

// Unused pseudo-operations:

TRIMEDIA_iles,
TRIMEDIA_ileq,
TRIMEDIA_ules,
TRIMEDIA_uleq,
TRIMEDIA_fles,
TRIMEDIA_fleq,
TRIMEDIA_ueql,
TRIMEDIA_uneq,
TRIMEDIA_flesflags,
TRIMEDIA_fleqflags,
TRIMEDIA_borrow,
TRIMEDIA_umin,
TRIMEDIA_lsl,
TRIMEDIA_lsli,

TRIMEDIA_last,

    };

/*
 *      NEC 78K0 processor module for IDA.
 *      Copyright (c) 2006 Konstantin Norvatoff, <konnor@bk.ru>
 *      Freeware.
 */


extern instruc_t Instructions[];

enum 
{
NEC_78K_0_null = 0,       // Unknown Operation

NEC_78K_0_mov,            // Move Byte Data Transfer
NEC_78K_0_xch,            // Exchange Byte Data
NEC_78K_0_movw,           // Move Word Data Transfer / Word Data Transfer with Stack Pointer
NEC_78K_0_xchw,           // Exchange Word Data

NEC_78K_0_add,            // Add Byte Data Addition
NEC_78K_0_addc,           // Add with Carry Addition of Byte Data with Carry
NEC_78K_0_sub,            // Subtract Byte Data Subtraction
NEC_78K_0_subc,           // Subtract with Carry Subtraction of Byte Data with Carry
NEC_78K_0_and,            // And Logical Product of Byte Data
NEC_78K_0_or,             // Or Logical Sum of Byte Data
NEC_78K_0_xor,            // Exclusive Or Exclusive Logical Sum of Byte Data
NEC_78K_0_cmp,            // Compare Byte Data Comparison

NEC_78K_0_addw,           // Add Word Data Addition
NEC_78K_0_subw,           // Subtract Word Data Subtraction
NEC_78K_0_cmpw,           // Compare Word Data Comparison

NEC_78K_0_mulu,           // Multiply Unsigned Multiplication of Data
NEC_78K_0_divuw,          // Divide Unsigned Word Unsigned Division of Word Data

NEC_78K_0_inc,            // Increment Byte Data Increment
NEC_78K_0_dec,            // Decrement Byte Data Decrement
NEC_78K_0_incw,           // Increment Word Data Increment
NEC_78K_0_decw,           // Decrement Word Data Decrement

NEC_78K_0_ror,            // Rotate Right Byte Data Rotation to the Right
NEC_78K_0_rol,            // Rotate Left Byte Data Rotation to the Left
NEC_78K_0_rorc,           // Rotate Right with Carry Byte Data Rotation to the Right with Carry
NEC_78K_0_rolc,           // Rotate Left with Carry Byte Data Rotation to the Left with Carry
NEC_78K_0_ror4,           // Rotate Right Digit Digit Rotation to the Right
NEC_78K_0_rol4,           // Rotate Left Digit Digit Rotation to the Left

NEC_78K_0_adjba,          // Decimal Adjust Register for Addition Decimal Adjustment of Addition Result
NEC_78K_0_adjbs,          // Decimal Adjust Register for Subtraction Decimal Adjustment of Subtraction Result

NEC_78K_0_mov1,           // Move Single Bit 1 Bit Data Transfer
NEC_78K_0_and1,           // And Single Bit 1 Bit Data Logical Product
NEC_78K_0_or1,            // Or Single Bit 1 Bit Data Logical Sum
NEC_78K_0_xor1,           // Exclusive Or Single Bit 1 Bit Data Exclusive Logical Sum
NEC_78K_0_set1,           // Set Single Bit (Carry Flag) 1 Bit Data Set
NEC_78K_0_clr1,           // Clear Single Bit (Carry Flag) 1 Bit Data Clear
NEC_78K_0_not1,           // Not Single Bit (Carry Flag) 1 Bit Data Logical Negation

NEC_78K_0_call,           // Call Subroutine Call (16 Bit Direct)
NEC_78K_0_callf,          // Call Flag Subroutine Call (11 Bit Direct Specification)
NEC_78K_0_callt,          // Call Table Subroutine Call (Refer to the Call Table)
NEC_78K_0_brk,            // Break Software Vectored Interrupt
NEC_78K_0_ret,            // Return Return from Subroutine
NEC_78K_0_retb,           // Return from Interrupt Return from Hardware Vectored Interrupt
NEC_78K_0_reti,           // Return from Break Return from Software Vectored Interrupt

NEC_78K_0_push,           // Push
NEC_78K_0_pop,            // Pop

NEC_78K_0_br,             // Branch Unconditional Branch
NEC_78K_0_bc,             // Branch if Carry Conditional Branch with Carry Flag (CY = 1)
NEC_78K_0_bnc,            // Branch if Not Carry Conditional Branch with Carry Flag (CY = 0)
NEC_78K_0_bz,             // Branch if Zero Conditional Branch with Zero Flag (Z = 1)
NEC_78K_0_bnz,            // Branch if Not Zero Conditional Branch with Zero Flag (Z = 0)
NEC_78K_0_bt,             // Branch if True Conditional Branch by Bit Test (Byte Data Bit = 1)
NEC_78K_0_bf,             // Branch if False Conditional Branch by Bit Test (Byte Data Bit = 0)
NEC_78K_0_btclr,          // Branch if True and Clear Conditional Branch and Clear by Bit Test (Byte Data Bit = 1)
NEC_78K_0_dbnz,           // Decrement and Branch if Not Zero Conditional Loop (R1!= 0)

NEC_78K_0_sel,            // Select Register Bank Register Bank Selection


NEC_78K_0_nop,            // No Operation
NEC_78K_0_EI,             // Enable Interrupt
NEC_78K_0_DI,             // Disable Interrupt
NEC_78K_0_HALT,           // HALT Mode Set
NEC_78K_0_STOP,           // Stop Mode Set


NEC_78K_0_last

    };

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
NEC_78K_0S_null = 0,           // Unknown Operation
NEC_78K_0S_cmp,                // Compare Byte Data Comparison
NEC_78K_0S_xor,                // Exclusive Or Exclusive Logical Sum of Byte Data
NEC_78K_0S_and,                // AND Logical Product of Byte Data
NEC_78K_0S_or,                 // OR Logical Sum of Byte Data
NEC_78K_0S_add,                // ADD Byte Data Addition
NEC_78K_0S_sub,                // Subtract Byte Data Subtraction
NEC_78K_0S_addc,               // Add with Carry Addition of Byte Data with Carry
NEC_78K_0S_subc,               // Subtract with Carry Subtraction of Byte Data with Carry
NEC_78K_0S_subw,               // Subtract Word Data Subtraction
NEC_78K_0S_addw,               // Add Word Data Addition
NEC_78K_0S_cmpw,               // Compare Word Data Comparison
NEC_78K_0S_inc,                // Increment Byte Data Increment
NEC_78K_0S_dec,                // Decrement Byte Data Decrement
NEC_78K_0S_incw,               // Increment Word Data Increment
NEC_78K_0S_decw,               // Decrement Word Data Decrement
NEC_78K_0S_ror,                // Rotate Right Byte Data Rotation to the Right
NEC_78K_0S_rol,                // Rotate Left Byte Data Rotation to the Left
NEC_78K_0S_rorc,               // Rotate Right with Carry Byte Data Rotation to the Right with Carry
NEC_78K_0S_rolc,               // Rotate Left with Carry Byte Data Rotation to the Left with Carry
NEC_78K_0S_call,               // CALL Subroutine Call (16 Bit Direct)
NEC_78K_0S_callt,              // Call Table Subroutine Call (Call Table Reference)
NEC_78K_0S_ret,                // Return from Subroutine
NEC_78K_0S_reti,               // Return from Interrupt / Return from Hardware Vectored Interrupt
NEC_78K_0S_mov,                // Move Byte Data Transfer
NEC_78K_0S_xch,                // Exchange Byte Data Exchange
NEC_78K_0S_xchw,               // Exchange Word Data Exchange
NEC_78K_0S_set1,               // Set Single Bit (Carry Flag) 1 Bit Data Set
NEC_78K_0S_clr1,               // Clear Single Bit (Carry Flag) 1 Bit Data Clear
NEC_78K_0S_not1,               // Not Single Bit (Carry Flag) 1 Bit Data Logical Negation
NEC_78K_0S_push,               // Push
NEC_78K_0S_pop,                // Pop
NEC_78K_0S_movw,               // Move Word Data Transfer / Word Data Transfer with Stack Pointer
NEC_78K_0S_br,                 // Unconditional Branch
NEC_78K_0S_bc,                 // Branch if Carry Conditional Branch with Carry Flag (CY = 1)
NEC_78K_0S_bnc,                // Branch if Not Carry Conditional Branch with Carry Flag (CY = 0)
NEC_78K_0S_bz,                 // Branch if Zero Conditional Branch with Zero Flag (Z = 1)
NEC_78K_0S_bnz,                // Branch if Not Zero Conditional Branch with Zero Flag (Z = 0)
NEC_78K_0S_bt,                 // Branch if True Conditional Branch by Bit Test (Byte Data Bit = 1)
NEC_78K_0S_bf,                 // Branch if False Conditional Branch by Bit Test (Byte Data Bit = 0)
NEC_78K_0S_dbnz,               // Decrement and Branch if Not Zero Conditional Loop (R1 != 0)
NEC_78K_0S_nop,                // No Operation
NEC_78K_0S_EI,                 // Enable Interrupt
NEC_78K_0S_DI,                 // Disable Interrupt
NEC_78K_0S_HALT,               // HALT Mode Set
NEC_78K_0S_STOP,               // Stop Mode Set
NEC_78K_0S_last
};



extern struct instruc_t Instructions[];

// The instruction types (``itype''s)
// M16C/60, M16C/20, M16C/Tiny  CPUs implements.
//
// Please, don't put anything else in this file but the itypes: 
// IDA possesses build scripts that 
// will parse ins.hpp files, and build a database of
// <cpu> + <itype> -> <autocomment>
// Where 'autocomment' is the contents of the comment string 
// associated to the itype.

enum m16c_itype_t
{
  M16C_null=0,      // Unknown Operation
  M16C_abs,         // Absolute value
  M16C_adc,         // Add with carry
  M16C_adcf,        // Add carry flag
  M16C_add,         // Add without carry
  M16C_adjnz,       // Add & conditional jump
  M16C_and,         // Logically AND
  M16C_band,        // Logically AND bits
  M16C_bclr,        // Clear bit
  M16C_bmcnd,       // Conditional bit transfer
  // conds (order is important; don't shuffle)
  M16C_bmgeu,       // Conditional bit transfer: Equal to or greater than; C == 1
  M16C_bmgtu,       // Conditional bit transfer: Greater than
  M16C_bmeq,        // Conditional bit transfer: Equal to; Z == 1
  M16C_bmn,         // Conditional bit transfer: Negative
  M16C_bmle,        // Conditional bit transfer: Equal to or smaller than (signed)
  M16C_bmo,         // Conditional bit transfer: Overflow; O == 1
  M16C_bmge,        // Conditional bit transfer: Equal to or greater than (signed)
  M16C_bmltu,       // Conditional bit transfer: Smaller than; C == 0
  M16C_bmleu,       // Conditional bit transfer: Equal to or smaller than
  M16C_bmne,        // Conditional bit transfer: Not equal; Z == 0
  M16C_bmpz,        // Conditional bit transfer: Positive or zero
  M16C_bmgt,        // Conditional bit transfer: Greater than (signed)
  M16C_bmno,        // Conditional bit transfer: No overflow; O == 0
  M16C_bmlt,        // Conditional bit transfer: Smaller than (signed)
  // ---
  M16C_bnand,       // Logically AND inverted bits
  M16C_bnor,        // Logically OR inverted bits
  M16C_bnot,        // Invert bit
  M16C_bntst,       // Test inverted bit
  M16C_bnxor,       // Exclusive OR inverted bits
  M16C_bor,         // Logically OR bits
  M16C_brk,         // Debug interrupt
  M16C_bset,        // Set bit
  M16C_btst,        // Test bit
  M16C_btstc,       // Test bit & clear
  M16C_btsts,       // Test bit & set
  M16C_bxor,        // Exclusive OR bits
  M16C_cmp,         // Compare
  M16C_dadc,        // Decimal add with carry
  M16C_dadd,        // Decimal add without carry
  M16C_dec,         // Decrement
  M16C_div,         // Signed divide
  M16C_divu,        // Unsigned divide
  M16C_divx,        // Signed divide (extension)
  M16C_dsbb,        // Decimal subtract with borrow
  M16C_dsub,        // Decimal subtract without borrow
  M16C_enter,       // Build stack frame
  M16C_exitd,       // Deallocate stack frame
  M16C_exts,        // Extend sign
  M16C_fclr,        // Clear flag register bit
  M16C_fset,        // Set flag register bit
  M16C_inc,         // Increment
  M16C_int,         // Interrupt by INT instruction
  M16C_into,        // Interrupt on overflow
  M16C_jcnd,        // Jump on condition
  // conds (order is important; don't shuffle)
  M16C_jgeu,        // Conditional jump: Equal to or greater than; C == 1
  M16C_jgtu,        // Conditional jump: Greater than
  M16C_jeq,         // Conditional jump: Equal to; Z == 1
  M16C_jn,          // Conditional jump: Negative
  M16C_jle,         // Conditional jump: Equal to or smaller than (signed)
  M16C_jo,          // Conditional jump: Overflow; O == 1
  M16C_jge,         // Conditional jump: Equal to or greater than (signed)
  M16C_jltu,        // Conditional jump: Smaller than; C == 0
  M16C_jleu,        // Conditional jump: Equal to or smaller than
  M16C_jne,         // Conditional jump: Not equal; Z == 0
  M16C_jpz,         // Conditional jump: Positive or zero
  M16C_jgt,         // Conditional jump: Greater than (signed)
  M16C_jno,         // Conditional jump: No overflow; O == 0
  M16C_jlt,         // Conditional jump: Smaller than (signed)
  // ---
  M16C_jmp,         // Unconditional jump
  M16C_jmpi,        // Jump indirect
  M16C_jmps,        // Jump to special page
  M16C_jsr,         // Subroutine call
  M16C_jsri,        // Indirect subroutine call
  M16C_jsrs,        // Special page subroutine call
  M16C_ldc,         // Transfer to control register
  M16C_ldctx,       // Restore context
  M16C_lde,         // Transfer from extended data area
  M16C_ldintb,      // Load to INTB register
  M16C_ldipl,       // Set interrupt enable level
  M16C_mov,         // Transfer
  M16C_mova,        // Transfer effective address
  M16C_movhh,       // Xfer 4-bit data: src(8-bits)'s 4 hi bits -> dest(8-bits)'s 4 hi bits.
  M16C_movhl,       // Xfer 4-bit data: src(8-bits)'s 4 hi bits -> dest(8-bits)'s 4 lo bits.
  M16C_movlh,       // Xfer 4-bit data: src(8-bits)'s 4 lo bits -> dest(8-bits)'s 4 hi bits.
  M16C_movll,       // Xfer 4-bit data: src(8-bits)'s 4 lo bits -> dest(8-bits)'s 4 lo bits.
  M16C_mul,         // Signed multiply
  M16C_mulu,        // Unsigned multiply
  M16C_neg,         // Two's complement
  M16C_nop,         // No operation
  M16C_not,         // Invert all bits
  M16C_or,          // Logically OR
  M16C_pop,         // Restore register/memory
  M16C_popc,        // Restore control register
  M16C_popm,        // Restore multiple registers
  M16C_push,        // Save register/memory/immediate data
  M16C_pusha,       // Save effective address
  M16C_pushc,       // Save control register
  M16C_pushm,       // Save multiple registers
  M16C_reit,        // Return from interrupt
  M16C_rmpa,        // Calculate sum-of-products
  M16C_rolc,        // Rotate left with carry
  M16C_rorc,        // Rotate right with carry
  M16C_rot,         // Rotate
  M16C_rts,         // Return from subroutine
  M16C_sbb,         // Subtract with borrow
  M16C_sbjnz,       // Subtract & conditional jump
  M16C_sha,         // Shift arithmetic
  M16C_shl,         // Shift logical
  M16C_smovb,       // Transfer string backward
  M16C_smovf,       // Transfer string forward
  M16C_sstr,        // Store string
  M16C_stc,         // Transfer from control register
  M16C_stctx,       // Save context
  M16C_ste,         // Store to extended data area
  M16C_stnz,        // Conditional transfer: store on not zero
  M16C_stz,         // Conditional transfer: store on zero
  M16C_stzx,        // Conditional transfer: store on zero extension
  M16C_sub,         // Subtract without borrow
  M16C_tst,         // Test
  M16C_und,         // Interrupt for undefined instruction
  M16C_wait,        // Wait
  M16C_xchg,        // Exchange
  M16C_xor,         // Exclusive OR
  M16C_last  
};







// exporting the ins.cpp array
extern instruc_t Instructions[];

// m32r instructions declaration
enum 
{
    m32r_null = 0,     // Null instruction
    m32r_add,          // Add
    m32r_add3,         // Add 3-operand
    m32r_addi,         // Add immediate
    m32r_addv,         // Add with overflow checking
    m32r_addv3,        // Add 3-operand with overflow checking
    m32r_addx,         // Add with carry
    m32r_and,          // AND
    m32r_and3,         // AND 3-operand
    m32r_bc,           // Branch on C-bit
    m32r_beq,          // Branch on equal
    m32r_beqz,         // Branch on equal zero
    m32r_bgez,         // Branch on greater than or equal zero
    m32r_bgtz,         // Branch on greater than zero
    m32r_bl,           // Branch and link
    m32r_blez,         // Branch on less than or equal zero
    m32r_bltz,         // Branch on less than zero
    m32r_bnc,          // Branch on not C-bit
    m32r_bne,          // Branch on not equal
    m32r_bnez,         // Branch on not equal zero
    m32r_bra,          // Branch
    m32r_cmp,          // Compare
    m32r_cmpi,         // Compare immediate
    m32r_cmpu,         // Compare unsigned
    m32r_cmpui,        // Compare unsigned immediate
    m32r_div,          // Divide
    m32r_divu,         // Divide unsigned
    m32r_jl,           // Jump and link
    m32r_jmp,          // Jump
    m32r_ld,           // Load
    m32r_ld24,         // Load 24-bit immediate
    m32r_ldb,          // Load byte
    m32r_ldh,          // Load halfword
    m32r_ldi,          // Load immediate
    m32r_ldub,         // Load unsigned byte
    m32r_lduh,         // Load unsigned halfword
    m32r_lock,         // Load locked
    m32r_machi,        // Multiply-accumulate high-order halfwords
    m32r_maclo,        // Multiply-accumulate low-order halfwords
    m32r_macwhi,       // Multiply-accumulate word and high-order halfword
    m32r_macwlo,       // Multiply-accumulate word and low-order halfword
    m32r_mul,          // Multiply
    m32r_mulhi,        // Multiply high-order halfwords
    m32r_mullo,        // Multiply low-order halfwords
    m32r_mulwhi,       // Multiply word high-order halfwords
    m32r_mulwlo,       // Multiply word low-order halfwords
    m32r_mv,           // Move register
    m32r_mvfachi,      // Move from accumulator high-order word
    m32r_mvfaclo,      // Move from accumulator low-order word
    m32r_mvfacmi,      // Move from accumulator middle-order word
    m32r_mvfc,         // Move from control register
    m32r_mvtachi,      // Move to accumulator high-order word
    m32r_mvtaclo,      // Move to accumulator low-order word
    m32r_mvtc,         // Move to control register
    m32r_neg,          // Negate
    m32r_nop,          // No operation
    m32r_not,          // Logical NOT
    m32r_or,           // OR
    m32r_or3,          // OR 3-operand
    m32r_push,         // Push, mnem for st reg, @-sp
    m32r_pop,          // Pop, mnem for ld reg, @sp+
    m32r_rac,          // Round accumulator
    m32r_rach,         // Round accumulator halfword
    m32r_rem,          // Remainder
    m32r_remu,         // Remainder unsigned
    m32r_rte,          // Return from EIT
    m32r_seth,         // Set high-order 16-bit
    m32r_sll,          // Shift left logical
    m32r_sll3,         // Shift left logical 3-operand
    m32r_slli,         // Shift left logical immediate
    m32r_sra,          // Shirt right arithmetic
    m32r_sra3,         // Shirt right arithmetic 3-operand
    m32r_srai,         // Shirt right arithmetic immediate
    m32r_srl,          // Shift right logical
    m32r_srl3,         // Shift right logical 3-operand
    m32r_srli,         // Shift right logical immediate
    m32r_st,           // Store
    m32r_stb,          // Store byte
    m32r_sth,          // Store halfword
    m32r_sub,          // Substract
    m32r_subv,         // Substract with overflow checking
    m32r_subx,         // Substract with borrow
    m32r_trap,         // Trap
    m32r_unlock,       // Store unlocked
    m32r_xor,          // Exclusive OR
    m32r_xor3,         // Exclusive OR 3-operand

    // M32RX :

    m32rx_bcl,
    m32rx_bncl,
    m32rx_cmpeq,
    m32rx_cmpz,
    m32rx_divh,
    m32rx_jc,
    m32rx_jnc,
    m32rx_machi,         // 'machi' 3-operand
    m32rx_maclo,         // 'maclo' 3-operand
    m32rx_macwhi,        // 'macwhi' 3-operand
    m32rx_macwlo,        // 'macwlo' 3-operand
    m32rx_mulhi,         // 'mulhi' 3-operand
    m32rx_mullo,         // 'mullo' 3-operand
    m32rx_mulwhi,        // 'mulwhi' 3-operand
    m32rx_mulwlo,        // 'mulwlo' 3-operand
    m32rx_mvfachi,       // 'mvfachi' 3-operand
    m32rx_mvfaclo,       // 'mvfaclo' 3-operand
    m32rx_mvfacmi,       // 'mvfacmi' 3-operand
    m32rx_mvtachi,       // 'mvtachi' 3-operand
    m32rx_mvtaclo,       // 'mvtaclo' 3-operand
    m32rx_rac,           // 'rac' 3 operand
    m32rx_rach,          // 'rach' 3 operand
    m32rx_satb,
    m32rx_sath,
    m32rx_sat,
    m32rx_pcmpbz,
    m32rx_sadd,
    m32rx_macwu1,
    m32rx_msblo,
    m32rx_mulwu1,
    m32rx_maclh1,
    m32rx_sc,
    m32rx_snc,

// Floating point
    m32r_fadd,           // Floating-point add
    m32r_fsub,           // Floating-point subtract
    m32r_fmul,           // Floating-point multiply
    m32r_fdiv,           // Floating-point divede
    m32r_fmadd,          // Floating-point multiply and add
    m32r_fmsub,          // Floating-point multiply and subtract
    m32r_itof,           // Integer to float
    m32r_utof,           // Unsigned integer to float
    m32r_ftoi,           // Float to integer
    m32r_ftos,           // Float to short
    m32r_fcmp,           // Floating-point compare
    m32r_fcmpe,          // Floating-point compare with exeption if unordered
// Bit Operation Instructions
    m32r_bset,           // Bit set
    m32r_bclr,           // Bit clear
    m32r_btst,           // Bit test
    m32r_setpsw,         // Set PSW
    m32r_clrpsw,         // Clear PSW

    m32r_last
};




extern instruc_t Instructions[];

enum 
{
    m740_null = 0,           // null instruction
    m740_adc,                // add with carry
    m740_and,                // logical and
    m740_asl,                // arithmetic shift left
    m740_bbc,                // branch on bit clear
    m740_bbs,                // branch on bit set
    m740_bcc,                // branch on carry clear
    m740_bcs,                // branch on carry set
    m740_beq,                // branch on equal
    m740_bit,                // test bit in memory with accumulator
    m740_bmi,                // branch on result minus
    m740_bne,                // branch on not equal
    m740_bpl,                // branch on result plus
    m740_bra,                // branch always
    m740_brk,                // force break
    m740_bvc,                // branch on overflow clear
    m740_bvs,                // branch on overflow set
    m740_clb,                // clear bit
    m740_clc,                // clear carry flag
    m740_cld,                // clear decimal mode
    m740_cli,                // clear interrupt disable status
    m740_clt,                // clear transfer flag
    m740_clv,                // clear overflow flag
    m740_cmp,                // compare
    m740_com,                // complement
    m740_cpx,                // compare memory and index register X
    m740_cpy,                // compare memory and index register Y
    m740_dec,                // decrement by one
    m740_dex,                // decrement index register X by one
    m740_dey,                // decrement index register Y by one
    m740_div,                // divide memory by accumulator
    m740_eor,                // exclusive or memory with accumulator
    m740_inc,                // increment by one
    m740_inx,                // increment index register X by one
    m740_iny,                // increment index register Y by one
    m740_jmp,                // jump
    m740_jsr,                // jump to subroutine
    m740_lda,                // load accumulator with memory
    m740_ldm,                // load immediate data to memory
    m740_ldx,                // load index register X from memory
    m740_ldy,                // load index register Y from memory
    m740_lsr,                // logical shift right
    m740_mul,                // multiply accumulator and memory
    m740_nop,                // no operation
    m740_ora,                // or memory with accumulator
    m740_pha,                // push accumulator on stack
    m740_php,                // push processor status on stack
    m740_pla,                // pull accumulator from stack
    m740_plp,                // pull processor status from stack
    m740_rol,                // rotate one bit left
    m740_ror,                // rotate one bit right
    m740_rrf,                // rotate right of four bits
    m740_rti,                // return from interrupt
    m740_rts,                // return from subroutine
    m740_sbc,                // subtract with carry
    m740_seb,                // set bit
    m740_sec,                // set carry flag
    m740_sed,                // set decimal mode
    m740_sei,                // set interrupt disable flag
    m740_set,                // set transfert flag
    m740_sta,                // store accumulator in memory
    m740_stp,                // stop
    m740_stx,                // store index register X in memory
    m740_sty,                // store index register Y in memory
    m740_tax,                // transfert accumulator to index register X
    m740_tay,                // transfert accumulator to index register Y
    m740_tst,                // test for negative or zero
    m740_tsx,                // transfert stack pointer to index register X
    m740_txa,                // transfert index register X to accumulator
    m740_txs,                // transfert index register X to stack pointer
    m740_tya,                // transfert index register Y to accumulator
    m740_wit,                // wait

    m740_last
};




extern instruc_t Instructions[];

enum 
{
    // 7700 :

    m7700_null = 0,         // null instruction
    m7700_adc,              // addition with carry
    m7700_and,              // logical AND
    m7700_asl,              // arithmetic shift left
    m7700_bbc,              // branch on bit clear
    m7700_bbs,              // branch on bit set
    m7700_bcc,              // branch on carry clear
    m7700_bcs,              // branch on carry set
    m7700_beq,              // branch on equal
    m7700_bmi,              // branch on result minus
    m7700_bne,              // branch on not equal
    m7700_bpl,              // branch on result plus
    m7700_bra,              // branch always
    m7700_brk,              // force break
    m7700_bvc,              // branch on overflow clear
    m7700_bvs,              // branch on overflow set
    m7700_clb,              // clear bit
    m7700_clc,              // clear carry flag
    m7700_cli,              // clear interrupt disable status
    m7700_clm,              // clear m flag
    m7700_clp,              // clear processor status
    m7700_clv,              // clear overflow flag
    m7700_cmp,              // compare
    m7700_cpx,              // compare memory and index register X
    m7700_cpy,              // compare memory and index register Y
    m7700_dec,              // decrement by one
    m7700_dex,              // decrement index register X by one
    m7700_dey,              // decrement index register Y by one
    m7700_div,              // divide
    m7700_eor,              // exclusive OR memory with accumulator
    m7700_inc,              // increment by one
    m7700_inx,              // increment index register X by one
    m7700_iny,              // increment index register Y by one
    m7700_jmp,              // jump
    m7700_jsr,              // jump to subroutine
    m7700_lda,              // load accumulator from memory
    m7700_ldm,              // load immediate to memory
    m7700_ldt,              // load immediate to data bank register
    m7700_ldx,              // load index register X from memory
    m7700_ldy,              // load index register Y from memory
    m7700_lsr,              // logical shift right
    m7700_mpy,              // multiply
    m7700_mvn,              // move negative
    m7700_mvp,              // move positive
    m7700_nop,              // no operation
    m7700_ora,              // OR memory with accumulator
    m7700_pea,              // push effective address
    m7700_pei,              // push effective indirect address
    m7700_per,              // push effective program counter relative address
    m7700_pha,              // push accumulator A on stack
    m7700_phb,              // push accumulator B on stack
    m7700_phd,              // push direct page register on stack
    m7700_phg,              // push program bank register on stack
    m7700_php,              // push processor status on stack
    m7700_pht,              // push data bank register on stack
    m7700_phx,              // push index register X on stack
    m7700_phy,              // push index register Y on stack
    m7700_pla,              // pull accumulator A from stack
    m7700_plb,              // pull accumulator B from stack
    m7700_pld,              // pull direct page register from stack
    m7700_plp,              // pull processor status from stack
    m7700_plt,              // pull data bank register from stack
    m7700_plx,              // pull index register X from stack
    m7700_ply,              // pull index register Y from stack
    m7700_psh,              // push
    m7700_pul,              // pull
    m7700_rla,              // rotate left accumulator A
    m7700_rol,              // rotate one bit left
    m7700_ror,              // rotate one bit right
    m7700_rti,              // return from interrupt
    m7700_rtl,              // return from subroutine long
    m7700_rts,              // return from subroutine
    m7700_sbc,              // subtract with carry
    m7700_seb,              // set bit
    m7700_sec,              // set carry flag
    m7700_sei,              // set interrupt disable status
    m7700_sem,              // set m flag
    m7700_sep,              // set processor status
    m7700_sta,              // store accumulator in memory
    m7700_stp,              // stop
    m7700_stx,              // store index register X in memory
    m7700_sty,              // store index register Y in memory
    m7700_tad,              // transfer accumulator A to direct page register
    m7700_tas,              // transfer accumulator A to stack pointer
    m7700_tax,              // transfer accumulator A to index register X
    m7700_tay,              // transfer accumulator A to index register Y
    m7700_tbd,              // transfer accumulator B to direct page register
    m7700_tbs,              // transfer accumulator B to stack pointer
    m7700_tbx,              // transfer accumulator B to index register X
    m7700_tby,              // transfer accumulator B to index register Y
    m7700_tda,              // transfer direct page register to accumulator A
    m7700_tdb,              // transfer direct page register to accumulator B
    m7700_tsa,              // transfer stack pointer to accumulator A
    m7700_tsb,              // transfer stack pointer to accumulator B
    m7700_tsx,              // transfer stack pointer to index register X
    m7700_txa,              // transfer index register X to accumulator A
    m7700_txb,              // transfer index register X to accumulator B
    m7700_txs,              // transfer index register X to stack pointer
    m7700_txy,              // transfer index register X to Y
    m7700_tya,              // transfer index register Y to accumulator A
    m7700_tyb,              // transfer index register Y to accumulator B
    m7700_tyx,              // transfer index register Y to X
    m7700_wit,              // wait
    m7700_xab,              // exchange accumulator A and B

    // 7750 :

    m7750_asr,              // arithmetic shift right
    m7750_divs,             // divide with sign
    m7750_exts,             // extention with sign
    m7750_extz,             // extention zero
    m7750_mpys,             // multiply with sign

    m7700_last
};


/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      MITSUBISHI 7900 Family
 */


extern instruc_t Instructions[];

enum 
{
m7900_null = 0,           // Unknown Operation


m7900_abs,   // ABSolute
m7900_absd,  // ABSolute at Double-word

m7900_adc,   // ADd with Carry
m7900_adcb,  // ADd with Carry at Byte
m7900_adcd,  // ADd with Carry at Double-word

m7900_add,    // Addition

m7900_addb,   // ADD at Byte
m7900_addd,   // ADD at Double-word
m7900_addm,   // ADD immediate and Memory
m7900_addmb,  // ADD immediate and Memory at Byte
m7900_addmd,  // ADD immediate and Memory at Double-word
m7900_adds,   // ADD Stack pointer and immediate
m7900_addx,   // ADD index register X and immediate
m7900_addy,   // ADD index register Y and immediate

m7900_and,    // Logical AND
m7900_andb,   // logical AND between immediate (Byte)

m7900_andm,   // logical AND between immediate value and Memory
m7900_andmb,  // logical AND between immediate value and Memory (Byte)
m7900_andmd,  // logical AND between immediate value and Memory (Double word)

m7900_asl,    // Arithmetic Shift to Left
m7900_asln,   // Arithmetic Shift to Left by n bits
m7900_asldn,  // Arithmetic Shift to Left by n bits (Double word)


m7900_asr,    // Arithmeticshift to the right
m7900_asrn,   // Arithmetic Shift to Right by n bits
m7900_asrdn,   //Arithmetic Shift to Right by n bits (Double word)

m7900_bbc,    // Branch on Bit Clear
m7900_bbcb,   // Branch on Bit Clear (Byte)
m7900_bbs,    // Branch on Bit Set
m7900_bbsb,   // Branch on Bit Set (Byte)

m7900_bcc,    // Branch on Carry Clear
m7900_bcs,    // Branch on Carry Set
m7900_beq,    // Branch on EQual
m7900_bge,    // Branch on Greater or Equal
m7900_bgt,    // Branch on Greater Than
m7900_bgtu,   // Branch on Greater Than with Unsign
m7900_ble,    // Branch on Less or Equal
m7900_bleu,   // Branch on Less Equal with Unsign
m7900_blt,    // Branch on Less Than
m7900_bmi,    // Branch on result MInus
m7900_bne,    // Branch on Not Equal
m7900_bpl,    // Branch on result PLus
m7900_bra,    // BRanch Always
m7900_bral,   // BRanch Always

m7900_brk,    // force BReaK

m7900_bsc,    // Branch on Single bit Clear
m7900_bsr,    // Branch to SubRoutine
m7900_bss,    // Branch on Single bit Set

m7900_bvc,    // Branch on oVerflow Clear
m7900_bvs,    // Branch on oVerflow Set

m7900_cbeq,   // Compare immediate and Branch on EQual
m7900_cbeqb,  // Compare immediate and Branch on EQual at Byte
m7900_cbne,   // Compare immediate and Branch on Not Equal
m7900_cbneb,  // Compare immediate and Branch on Not Equal at Byte

m7900_clc,    // CLear Carry flag
m7900_cli,    // CLear Interrupt disable status
m7900_clm,    // CLear M flag
m7900_clp,    // CLear Processor status

m7900_clr,    // CLeaR accumulator
m7900_clrb,   // CLeaR accumulator at Byte
m7900_clrm,   // CLeaR Memory
m7900_clrmb,  // CLeaR Memory at Byte
m7900_clrx,   // CLeaR index register X
m7900_clry,   // CLeaR index register Y

m7900_clv,    // CLear oVerflow flag

m7900_cmp,    // CoMPare
m7900_cmpb,   // CoMPare at Byte
m7900_cmpd,   // CoMPare at Double-word
m7900_cmpm,   // CoMPare immediate with Memory
m7900_cmpmb,  // CoMPare immediate with Memory at Byte
m7900_cmpmd,  // CoMPare immediate with Memory at Double-word

m7900_cpx,    // ComPare memory and index register X
m7900_cpy,    // ComPare memory and index register Y

m7900_debne,   // DEcrement memory and Branch on Not Equal

m7900_dec,    // DECrement by one
m7900_dex,    // DEcrement index register X by one
m7900_dey,    // DEcrement index register Y by one

m7900_div,    // DIVide unsigned
m7900_divs,   // DIVide with Sign
m7900_dxbne,  // Decrement index register X and Branch on Not Equal
m7900_dybne,  // Decrement index register Y and Branch on Not Equal

m7900_eor,    // Exclusive OR memory with accumulator
m7900_eorb,   // Exclusive OR immediate with accumulator at Byte
m7900_eorm,   // Exclusive OR immediate with Memory
m7900_eormb,  // Exclusive OR immediate with Memory at Byte
m7900_eormd,  // Exclusive OR immediate with Memory at Double-word

m7900_exts,   // EXTension Sign
m7900_extsd,  // EXTension Sign at Double-word
m7900_extz,   // EXTension Zero
m7900_extzd,  // EXTension Zero at Double-word

m7900_inc,    // INCrement by one
m7900_inx,    // INcrement index register X by one
m7900_iny,    // INcrement index register y by one

m7900_jmp,    // Jump
m7900_jmpl,   // Jump

m7900_jsr,    // Jump to SubRoutine
m7900_jsrl,   // Jump to SubRoutine


m7900_lda,    // LoaD Accumulator from memory
m7900_ldab,   // LoaD Accumulator from memory at Byte
m7900_ldad,   // LoaD Accumulator from memory at Double-word
m7900_lddn,   // LoaD immediate to Direct page register n

m7900_ldt,    // LoaD immediate to DaTa bank register
m7900_ldx,    // LoaD index register X from memory
m7900_ldxb,   // LoaD index register X from memory at Byte
m7900_ldy,    // LoaD index register Y from memory
m7900_ldyb,   // LoaD index register Y from memory at Byte

m7900_lsr,    // Logical Shift Right
m7900_lsrn,   // Logical n bits Shift Right
m7900_lsrdn,  // Logical n bits Shift Right at Double-word

m7900_movm,   // MOVe Memory to memory
m7900_movmb,  // MOVe Memory to memory at Byte
m7900_movr,   // MOVe Repeat memory to memory
m7900_movrb,  // MOVe Repeat memory to memory at Byte

m7900_mpy,    // MultiPlY
m7900_mpys,   // MultiPlY with Sign

m7900_mvn,    // MoVe Negative
m7900_mvp,    // MoVe Positive

m7900_neg,    // NEGative
m7900_negd,   // NEGative at Double-word

m7900_nop,    // No OPeration

m7900_ora,    // OR memory with Accumulator
m7900_orab,   // OR immediate with Accumulator at Byte
m7900_oram,   // OR immediAte with Memory
m7900_oramb,  // OR immediAte with Memory at Byte
m7900_oramd,  // OR immediAte with Memory at Double-word

m7900_pea,    // Push Effective Address
m7900_pei,    // Push Effective Indirect address
m7900_per,    // Push Effective program counter Relative address
m7900_pha,    // PusH accumulator A on stack
m7900_phb,    // PusH accumulator B on stack
m7900_phd,    // PusH Direct page register on stack
m7900_phdn,   // PusH Direct page register n on stack
m7900_phg,    // PusH proGram bank register on stack

m7900_phldn,  // PusH dpr n to stack and Load immediate to Dpr n

m7900_php,    // PusH Processor status on stack
m7900_pht,    // PusH daTa bank register on stack
m7900_phx,    // PusH index register X on stack
m7900_phy,    // PusH index register Y on stack

m7900_pla,    // PuLl accumulator A from stack
m7900_plb,    // PuLl accumulator B from stack
m7900_pld,    // PuLl Direct page register from stack
m7900_pldn,   // PuLl Direct page register n from stack
m7900_plp,    // PuLl Processor status from stack
m7900_plt,    // PuLl daTa bank register from stack
m7900_plx,    // PuLl index register X from stack
m7900_ply,    // PuLl index register Y from stack

m7900_psh,    // PuSH
m7900_pul,    // PuLl

m7900_rla,    // Rotate Left accumulator A
m7900_rmpa,   // Repeat Multiply and Accumulate

m7900_rol,    // ROtate one bit Left
m7900_roln,   // n bits ROtate Left

m7900_roldn,  // n bits ROtate Left at Double-word

m7900_ror,    // ROtate one bit Right
m7900_rorn,   // n bits ROtate Right
m7900_rordn,  // n bits ROtate Right at Double-word

m7900_rti,    // Return from Interrupt
m7900_rtl,    // ReTurn from subroutine Long
m7900_rtld,  // ReTurn from subroutine Long and pull Direct page register n
m7900_rts,    // ReTurn from Subroutine
m7900_rtsdn,  // ReTurn from Subroutine and pull Direct page register n


m7900_sbc,    // SuBtract with Carry
m7900_sbcb,   // SuBtract with Carry at Byte
m7900_sbcd,   // SuBtract with Carry at Double-word


m7900_sec,    // SEt Carry flag
m7900_sei,    // SEt Interrupt disable status
m7900_sem,    // SEt M flag
m7900_sep,    // SEt Processor status


m7900_sta,    // STore Accumulator in memory
m7900_stab,   // STore Accumulator in memory at Byte
m7900_stad,   // STore Accumulator in memory at Double-word


m7900_stp,   // SToP
m7900_stx,   // STore index register X in memory
m7900_sty,   // STore index register Y in memory

m7900_sub,   // SUBtract
m7900_subb,  // SUBtract at Byte
m7900_subd,  // SUBtract at Double-word
m7900_subm,  // SUBtract immediate from Memory
m7900_submb, // SUBtract immediate from Memory at Byte
m7900_submd, // SUBtract immediate from Memory at Double-word
m7900_subs,  // SUBtract Stack pointer
m7900_subx,  // SUBtract immediate from index register X
m7900_suby,  // SUBtract immediate from index register Y



m7900_tadn,  // Transfer accumulator A to Direct page register n


m7900_tas,   // Transfer accumulator A to Stack pointer
m7900_tax,   // Transfer accumulator A to index register X
m7900_tay,   // Transfer accumulator A to index register Y

m7900_tbdn,  // Transfer accumulator B to Direct page register n

m7900_tbs,   // Transfer accumulator B to Stack pointer
m7900_tbx,   // Transfer accumulator B to index register X
m7900_tby,   // Transfer accumulator B to index register Y

m7900_tdan,  // Transfer Direct page register n to accumulator A
m7900_tdbn,  // Transfer Direct page register n to accumulator B

m7900_tds,   // Transfer Direct page register to Stack pointer


m7900_tsa,   // Transfer Stack pointer to accumulator A
m7900_tsb,   // Transfer Stack pointer to accumulator B
m7900_tsd,   // Transfer Stack pointer to Direct page register
m7900_tsx,   // Transfer Stack pointer to index register X
m7900_txa,   // Transfer index register X to accumulator A
m7900_txb,   // Transfer index register X to accumulator B
m7900_txs,   // Transfer index register X to Stack pointer
m7900_txy,   // Transfer index register X to Y
m7900_tya,   // Transfer index register Y to accumulator A
m7900_tyb,   // Transfer index register Y to accumulator B
m7900_tyx,   // Transfer index register Y to X

m7900_wit,   // WaIT

m7900_xab,   // eXchange accumulator A and B

m7900_last   //

    };



extern instruc_t Instructions[];

enum 
{
    st9_null = 0,           // Unknown Operation.
    st9_ld,                 // Load.
    st9_ldw,                // Load word.
    st9_ldpp,               // Load (using CSR) => (using CSR).
    st9_ldpd,               // Load (using DPRx) => (using CSR).
    st9_lddp,               // Load (using CSR) => (using DPRx).
    st9_lddd,               // Load (using DPRx) => (using DPRx).
    st9_add,                // Add.
    st9_addw,               // Add Word.
    st9_adc,                // Add with Carry.
    st9_adcw,               // Add Word with Carry.
    st9_sub,                // Substract.
    st9_subw,               // Substract Word.
    st9_sbc,                // Substract with Carry.
    st9_sbcw,               // Substract Word with Carry.
    st9_and,                // Logical AND.
    st9_andw,               // Logical Word AND.
    st9_or,                 // Logical OR.
    st9_orw,                // Logical Word OR.
    st9_xor,                // Logical Exclusive OR.
    st9_xorw,               // Logical Word Exclusive OR.
    st9_cp,                 // Compare.
    st9_cpw,                // Compare Word.
    st9_tm,                 // Test under Mask.
    st9_tmw,                // Test Word under Mask.
    st9_tcm,                // Test Complement under Mask.
    st9_tcmw,               // Test Word Complement under Mask.
    st9_inc,                // Increment.
    st9_incw,               // Increment Word.
    st9_dec,                // Decrement.
    st9_decw,               // Decrement Word.
    st9_sla,                // Shift Left Arithmetic.
    st9_slaw,               // Shift Word Left Arithmetic.
    st9_sra,                // Shift Right Arithmetic.
    st9_sraw,               // Shift Word Right Arithmetic.
    st9_rrc,                // Rotate Right through Carry.
    st9_rrcw,               // Rotate Word Right through Carry.
    st9_rlc,                // Rotate Left through Carry.
    st9_rlcw,               // Rotate Word Left through Carry.
    st9_ror,                // Rotate Right.
    st9_rol,                // Rotate Left.
    st9_clr,                // Clear Register.
    st9_cpl,                // Complement Register.
    st9_swap,               // Swap Nibbles.
    st9_da,                 // Decimal ajust.
    st9_push,               // Push on System Stack.
    st9_pushw,              // Push Word on System Stack.
    st9_pea,                // Push Effective Address on System Stack.
    st9_pop,                // Pop from System Stack.
    st9_popw,               // Pop Word from System Stack.
    st9_pushu,              // Push on User Stack.
    st9_pushuw,             // Push Word on User Stack.
    st9_peau,               // Push Effective Address on User Stack.
    st9_popu,               // Pop from User Stack.
    st9_popuw,              // Pop Word from User Stack.
    st9_link,               // Move System Stack Pointer upward; support for high-level language.
    st9_unlink,             // Move System Stack Pointer backward; support for high-level language.
    st9_linku,              // Move User Stack Pointer upward; support for high-level language.
    st9_unlinku,            // Move User Stack Pointer backward; support for high-level language.
    st9_mul,                // Multiply 8x8.
    st9_div,                // Divide 8x8.
    st9_divws,              // Divide Word Stepped 32/16.
    st9_bset,               // Bit Set.
    st9_bres,               // Bit Reset    .
    st9_bcpl,               // Bit Complement.
    st9_btset,              // Bit Test and Set.
    st9_bld,                // Bit Load.
    st9_band,               // Bit AND.
    st9_bor,                // Bit OR.
    st9_bxor,               // Bit XOR.
    st9_ret,                // Return from Subroutine.
    st9_rets,               // Inter-segment Return to Subroutine.
    st9_iret,               // Return from Interrupt.
    st9_jrcc,               // Jump Relative if Condition ``cc'' is Met.
    st9_jpcc,               // Jump if Condition ``cc'' is Met.
    st9_jp,                 // Unconditional Jump.
    st9_jps,                // Unconditional Inter-segment Jump.
    st9_call,               // Unconditional Call.
    st9_calls,              // Inter-segment Call to Subroutine.
    st9_btjf,               // Bit Test and Jump if False.
    st9_btjt,               // Bit Test and Jump if True.
    st9_djnz,               // Decrement a Working Register and Jump if Non Zero.
    st9_dwjnz,              // Decrement a Register Pair and Jump if Non Zero.
    st9_cpjfi,              // Compare and Jump on False.  Otherwise Post Increment.
    st9_cpjti,              // Compare and Jump on True.  Otherwise Post Increment.
    st9_xch,                // Exchange Registers.
    st9_srp,                // Set Register Pointer Long (16 working registers).
    st9_srp0,               // Set Register Pointer 0 (8 LSB working registers).
    st9_srp1,               // Set Register Pointer 1 (8 MSB working registers).
    st9_spp,                // Set Page Pointer.
    st9_ext,                // Sign Extend.
    st9_ei,                 // Enable Interrupts.
    st9_di,                 // Disable Interrupts.
    st9_scf,                // Set Carry Flag.
    st9_rcf,                // Reset Carry Flag.
    st9_ccf,                // Complement Carry Flag.
    st9_spm,                // Select Extended Memory addressing scheme through CSR Register.
    st9_sdm,                // Select Extended Memory addressing scheme through DPR Registers.
    st9_nop,                // No Operation.
    st9_wfi,                // Stop Program Execution and Wait for the next Enable Interrupt.
    st9_halt,               // Stop Program Execution until System Reset.
    st9_etrap,              // Undocumented instruction.
    st9_eret,               // Undocumented instruction.
    st9_ald,                // PSEUDO INSTRUCTION.  SHOULD NEVER BE USED.
    st9_aldw,               // PSEUDO INSTRUCTION.  SHOULD NEVER BE USED.
    st9_last
};




extern instruc_t Instructions[];

enum 
{
    fr_null = 0,            // null instruction

    fr_add,                 // add word data of source register / 4-bit immediate data to destination register
    fr_add2,                // add 4-bit immediate data to destination register
    fr_addc,                // add word data of source register and carry bit to destination register
    fr_addn,                // add word data of source register / immediate data to destination register
    fr_addn2,               // add immediate data to destination register
    fr_sub,                 // subtract word data in source register from destination register
    fr_subc,                // subtract word data in source register and carry bit from destination register
    fr_subn,                // subtract word data in source register from destination register
    fr_cmp,                 // compare word / immediate data in source register and destination register
    fr_cmp2,                // compare immediate data and destination register
    fr_and,                 // and word data of source register to destination register / data in memory
    fr_andh,                // and half-word data of source register to data in memory
    fr_andb,                // and byte data of source register to data in memory
    fr_or,                  // or word data of source register to destination register / data in memory
    fr_orh,                 // or half-word data of source register to data in memory
    fr_orb,                 // or byte data of source register to data in memory
    fr_eor,                 // exclusive or word data of source register to destination register / data in memory
    fr_eorh,                // exclusive or half-word data of source register to data in memory
    fr_eorb,                // exclusive or byte data of source register to data in memory
    fr_bandl,               // and 4-bit immediate data to lower 4 bits of byte data in memory
    fr_bandh,               // and 4-bit immediate data to higher 4 bits of byte data in memory
    fr_borl,                // or 4-bit immediate data to lower 4 bits of byte data in memory
    fr_borh,                // or 4-bit immediate data to higher 4 bits of byte data in memory
    fr_beorl,               // eor 4-bit immediate data to lower 4 bits of byte data in memory
    fr_beorh,               // eor 4-bit immediate data to higher 4 bits of byte data in memory
    fr_btstl,               // test lower 4 bits of byte data in memory
    fr_btsth,               // test higher 4 bits of byte data in memory
    fr_mul,                 // multiply word data
    fr_mulu,                // multiply unsigned word data
    fr_mulh,                // multiply half-word data
    fr_muluh,               // multiply unsigned half-word data
    fr_div0s,               // initial setting up for signed division
    fr_div0u,               // initial setting up for unsigned division
    fr_div1,                // main process of division
    fr_div2,                // correction when remainder is 0
    fr_div3,                // correction when remainder is 0
    fr_div4s,               // correction answer for signed division
    fr_lsl,                 // logical shift to the left direction
    fr_lsl2,                // logical shift to the left direction
    fr_lsr,                 // logical shift to the right direction
    fr_lsr2,                // logical shift to the right direction
    fr_asr,                 // arithmetic shift to the right direction
    fr_asr2,                // arithmetic shift to the right direction
    fr_ldi_32,              // load immediate 32-bit data to destination register
    fr_ldi_20,              // load immediate 20-bit data to destination register
    fr_ldi_8,               // load immediate 8-bit data to destination register
    fr_ld,                  // load word data in memory to register / program status register
    fr_lduh,                // load half-word data in memory to register
    fr_ldub,                // load byte data in memory to register
    fr_st,                  // store word data in register / program status register to memory
    fr_sth,                 // store half-word data in register to memory
    fr_stb,                 // store byte data in register to memory
    fr_mov,                 // move word data in source register / program status register to destination register / program status register
    fr_jmp,                 // jump
    fr_call,                // call subroutine
    fr_ret,                 // return from subroutine
    fr_int,                 // software interrupt
    fr_inte,                // software interrupt for emulator
    fr_reti,                // return from interrupt
    fr_bra,                 // branch relative if condition satisfied
    fr_bno,                 // branch relative if condition satisfied
    fr_beq,                 // branch relative if condition satisfied
    fr_bne,                 // branch relative if condition satisfied
    fr_bc,                  // branch relative if condition satisfied
    fr_bnc,                 // branch relative if condition satisfied
    fr_bn,                  // branch relative if condition satisfied
    fr_bp,                  // branch relative if condition satisfied
    fr_bv,                  // branch relative if condition satisfied
    fr_bnv,                 // branch relative if condition satisfied
    fr_blt,                 // branch relative if condition satisfied
    fr_bge,                 // branch relative if condition satisfied
    fr_ble,                 // branch relative if condition satisfied
    fr_bgt,                 // branch relative if condition satisfied
    fr_bls,                 // branch relative if condition satisfied
    fr_bhi,                 // branch relative if condition satisfied
    fr_dmov,                // move word data from register / address to register / address
    fr_dmovh,               // move half-word data from register / address to register / address
    fr_dmovb,               // move byte data from register / address to register / address
    fr_ldres,               // load word data in memory to resource
    fr_stres,               // store word data in resource to memory
    fr_copop,               // coprocessor operation
    fr_copld,               // load 32-bit data from register to coprocessor register
    fr_copst,               // store 32-bit data from coprocessor register to register
    fr_copsv,               // save 32-bit data from coprocessor register to register
    fr_nop,                 // no operation
    fr_andccr,              // and condition code register and immediate data
    fr_orccr,               // or condition code register and immediate data
    fr_stilm,               // set immediate data to interrupt level mask register
    fr_addsp,               // add stack pointer and immediate data
    fr_extsb,               // sign extend from byte data to word data
    fr_extub,               // unsign extend from byte data to word data
    fr_extsh,               // sign extend from byte data to word data
    fr_extuh,               // unsigned extend from byte data to word data
    fr_ldm0,                // load multiple registers
    fr_ldm1,                // load multiple registers
    fr_stm0,                // store multiple registers
    fr_stm1,                // store multiple registers
    fr_enter,               // enter function
    fr_leave,               // leave function
    fr_xchb,                // exchange byte data
    fr_last                 // last instruction
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{
ALPHA_null = 0,     // Unknown Operation

ALPHA_addf,         // Add F_floating
ALPHA_addg,         // Add G_floating
ALPHA_addl,         // Add longword
ALPHA_addl_v,       // Add longword with overflow
ALPHA_addq,         // Add quadword
ALPHA_addq_v,       // Add quadword with overflow
ALPHA_adds,         // Add S_floating
ALPHA_addt,         // Add T_floating
ALPHA_amask,        // Architecture mask
ALPHA_and,          // Logical product
ALPHA_beq,          // Branch if = zero
ALPHA_bge,          // Branch if >= zero
ALPHA_bgt,          // Branch if > zero
ALPHA_bic,          // Bit clear
ALPHA_bis,          // Logical sum
ALPHA_blbc,         // Branch if low bit clear
ALPHA_blbs,         // Branch if low bit set
ALPHA_ble,          // Branch if <= zero
ALPHA_blt,          // Branch if < zero
ALPHA_bne,          // Branch if != zero
ALPHA_br,           // Unconditional branch
ALPHA_bsr,          // Branch to subroutine
ALPHA_call_pal,     // Trap to PALcode
ALPHA_cmoveq,       // CMOVE if = zero
ALPHA_cmovge,       // CMOVE if >= zero
ALPHA_cmovgt,       // CMOVE if > zero
ALPHA_cmovlbc,      // CMOVE if low bit clear
ALPHA_cmovlbs,      // CMOVE if low bit set
ALPHA_cmovle,       // CMOVE if <= zero
ALPHA_cmovlt,       // CMOVE if < zero
ALPHA_cmovne,       // CMOVE if != zero
ALPHA_cmpbge,       // Compare byte
ALPHA_cmpeq,        // Compare signed quadword equal
ALPHA_cmpgeq,       // Compare G_floating equal
ALPHA_cmpgle,       // Compare G_floating less than or equal
ALPHA_cmpglt,       // Compare G_floating less than
ALPHA_cmple,        // Compare signed quadword less than or equal
ALPHA_cmplt,        // Compare signed quadword less than
ALPHA_cmpteq,       // Compare T_floating equal
ALPHA_cmptle,       // Compare T_floating less than or equal
ALPHA_cmptlt,       // Compare T_floating less than
ALPHA_cmptun,       // Compare T_floating unordered
ALPHA_cmpule,       // Compare unsigned quadword less than or equal
ALPHA_cmpult,       // Compare unsigned quadword less than
ALPHA_cpys,         // Copy sign
ALPHA_cpyse,        // Copy sign and exponent
ALPHA_cpysn,        // Copy sign negate
ALPHA_ctlz,         // Count leading zero
ALPHA_ctpop,        // Count population
ALPHA_cttz,         // Count trailing zero
ALPHA_cvtdg,        // Convert D_floating to G_floating
ALPHA_cvtgd,        // Convert G_floating to D_floating
ALPHA_cvtgf,        // Convert G_floating to F_floating
ALPHA_cvtgq,        // Convert G_floating to quadword
ALPHA_cvtlq,        // Convert longword to quadword
ALPHA_cvtqf,        // Convert quadword to F_floating
ALPHA_cvtqg,        // Convert quadword to G_floating
ALPHA_cvtql,        // Convert quadword to longword
ALPHA_cvtqs,        // Convert quadword to S_floating
ALPHA_cvtqt,        // Convert quadword to T_floating
ALPHA_cvtst,        // Convert S_floating to T_floating
ALPHA_cvttq,        // Convert T_floating to quadword
ALPHA_cvtts,        // Convert T_floating to S_floating
ALPHA_divf,         // Divide F_floating
ALPHA_divg,         // Divide G_floating
ALPHA_divs,         // Divide S_floating
ALPHA_divt,         // Divide T_floating
ALPHA_ecb,          // Evict cache block
ALPHA_eqv,          // Logical equivalence
ALPHA_excb,         // Exception barrier
ALPHA_extbl,        // Extract byte low
ALPHA_extlh,        // Extract longword high
ALPHA_extll,        // Extract longword low
ALPHA_extqh,        // Extract quadword high
ALPHA_extql,        // Extract quadword low
ALPHA_extwh,        // Extract word high
ALPHA_extwl,        // Extract word low
ALPHA_fbeq,         // Floating branch if = zero
ALPHA_fbge,         // Floating branch if >= zero
ALPHA_fbgt,         // Floating branch if > zero
ALPHA_fble,         // Floating branch if <= zero
ALPHA_fblt,         // Floating branch if < zero
ALPHA_fbne,         // Floating branch if != zero
ALPHA_fcmoveq,      // FCMOVE if = zero
ALPHA_fcmovge,      // FCMOVE if >= zero
ALPHA_fcmovgt,      // FCMOVE if > zero
ALPHA_fcmovle,      // FCMOVE if <= zero
ALPHA_fcmovlt,      // FCMOVE if < zero
ALPHA_fcmovne,      // FCMOVE if != zero
ALPHA_fetch,        // Prefetch data
ALPHA_fetch_m,      // Prefetch data, modify intent
ALPHA_ftois,        // Floating to integer move, S_floating
ALPHA_ftoit,        // Floating to integer move, T_floating
ALPHA_implver,      // Implementation version
ALPHA_insbl,        // Insert byte low
ALPHA_inslh,        // Insert longword high
ALPHA_insll,        // Insert longword low
ALPHA_insqh,        // Insert quadword high
ALPHA_insql,        // Insert quadword low
ALPHA_inswh,        // Insert word high
ALPHA_inswl,        // Insert word low
ALPHA_itoff,        // Integer to floating move, F_floating
ALPHA_itofs,        // Integer to floating move, S_floating
ALPHA_itoft,        // Integer to floating move, T_floating
ALPHA_jmp,          // Jump
ALPHA_jsr,          // Jump to subroutine
ALPHA_jsr_coroutine,// Jump to subroutine return
ALPHA_lda,          // Load address
ALPHA_ldah,         // Load address high
ALPHA_ldbu,         // Load zero-extended byte
ALPHA_ldwu,         // Load zero-extended word
ALPHA_ldf,          // Load F_floating
ALPHA_ldg,          // Load G_floating
ALPHA_ldl,          // Load sign-extended longword
ALPHA_ldl_l,        // Load sign-extended longword locked
ALPHA_ldq,          // Load quadword
ALPHA_ldq_l,        // Load quadword locked
ALPHA_ldq_u,        // Load unaligned quadword
ALPHA_lds,          // Load S_floating
ALPHA_ldt,          // Load T_floating
ALPHA_maxsb8,       // Vector signed byte maximum
ALPHA_maxsw4,       // Vector signed word maximum
ALPHA_maxub8,       // Vector unsigned byte maximum
ALPHA_maxuw4,       // Vector unsigned word maximum
ALPHA_mb,           // Memory barrier
ALPHA_mf_fpcr,      // Move from FPCR
ALPHA_minsb8,       // Vector signed byte minimum
ALPHA_minsw4,       // Vector signed word minimum
ALPHA_minub8,       // Vector unsigned byte minimum
ALPHA_minuw4,       // Vector unsigned word minimum
ALPHA_mskbl,        // Mask byte low
ALPHA_msklh,        // Mask longword high
ALPHA_mskll,        // Mask longword low
ALPHA_mskqh,        // Mask quadword high
ALPHA_mskql,        // Mask quadword low
ALPHA_mskwh,        // Mask word high
ALPHA_mskwl,        // Mask word low
ALPHA_mt_fpcr,      // Move to FPCR
ALPHA_mulf,         // Multiply F_floating
ALPHA_mulg,         // Multiply G_floating
ALPHA_mull,         // Multiply longword
ALPHA_mull_v,       // Multiply longword with overflow
ALPHA_mulq,         // Multiply quadword
ALPHA_mulq_v,       // Multiply quadword with overflow
ALPHA_muls,         // Multiply S_floating
ALPHA_mult,         // Multiply T_floating
ALPHA_ornot,        // Logical sum with complement
ALPHA_perr,         // Pixel error
ALPHA_pklb,         // Pack longwords to bytes
ALPHA_pkwb,         // Pack words to bytes
ALPHA_rc,           // Read and clear
ALPHA_ret,          // Return from subroutine
ALPHA_rpcc,         // Read process cycle counter
ALPHA_rs,           // Read and set
ALPHA_s4addl,       // Scaled add longword by 4
ALPHA_s4addq,       // Scaled add quadword by 4
ALPHA_s4subl,       // Scaled subtract longword by 4
ALPHA_s4subq,       // Scaled subtract quadword by 4
ALPHA_s8addl,       // Scaled add longword by 8
ALPHA_s8addq,       // Scaled add quadword by 8
ALPHA_s8subl,       // Scaled subtract longword by 8
ALPHA_s8subq,       // Scaled subtract quadword by 8
ALPHA_sextb,        // Sign extend byte
ALPHA_sextw,        // Sign extend word
ALPHA_sll,          // Shift left logical
ALPHA_sqrtf,        // Square root F_floating
ALPHA_sqrtg,        // Square root G_floating
ALPHA_sqrts,        // Square root S_floating
ALPHA_sqrtt,        // Square root T_floating
ALPHA_sra,          // Shift right arithmetic
ALPHA_srl,          // Shift right logical
ALPHA_stb,          // Store byte
ALPHA_stf,          // Store F_floating
ALPHA_stg,          // Store G_floating
ALPHA_sts,          // Store S_floating
ALPHA_stl,          // Store longword
ALPHA_stl_c,        // Store longword conditional
ALPHA_stq,          // Store quadword
ALPHA_stq_c,        // Store quadword conditional
ALPHA_stq_u,        // Store unaligned quadword
ALPHA_stt,          // Store T_floating
ALPHA_stw,          // Store word
ALPHA_subf,         // Subtract F_floating
ALPHA_subg,         // Subtract G_floating
ALPHA_subl,         // Subtract longword
ALPHA_subl_v,       // Subtract longword with overflow
ALPHA_subq,         // Subtract quadword
ALPHA_subq_v,       // Subtract quadword with overflow
ALPHA_subs,         // Subtract S_floating
ALPHA_subt,         // Subtract T_floating
ALPHA_trapb,        // Trap barrier
ALPHA_umulh,        // Unsigned multiply quadword high
ALPHA_unpkbl,       // Unpack bytes to longwords
ALPHA_unpkbw,       // Unpack bytes to words
ALPHA_wh64,         // Write hint 64 bytes
ALPHA_wmb,          // Write memory barrier
ALPHA_xor,          // Logical difference
ALPHA_zap,          // Zero bytes
ALPHA_zapnot,       // Zero bytes not

// Macros

ALPHA_unop,         // Universal NOP
ALPHA_nop,          // Integer NOP
ALPHA_fnop,         // Floating point NOP
ALPHA_clr,          // Clear
ALPHA_fabs,         // Floating point absolute value
ALPHA_fclr,         // Floating point clear
ALPHA_fmov,         // Floating point register move
ALPHA_fneg,         // Floating point negate
ALPHA_mov,          // Move to register
ALPHA_negl,         // Negate longword
ALPHA_negl_v,       // Negate longword with overflow
ALPHA_negq,         // Negate quadword
ALPHA_negq_v,       // Negate quadword with overflow
ALPHA_negf,         // Negate F_floating
ALPHA_negg,         // Negate G_floating
ALPHA_negs,         // Negate S_floating
ALPHA_negt,         // Negate T_floating
ALPHA_not,          // Bitwise NOT
ALPHA_sextl,        // Move sign extended
ALPHA_or,           // Bitwise OR
ALPHA_andnot,       // Bit clear
ALPHA_xornot,       // Logical equivalence
ALPHA_br0,          // Branch

ALPHA_last,

};



extern instruc_t Instructions[];

enum 
{
KR1878_null = 0,     // Unknown Operation

KR1878_mov,     
KR1878_cmp,      // Compare
KR1878_add,      // Addition
KR1878_sub,      // Subtract
KR1878_and,      // Logical AND
KR1878_or,       // Logical Inclusive OR
KR1878_xor,      // Logical Exclusive OR

KR1878_movl,     
KR1878_cmpl,     // Compare
KR1878_addl,     // Addition
KR1878_subl,     // Subtract
KR1878_bic,
KR1878_bis,
KR1878_btg,
KR1878_btt,

KR1878_swap,
KR1878_neg,
KR1878_not,
KR1878_shl,      // Shift Left
KR1878_shr,      // Shift Right
KR1878_shra,     // Arithmetic Shift Right
KR1878_rlc,      // Rotate Left
KR1878_rrc,      // Rotate Right
KR1878_adc,      // Add with Carry
KR1878_sbc,      // Subtract with Carry

KR1878_ldr,     
KR1878_mtpr,     
KR1878_mfpr,     
KR1878_push,     
KR1878_pop,     
KR1878_sst,     
KR1878_cst,     
KR1878_tof,     
KR1878_tdc,     

KR1878_jmp,      // Jump
KR1878_jsr,      // Jump to Subroutine
KR1878_jnz,      // Jump
KR1878_jz,       // Jump
KR1878_jns,      // Jump
KR1878_js,       // Jump
KR1878_jnc,      // Jump
KR1878_jc,       // Jump
KR1878_ijmp,     // Jump
KR1878_ijsr,     // Jump to Subroutine
KR1878_rts,      // Return from Subroutine
KR1878_rtsc,     // Return from Subroutine
KR1878_rti,      // Return from Interrupt

KR1878_nop,      // No Operation
KR1878_wait,     
KR1878_stop,     
KR1878_reset,     
KR1878_sksp,     

KR1878_last,

};



extern instruc_t Instructions[];

enum 
{
AD218X_null = 0,        // Unknown Operation
// AMF subcodes
AD218X_amf_01,          // op = op * op (rnd)
AD218X_amf_03,          // op = mr + op * op (rnd)
AD218X_amf_02,          // op = mr - op * op (rnd)
AD218X_amf_04,          // op = op * op (ss)
AD218X_amf_05,          // op = op * op (su)
AD218X_amf_06,          // op = op * op (us)
AD218X_amf_07,          // op = op * op (uu)
AD218X_amf_08,          // op = mr + op * op
AD218X_amf_09,          // op = mr + op * op
AD218X_amf_0a,          // op = mr + op * op
AD218X_amf_0b,          // op = mr + op * op
AD218X_amf_0c,          // op = mr - op * op
AD218X_amf_0d,          // op = mr - op * op
AD218X_amf_0e,          // op = mr - op * op
AD218X_amf_0f,          // op = mr - op * op
AD218X_amf_10,          // op = op
AD218X_amf_11,          // op = op + 1
AD218X_amf_12,          // op = op + op + C
AD218X_amf_13,          // op = op + op
AD218X_amf_14,          // op = not op
AD218X_amf_15,          // op = - op
AD218X_amf_16,          // op = op - op + C - 1
AD218X_amf_17,          // op = op - op
AD218X_amf_18,          // op = op - 1
AD218X_amf_19,          // op = op - op
AD218X_amf_1a,          // op = op - op + C - 1
AD218X_amf_1b,          // op = not op
AD218X_amf_1c,          // op = op and op
AD218X_amf_1d,          // op = op or op
AD218X_amf_1e,          // op = op xor op
AD218X_amf_1f,          // op = abs op
// Shifter subcodes
AD218X_shft_0,          // sr = lshift op (hi)
AD218X_shft_1,          // sr = sr or lshift op (hi)
AD218X_shft_2,          // sr = lshift op (lo)
AD218X_shft_3,          // sr = sr or lshift op (lo)
AD218X_shft_4,          // sr = ashift op (hi)
AD218X_shft_5,          // sr = sr or ashift op (hi)
AD218X_shft_6,          // sr = ashift op (lo)
AD218X_shft_7,          // sr = sr or ashift op (lo)
AD218X_shft_8,          // sr = norm op (hi)
AD218X_shft_9,          // sr = sr or norm op (hi)
AD218X_shft_a,          // sr = norm op (lo)
AD218X_shft_b,          // sr = sr or norm op (lo)
AD218X_shft_c,          // sr = exp op (hi)
AD218X_shft_d,          // sr = exp op (hix)
AD218X_shft_e,          // sr = exp op (lo)
AD218X_shft_f,          // sr = expadj op
// ALU/MAC specific options
AD218X_alu_00,          // op = op + op + C                     [IF  COND] X + Const  
AD218X_alu_01,          // op = op + op 
AD218X_alu_02,          // op = op - op + C - 1                 [IF  COND] X - Const
AD218X_alu_03,          // op = op - op
AD218X_alu_04,          // op = op - op + C - 1                 [IF  COND] Const - X  
AD218X_alu_05,          // op = op - op
AD218X_alu_06,          // op = op and op                       [IF  COND] X and,or,xor Const
AD218X_alu_07,          // op = op or op
AD218X_alu_08,          // op = op xor op
AD218X_alu_09,          // op = op                              [IF  COND] PASS constant; constant <> 0,1, -1
AD218X_alu_0a,          // op = op + 1
AD218X_alu_0b,          // op = op - 1
AD218X_alu_0c,          // divq op                              ALU Type 23
AD218X_alu_0d,          // divs op, op                          ALU Type 24
AD218X_alu_0e,          // none = op                            ALU Type 9, with no result
AD218X_alu_0f,          // none = op + 1
AD218X_alu_10,          // none = op + op + C
AD218X_alu_11,          // none = op + op
AD218X_alu_12,          // none = not op
AD218X_alu_13,          // none = - op
AD218X_alu_14,          // none = op - op + C - 1
AD218X_alu_15,          // none = op - op
AD218X_alu_16,          // none = op - 1
AD218X_alu_17,          // none = op - op + C - 1
AD218X_alu_18,          // none = op - op
AD218X_alu_19,          // none = not op
AD218X_alu_1a,          // none = op and op
AD218X_alu_1b,          // none = op or op
AD218X_alu_1c,          // none = op xor op
AD218X_alu_1d,          // none = abs op
AD218X_mac_0,           // op = 0                               [IF  COND] MAC CLEAR
AD218X_mac_1,           // op = mr (rnd)                        [IF  COND] MAC TRANSFER
AD218X_mac_2,           // op = op * op (ss)                    [IF  COND] X * X
AD218X_mac_3,           // op = op * op (uu)
AD218X_mac_4,           // op = op * op (rnd)
AD218X_mac_5,           // op = mr + op * op (ss)               [IF  COND] MR + X * X
AD218X_mac_6,           // op = mr + op * op (uu)
AD218X_mac_7,           // op = mr + op * op (rnd)
AD218X_mac_8,           // op = mr - op * op (ss)               [IF  COND] MR - X * X
AD218X_mac_9,           // op = mr - op * op (uu)
AD218X_mac_a,           // op = mr - op * op (rnd)
AD218X_mac_b,           // if mv sat mr                         ALU/MAC Type 25
// ALU/MAC Common case
AD218X_amf,             // [IF  COND] AMF
// Shifter Common case
AD218X_shft,            // [IF  COND] Shifter    
// Shifter specific options. Shifter Type 15, const
AD218X_shifter_0,       // sr = ashift op by op (hi)
AD218X_shifter_1,       // sr = sr or ashift op by op (hi)
AD218X_shifter_2,       // sr = ashift op by op (lo)
AD218X_shifter_3,       // sr = sr or ashift op by op (lo)
AD218X_shifter_4,       // sr = lshift op by op (hi)
AD218X_shifter_5,       // sr = sr or lshift op by op (hi)
AD218X_shifter_6,       // sr = lshift op by op (lo)
AD218X_shifter_7,       // sr = sr or lshift op by op (lo)
// Data Movement
AD218X_move_0,          // op = op                              Internal Data Move, Instruction Type 17:
AD218X_move_1,          // op = op                              Load Register Immediate, Instruction Type 6:
AD218X_move_2,          // op = op                              Load Non-Data Register Immediate, Instruction Type 7:
AD218X_move_3,          // op = dm(op)                          Data Memory Read (Direct Address), Instruction Type 3:
AD218X_move_4,          // op = dm(op, op)                      Data Memory Read, Instruction Type 4:        
AD218X_move_5,          // op = pm(op, op)                      Program Memory Read
AD218X_move_6,          // dm(op) = op                          Data Memory Write (Direct Address)   
AD218X_move_7,          // dm(op, op) = op                      Data Memory Write, Instruction Type 4:
AD218X_move_8,          // dm(op, op) = op                      Data Memory Write, Immediate Data, Instruction Type 2:
AD218X_move_9,          // pm(op, op) = op                      Program Memory Write, Instruction Type 5
AD218X_move_a,          // op = io(op)                          I/O Memory Space Read/Write, Instruction Type 29:
AD218X_move_b,          // io(op) = op
// Flow control
AD218X_jump,            // Unconditional JUMP Direct, Instruction Type 10:
AD218X_jump_1,          // Conditional JUMP Direct, Instruction Type 10:
AD218X_jump_2,          // Unconditional JUMP Indirect, Instruction Type 19:
AD218X_jump_3,          // Conditional JUMP Indirect, Instruction Type 19:
AD218X_jump_4,          // Conditional JUMP on Flag In Direct Instruction Type 27:
AD218X_call,            // Conditional CALL Direct, Instruction Type 10:
AD218X_call_1,          // Conditional CALL Indirect, Instruction Type 19:
AD218X_call_2,          // Conditional CALL on Flag In Direct Instruction Type 27:
AD218X_rts,             // Unconditional Return, Instruction Type 20:
AD218X_rts_cond,        // Conditional Return, Instruction Type 20:
AD218X_rti,             // Unconditional Return, Instruction Type 20:
AD218X_rti_cond,        // Conditional Return, Instruction Type 20:
AD218X_nop,             // No operation, Instruction Type 30
AD218X_do,              // Do Until, Instruction Type 11:
// Misc
AD218X_idle,            // Idle, Instruction Type 31:        
AD218X_idle_1,
AD218X_flag_out,        // Flag Out Mode Control Instruction Type 28:
AD218X_stack_ctl,       // Stack Control, Instruction Type 26:
AD218X_mode_ctl,        // Mode Control, Instruction Type 18:
AD218X_tops_w,          // toppcstack = op                      Internal Data Move, Instruction Type 17:
AD218X_tops_r,          // op = toppcstack
AD218X_ints_dis,        // dis ints
AD218X_ints_ena,        // ena ints
AD218X_modify,          // modify(op, op)                       Modify Address Register, Instruction Type 21:        
// ALU/MAC operation with Data Memory Read/Write
AD218X_double_move,     // op = dm(op, op), op = pm(op, op)     Data & Program Memory Read, Instruction Type 1:
AD218X_amf_move_0,      // amf_cmd, op = dm(op, op)             ALU/MAC operation with Data Memory Read, Instruction Type 4:
AD218X_amf_move_1,      // amf_cmd, op = pm(op, op)             ALU/MAC operation with Program Memory Read, Instruction Type 5:        
AD218X_amf_move_2,      // amf_cmd, op = dm(op, op)             Shift operation with Data Memory Read, Instruction Type 12:
AD218X_amf_move_3,      // amf_cmd, op = pm(op, op)             Shift operation with Program Memory Read, Instruction Type 13:
AD218X_amf_move_4,      // amf_cmd, op = op                     ALU/MAC operation with Data Register Move, Instruction Type 8:
AD218X_amf_move_5,      // amf_cmd, op = op                     Shift operation with Data Register Move, Instruction Type 14:
AD218X_amf_move_6,      // dm(op, op) = op, amf_cmd             ALU/MAC operation with Data Memory Write, Instruction Type 4:
AD218X_amf_move_7,      // pm(op, op) = op, amf_cmd             ALU/MAC operation with Program Memory Write, Instruction Type 5:        
AD218X_amf_move_8,      // dm(op, op) = op, amf_cmd             Shift operation with Data Memory Write, Instruction Type 12:
AD218X_amf_move_9,      // pm(op, op) = op, amf_cmd             Shift operation with Program Memory Write, Instruction Type 13:
AD218X_amf_move_a,      // amf_cmd, op = dm(op, op), op = pm(op, op)    ALU/MAC with Data and Program Memory Read, Instruction Type 1:
AD218X_last,

};



extern instruc_t Instructions[];

enum 
{
OAK_Dsp_null = 0,       // Unknown Operation
OAK_Dsp_proc,           // cmd need further process
// ALU-ALM subcodes
OAK_Dsp_or,             //000  Logical Or
OAK_Dsp_and,            //001  And
OAK_Dsp_xor,            //010  Exclusive Or
OAK_Dsp_add,            //011  Add
OAK_Dsp_alm_tst0,       //100  Test Bit-field for Zeros
OAK_Dsp_alm_tst1,       //101  Test Bit-field for Ones
OAK_Dsp_cmp,            //110  Compare
OAK_Dsp_sub,            //111  Subtract
// ALM subcodes
OAK_Dsp_alm_msu,        //1000  Multiply and Subtract Previous Product
OAK_Dsp_addh,           //1001  Add to High Accumulator
OAK_Dsp_addl,           //1010  Add to Low Accumulator
OAK_Dsp_subh,           //1011  Subtract from High Accumulator
OAK_Dsp_subl,           //1100  Subtract from Low Accumulator
OAK_Dsp_sqr,            //1101  Square
OAK_Dsp_sqra,           //1110  Square and Accumulate Previous Product
OAK_Dsp_cmpu,           //1111  Compare Unsigned
// MODA-MODB subcodes conditional
OAK_Dsp_shr,            //000  Shift Accumulator Right
OAK_Dsp_shr4,           //001  Shift Accumulator Right by 4 Bits
OAK_Dsp_shl,            //010  Shift Accumulator Left
OAK_Dsp_shl4,           //011  Shift Accumulator Left by 4 Bits
OAK_Dsp_ror,            //100  Rotate Accumulator Right through Carry
OAK_Dsp_rol,            //101  Rotate Accumulator Left through Carry
OAK_Dsp_clr,            //110  Clear Accumulator
OAK_Dsp_mod_reserved,   //111  Mod Reserved
// MODA subcodes conditional
OAK_Dsp_not,            //1000  Logical Not
OAK_Dsp_neg,            //1001  2's Complement of aX-accumulator
OAK_Dsp_rnd,            //1010  Round Upper 20 Bits of aX-accumulator
OAK_Dsp_pacr,           //1011  Product Move and Round to aX-accumulator
OAK_Dsp_clrr,           //1100  Clear and Round aX-accumulator
OAK_Dsp_inc,            //1101  Increment Accumulator by One
OAK_Dsp_dec,            //1110  Decrement aX-accumulator by One
OAK_Dsp_copy,           //1111  Copy aX-accumulator
// ---
OAK_Dsp_norm,           //Normalize
OAK_Dsp_divs,           //Division Step
// ALB subcodes 
OAK_Dsp_set,            //000  Set Bit-field
OAK_Dsp_rst,            //001  Reset Bit-field
OAK_Dsp_chng,           //010  Change Bit-field
OAK_Dsp_addv,           //011  Add Long Immediate Value or Data Memory Location
OAK_Dsp_alb_tst0,       //100  Test Bit-field for Zeros
OAK_Dsp_alb_tst1,       //101  Test Bit-field for Ones
OAK_Dsp_cmpv,           //110  Compare Long Immediate Value to Register or Data Memory Location
OAK_Dsp_subv,           //111  Subtract Long Immediate Value from a Register or a Data Memory Location
// ---
OAK_Dsp_maxd,           //Maximum between Data Memory Location and Accumulator
OAK_Dsp_max,            //Maximum between Two Accumulators
OAK_Dsp_min,            //Minimum between Two Accumulators
OAK_Dsp_lim,            //Limit Accumulator     (lim aX[, aX])
// MUL subcodes 
OAK_Dsp_mpy,            //000  Multiply
OAK_Dsp_mpysu,          //001  Multiply Signed by Unsigned
OAK_Dsp_mac,            //010  Multiply and Accumulate Previous Product
OAK_Dsp_macus,          //011  Multiply Unsigned by Signed and Accumulate Previous Product
OAK_Dsp_maa,            //100  Multiply and Accumulate Aligned Previous Product
OAK_Dsp_macuu,          //101  Multiply Unsigned by Unsigned and Accumulate Previous Product
OAK_Dsp_macsu,          //110  Multiply Signed by Unsigned and Accumulate Previous Product
OAK_Dsp_maasu,          //111  Multiply Signed by Unsigned and Accumulate Aligned Previous Product
//---
OAK_Dsp_mpyi,           //Multiply Signed Short Immediate
OAK_Dsp_msu,            //Multiply and Subtract Previous Product
OAK_Dsp_tstb,           //Test Specific Bit
OAK_Dsp_shfc,           //Shift Accumulators according to Shift Value Register
OAK_Dsp_shfi,           //Shift Accumulators by an Immediate Shift Value
OAK_Dsp_exp,            //Evaluate the Exponent Value
//---
OAK_Dsp_mov,            //Move Data
OAK_Dsp_movp,           //Move from Program Memory into Data Memory
OAK_Dsp_movs,           //Move and Shift According to Shift Value Register
OAK_Dsp_movsi,          //Move and Shift According to an Immediate Shift Value
OAK_Dsp_movr,           //Move and Round
OAK_Dsp_movd,           //Move from Data Memory into Program Memory
//---
OAK_Dsp_push,           //Push Register or Long Immediate Value onto Stack
OAK_Dsp_pop,            //Pop from Stack into Register
//---
OAK_Dsp_swap,           //Swap aX- and bX-accumulators
OAK_Dsp_banke,          //Bank Exchange
OAK_Dsp_rep,            //Repeat Next Instruction
OAK_Dsp_bkrep,          //Block-Repeat
OAK_Dsp_break,          //Break from Block-repeat
//---
OAK_Dsp_br,             //Conditional Branch
OAK_Dsp_brr,            //Relative Conditional Branch
OAK_Dsp_br_u,           //UnConditional Branch
OAK_Dsp_brr_u,          //Relative UnConditional Branch
OAK_Dsp_call,           //Conditional Call Subroutine
OAK_Dsp_callr,          //Relative Conditional Call Subroutine
OAK_Dsp_calla,          //Call Subroutine at Location Specified by Accumulator
//---
OAK_Dsp_ret,            //Return Conditionally
OAK_Dsp_ret_u,          //Return UnConditionally
OAK_Dsp_retd,           //Delayed Return
OAK_Dsp_reti,           //Return from Interrupt Conditionally
OAK_Dsp_reti_u,         //Return from Interrupt UnConditionally
OAK_Dsp_retid,          //Delayed Return from Interrupt
OAK_Dsp_rets,           //Return with Short Immediate Parameter
//---
OAK_Dsp_cntx,           //Context Switching Store or Restore
OAK_Dsp_nop,            //No operation
OAK_Dsp_modr,           //Modify rN
OAK_Dsp_dint,           //Disable Interrupt
OAK_Dsp_eint,           //Enable Interrupt
//---
OAK_Dsp_trap,           //Software Interrupt
//---
OAK_Dsp_lpg,            //Load the Page Bits
OAK_Dsp_load,           //Load Specific Fields into Registers
OAK_Dsp_mov_eu,         //Move Data, eu
OAK_Dsp_last,

};

/*
 *      TLCS900 processor module for IDA.
 *      Copyright (c) 1998-2006 Konstantin Norvatoff, <konnor@bk.ru>
 *      Freeware.
 */


// List of instructions
extern instruc_t Instructions[];

//
enum 
{
T900_null = 0,           // Unknown Operation
T900_ld,
T900_ldw,
T900_push,
T900_pushw,
T900_pop,
T900_popw,
T900_lda,
T900_ldar,
T900_ex,
T900_mirr,
T900_ldi,
T900_ldiw,
T900_ldir,
T900_ldirw,
T900_ldd,
T900_lddw,
T900_lddr,
T900_lddrw,
T900_cpi,
T900_cpir,
T900_cpd,
T900_cpdr,
T900_add,
T900_addw,
T900_adc,
T900_adcw,
T900_sub,
T900_subw,
T900_sbc,
T900_sbcw,
T900_cp,
T900_cpw,
T900_inc,
T900_incw,
T900_dec,
T900_decw,
T900_neg,
T900_extz,
T900_exts,
T900_daa,
T900_paa,
T900_cpl,
T900_mul,
T900_muls,
T900_div,
T900_divs,
T900_mula,
T900_minc1,
T900_minc2,
T900_minc4,
T900_mdec1,
T900_mdec2,
T900_mdec4,
T900_and,
T900_andw,
T900_or,
T900_orw,
T900_xor,
T900_xorw,
T900_ldcf,
T900_stcf,
T900_andcf,
T900_orcf,
T900_xorcf,
T900_rcf,
T900_scf,
T900_ccf,
T900_zcf,
T900_bit,
T900_res,
T900_set,
T900_chg,
T900_tset,
T900_bs1f,
T900_bs1b,
T900_nop,
T900_ei,
T900_di,
T900_swi,
T900_halt,
T900_ldc,
T900_ldx,
T900_link,
T900_unlk,
T900_ldf,
T900_incf,
T900_decf,
T900_scc,
T900_rlc,
T900_rlc_mem,
T900_rlcw_mem,
T900_rrc,
T900_rrc_mem,
T900_rrcw_mem,
T900_rl,
T900_rl_mem,
T900_rlw_mem,
T900_rr,
T900_rr_mem,
T900_rrw_mem,
T900_sla,
T900_sla_mem,
T900_slaw_mem,
T900_sra,
T900_sra_mem,
T900_sraw_mem,
T900_sll,
T900_sll_mem,
T900_sllw_mem,
T900_srl,
T900_srl_mem,
T900_srlw_mem,
T900_rld,
T900_rrd,
T900_jp,
T900_jp_cond,
T900_jr,
T900_jr_cond,
T900_jrl,
T900_jrl_cond,
T900_call,
T900_calr,
T900_djnz,
T900_ret,
T900_ret_cond,
T900_retd,
T900_reti,
T900_max,
T900_normal,
T900_last
};

/*
 *      Rockwell C39 processor module for IDA.
 *      Copyright (c) 2000-2006 Konstantin Norvatoff, <konnor@bk.ru>
 *      Freeware.
 */


// List of instructions
extern instruc_t Instructions[];

//
enum 
{
C39_null = 0,           // Unknown Operation
C39_adc,
C39_add,
C39_anc,
C39_and,
C39_ane,
C39_arr,
C39_asl,
C39_asr,
C39_bar,
C39_bas,
C39_bbr,
C39_bbs,
C39_bcc,
C39_bcs,
C39_beq,
C39_bit,
C39_bmi,
C39_bne,
C39_bpl,
C39_bra,
C39_brk,
C39_bvc,
C39_bvs,
C39_clc,
C39_cld,
C39_cli,
C39_clv,
C39_clw,
C39_cmp,
C39_cpx,
C39_cpy,
C39_dcp,
C39_dec,
C39_dex,
C39_dey,
C39_eor,
C39_exc,
C39_inc,
C39_ini,
C39_inx,
C39_iny,
C39_isb,
C39_jmp,
C39_jpi,
C39_jsb,
C39_jsr,
C39_lab,
C39_lae,
C39_lai,
C39_lan,
C39_lax,
C39_lda,
C39_ldx,
C39_ldy,
C39_lii,
C39_lsr,
C39_lxa,
C39_mpa,
C39_mpy,
C39_neg,
C39_nop,
C39_nxt,
C39_ora,
C39_pha,
C39_phi,
C39_php,
C39_phw,
C39_phx,
C39_phy,
C39_pia,
C39_pla,
C39_pli,
C39_plp,
C39_plw,
C39_plx,
C39_ply,
C39_psh,
C39_pul,
C39_rba,
C39_rla,
C39_rmb,
C39_rnd,
C39_rol,
C39_ror,
C39_rra,
C39_rti,
C39_rts,
C39_sax,
C39_sba,
C39_sbc,
C39_sbx,
C39_sec,
C39_sed,
C39_sei,
C39_sha,
C39_shs,
C39_shx,
C39_shy,
C39_slo,
C39_smb,
C39_sre,
C39_sta,
C39_sti,
C39_stx,
C39_sty,
C39_tax,
C39_tay,
C39_taw,
C39_tip,
C39_tsx,
C39_twa,
C39_txa,
C39_txs,
C39_tya,
C39_last
};

/*
 *      National Semiconductor Corporation CR16 processor module for IDA.
 *      Copyright (c) 2002-2006 Konstantin Norvatoff, <konnor@bk.ru>
 *      Freeware.
 */


// list of instructions
extern instruc_t Instructions[];

enum 
{
CR16_null = 0,           // Unknown Operation
CR16_addb,
CR16_addw,
CR16_addub,
CR16_adduw,
CR16_addcb,
CR16_addcw,
CR16_andb,
CR16_andw,
CR16_ashub,
CR16_ashuw,
// !!! don't change sequence !!!
CR16_beq,
CR16_bne,
CR16_bcs,
CR16_bcc,
CR16_bhi,
CR16_bls,
CR16_bgt,
CR16_ble,
CR16_bfs,
CR16_bfc,
CR16_blo,
CR16_bhs,
CR16_blt,
CR16_bge,
CR16_br,
//----------------------------
CR16_bal,
CR16_cmpb,
CR16_cmpw,
CR16_beq1b,
CR16_beq1w,
CR16_beq0b,
CR16_beq0w,
CR16_bne1b,
CR16_bne1w,
CR16_bne0b,
CR16_bne0w,
CR16_di,
CR16_ei,
CR16_excp,
// !!! don't change sequence !!!
CR16_jeq,
CR16_jne,
CR16_jcs,
CR16_jcc,
CR16_jhi,
CR16_jls,
CR16_jgt,
CR16_jle,
CR16_jfs,
CR16_jfc,
CR16_jlo,
CR16_jhs,
CR16_jlt,
CR16_jge,
CR16_jump,
//----------------------------
CR16_jal,
CR16_loadb,
CR16_loadw,
CR16_loadm,
CR16_lpr,
CR16_lshb,
CR16_lshw,
CR16_movb,
CR16_movw,
CR16_movxb,
CR16_movzb,
CR16_movd,
CR16_mulb,
CR16_mulw,
CR16_mulsb,
CR16_mulsw,
CR16_muluw,
CR16_nop,
CR16_orb,
CR16_orw,
CR16_push,
CR16_pop,
CR16_popret,
CR16_retx,
// !!! don't change sequence !!!
CR16_seq,
CR16_sne,
CR16_scs,
CR16_scc,
CR16_shi,
CR16_sls,
CR16_sgt,
CR16_sle,
CR16_sfs,
CR16_sfc,
CR16_slo,
CR16_shs,
CR16_slt,
CR16_sge,
//----------------------------
CR16_spr,
CR16_storb,
CR16_storw,
CR16_storm,
CR16_subb,
CR16_subw,
CR16_subcb,
CR16_subcw,
CR16_tbit,
CR16_tbitb,
CR16_tbitw,
CR16_sbitb,
CR16_sbitw,
CR16_cbitb,
CR16_cbitw,
CR16_wait,
CR16_eiwait,
CR16_xorb,
CR16_xorw,
CR16_last
};

/*
 *      Panasonic MN102 (PanaXSeries) processor module for IDA.
 *      Copyright (c) 2000-2006 Konstantin Norvatoff, <konnor@bk.ru>
 *      Freeware.
 */


// List of instructions
extern instruc_t Instructions[];

enum 
{
mn102_null = 0,           // Unknown Operation
mn102_add,
mn102_addc,
mn102_addnf,
mn102_and,
mn102_asr,
mn102_bcc,
mn102_bccx,
mn102_bclr,
mn102_bcs,
mn102_bcsx,
mn102_beq,
mn102_beqx,
mn102_bge,
mn102_bgex,
mn102_bgt,
mn102_bgtx,
mn102_bhi,
mn102_bhix,
mn102_ble,
mn102_blex,
mn102_bls,
mn102_blsx,
mn102_blt,
mn102_bltx,
mn102_bnc,
mn102_bncx,
mn102_bne,
mn102_bnex,
mn102_bns,
mn102_bnsx,
mn102_bra,
mn102_bset,
mn102_btst,
mn102_bvc,
mn102_bvcx,
mn102_bvs,
mn102_bvsx,
mn102_cmp,
mn102_divu,
mn102_ext,
mn102_extx,
mn102_extxb,
mn102_extxbu,
mn102_extxu,
mn102_jmp,
mn102_jsr,
mn102_lsr,
mn102_mov,
mn102_movb,
mn102_movbu,
mn102_movx,
mn102_mul,
mn102_mulq,
mn102_mulqh,
mn102_mulql,
mn102_mulu,
mn102_nop,
mn102_not,
mn102_or,
mn102_pxst,
mn102_rol,
mn102_ror,
mn102_rti,
mn102_rts,
mn102_sub,
mn102_subc,
mn102_tbnz,
mn102_tbz,
mn102_xor,
mn102_last
};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      PowerPC - RISC with very complex instructions
 *
 */


extern instruc_t Instructions[];

enum 
{

PPC_null = 0,   // Unknown Operation
PPC_add,        // Add
PPC_addc,       // Add Carrying
PPC_adde,       // Add Extended
PPC_addi,       // Add Immediate
PPC_addic,      // Add Immediate Carrying
PPC_addis,      // Add Immediate Shifted
PPC_addme,      // Add to Minus One Extended
PPC_addze,      // Add to Zero Extended
PPC_and,        // AND
PPC_andc,       // AND with Complement
PPC_andi,       // AND Immediate
PPC_andis,      // AND Immediate Shifted
PPC_b,          // Branch
PPC_bc,         // Branch Conditional
PPC_bcctr,      // Branch Conditional to Count Register
PPC_bclr,       // Branch Conditional to Link Register
PPC_cmp,        // Compare
PPC_cmpi,       // Compare Immediate
PPC_cmpl,       // Compare Logical
PPC_cmpli,      // Compare Logical Immediate
PPC_cntlzd,     // Count Leading Zeros Double Word
PPC_cntlzw,     // Count Leading Zeros Word
PPC_crand,      // Condition Register AND
PPC_crandc,     // Condition Register AND with Complement
PPC_creqv,      // Condition Register Equivalent
PPC_crnand,     // Condition Register NAND
PPC_crnor,      // Condition Register NOR
PPC_cror,       // Condiiton Register OR
PPC_crorc,      // Condition Register OR with Comlement
PPC_crxor,      // Condition Register XOR
PPC_dcba,       //
PPC_dcbf,       // Data Cache Block Flush
PPC_dcbi,       // Data Cache Block Invalidate
PPC_dcbst,      // Data Cache Block Store
PPC_dcbt,       // Data Cache Block Touch
PPC_dcbtst,     // Data Cache Block Touch for Store
PPC_dcbz,       // Data Cache Block Set to Zero
PPC_divd,       // Divide Double Word
PPC_divdu,      // Divide Double Word Unsigned
PPC_divw,       // Divide Word
PPC_divwu,      // Divide Word Unsigned
PPC_eciwx,      // External Control In Word Indexed
PPC_ecowx,      // External Control Out Word Indexed
PPC_eieio,      // Enforce In-Order Execution of I/O
PPC_eqv,        // Equivalent
PPC_extsb,      // Extend Sign Byte
PPC_extsh,      // Extend Sign Half Word
PPC_extsw,      // Extend Sign Word
PPC_fabs,       // Floating-Point Absolute Value
PPC_fadd,       // Floating-Point Add
PPC_fadds,      // Floating-Point Add (Single-Precision)
PPC_fcfid,      // Floating-Point Convert from Integer Double Word
PPC_fcmpo,      // Floating-Point Compare Ordered
PPC_fcmpu,      // Floating-Point Compare Unordered
PPC_fctid,      // Floating-Point Convert to Integer Double Word
PPC_fctidz,     // Floating-Point Convert to Integer Double Word with Round toward Zero
PPC_fctiw,      // Floating-Point Convert to Integer Word
PPC_fctiwz,     // Floating-Point Convert to Integer Word with Round toward Zero
PPC_fdiv,       // Floating-Point Divide
PPC_fdivs,      // Floating-Point Divide Single-Precision
PPC_fmadd,      // Floating-Point Multiply-Add
PPC_fmadds,     // Floating-Point Multiply-Add Single-Precision
PPC_fmr,        // Floating-Point Move Register
PPC_fmsub,      // Floating-Point Multiply-Subtract
PPC_fmsubs,     // Floating-Point Multiply-Subtract (Single-Precision)
PPC_fmul,       // Floating-Point Multiply
PPC_fmuls,      // Floating-Point Multiply Single-Precision
PPC_fnabs,      // Floating-Point Negative Absolute Value
PPC_fneg,       // Floating-Point Negate
PPC_fnmadd,     // Floating-Point Negative Multiply-Add
PPC_fnmadds,    // Floating-Point Negative Multiply-Add Single-Precision
PPC_fnmsub,     // Floating-Point Negative Multiply-Subtract
PPC_fnmsubs,    // Floating-Point Negative Multiply-Subtract Single-Precision
PPC_fres,       // Floating-Point Reciprocal Estimate Single-Precision
PPC_frsp,       // Floating-Point Round to Single-Precision
PPC_frsqrte,    // Floating-Point Reciprocal Square Root Estimate
PPC_fsel,       // Floating-Point Select
PPC_fsqrt,      // Floating-Point Square Root
PPC_fsqrts,     // Floating-Point Square Root Single-Precision
PPC_fsub,       // Floating-Point Subtract
PPC_fsubs,      // Floating-Point Subtract Single-Precision
PPC_icbi,       // Instruction Cache Block Invalidate
PPC_isync,      // Instruction Synchronize
PPC_lbz,        // Load Byte and Zero
PPC_lbzu,       // Load Byte and Zero with Update
PPC_lbzux,      // Load Byte and Zero with Update Indexed
PPC_lbzx,       // Load Byte and Zero Indexed
PPC_ld,         // Load Double Word
PPC_ldarx,      // Load Double Word and Reserve Indexed
PPC_ldu,        // Load Double Word with Update
PPC_ldux,       // Load Double Word with Update Indexed
PPC_ldx,        // Load Double Word Indexed
PPC_lfd,        // Load Floating-Point Double-Precision
PPC_lfdu,       // Load Floating-Point Double-Precision with Update
PPC_lfdux,      // Load Floating-Point Double-Precision with Update Indexed
PPC_lfdx,       // Load Floating-Point Double-Precision Indexed
PPC_lfs,        // Load Floating-Point Single-Precision
PPC_lfsu,       // Load Floating-Point Single-Precision with Update
PPC_lfsux,      // Load Floating-Point Single-Precision with Update Indexed
PPC_lfsx,       // Load Floating-Point Single-Precision Indexed
PPC_lha,        // Load Half Word Algebraic
PPC_lhau,       // Load Half Word Algebraic with Update
PPC_lhaux,      // Load Half Word Algebraic with Update Indexed
PPC_lhax,       // Load Half Word Algebraic Indexed
PPC_lhbrx,      // Load Half Word Byte-reverse Indexed
PPC_lhz,        // Load Half Word and Zero
PPC_lhzu,       // Load Half Word and Zero with Update
PPC_lhzux,      // Load Half Word and Zero with Update Indexed
PPC_lhzx,       // Load Half Word and Zero Indexed
PPC_lmw,        // Load Multiple Word
PPC_lswi,       // Load String Word Immediate
PPC_lswx,       // Load String Word Indexed
PPC_lwa,        // Load Word Algebraic
PPC_lwarx,      // Load Word and Reserve Indexed
PPC_lwaux,      // Load Word Algebraic with Update Indexed
PPC_lwax,       // Load Word Algebraic Indexed
PPC_lwbrx,      // Load Word Byte-Reverse Indexed
PPC_lwz,        // Load Word and Zero
PPC_lwzu,       // Load Word and Zero with Update
PPC_lwzux,      // Load Word and Zero with Update Indexed
PPC_lwzx,       // Load Word and Zero Indexed
PPC_mcrf,       // Move Condition register Field
PPC_mcrfs,      // Move to Condition Register from FPSCR
PPC_mcrxr,      // Move to Condition Register from XER
PPC_mfcr,       // Move from Condition Register
PPC_mffs,       // Move from FPSCR
PPC_mfmsr,      // Move from Machine State Register
PPC_mfspr,      // Move from Special Purpose Register
PPC_mfsr,       // Move from Segment Register
PPC_mfsrin,     // Move from Segment Register Indexed
PPC_mftb,       // Move from Time Base
PPC_mtcrf,      // Move to Condition Register Fields
PPC_mtfsb0,     // Move to FPSCR Bit 0
PPC_mtfsb1,     // Move to FPSCR Bit 1
PPC_mtfsf,      // Move to FPSCR Fields
PPC_mtfsfi,     // Move to FPSCR Field Immediate
PPC_mtmsr,      // Move to Machine State Register
PPC_mtmsrd,     // Move to Machine State Register Double Word
PPC_mtspr,      // Move to Special Purpose Register
PPC_mtsr,       // Move to Segment Register
PPC_mtsrd,      // Move to Segment Register Double Word
PPC_mtsrdin,    // Move to Segment Register Indirect Double
PPC_mtsrin,     // Move to Segment Register Indirect
PPC_mulhd,      // Multiply High Double Word
PPC_mulhdu,     // Multiply High Double Word Unsigned
PPC_mulhw,      // Multiply High Word
PPC_mulhwu,     // Multiply High Word Unsigned
PPC_mulld,      // Multiply Low Double Word
PPC_mulli,      // Multiply Low Immediate
PPC_mullw,      // Multiply Low
PPC_nand,       // NAND (not AND)
PPC_neg,        // Negate
PPC_nor,        // NOR (not OR)
PPC_or,         // OR
PPC_orc,        // OR with Complement
PPC_ori,        // OR Immediate
PPC_oris,       // OR Immediate Shifted
PPC_rfi,        // Return from Interrupt
PPC_rfid,       // Return from Interrupt Double Word
PPC_rldcl,      // Rotate Left Double Word then Clear Left
PPC_rldcr,      // Rotate Left Double Word then Clear Right
PPC_rldic,      // Rotate Left Double Word Immediate then Clear
PPC_rldicl,     // Rotate Left Double Word Immediate then Clear Left
PPC_rldicr,     // Rotate Left Double Word Immediate then Clear Right
PPC_rldimi,     // Rotate Left Double Word Immediate then Mask Insert
PPC_rlwimi,     // Rotate Left Word Immediate then Mask Insert
PPC_rlwinm,     // Rotate Left Word Immediate then AND with Mask
PPC_rlwnm,      // Rotate Left Word then AND with Mask
PPC_sc,         // System Call
PPC_slbia,      // SLB Invalidate All
PPC_slbie,      // SLB Invalidate Entry
PPC_sld,        // Shift Left Double Word
PPC_slw,        // Shift Left Word
PPC_srad,       // Shift Right Algebraic Double Word
PPC_sradi,      // Shift Right Algebraic Double Word Immediate
PPC_sraw,       // Shift Right Algebraic Word
PPC_srawi,      // Shift Right Algebraic Word Immediate
PPC_srd,        // Shift Right Double Word
PPC_srw,        // Shift Right Word
PPC_stb,        // Store Byte
PPC_stbu,       // Store Byte with Update
PPC_stbux,      // Store Byte with Update Indexed
PPC_stbx,       // Store Byte Indexed
PPC_std,        // Store Double Word
PPC_stdcx,      // Store Double Word Conditional Indexed
PPC_stdu,       // Store Double Word with Update
PPC_stdux,      // Store Double Word with Update Indexed
PPC_stdx,       // Store Double Word Indexed
PPC_stfd,       // Store Floating-Point Double-Precision
PPC_stfdu,      // Store Floating-Point Double-Precision wiht Update
PPC_stfdux,     // Store Floating-Point Double-Precision wiht Update Indexed
PPC_stfdx,      // Store Floating-Point Double-Precision Indexed
PPC_stfiwx,     // Store Floating-Point as Integer Word Indexed
PPC_stfs,       // Store Floating-Point Single-Precision
PPC_stfsu,      // Store Floating-Point Single-Precision with Update
PPC_stfsux,     // Store Floating-Point Single-Precision with Update Indexed
PPC_stfsx,      // Store Floating-Point Single-Precision Indexed
PPC_sth,        // Store Half Word
PPC_sthbrx,     // Store Half Word Byte-Reverse Indexed
PPC_sthu,       // Store Half Word with Update
PPC_sthux,      // Store Half Word with Update Indexed
PPC_sthx,       // Store Half Word Indexed
PPC_stmw,       // Store Multiple Word
PPC_stswi,      // Store String Word Immediate
PPC_stswx,      // Store String Word Indexed
PPC_stw,        // Store Word
PPC_stwbrx,     // Store Word Byte-Reverse Indexed
PPC_stwcx,      // Store Word Conditional Indexed
PPC_stwu,       // Store Word with Update
PPC_stwux,      // Store Word with Update Indexed
PPC_stwx,       // Store Word Indexed
PPC_subf,       // Subtract from
PPC_subfc,      // Subtract from Carrying
PPC_subfe,      // Subtract from Extended
PPC_subfic,     // Subtract from Immediate Carrying
PPC_subfme,     // Subtract from Minus One Extended
PPC_subfze,     // Subtract from Zero Extended
PPC_sync,       // Synchronize
PPC_td,         // Trap Double Word
PPC_tdi,        // Trap Double Word Immediate
PPC_tlbia,      // TLB Invalidate All
PPC_tlbie,      // TLB Invalidate Entry
PPC_tlbsync,    // TLB Synchronize
PPC_tw,         // Trap Word
PPC_twi,        // Trap Word Immediate
PPC_xor,        // XOR
PPC_xori,       // XOR Immediate
PPC_xoris,      // XOR Immediate Shifted

PPC_last_basic = PPC_xoris,

//
//      Simplified mnemonics
//

PPC_cmpwi,      // Compare Word Immediate
PPC_cmpw,       // Compare Word
PPC_cmplwi,     // Compare Logical Word Immediate
PPC_cmplw,      // Compare Logical Word
PPC_cmpdi,      // Compare Double Word Immediate
PPC_cmpd,       // Compare Double Word
PPC_cmpldi,     // Compare Logical Double Word Immediate
PPC_cmpld,      // Compare Logical Double Word

PPC_trap,       // Trap Word Unconditionally
// "trapd" seems like a bug; no such mnemonic appears to exist as just plain
// "trap" is used regardless of word/doubleword, but we have to leave it here
// to not mess up existing databases
PPC_trapd,      // Trap Double Word Unconditionally
PPC_twlgt,      // Trap Word if Logically Greater Than
PPC_twllt,      // Trap Word if Logically Less Than
PPC_tweq,       // Trap Word if Equal
PPC_twlge,      // Trap Word if Logically Greater Than or Equal
PPC_twlle,      // Trap Word if Logically Less Than or Equal
PPC_twgt,       // Trap Word if Greater Than
PPC_twge,       // Trap Word if Greater Than or Equal
PPC_twlt,       // Trap Word if Less Than
PPC_twle,       // Trap Word if Less Than or Equal
PPC_twne,       // Trap Word if Not Equal
PPC_twlgti,     // Trap Word Immediate if Logically Greater Than
PPC_twllti,     // Trap Word Immediate if Logically Less Than
PPC_tweqi,      // Trap Word Immediate if Equal
PPC_twlgei,     // Trap Word Immediate if Logically Greater Than or Equal
PPC_twllei,     // Trap Word Immediate if Logically Less Than or Equal
PPC_twgti,      // Trap Word Immediate if Greater Than
PPC_twgei,      // Trap Word Immediate if Greater Than or Equal
PPC_twlti,      // Trap Word Immediate if Less Than
PPC_twlei,      // Trap Word Immediate if Less Than or Equal
PPC_twnei,      // Trap Word Immediate if Not Equal
PPC_tdlgt,      // Trap Double Word if Logically Greater Than
PPC_tdllt,      // Trap Double Word if Logically Less Than
PPC_tdeq,       // Trap Double Word if Equal
PPC_tdlge,      // Trap Double Word if Logically Greater Than or Equal
PPC_tdlle,      // Trap Double Word if Logically Less Than or Equal
PPC_tdgt,       // Trap Double Word if Greater Than
PPC_tdge,       // Trap Double Word if Greater Than or Equal
PPC_tdlt,       // Trap Double Word if Less Than
PPC_tdle,       // Trap Double Word if Less Than or Equal
PPC_tdne,       // Trap Double Word if Not Equal
PPC_tdlgti,     // Trap Double Word Immediate if Logically Greater Than
PPC_tdllti,     // Trap Double Word Immediate if Logically Less Than
PPC_tdeqi,      // Trap Double Word Immediate if Equal
PPC_tdlgei,     // Trap Double Word Immediate if Logically Greater Than or Equal
PPC_tdllei,     // Trap Double Word Immediate if Logically Less Than or Equal
PPC_tdgti,      // Trap Double Word Immediate if Greater Than
PPC_tdgei,      // Trap Double Word Immediate if Greater Than or Equal
PPC_tdlti,      // Trap Double Word Immediate if Less Than
PPC_tdlei,      // Trap Double Word Immediate if Less Than or Equal
PPC_tdnei,      // Trap Double Word Immediate if Not Equal
// all trap mnemonics are covered as of Power ISA 2.07; t[dw]nl*, t[dw]lnl*,
// and t[dw]lng* are intentionally missing since they are redundant.  in other
// words, the assembler supports (e.g.) both "logically less than or equal"
// (tdlei) in addition to "logically not greater than" (tdlngi)--a disassembler
// need not support both

PPC_nop,        // No Operation
PPC_not,        // Complement Register
PPC_mr,         // Move Register

PPC_subi,       // Subtract Immediate
PPC_subic,      // Subtract Immediate Carrying
PPC_subis,      // Subtract Immediate Shifted
PPC_li,         // Load Immediate
PPC_lis,        // Load Immediate Shifted

PPC_crset,      // Condition Register Set
PPC_crnot,      // Condition Register NOT
PPC_crmove,     // Condition Register Move
PPC_crclr,      // Condition Register Clear

PPC_mtxer,      // Move to integer unit exception register
PPC_mtlr,       // Move to link register
PPC_mtctr,      // Move to count register
PPC_mtdsisr,    // Move to DAE/source instruction service register
PPC_mtdar,      // Move to data address register
PPC_mtdec,      // Move to decrementer register
PPC_mtsrr0,     // Move to status save/restore register 0
PPC_mtsrr1,     // Move to status save/restore register 1
PPC_mtsprg0,    // Move to general special purpose register 0
PPC_mtsprg1,    // Move to general special purpose register 1
PPC_mtsprg2,    // Move to general special purpose register 2
PPC_mtsprg3,    // Move to general special purpose register 3
PPC_mttbl,      // Move to time base register (lower)
PPC_mttbu,      // Move to time base register (upper)
PPC_mfxer,      // Move from integer unit exception register
PPC_mflr,       // Move from link register
PPC_mfctr,      // Move from count register
PPC_mfdsisr,    // Move from DAE/source instruction service register
PPC_mfdar,      // Move from data address register
PPC_mfdec,      // Move from decrementer register
PPC_mfsrr0,     // Move from status save/restore register 0
PPC_mfsrr1,     // Move from status save/restore register 1
PPC_mfsprg0,    // Move from general special purpose register 0
PPC_mfsprg1,    // Move from general special purpose register 1
PPC_mfsprg2,    // Move from general special purpose register 2
PPC_mfsprg3,    // Move from general special purpose register 3
PPC_mftbl,      // Move from time base register (lower)
PPC_mftbu,      // Move from time base register (upper)
PPC_mfpvr,      // Move from processor version register

// suffixes:
//      lr      - goto lr
//      ctr     - goto ctr
//      l       - update lr
//      a       - absolute

// branch forms not listed here or elsewhere in the file (e.g. bltlr) are
// being handled by emitting extra characters when printing the instructions
// instead of having separate enums

PPC_balways,    // Branch unconditionally
PPC_bt,         // Branch if true
PPC_bf,         // Branch if false
PPC_bdnz,       // CTR--; branch if CTR non-zero
PPC_bdnzt,      // CTR--; branch if CTR non-zero and condition is true
PPC_bdnzf,      // CTR--; branch if CTR non-zero and condition is false
PPC_bdz,        // CTR--; branch if CTR zero
PPC_bdzt,       // CTR--; branch if CTR zero and condition is true
PPC_bdzf,       // CTR--; branch if CTR zero and condition is false

PPC_blt,        // Branch if less than
PPC_ble,        // Branch if less than or equal
PPC_beq,        // Branch if equal
PPC_bge,        // Branch if greater than or equal
PPC_bgt,        // Branch if greater than
PPC_bne,        // Branch if not equal
PPC_bso,        // Branch if summary overflow
PPC_bns,        // Branch if not summary overflow
// bun and bnu are intentionally missing from this list; they are handled by
// bso/bns (this is the result of historical differences between POWER and PPC
// mnemonics)

PPC_extlwi,     // Extract and Left Justify Immediate
PPC_extrwi,     // Extract and Right Justify Immediate
PPC_inslwi,     // Insert from Left Immediate
PPC_insrwi,     // Insert from Right Immediate
PPC_rotlwi,     // Rotate Left Immediate
PPC_rotrwi,     // Rotate Right Immediate
PPC_rotlw,      // Rotate Left
PPC_slwi,       // Shift Left Immediate
PPC_srwi,       // Shift Right Immediate
PPC_clrlwi,     // Clear Left Immediate
PPC_clrrwi,     // Clear Right Immediate
PPC_clrlslwi,   // Clear Left and Shift Left Immediate

//
//  PowerPC Embedded Controller Instructions
//

PPC_dccci,      // Data cache congruence class invalidate (p.438-439)
PPC_dcread,     // Data cache read (p.440-441)
PPC_icbt,       // Instruction cache block touch (p.450-451)
PPC_iccci,      // Instruction cache congruence class invalidate (p.452-453)
PPC_icread,     // Instruction cache read (p.454-455)
PPC_mfdcr,      // Move from device control register (p.484-485)
PPC_mtdcr,      // Move to device control register (p.491-492)
PPC_rfci,       // Return from critical interrupt (p.507)
PPC_tlbre,      // TLB read entry (p.548-549)
PPC_tlbsx,      // TLB search indexed (p.550)
PPC_tlbwe,      // TLB write entry (p.552-553)
PPC_wrtee,      // Write external enable (p.560)
PPC_wrteei,     // Write external enable immediate (p.561)

//
//  New PowerPC instructions
//

PPC_abs,        // Absolute
PPC_clcs,       // Cache Lines Compute Size
PPC_clf,        // Cache Line Flush
PPC_cli,        // Cache Line Invalidate
PPC_dclst,      // Data Cache Line Store
PPC_div,        // Divide
PPC_divs,       // Divide Short
PPC_doz,        // Different Or Zero
PPC_dozi,       // Different Or Zero Immediate
PPC_frsqrtes,   // Floating Reciprocal Square Root Estimate Single
PPC_hrfid,      // Hypervisor Return from Interrupt Doubleword
PPC_lscbx,      // Load String And Compare Byte Indexed
PPC_maskg,      // Mask Generate
PPC_maskir,     // Mask Insert From Register
PPC_mfsri,      // Move From Segment Register Indirect
PPC_mul,        // Multiply
PPC_nabs,       // Negative Absolute
PPC_popcntb,    // Population Count Bytes
PPC_rac,        // Real Address Compute
PPC_rfsvc,      // Return From SVC
PPC_rlmi,       // Rotate Left Then Mask Insert
PPC_rrib,       // Rotate Right And Insert Bit
PPC_slbmfee,    // SLB Move From Entry ESID
PPC_slbmfev,    // SLB Move From Entry VSID
PPC_slbmte,     // SLB Move To Entry
PPC_sle,        // Shift Left Extended
PPC_sleq,       // Shift Left Extended With MQ
PPC_sliq,       // Shift Left Immediate With MQ
PPC_slliq,      // Shift Left Long Immediate With MQ
PPC_sllq,       // Shift Left Long With MQ
PPC_slq,        // Shift Left With MQ
PPC_sraiq,      // Shift Right Algebraic Immediate With MQ
PPC_sraq,       // Shift Right Algebraic With MQ
PPC_sre,        // Shift Right Extended
PPC_srea,       // Shift Right Extended Algebraic
PPC_sreq,       // Shift Right Extended With MQ
PPC_sriq,       // Shift Right Immediate With MQ
PPC_srliq,      // Shift Right Long Immediate With MQ
PPC_srlq,       // Shift Right Long With MQ
PPC_srq,        // Shift Right With MQ

// New instructions

PPC_mtocrf,     // Move To One Condition Register Field
PPC_mfocrf,     // Move From One Condition Register Field

// Freescale e200z6 instructions
PPC_isel,       // Integer Select
PPC_isellt,     // Integer Select Less Than
PPC_iselgt,     // Integer Select Greater Than
PPC_iseleq,     // Integer Select Equal
PPC_dcblc,      // Data Cache Block Lock Clear
PPC_dcbtls,     // Data Cache Block Touch and Lock Set
PPC_dcbtstls,   // Data Cache Block Touch for Store and Lock Set
PPC_icblc,      // Instruction Cache Block Lock Clear
PPC_icbtls,     // Instruction Cache Block Touch and Lock Set
PPC_tlbivax,    // TLB Invalidate Virtual Address Indexed
PPC_rfdi,       // Return from Debug Interrupt

// Freescale MPC603e instructions (G2 core)

PPC_tlbld,      // Load Data TLB Entry
PPC_tlbli,      // Load Instruction TLB Entry

// SPE (Signal Processing Engine) instructions

PPC_brinc,            // Bit Reversed Increment
PPC_evabs,            // Vector Absolute Value
PPC_evaddiw,          // Vector Add Immediate Word
PPC_evaddsmiaaw,      // Vector Add Signed, Modulo, Integer to Accumulator Word
PPC_evaddssiaaw,      // Vector Add Signed, Saturate, Integer to Accumulator Word
PPC_evaddumiaaw,      // Vector Add Unsigned, Modulo, Integer to Accumulator Word
PPC_evaddusiaaw,      // Vector Add Unsigned, Saturate, Integer to Accumulator Word
PPC_evaddw,           // Vector Add Word
PPC_evand,            // Vector AND
PPC_evandc,           // Vector AND with Complement
PPC_evcmpeq,          // Vector Compare Equal
PPC_evcmpgts,         // Vector Compare Greater Than Signed
PPC_evcmpgtu,         // Vector Compare Greater Than Unsigned
PPC_evcmplts,         // Vector Compare Less Than Signed
PPC_evcmpltu,         // Vector Compare Less Than Unsigned
PPC_evcntlsw,         // Vector Count Leading Signed Bits Word
PPC_evcntlzw,         // Vector Count Leading Zeros Word
PPC_evdivws,          // Vector Divide Word Signed
PPC_evdivwu,          // Vector Divide Word Unsigned
PPC_eveqv,            // Vector Equivalent
PPC_evextsb,          // Vector Extend Sign Byte
PPC_evextsh,          // Vector Extend Sign Halfword
PPC_evldd,            // Vector Load Double Word into Double Word
PPC_evlddx,           // Vector Load Double Word into Double Word Indexed
PPC_evldh,            // Vector Load Double into Four Halfwords
PPC_evldhx,           // Vector Load Double into Four Halfwords Indexed
PPC_evldw,            // Vector Load Double into Two Words
PPC_evldwx,           // Vector Load Double into Two Words Indexed
PPC_evlhhesplat,      // Vector Load Halfword into Halfwords Even and Splat
PPC_evlhhesplatx,     // Vector Load Halfword into Halfwords Even and Splat Indexed
PPC_evlhhossplat,     // Vector Load Halfword into Halfword Odd Signed and Splat
PPC_evlhhossplatx,    // Vector Load Halfword into Halfword Odd Signed and Splat Indexed
PPC_evlhhousplat,     // Vector Load Halfword into Halfword Odd Unsigned and Splat
PPC_evlhhousplatx,    // Vector Load Halfword into Halfword Odd Unsigned and Splat Indexed
PPC_evlwhe,           // Vector Load Word into Two Halfwords Even
PPC_evlwhex,          // Vector Load Word into Two Halfwords Even Indexed
PPC_evlwhos,          // Vector Load Word into Two Halfwords Odd Signed (with sign extension)
PPC_evlwhosx,         // Vector Load Word into Two Halfwords Odd Signed Indexed (with sign extension)
PPC_evlwhou,          // Vector Load Word into Two Halfwords Odd Unsigned (zero-extended)
PPC_evlwhoux,         // Vector Load Word into Two Halfwords Odd Unsigned Indexed (zero-extended)
PPC_evlwhsplat,       // Vector Load Word into Two Halfwords and Splat
PPC_evlwhsplatx,      // Vector Load Word into Two Halfwords and Splat Indexed
PPC_evlwwsplat,       // Vector Load Word into Word and Splat
PPC_evlwwsplatx,      // Vector Load Word into Word and Splat Indexed
PPC_evmergehi,        // Vector Merge High
PPC_evmergehilo,      // Vector Merge High/Low
PPC_evmergelo,        // Vector Merge Low
PPC_evmergelohi,      // Vector Merge Low/High
PPC_evmhegsmfaa,      // Vector Multiply Halfwords, Even, Guarded, Signed, Modulo, Fractional and Accumulate
PPC_evmhegsmfan,      // Vector Multiply Halfwords, Even, Guarded, Signed, Modulo, Fractional and Accumulate Negative
PPC_evmhegsmiaa,      // Vector Multiply Halfwords, Even, Guarded, Signed, Modulo, Integer and Accumulate
PPC_evmhegsmian,      // Vector Multiply Halfwords, Even, Guarded, Signed, Modulo, Integer and Accumulate Negative
PPC_evmhegumiaa,      // Vector Multiply Halfwords, Even, Guarded, Unsigned, Modulo, Integer and Accumulate
PPC_evmhegumian,      // Vector Multiply Halfwords, Even, Guarded, Unsigned, Modulo, Integer and Accumulate Negative
PPC_evmhesmf,         // Vector Multiply Halfwords, Even, Signed, Modulo, Fractional
PPC_evmhesmfa,        // Vector Multiply Halfwords, Even, Signed, Modulo, Fractional to Accumulator
PPC_evmhesmfaaw,      // Vector Multiply Halfwords, Even, Signed, Modulo, Fractional and Accumulate into Words
PPC_evmhesmfanw,      // Vector Multiply Halfwords, Even, Signed, Modulo, Fractional and Accumulate Negative into Words
PPC_evmhesmi,         // Vector Multiply Halfwords, Even, Signed, Modulo, Integer
PPC_evmhesmia,        // Vector Multiply Halfwords, Even, Signed, Modulo, Integer to Accumulator
PPC_evmhesmiaaw,      // Vector Multiply Halfwords, Even, Signed, Modulo, Integer and Accumulate into Words
PPC_evmhesmianw,      // Vector Multiply Halfwords, Even, Signed, Modulo, Integer and Accumulate Negative into Words
PPC_evmhessf,         // Vector Multiply Halfwords, Even, Signed, Saturate, Fractional
PPC_evmhessfa,        // Vector Multiply Halfwords, Even, Signed, Saturate, Fractional to Accumulator
PPC_evmhessfaaw,      // Vector Multiply Halfwords, Even, Signed, Saturate, Fractional and Accumulate into Words
PPC_evmhessfanw,      // Vector Multiply Halfwords, Even, Signed, Saturate, Fractional and Accumulate Negative into Words
PPC_evmhessiaaw,      // Vector Multiply Halfwords, Even, Signed, Saturate, Integer and Accumulate into Words
PPC_evmhessianw,      // Vector Multiply Halfwords, Even, Signed, Saturate, Integer and Accumulate Negative into Words
PPC_evmheumi,         // Vector Multiply Halfwords, Even, Unsigned, Modulo, Integer
PPC_evmheumia,        // Vector Multiply Halfwords, Even, Unsigned, Modulo, Integer to Accumulator
PPC_evmheumiaaw,      // Vector Multiply Halfwords, Even, Unsigned, Modulo, Integer and Accumulate into Words
PPC_evmheumianw,      // Vector Multiply Halfwords, Even, Unsigned, Modulo, Integer and Accumulate Negative into Words
PPC_evmheusiaaw,      // Vector Multiply Halfwords, Even, Unsigned, Saturate, Integer and Accumulate into Words
PPC_evmheusianw,      // Vector Multiply Halfwords, Even, Unsigned, Saturate, Integer and Accumulate Negative into Words
PPC_evmhogsmfaa,      // Vector Multiply Halfwords, Odd, Guarded, Signed, Modulo, Fractional and Accumulate
PPC_evmhogsmfan,      // Vector Multiply Halfwords, Odd, Guarded, Signed, Modulo, Fractional and Accumulate Negative
PPC_evmhogsmiaa,      // Vector Multiply Halfwords, Odd, Guarded, Signed, Modulo, Integer and Accumulate
PPC_evmhogsmian,      // Vector Multiply Halfwords, Odd, Guarded, Signed, Modulo, Integer and Accumulate Negative
PPC_evmhogumiaa,      // Vector Multiply Halfwords, Odd, Guarded, Unsigned, Modulo, Integer and Accumulate
PPC_evmhogumian,      // Vector Multiply Halfwords, Odd, Guarded, Unsigned, Modulo, Integer and Accumulate Negative
PPC_evmhosmf,         // Vector Multiply Halfwords, Odd, Signed, Modulo, Fractional
PPC_evmhosmfa,        // Vector Multiply Halfwords, Odd, Signed, Modulo, Fractional to Accumulator
PPC_evmhosmfaaw,      // Vector Multiply Halfwords, Odd, Signed, Modulo, Fractional and Accumulate into Words
PPC_evmhosmfanw,      // Vector Multiply Halfwords, Odd, Signed, Modulo, Fractional and Accumulate Negative into Words
PPC_evmhosmi,         // Vector Multiply Halfwords, Odd, Signed, Modulo, Integer
PPC_evmhosmia,        // Vector Multiply Halfwords, Odd, Signed, Modulo, Integer to Accumulator
PPC_evmhosmiaaw,      // Vector Multiply Halfwords, Odd, Signed, Modulo, Integer and Accumulate into Words
PPC_evmhosmianw,      // Vector Multiply Halfwords, Odd, Signed, Modulo, Integer and Accumulate Negative into Words
PPC_evmhossf,         // Vector Multiply Halfwords, Odd, Signed, Saturate, Fractional
PPC_evmhossfa,        // Vector Multiply Halfwords, Odd, Signed, Saturate, Fractional to Accumulator
PPC_evmhossfaaw,      // Vector Multiply Halfwords, Odd, Signed, Saturate, Fractional and Accumulate into Words
PPC_evmhossfanw,      // Vector Multiply Halfwords, Odd, Signed, Saturate, Fractional and Accumulate Negative into Words
PPC_evmhossiaaw,      // Vector Multiply Halfwords, Odd, Signed, Saturate, Integer and Accumulate into Words
PPC_evmhossianw,      // Vector Multiply Halfwords, Odd, Signed, Saturate, Integer and Accumulate Negative into Words
PPC_evmhoumi,         // Vector Multiply Halfwords, Odd, Unsigned, Modulo, Integer
PPC_evmhoumia,        // Vector Multiply Halfwords, Odd, Unsigned, Modulo, Integer to Accumulator
PPC_evmhoumiaaw,      // Vector Multiply Halfwords, Odd, Unsigned, Modulo, Integer and Accumulate into Words
PPC_evmhoumianw,      // Vector Multiply Halfwords, Odd, Unsigned, Modulo, Integer and Accumulate Negative into Words
PPC_evmhousiaaw,      // Vector Multiply Halfwords, Odd, Unsigned, Saturate, Integer and Accumulate into Words
PPC_evmhousianw,      // Vector Multiply Halfwords, Odd, Unsigned, Saturate, Integer and Accumulate Negative into Words
PPC_evmra,            // Initialize Accumulator
PPC_evmwhsmf,         // Vector Multiply Word High Signed, Modulo, Fractional
PPC_evmwhsmfa,        // Vector Multiply Word High Signed, Modulo, Fractional to Accumulator
PPC_evmwhsmi,         // Vector Multiply Word High Signed, Modulo, Integer
PPC_evmwhsmia,        // Vector Multiply Word High Signed, Modulo, Integer to Accumulator
PPC_evmwhssf,         // Vector Multiply Word High Signed, Saturate, Fractional
PPC_evmwhssfa,        // Vector Multiply Word High Signed, Saturate, Fractional to Accumulator
PPC_evmwhumi,         // Vector Multiply Word High Unsigned, Modulo, Integer
PPC_evmwhumia,        // Vector Multiply Word High Unsigned, Modulo, Integer to Accumulator
PPC_evmwlsmiaaw,      // Vector Multiply Word Low Signed, Modulo, Integer and Accumulate into Words
PPC_evmwlsmianw,      // Vector Multiply Word Low Signed, Modulo, Integer and Accumulate Negative in Words
PPC_evmwlssiaaw,      // Vector Multiply Word Low Signed, Saturate, Integer and Accumulate into Words
PPC_evmwlssianw,      // Vector Multiply Word Low Signed, Saturate, Integer and Accumulate Negative in Words
PPC_evmwlumi,         // Vector Multiply Word Low Unsigned, Modulo, Integer
PPC_evmwlumia,        // Vector Multiply Word Low Unsigned, Modulo, Integer to Accumulator
PPC_evmwlumiaaw,      // Vector Multiply Word Low Unsigned, Modulo, Integer and Accumulate into Words
PPC_evmwlumianw,      // Vector Multiply Word Low Unsigned, Modulo, Integer and Accumulate Negative in Words
PPC_evmwlusiaaw,      // Vector Multiply Word Low Unsigned, Saturate, Integer and Accumulate into Words
PPC_evmwlusianw,      // Vector Multiply Word Low Unsigned, Saturate, Integer and Accumulate Negative in Words
PPC_evmwsmf,          // Vector Multiply Word Signed, Modulo, Fractional
PPC_evmwsmfa,         // Vector Multiply Word Signed, Modulo, Fractional to Accumulator
PPC_evmwsmfaa,        // Vector Multiply Word Signed, Modulo, Fractional and Accumulate
PPC_evmwsmfan,        // Vector Multiply Word Signed, Modulo, Fractional and Accumulate Negative
PPC_evmwsmi,          // Vector Multiply Word Signed, Modulo, Integer
PPC_evmwsmia,         // Vector Multiply Word Signed, Modulo, Integer to Accumulator
PPC_evmwsmiaa,        // Vector Multiply Word Signed, Modulo, Integer and Accumulate
PPC_evmwsmian,        // Vector Multiply Word Signed, Modulo, Integer and Accumulate Negative
PPC_evmwssf,          // Vector Multiply Word Signed, Saturate, Fractional
PPC_evmwssfa,         // Vector Multiply Word Signed, Saturate, Fractional to Accumulator
PPC_evmwssfaa,        // Vector Multiply Word Signed, Saturate, Fractional and Accumulate
PPC_evmwssfan,        // Vector Multiply Word Signed, Saturate, Fractional and Accumulate Negative
PPC_evmwumi,          // Vector Multiply Word Unsigned, Modulo, Integer
PPC_evmwumia,         // Vector Multiply Word Unsigned, Modulo, Integer to Accumulator
PPC_evmwumiaa,        // Vector Multiply Word Unsigned, Modulo, Integer and Accumulate
PPC_evmwumian,        // Vector Multiply Word Unsigned, Modulo, Integer and Accumulate Negative
PPC_evnand,           // Vector NAND
PPC_evneg,            // Vector Negate
PPC_evnor,            // Vector NOR
PPC_evor,             // Vector OR
PPC_evorc,            // Vector OR with Complement Form
PPC_evrlw,            // Vector Rotate Left Word
PPC_evrlwi,           // Vector Rotate Left Word Immediate
PPC_evrndw,           // Vector Round Word
PPC_evsel,            // Vector Select
PPC_evslw,            // Vector Shift Left Word
PPC_evslwi,           // Vector Shift Left Word Immediate
PPC_evsplatfi,        // Vector Splat Fractional Immediate
PPC_evsplati,         // Vector Splat Immediate
PPC_evsrwis,          // Vector Shift Right Word Immediate Signed
PPC_evsrwiu,          // Vector Shift Right Word Immediate Unsigned
PPC_evsrws,           // Vector Shift Right Word Signed
PPC_evsrwu,           // Vector Shift Right Word Unsigned
PPC_evstdd,           // Vector Store Double of Double
PPC_evstddx,          // Vector Store Double of Double Indexed
PPC_evstdh,           // Vector Store Double of Four Halfwords
PPC_evstdhx,          // Vector Store Double of Four Halfwords Indexed
PPC_evstdw,           // Vector Store Double of Two Words
PPC_evstdwx,          // Vector Store Double of Two Words Indexed
PPC_evstwhe,          // Vector Store Word of Two Halfwords from Even
PPC_evstwhex,         // Vector Store Word of Two Halfwords from Even Indexed
PPC_evstwho,          // Vector Store Word of Two Halfwords from Odd
PPC_evstwhox,         // Vector Store Word of Two Halfwords from Odd Indexed
PPC_evstwwe,          // Vector Store Word of Word from Even
PPC_evstwwex,         // Vector Store Word of Word from Even Indexed
PPC_evstwwo,          // Vector Store Word of Word from Odd
PPC_evstwwox,         // Vector Store Word of Word from Odd Indexed
PPC_evsubfsmiaaw,     // Vector Subtract Signed, Modulo, Integer to Accumulator Word
PPC_evsubfssiaaw,     // Vector Subtract Signed, Saturate, Integer to Accumulator Word
PPC_evsubfumiaaw,     // Vector Subtract Unsigned, Modulo, Integer to Accumulator Word
PPC_evsubfusiaaw,     // Vector Subtract Unsigned, Saturate, Integer to Accumulator Word
PPC_evsubfw,          // Vector Subtract from Word
PPC_evsubifw,         // Vector Subtract Immediate from Word
PPC_evxor,            // Vector XOR

// SPE.Embedded Float Scalar Double

PPC_efdabs,           // Floating-Point Double-Precision Absolute Value
PPC_efdadd,           // Floating-Point Double-Precision Add
PPC_efdcfs,           // Floating-Point Double-Precision Convert from Single-Precision
PPC_efdcfsf,          // Convert Floating-Point Double-Precision from Signed Fraction
PPC_efdcfsi,          // Convert Floating-Point Double-Precision from Signed Integer
PPC_efdcfsid,         // Convert Floating-Point Double-Precision from Signed Integer Doubleword
PPC_efdcfuf,          // Convert Floating-Point Double-Precision from Unsigned Fraction
PPC_efdcfui,          // Convert Floating-Point Double-Precision from Unsigned Integer
PPC_efdcfuid,         // Convert Floating-Point Double-Precision from Unsigned Integer Doubleword
PPC_efdcmpeq,         // Floating-Point Double-Precision Compare Equal
PPC_efdcmpgt,         // Floating-Point Double-Precision Compare Greater Than
PPC_efdcmplt,         // Floating-Point Double-Precision Compare Less Than
PPC_efdctsf,          // Convert Floating-Point Double-Precision to Signed Fraction
PPC_efdctsi,          // Convert Floating-Point Double-Precision to Signed Integer
PPC_efdctsidz,        // Convert Floating-Point Double-Precision to Signed Integer Doubleword with Round toward Zero
PPC_efdctsiz,         // Convert Floating-Point Double-Precision to Signed Integer with Round toward Zero
PPC_efdctuf,          // Convert Floating-Point Double-Precision to Unsigned Fraction
PPC_efdctui,          // Convert Floating-Point Double-Precision to Unsigned Integer
PPC_efdctuidz,        // Convert Floating-Point Double-Precision to Unsigned Integer Doubleword with Round toward Zero
PPC_efdctuiz,         // Convert Floating-Point Double-Precision to Unsigned Integer with Round toward Zero
PPC_efddiv,           // Floating-Point Double-Precision Divide
PPC_efdmul,           // Floating-Point Double-Precision Multiply
PPC_efdnabs,          // Floating-Point Double-Precision Negative Absolute Value
PPC_efdneg,           // Floating-Point Double-Precision Negate
PPC_efdsub,           // Floating-Point Double-Precision Subtract
PPC_efdtsteq,         // Floating-Point Double-Precision Test Equal
PPC_efdtstgt,         // Floating-Point Double-Precision Test Greater Than
PPC_efdtstlt,         // Floating-Point Double-Precision Test Less Than
PPC_efscfd,           // Floating-Point Single-Precision Convert from Double-Precision

// SPE.Embedded Float Scalar Single

PPC_efsabs,           // Floating-Point Single-Precision Absolute Value
PPC_efsadd,           // Floating-Point Single-Precision Add
PPC_efscfsf,          // Convert Floating-Point Single-Precision from Signed Fraction
PPC_efscfsi,          // Convert Floating-Point Single-Precision from Signed Integer
PPC_efscfuf,          // Convert Floating-Point Single-Precision from Unsigned Fraction
PPC_efscfui,          // Convert Floating-Point Single-Precision from Unsigned Integer
PPC_efscmpeq,         // Floating-Point Single-Precision Compare Equal
PPC_efscmpgt,         // Floating-Point Single-Precision Compare Greater Than
PPC_efscmplt,         // Floating-Point Single-Precision Compare Less Than
PPC_efsctsf,          // Convert Floating-Point Single-Precision to Signed Fraction
PPC_efsctsi,          // Convert Floating-Point Single-Precision to Signed Integer
PPC_efsctsiz,         // Convert Floating-Point Single-Precision to Signed Integer with Round toward Zero
PPC_efsctuf,          // Convert Floating-Point Single-Precision to Unsigned Fraction
PPC_efsctui,          // Convert Floating-Point Single-Precision to Unsigned Integer
PPC_efsctuiz,         // Convert Floating-Point Single-Precision to Unsigned Integer with Round toward Zero
PPC_efsdiv,           // Floating-Point Single-Precision Divide
PPC_efsmul,           // Floating-Point Single-Precision Multiply
PPC_efsnabs,          // Floating-Point Single-Precision Negative Absolute Value
PPC_efsneg,           // Floating-Point Single-Precision Negate
PPC_efssub,           // Floating-Point Single-Precision Subtract
PPC_efststeq,         // Floating-Point Single-Precision Test Equal
PPC_efststgt,         // Floating-Point Single-Precision Test Greater Than
PPC_efststlt,         // Floating-Point Single-Precision Test Less Than

// SPE.Embedded Float Vector

PPC_evfsabs,          // Vector Floating-Point Single-Precision Absolute Value
PPC_evfsadd,          // Vector Floating-Point Single-Precision Add
PPC_evfscfsf,         // Vector Convert Floating-Point Single-Precision from Signed Fraction
PPC_evfscfsi,         // Vector Convert Floating-Point Single-Precision from Signed Integer
PPC_evfscfuf,         // Vector Convert Floating-Point Single-Precision from Unsigned Fraction
PPC_evfscfui,         // Vector Convert Floating-Point Single-Precision from Unsigned Integer
PPC_evfscmpeq,        // Vector Floating-Point Single-Precision Compare Equal
PPC_evfscmpgt,        // Vector Floating-Point Single-Precision Compare Greater Than
PPC_evfscmplt,        // Vector Floating-Point Single-Precision Compare Less Than
PPC_evfsctsf,         // Vector Convert Floating-Point Single-Precision to Signed Fraction
PPC_evfsctsi,         // Vector Convert Floating-Point Single-Precision to Signed Integer
PPC_evfsctsiz,        // Vector Convert Floating-Point Single-Precision to Signed Integer with Round toward Zero
PPC_evfsctuf,         // Vector Convert Floating-Point Single-Precision to Unsigned Fraction
PPC_evfsctui,         // Vector Convert Floating-Point Single-Precision to Unsigned Integer
PPC_evfsctuiz,        // Vector Convert Floating-Point Single-Precision to Unsigned Integer with Round toward Zero
PPC_evfsdiv,          // Vector Floating-Point Single-Precision Divide
PPC_evfsmul,          // Vector Floating-Point Single-Precision Multiply
PPC_evfsnabs,         // Vector Floating-Point Single-Precision Negative Absolute Value
PPC_evfsneg,          // Vector Floating-Point Single-Precision Negate
PPC_evfssub,          // Vector Floating-Point Single-Precision Subtract
PPC_evfststeq,        // Vector Floating-Point Single-Precision Test Equal
PPC_evfststgt,        // Vector Floating-Point Single-Precision Test Greater Than
PPC_evfststlt,        // Vector Floating-Point Single-Precision Test Less Than

// Missing instructions from Power ISA 2.06

// category: 64-bit
PPC_bpermd,           // Bit Permute Doubleword
PPC_divde,            // Divide Doubleword Extended
PPC_divdeu,           // Divide Doubleword Extended Unsigned
PPC_ldbrx,            // Load Doubleword Byte-Reverse Indexed
PPC_prtyd,            // Parity Doubleword
PPC_stdbrx,           // Store Doubleword Byte-Reverse Indexed
// category: Base
PPC_cmpb,             // Compare Bytes
PPC_divwe,            // Divide Word Extended
PPC_divweu,           // Divide Word Extended Unsigned
PPC_lbarx,            // Load Byte and Reserve Indexed
PPC_lharx,            // Load Halfword and Reserve Indexed
PPC_popcntd,          // Population Count Doubleword
PPC_popcntw,          // Population Count Word
PPC_prtyw,            // Parity Word
PPC_stbcx,            // Store Byte Conditional Indexed
PPC_sthcx,            // Store Halfword Conditional Indexed
// category: Binary Coded Decimal Assist
PPC_addg6s,           // Add and Generate Sixes
PPC_cbcdtd,           // Convert Binary Coded Decimal to Declets
PPC_cdtbcd,           // Convert Declets To Binary Coded Decimal
// category: Decimal Floating-Point
PPC_dadd,             // DFP Add
PPC_daddq,            // DFP Add Quad
PPC_dcffix,           // DFP Convert From Fixed
PPC_dcffixq,          // DFP Convert From Fixed Quad
PPC_dcmpo,            // DFP Compare Ordered
PPC_dcmpoq,           // DFP Compare Ordered Quad
PPC_dcmpu,            // DFP Compare Unordered
PPC_dcmpuq,           // DFP Compare Unordered Quad
PPC_dctdp,            // DFP Convert To DFP Long
PPC_dctfix,           // DFP Convert To Fixed
PPC_dctfixq,          // DFP Convert To Fixed Quad
PPC_dctqpq,           // DFP Convert To DFP Extended
PPC_ddedpd,           // DFP Decode DPD To BCD
PPC_ddedpdq,          // DFP Decode DPD To BCD Quad
PPC_ddiv,             // DFP Divide
PPC_ddivq,            // DFP Divide Quad
PPC_denbcd,           // DFP Encode BCD To DPD
PPC_denbcdq,          // DFP Encode BCD To DPD Quad
PPC_diex,             // DFP Insert Biased Exponent
PPC_diexq,            // DFP Insert Biased Exponent Quad
PPC_dmul,             // DFP Multiply
PPC_dmulq,            // DFP Multiply Quad
PPC_dqua,             // DFP Quantize
PPC_dquai,            // DFP Quantize Immediate
PPC_dquaiq,           // DFP Quantize Immediate Quad
PPC_dquaq,            // DFP Quantize Quad
PPC_drdpq,            // DFP Round To DFP Long
PPC_drintn,           // DFP Round To FP Integer Without Inexact
PPC_drintnq,          // DFP Round To FP Integer Without Inexact Quad
PPC_drintx,           // DFP Round To FP Integer With Inexact
PPC_drintxq,          // DFP Round To FP Integer With Inexact Quad
PPC_drrnd,            // DFP Reround
PPC_drrndq,           // DFP Reround Quad
PPC_drsp,             // DFP Round To DFP Short
PPC_dscli,            // DFP Shift Significand Left Immediate
PPC_dscliq,           // DFP Shift Significand Left Immediate Quad
PPC_dscri,            // DFP Shift Significand Right Immediate
PPC_dscriq,           // DFP Shift Significand Right Immediate Quad
PPC_dsub,             // DFP Subtract
PPC_dsubq,            // DFP Subtract Quad
PPC_dtstdc,           // DFP Test Data Class
PPC_dtstdcq,          // DFP Test Data Class Quad
PPC_dtstdg,           // DFP Test Data Group
PPC_dtstdgq,          // DFP Test Data Group Quad
PPC_dtstex,           // DFP Test Exponent
PPC_dtstexq,          // DFP Test Exponent Quad
PPC_dtstsf,           // DFP Test Significance
PPC_dtstsfq,          // DFP Test Significance Quad
PPC_dxex,             // DFP Extract Biased Exponent
PPC_dxexq,            // DFP Extract Biased Exponent Quad
// category: Decorated Storage
PPC_dsn,              // Decorated Storage Notify
PPC_lbdx,             // Load Byte with Decoration Indexed
PPC_lddx,             // Load Doubleword with Decoration Indexed
PPC_lfddx,            // Load Floating Doubleword with Decoration Indexed
PPC_lhdx,             // Load Halfword with Decoration Indexed
PPC_lwdx,             // Load Word with Decoration Indexed
PPC_stbdx,            // Store Byte with Decoration Indexed
PPC_stddx,            // Store Doubleword with Decoration Indexed
PPC_stfddx,           // Store Floating Doubleword with Decoration Indexed
PPC_sthdx,            // Store Halfword with Decoration Indexed
PPC_stwdx,            // Store Word with Decoration Indexed
// category: Embedded
PPC_mbar,             // Memory Barrier
PPC_rfmci,            // Return From Machine Check Interrupt
PPC_tlbilx,           // TLB Invalidate Local
PPC_dci,              // Data Cache Invalidate
PPC_ici,              // Instruction Cache Invalidate
PPC_mfdcrux,          // Move From Device Control Register User-mode Indexed
PPC_mfdcrx,           // Move From Device Control Register Indexed
PPC_mtdcrux,          // Move To Device Control Register User-mode Indexed
PPC_mtdcrx,           // Move To Device Control Register Indexed
// category: Embedded.Enhanced Debug
PPC_dnh,              // Debugger Notify Halt
// category: Embedded.Hypervisor
PPC_ehpriv,           // Embedded Hypervisor Privilege
PPC_rfgi,             // Return From Guest Interrupt
// category: Embedded.Processor Control
PPC_msgclr,           // Message Clear
PPC_msgsnd,           // Message Send
// category: Embedded.External PID
PPC_dcbfep,           // Data Cache Block Flush by External PID
PPC_dcbstep,          // Data Cache Block Store by External PID
PPC_dcbtep,           // Data Cache Block Touch by External PID
PPC_dcbtstep,         // Data Cache Block Touch for Store by External PID
PPC_dcbzep,           // Data Cache Block set to Zero by External PID
PPC_evlddepx,         // Vector Load Doubleword into Doubleword by External Process ID Indexed
PPC_evstddepx,        // Vector Store Doubleword into Doubleword by External Process ID Indexed
PPC_icbiep,           // Instruction Cache Block Invalidate by External PID
PPC_lbepx,            // Load Byte by External Process ID Indexed
PPC_lfdepx,           // Load Floating-Point Double by External Process ID Indexed
PPC_lhepx,            // Load Halfword by External Process ID Indexed
PPC_lvepx,            // Load Vector by External Process ID Indexed
PPC_lvepxl,           // Load Vector by External Process ID Indexed LRU
PPC_lwepx,            // Load Word by External Process ID Indexed
PPC_stbepx,           // Store Byte by External Process ID Indexed
PPC_stfdepx,          // Store Floating-Point Double by External Process ID Indexed
PPC_sthepx,           // Store Halfword by External Process ID Indexed
PPC_stvepx,           // Store Vector by External Process ID Indexed
PPC_stvepxl,          // Store Vector by External Process ID Indexed LRU
PPC_stwepx,           // Store Word by External Process ID Indexed
PPC_ldepx,            // Load Doubleword by External Process ID Indexed
PPC_stdepx,           // Store Doubleword by External Process ID Indexed
// category: Embedded.Performance Monitor
PPC_mfpmr,            // Move From Performance Monitor Register
PPC_mtpmr,            // Move To Performance Monitor Register
// category: Embedded.Thread Management
PPC_mftmr,            // Move From Thread Management Register
PPC_mttmr,            // Move To Thread Management Register
// category: Embedded.TLB Write Conditional
PPC_tlbsrx,           // TLB Search and Reserve
// category: Floating-point
PPC_fcfids,           // Floating Convert From Integer Doubleword Single
PPC_fcfidu,           // Floating Convert From Integer Doubleword Unsigned
PPC_fcfidus,          // Floating Convert From Integer Doubleword Unsigned Single
PPC_fctidu,           // Floating Convert To Integer Doubleword Unsigned
PPC_fctiduz,          // Floating Convert To Integer Doubleword Unsigned with round toward Zero
PPC_fctiwu,           // Floating Convert To Integer Word Unsigned
PPC_fctiwuz,          // Floating Convert To Integer Word Unsigned with round toward Zero
PPC_ftdiv,            // Floating Test for software Divide
PPC_ftsqrt,           // Floating Test for software Square Root
PPC_lfiwax,           // Load Floating-Point as Integer Word Algebraic Indexed
PPC_lfiwzx,           // Load Floating-Point as Integer Word and Zero Indexed
PPC_lfdp,             // Load Floating-Point Double Pair
PPC_lfdpx,            // Load Floating-Point Double Pair Indexed
PPC_stfdp,            // Store Floating-Point Double Pair
PPC_stfdpx,           // Store Floating-Point Double Pair Indexed
PPC_fcpsgn,           // Floating Copy Sign
PPC_fre,              // Floating Reciprocal Estimate
PPC_frim,             // Floating Round to Integer Minus
PPC_frin,             // Floating Round to Integer Nearest
PPC_frip,             // Floating Round to Integer Plus
PPC_friz,             // Floating Round to Integer Toward Zero
// category: Legacy Integer Multiply-Accumulate
PPC_macchw,           // Multiply Accumulate Cross Halfword to Word Modulo Signed
PPC_macchws,          // Multiply Accumulate Cross Halfword to Word Saturate Signed
PPC_macchwsu,         // Multiply Accumulate Cross Halfword to Word Saturate Unsigned
PPC_macchwu,          // Multiply Accumulate Cross Halfword to Word Modulo Unsigned
PPC_machhw,           // Multiply Accumulate High Halfword to Word Modulo Signed
PPC_machhws,          // Multiply Accumulate High Halfword to Word Saturate Signed
PPC_machhwsu,         // Multiply Accumulate High Halfword to Word Saturate Unsigned
PPC_machhwu,          // Multiply Accumulate High Halfword to Word Modulo Unsigned
PPC_maclhw,           // Multiply Accumulate Low Halfword to Word Modulo Signed
PPC_maclhws,          // Multiply Accumulate Low Halfword to Word Saturate Signed
PPC_maclhwsu,         // Multiply Accumulate Low Halfword to Word Saturate Unsigned
PPC_maclhwu,          // Multiply Accumulate Low Halfword to Word Modulo Unsigned
PPC_mulchw,           // Multiply Cross Halfword to Word Signed
PPC_mulchwu,          // Multiply Cross Halfword to Word Unsigned
PPC_mulhhw,           // Multiply High Halfword to Word Signed
PPC_mulhhwu,          // Multiply High Halfword to Word Unsigned
PPC_mullhw,           // Multiply Low Halfword to Word Signed
PPC_mullhwu,          // Multiply Low Halfword to Word Unsigned
PPC_nmacchw,          // Negative Multiply Accumulate Cross Halfword to Word Modulo Signed
PPC_nmacchws,         // Negative Multiply Accumulate Cross Halfword to Word Saturate Signed
PPC_nmachhw,          // Negative Multiply Accumulate High Halfword to Word Modulo Signed
PPC_nmachhws,         // Negative Multiply Accumulate High Halfword to Word Saturate Signed
PPC_nmaclhw,          // Negative Multiply Accumulate Low Halfword to Word Modulo Signed
PPC_nmaclhws,         // Negative Multiply Accumulate Low Halfword to Word Saturate Signed
// category: Legacy Move Assist
PPC_dlmzb,            // Determine Leftmost Zero Byte
// category: Load/Store Quadword
PPC_lq,               // Load Quadword
PPC_stq,              // Store Quadword
// category: Server
PPC_doze,             // Doze
PPC_lbzcix,           // Load Byte and Zero Caching Inhibited Indexed
PPC_ldcix,            // Load Doubleword Caching Inhibited Indexed
PPC_lhzcix,           // Load Halfword and Zero Caching Inhibited Indexed
PPC_lwzcix,           // Load Word and Zero Caching Inhibited Indexed
PPC_nap,              // Nap
PPC_rvwinkle,         // Rip Van Winkle
PPC_slbfee,           // SLB Find Entry ESID
PPC_sleep,            // Sleep
PPC_stbcix,           // Store Byte Caching Inhibited Indexed
PPC_stdcix,           // Store Doubleword Caching Inhibited Indexed
PPC_sthcix,           // Store Halfword Caching Inhibited Indexed
PPC_stwcix,           // Store Word Caching Inhibited Indexed
PPC_tlbiel,           // TLB Invalidate Entry Local
// category: Vector
PPC_lvebx,            // Load Vector Element Byte Indexed
PPC_lvehx,            // Load Vector Element Halfword Indexed
PPC_lvewx,            // Load Vector Element Word Indexed
PPC_lvsl,             // Load Vector for Shift Left Indexed
PPC_lvsr,             // Load Vector for Shift Right Indexed
PPC_lvx,              // Load Vector Indexed
PPC_lvxl,             // Load Vector Indexed LRU
PPC_mfvscr,           // Move From Vector Status and Control Register
PPC_mtvscr,           // Move To Vector Status and Control Register
PPC_stvebx,           // Store Vector Element Byte Indexed
PPC_stvehx,           // Store Vector Element Halfword Indexed
PPC_stvewx,           // Store Vector Element Word Indexed
PPC_stvx,             // Store Vector Indexed
PPC_stvxl,            // Store Vector Indexed LRU
PPC_vaddcuw,          // Vector Add and Write Carry-Out Unsigned Word
PPC_vaddfp,           // Vector Add Single-Precision
PPC_vaddsbs,          // Vector Add Signed Byte Saturate
PPC_vaddshs,          // Vector Add Signed Halfword Saturate
PPC_vaddsws,          // Vector Add Signed Word Saturate
PPC_vaddubm,          // Vector Add Unsigned Byte Modulo
PPC_vaddubs,          // Vector Add Unsigned Byte Saturate
PPC_vadduhm,          // Vector Add Unsigned Halfword Modulo
PPC_vadduhs,          // Vector Add Unsigned Halfword Saturate
PPC_vadduwm,          // Vector Add Unsigned Word Modulo
PPC_vadduws,          // Vector Add Unsigned Word Saturate
PPC_vand,             // Vector Logical AND
PPC_vandc,            // Vector Logical AND with Complement
PPC_vavgsb,           // Vector Average Signed Byte
PPC_vavgsh,           // Vector Average Signed Halfword
PPC_vavgsw,           // Vector Average Signed Word
PPC_vavgub,           // Vector Average Unsigned Byte
PPC_vavguh,           // Vector Average Unsigned Halfword
PPC_vavguw,           // Vector Average Unsigned Word
PPC_vcfsx,            // Vector Convert From Signed Fixed-Point Word
PPC_vcfux,            // Vector Convert From Unsigned Fixed-Point Word
PPC_vcmpbfp,          // Vector Compare Bounds Single-Precision
PPC_vcmpeqfp,         // Vector Compare Equal To Single-Precision
PPC_vcmpequb,         // Vector Compare Equal To Unsigned Byte
PPC_vcmpequh,         // Vector Compare Equal To Unsigned Halfword
PPC_vcmpequw,         // Vector Compare Equal To Unsigned Word
PPC_vcmpgefp,         // Vector Compare Greater Than or Equal To Single-Precision
PPC_vcmpgtfp,         // Vector Compare Greater Than Single-Precision
PPC_vcmpgtsb,         // Vector Compare Greater Than Signed Byte
PPC_vcmpgtsh,         // Vector Compare Greater Than Signed Halfword
PPC_vcmpgtsw,         // Vector Compare Greater Than Signed Word
PPC_vcmpgtub,         // Vector Compare Greater Than Unsigned Byte
PPC_vcmpgtuh,         // Vector Compare Greater Than Unsigned Halfword
PPC_vcmpgtuw,         // Vector Compare Greater Than Unsigned Word
PPC_vctsxs,           // Vector Convert To Signed Fixed-Point Word Saturate
PPC_vctuxs,           // Vector Convert To Unsigned Fixed-Point Word Saturate
PPC_vexptefp,         // Vector 2 Raised to the Exponent Estimate FloatingPoint
PPC_vlogefp,          // Vector Log Base 2 Estimate Floating-Point
PPC_vmaddfp,          // Vector Multiply-Add Single-Precision
PPC_vmaxfp,           // Vector Maximum Single-Precision
PPC_vmaxsb,           // Vector Maximum Signed Byte
PPC_vmaxsh,           // Vector Maximum Signed Halfword
PPC_vmaxsw,           // Vector Maximum Signed Word
PPC_vmaxub,           // Vector Maximum Unsigned Byte
PPC_vmaxuh,           // Vector Maximum Unsigned Halfword
PPC_vmaxuw,           // Vector Maximum Unsigned Word
PPC_vmhaddshs,        // Vector Multiply-High-Add Signed Halfword Saturate
PPC_vmhraddshs,       // Vector Multiply-High-Round-Add Signed Halfword Saturate
PPC_vminfp,           // Vector Minimum Single-Precision
PPC_vminsb,           // Vector Minimum Signed Byte
PPC_vminsh,           // Vector Minimum Signed Halfword
PPC_vminsw,           // Vector Minimum Signed Word
PPC_vminub,           // Vector Minimum Unsigned Byte
PPC_vminuh,           // Vector Minimum Unsigned Halfword
PPC_vminuw,           // Vector Minimum Unsigned Word
PPC_vmladduhm,        // Vector Multiply-Low-Add Unsigned Halfword Modulo
PPC_vmrghb,           // Vector Merge High Byte
PPC_vmrghh,           // Vector Merge High Halfword
PPC_vmrghw,           // Vector Merge High Word
PPC_vmrglb,           // Vector Merge Low Byte
PPC_vmrglh,           // Vector Merge Low Halfword
PPC_vmrglw,           // Vector Merge Low Word
PPC_vmsummbm,         // Vector Multiply-Sum Mixed Byte Modulo
PPC_vmsumshm,         // Vector Multiply-Sum Signed Halfword Modulo
PPC_vmsumshs,         // Vector Multiply-Sum Signed Halfword Saturate
PPC_vmsumubm,         // Vector Multiply-Sum Unsigned Byte Modulo
PPC_vmsumuhm,         // Vector Multiply-Sum Unsigned Halfword Modulo
PPC_vmsumuhs,         // Vector Multiply-Sum Unsigned Halfword Saturate
PPC_vmulesb,          // Vector Multiply Even Signed Byte
PPC_vmulesh,          // Vector Multiply Even Signed Halfword
PPC_vmuleub,          // Vector Multiply Even Unsigned Byte
PPC_vmuleuh,          // Vector Multiply Even Unsigned Halfword
PPC_vmulosb,          // Vector Multiply Odd Signed Byte
PPC_vmulosh,          // Vector Multiply Odd Signed Halfword
PPC_vmuloub,          // Vector Multiply Odd Unsigned Byte
PPC_vmulouh,          // Vector Multiply Odd Unsigned Halfword
PPC_vnmsubfp,         // Vector Negative Multiply-Subtract Single-Precision
PPC_vnor,             // Vector Logical NOR
PPC_vor,              // Vector Logical OR
PPC_vperm,            // Vector Permute
PPC_vpkpx,            // Vector Pack Pixel
PPC_vpkshss,          // Vector Pack Signed Halfword Signed Saturate
PPC_vpkshus,          // Vector Pack Signed Halfword Unsigned Saturate
PPC_vpkswss,          // Vector Pack Signed Word Signed Saturate
PPC_vpkswus,          // Vector Pack Signed Word Unsigned Saturate
PPC_vpkuhum,          // Vector Pack Unsigned Halfword Unsigned Modulo
PPC_vpkuhus,          // Vector Pack Unsigned Halfword Unsigned Saturate
PPC_vpkuwum,          // Vector Pack Unsigned Word Unsigned Modulo
PPC_vpkuwus,          // Vector Pack Unsigned Word Unsigned Saturate
PPC_vrefp,            // Vector Reciprocal Estimate Single-Precision
PPC_vrfim,            // Vector Round to Single-Precision Integer toward -Infinity
PPC_vrfin,            // Vector Round to Single-Precision Integer Nearest
PPC_vrfip,            // Vector Round to Single-Precision Integer toward +Infinity
PPC_vrfiz,            // Vector Round to Single-Precision Integer toward Zero
PPC_vrlb,             // Vector Rotate Left Byte
PPC_vrlh,             // Vector Rotate Left Halfword
PPC_vrlw,             // Vector Rotate Left Word
PPC_vrsqrtefp,        // Vector Reciprocal Square Root Estimate Single-Precision
PPC_vsel,             // Vector Select
PPC_vsl,              // Vector Shift Left
PPC_vslb,             // Vector Shift Left Byte
PPC_vsldoi,           // Vector Shift Left Double by Octet Immediate
PPC_vslh,             // Vector Shift Left Halfword
PPC_vslo,             // Vector Shift Left by Octet
PPC_vslw,             // Vector Shift Left Word
PPC_vspltb,           // Vector Splat Byte
PPC_vsplth,           // Vector Splat Halfword
PPC_vspltisb,         // Vector Splat Immediate Signed Byte
PPC_vspltish,         // Vector Splat Immediate Signed Halfword
PPC_vspltisw,         // Vector Splat Immediate Signed Word
PPC_vspltw,           // Vector Splat Word
PPC_vsr,              // Vector Shift Right
PPC_vsrab,            // Vector Shift Right Algebraic Byte
PPC_vsrah,            // Vector Shift Right Algebraic Halfword
PPC_vsraw,            // Vector Shift Right Algebraic Word
PPC_vsrb,             // Vector Shift Right Byte
PPC_vsrh,             // Vector Shift Right Halfword
PPC_vsro,             // Vector Shift Right by Octet
PPC_vsrw,             // Vector Shift Right Word
PPC_vsubcuw,          // Vector Subtract and Write Carry-Out Unsigned Word
PPC_vsubfp,           // Vector Subtract Single-Precision
PPC_vsubsbs,          // Vector Subtract Signed Byte Saturate
PPC_vsubshs,          // Vector Subtract Signed Halfword Saturate
PPC_vsubsws,          // Vector Subtract Signed Word Saturate
PPC_vsububm,          // Vector Subtract Unsigned Byte Modulo
PPC_vsububs,          // Vector Subtract Unsigned Byte Saturate
PPC_vsubuhm,          // Vector Subtract Unsigned Halfword Modulo
PPC_vsubuhs,          // Vector Subtract Unsigned Halfword Saturate
PPC_vsubuwm,          // Vector Subtract Unsigned Word Modulo
PPC_vsubuws,          // Vector Subtract Unsigned Word Saturate
PPC_vsum2sws,         // Vector Sum across Half Signed Word Saturate
PPC_vsum4sbs,         // Vector Sum across Quarter Signed Byte Saturate
PPC_vsum4shs,         // Vector Sum across Quarter Signed Halfword Saturate
PPC_vsum4ubs,         // Vector Sum across Quarter Unsigned Byte Saturate
PPC_vsumsws,          // Vector Sum across Signed Word Saturate
PPC_vupkhpx,          // Vector Unpack High Pixel
PPC_vupkhsb,          // Vector Unpack High Signed Byte
PPC_vupkhsh,          // Vector Unpack High Signed Halfword
PPC_vupklpx,          // Vector Unpack Low Pixel
PPC_vupklsb,          // Vector Unpack Low Signed Byte
PPC_vupklsh,          // Vector Unpack Low Signed Halfword
PPC_vxor,             // Vector Logical XOR
// category: Vector-Scalar Floating-Point
PPC_lxsdx,            // Load VSR Scalar Doubleword Indexed
PPC_lxvd2x,           // Load VSR Vector Doubleword*2 Indexed
PPC_lxvdsx,           // Load VSR Vector Doubleword & Splat Indexed
PPC_lxvw4x,           // Load VSR Vector Word*4 Indexed
PPC_stxsdx,           // Store VSR Scalar Doubleword Indexed
PPC_stxvd2x,          // Store VSR Vector Doubleword*2 Indexed
PPC_stxvw4x,          // Store VSR Vector Word*4 Indexed
PPC_xsabsdp,          // VSX Scalar Absolute Value Double-Precision
PPC_xsadddp,          // VSX Scalar Add Double-Precision
PPC_xscmpodp,         // VSX Scalar Compare Ordered Double-Precision
PPC_xscmpudp,         // VSX Scalar Compare Unordered Double-Precision
PPC_xscpsgndp,        // VSX Scalar Copy Sign Double-Precision
PPC_xscvdpsp,         // VSX Scalar Convert Double-Precision to SinglePrecision
PPC_xscvdpsxds,       // VSX Scalar truncate Double-Precision to integer and Convert to Signed Fixed-Point Doubleword format with Saturate
PPC_xscvdpsxws,       // VSX Scalar truncate Double-Precision to integer and Convert to Signed Fixed-Point Word format with Saturate
PPC_xscvdpuxds,       // VSX Scalar truncate Double-Precision to integer and Convert to Unsigned Fixed-Point Doubleword format with Saturate
PPC_xscvdpuxws,       // VSX Scalar truncate Double-Precision to integer and Convert to Unsigned Fixed-Point Word format with Saturate
PPC_xscvspdp,         // VSX Scalar Convert Single-Precision to DoublePrecision format
PPC_xscvsxddp,        // VSX Scalar Convert and round Signed Fixed-Point Doubleword to Double-Precision format
PPC_xscvuxddp,        // VSX Scalar Convert and round Unsigned Fixed-Point Doubleword to Double-Precision format
PPC_xsdivdp,          // VSX Scalar Divide Double-Precision
PPC_xsmaddadp,        // VSX Scalar Multiply-Add Type-A Double-Precision
PPC_xsmaddmdp,        // VSX Scalar Multiply-Add Type-M Double-Precision
PPC_xsmaxdp,          // VSX Scalar Maximum Double-Precision
PPC_xsmindp,          // VSX Scalar Minimum Double-Precision
PPC_xsmsubadp,        // VSX Scalar Multiply-Subtract Type-A Double-Precision
PPC_xsmsubmdp,        // VSX Scalar Multiply-Subtract Type-M Double-Precision
PPC_xsmuldp,          // VSX Scalar Multiply Double-Precision
PPC_xsnabsdp,         // VSX Scalar Negative Absolute Value Double-Precision
PPC_xsnegdp,          // VSX Scalar Negate Double-Precision
PPC_xsnmaddadp,       // VSX Scalar Negative Multiply-Add Type-A DoublePrecision
PPC_xsnmaddmdp,       // VSX Scalar Negative Multiply-Add Type-M DoublePrecision
PPC_xsnmsubadp,       // VSX Scalar Negative Multiply-Subtract Type-A DoublePrecision
PPC_xsnmsubmdp,       // VSX Scalar Negative Multiply-Subtract Type-M DoublePrecision
PPC_xsrdpi,           // VSX Scalar Round to Double-Precision Integer
PPC_xsrdpic,          // VSX Scalar Round to Double-Precision Integer using Current rounding mode
PPC_xsrdpim,          // VSX Scalar Round to Double-Precision Integer toward Infinity
PPC_xsrdpip,          // VSX Scalar Round to Double-Precision Integer toward + Infinity
PPC_xsrdpiz,          // VSX Scalar Round to Double-Precision Integer toward Zero
PPC_xsredp,           // VSX Scalar Reciprocal Estimate Double-Precision
PPC_xsrsqrtedp,       // VSX Scalar Reciprocal Square Root Estimate DoublePrecision
PPC_xssqrtdp,         // VSX Scalar Square Root Double-Precision
PPC_xssubdp,          // VSX Scalar Subtract Double-Precision
PPC_xstdivdp,         // VSX Scalar Test for software Divide Double-Precision
PPC_xstsqrtdp,        // VSX Scalar Test for software Square Root DoublePrecision
PPC_xvabsdp,          // VSX Vector Absolute Value Double-Precision
PPC_xvabssp,          // VSX Vector Absolute Value Single-Precision
PPC_xvadddp,          // VSX Vector Add Double-Precision
PPC_xvaddsp,          // VSX Vector Add Single-Precision
PPC_xvcmpeqdp,        // VSX Vector Compare Equal To Double-Precision
PPC_xvcmpeqsp,        // VSX Vector Compare Equal To Single-Precision
PPC_xvcmpgedp,        // VSX Vector Compare Greater Than or Equal To Double-Precision
PPC_xvcmpgesp,        // VSX Vector Compare Greater Than or Equal To SinglePrecision
PPC_xvcmpgtdp,        // VSX Vector Compare Greater Than Double-Precision
PPC_xvcmpgtsp,        // VSX Vector Compare Greater Than Single-Precision
PPC_xvcpsgndp,        // VSX Vector Copy Sign Double-Precision
PPC_xvcpsgnsp,        // VSX Vector Copy Sign Single-Precision
PPC_xvcvdpsp,         // VSX Vector round and Convert Double-Precision to Single-Precision format
PPC_xvcvdpsxds,       // VSX Vector truncate Double-Precision to integer and Convert to Signed Fixed-Point Doubleword Saturate
PPC_xvcvdpsxws,       // VSX Vector truncate Double-Precision to integer and Convert to Signed Fixed-Point Word Saturate
PPC_xvcvdpuxds,       // VSX Vector truncate Double-Precision to integer and Convert to Unsigned Fixed-Point Doubleword format with Saturate
PPC_xvcvdpuxws,       // VSX Vector truncate Double-Precision to integer and Convert to Unsigned Fixed-Point Word format with Saturate
PPC_xvcvspdp,         // VSX Vector Convert Single-Precision to DoublePrecision
PPC_xvcvspsxds,       // VSX Vector truncate Single-Precision to integer and Convert to Signed Fixed-Point Doubleword format with Saturate
PPC_xvcvspsxws,       // VSX Vector truncate Single-Precision to integer and Convert to Signed Fixed-Point Word format with Saturate
PPC_xvcvspuxds,       // VSX Vector truncate Single-Precision to integer and Convert to Unsigned Fixed-Point Doubleword format with Saturate
PPC_xvcvspuxws,       // VSX Vector truncate Single-Precision to integer and Convert to Unsigned Fixed-Point Word Saturate
PPC_xvcvsxddp,        // VSX Vector Convert and round Signed Fixed-Point Doubleword to Double-Precision format
PPC_xvcvsxdsp,        // VSX Vector Convert and round Signed Fixed-Point Doubleword to Single-Precision format
PPC_xvcvsxwdp,        // VSX Vector Convert Signed Fixed-Point Word to Double-Precision format
PPC_xvcvsxwsp,        // VSX Vector Convert and round Signed Fixed-Point Word to Single-Precision format
PPC_xvcvuxddp,        // VSX Vector Convert and round Unsigned Fixed-Point Doubleword to Double-Precision format
PPC_xvcvuxdsp,        // VSX Vector Convert and round Unsigned Fixed-Point Doubleword to Single-Precision format
PPC_xvcvuxwdp,        // VSX Vector Convert Unsigned Fixed-Point Word to Double-Precision format
PPC_xvcvuxwsp,        // VSX Vector Convert and round Unsigned Fixed-Point Word to Single-Precision format
PPC_xvdivdp,          // VSX Vector Divide Double-Precision
PPC_xvdivsp,          // VSX Vector Divide Single-Precision
PPC_xvmaddadp,        // VSX Vector Multiply-Add Type-A Double-Precision
PPC_xvmaddasp,        // VSX Vector Multiply-Add Type-A Single-Precision
PPC_xvmaddmdp,        // VSX Vector Multiply-Add Type-M Double-Precision
PPC_xvmaddmsp,        // VSX Vector Multiply-Add Type-M Single-Precision
PPC_xvmaxdp,          // VSX Vector Maximum Double-Precision
PPC_xvmaxsp,          // VSX Vector Maximum Single-Precision
PPC_xvmindp,          // VSX Vector Minimum Double-Precision
PPC_xvminsp,          // VSX Vector Minimum Single-Precision
PPC_xvmsubadp,        // VSX Vector Multiply-Subtract Type-A Double-Precision
PPC_xvmsubasp,        // VSX Vector Multiply-Subtract Type-A Single-Precision
PPC_xvmsubmdp,        // VSX Vector Multiply-Subtract Type-M Double-Precision
PPC_xvmsubmsp,        // VSX Vector Multiply-Subtract Type-M Single-Precision
PPC_xvmuldp,          // VSX Vector Multiply Double-Precision
PPC_xvmulsp,          // VSX Vector Multiply Single-Precision
PPC_xvnabsdp,         // VSX Vector Negative Absolute Value Double-Precision
PPC_xvnabssp,         // VSX Vector Negative Absolute Value Single-Precision
PPC_xvnegdp,          // VSX Vector Negate Double-Precision
PPC_xvnegsp,          // VSX Vector Negate Single-Precision
PPC_xvnmaddadp,       // VSX Vector Negative Multiply-Add Type-A DoublePrecision
PPC_xvnmaddasp,       // VSX Vector Negative Multiply-Add Type-A SinglePrecision
PPC_xvnmaddmdp,       // VSX Vector Negative Multiply-Add Type-M DoublePrecision
PPC_xvnmaddmsp,       // VSX Vector Negative Multiply-Add Type-M SinglePrecision
PPC_xvnmsubadp,       // VSX Vector Negative Multiply-Subtract Type-A DoublePrecision
PPC_xvnmsubasp,       // VSX Vector Negative Multiply-Subtract Type-A SinglePrecision
PPC_xvnmsubmdp,       // VSX Vector Negative Multiply-Subtract Type-M DoublePrecision
PPC_xvnmsubmsp,       // VSX Vector Negative Multiply-Subtract Type-M SinglePrecision
PPC_xvrdpi,           // VSX Vector Round to Double-Precision Integer
PPC_xvrdpic,          // VSX Vector Round to Double-Precision Integer using Current rounding mode
PPC_xvrdpim,          // VSX Vector Round to Double-Precision Integer toward Infinity
PPC_xvrdpip,          // VSX Vector Round to Double-Precision Integer toward + Infinity
PPC_xvrdpiz,          // VSX Vector Round to Double-Precision Integer toward Zero
PPC_xvredp,           // VSX Vector Reciprocal Estimate Double-Precision
PPC_xvresp,           // VSX Vector Reciprocal Estimate Single-Precision
PPC_xvrspi,           // VSX Vector Round to Single-Precision Integer
PPC_xvrspic,          // VSX Vector Round to Single-Precision Integer using Current rounding mode
PPC_xvrspim,          // VSX Vector Round to Single-Precision Integer toward Infinity
PPC_xvrspip,          // VSX Vector Round to Single-Precision Integer toward + Infinity
PPC_xvrspiz,          // VSX Vector Round to Single-Precision Integer toward Zero
PPC_xvrsqrtedp,       // VSX Vector Reciprocal Square Root Estimate DoublePrecision
PPC_xvrsqrtesp,       // VSX Vector Reciprocal Square Root Estimate SinglePrecision
PPC_xvsqrtdp,         // VSX Vector Square Root Double-Precision
PPC_xvsqrtsp,         // VSX Vector Square Root Single-Precision
PPC_xvsubdp,          // VSX Vector Subtract Double-Precision
PPC_xvsubsp,          // VSX Vector Subtract Single-Precision
PPC_xvtdivdp,         // VSX Vector Test for software Divide Double-Precision
PPC_xvtdivsp,         // VSX Vector Test for software Divide Single-Precision
PPC_xvtsqrtdp,        // VSX Vector Test for software Square Root DoublePrecision
PPC_xvtsqrtsp,        // VSX Vector Test for software Square Root SinglePrecision
PPC_xxland,           // VSX Logical AND
PPC_xxlandc,          // VSX Logical AND with Complement
PPC_xxlnor,           // VSX Logical NOR
PPC_xxlor,            // VSX Logical OR
PPC_xxlxor,           // VSX Logical XOR
PPC_xxmrghw,          // VSX Merge High Word
PPC_xxmrglw,          // VSX Merge Low Word
PPC_xxpermdi,         // VSX Permute Doubleword Immediate
PPC_xxsel,            // VSX Select
PPC_xxsldwi,          // VSX Shift Left Double by Word Immediate
PPC_xxspltw,          // VSX Splat Word
// category: Wait
PPC_wait,             // Wait

// Obsolete AltiVec instructions

PPC_dss,              // Data Stream Stop
PPC_dssall,           // Data Stream Stop All
PPC_dst,              // Data Stream Touch
PPC_dstt,             // Data Stream Touch Transient
PPC_dstst,            // Data Stream Touch for Store
PPC_dststt,           // Data Stream Touch for Store Transient

// Cell BE AltiVec extension
PPC_lvlx,             // Load Vector Left Indexed
PPC_lvlxl,            // Load Vector Left Indexed Last
PPC_lvrx,             // Load Vector Right Indexed
PPC_lvrxl,            // Load Vector Right Indexed Last
PPC_stvlx,            // Store Vector Left Indexed
PPC_stvlxl,           // Store Vector Left Indexed Last
PPC_stvrx,            // Store Vector Right Indexed
PPC_stvrxl,           // Store Vector Right Indexed Last

// VLE instructions that don't have matching mnemonics
PPC_add16i,           // [e_add16i] Add Immediate
PPC_add2i,            // [e_add2i] Add (2 operand) Immediate and Record
PPC_add2is,           // [e_add2is] Add (2 operand) Immediate Shifted
PPC_and2i,            // [e_and2i] AND (two operand) Immediate
PPC_and2is,           // [e_and2is] AND (2 operand) Immediate Shifted
PPC_cmp16i,           // [e_cmp16i] Compare Immediate Word
PPC_cmph,             // [e_cmph] Compare Halfword
PPC_cmph16i,          // [e_cmph16i] Compare Halfword Immediate
PPC_cmphl,            // [e_cmphl] Compare Halfword Logical
PPC_cmphl16i,         // [e_cmphl16i] Compare Halfword Logical Immediate
PPC_cmpl16i,          // [e_cmpl16i] Compare Logical Immediate Word
PPC_mull2i,           // [e_mull2i] Multiply (2 operand) Low Immediate
PPC_or2i,             // [e_or2i] OR (two operand) Immediate
PPC_or2is,            // [e_or2is] OR (2 operand) Immediate Shifted
PPC_rlw,              // [e_rlw] Rotate Left Word
PPC_rlwi,             // [e_rlwi] Rotate Left Word Immediate
PPC_bclri,            // [se_bclri] Bit Clear Immediate
//PPC_bctr,             // [se_bctr] Branch to Count Register 
//PPC_bctrl,            // [se_bctrl] Branch to Count Register and Link
PPC_bgeni,            // [se_bgeni] Bit Generate Immediate
PPC_bmaski,           // [se_bmaski] Bit Mask Generate Immediate
PPC_bseti,            // [se_bseti] Bit Set Immediate
PPC_btsti,            // [se_btsti] Bit Test Immediate
PPC_extzb,            // [se_extzb] Extend Zero Byte
PPC_extzh,            // [se_extzh] Extend Zero Halfword
PPC_illegal,          // [se_illegal] Illegal
PPC_mfar,             // [se_mfar] Move from Alternate Register
PPC_mtar,             // [se_mtar] Move To Alternate Register
PPC_sub,              // [se_sub] Subtract
// simplified VLE mnemonics
PPC_sub16i,           // [e_sub16i] Subtract Immediate
PPC_sub2i,            // [e_sub2i] Subtract (2 operand) Immediate
PPC_sub2is,           // [e_sub2is] Subtract (2 operand) Immediate Shifted

// misc extended mnemonics
PPC_extldi,     // Extract and Left Justify Immediate
PPC_extrdi,     // Extract and Right Justify Immediate
PPC_insrdi,     // Insert from Right Immediate
PPC_rotldi,     // Rotate Left Immediate
PPC_rotrdi,     // Rotate Right Immediate
PPC_rotld,      // Rotate Left
PPC_sldi,       // Shift Left Immediate
PPC_srdi,       // Shift Right Immediate
PPC_clrldi,     // Clear Left Immediate
PPC_clrrdi,     // Clear Right Immediate
PPC_clrlsldi,   // Clear Left and Shift Left Immediate

PPC_xnop,       // Extended No Operation
PPC_hnop,       // Reserved no-op hint
PPC_dcbfl,      // Data Cache Block Flush Local
PPC_dcbflp,     // Data Cache Block Flush Local Primary
PPC_dcbtt,      // Data Cache Block Touch
PPC_dcbtstt,    // Data Cache Block Touch for Store
PPC_lwsync,     // Lightweight Sync
PPC_ptesync,    // Page Table Entry Sync
PPC_waitrsv,    // Wait For Reservation Loss
PPC_waitimpl,   // Wait For Implementation-specific Condition
PPC_evmr,       // Vector Move
PPC_evnot,      // Vector Not
PPC_mtcr,       // Move to Condition Register
PPC_xvmovdp,    // VSX Vector Copy Double-Precision
PPC_xvmovsp,    // VSX Vector Copy Single-Precision
PPC_xxspltd,    // VSX Splat Doubleword
PPC_xxmrghd,    // VSX Merge High Doubleword
PPC_xxmrgld,    // VSX Merge Low Doubleword
PPC_xxswapd,    // VSX Swap Doubleword

// Xbox 360 specific
PPC_dcbz128,    // Data Cache Block Clear to Zero (128 bytes)
PPC_mtmsree,    // Move to Machine State Register EE

// new Xbox 360 instructions (VMX128)
PPC_vcfpsxws,   // Vector Convert From Single-Precision to Signed Fixed-point Word Saturate
PPC_vcfpuxws,   // Vector Convert From Single-Precision to Unsigned Fixed-point Word Saturate
PPC_vcsxwfp,    // Vector Convert from Signed Fixed-point Word to Single-Precision
PPC_vcuxwfp,    // Vector Convert from Unsigned Fixed-point Word to Single-Precision
PPC_vmaddcfp,   // Vector Convert from Unsigned Fixed-point Word to Single-Precision
PPC_vmsum3fp,   // Vector Multiply-Sum 3-way Single-Precision
PPC_vmsum4fp,   // Vector Multiply-Sum 4-way Single-Precision
PPC_vmulfp,     // Vector Multiply Single-Precision
PPC_vpermwi,    // Vector Permute Word Immediate
PPC_vpkd3d,     // Vector Pack D3Dtype, Rotate Left Immediate and Mask Insert
PPC_vrlimi,     // Vector Rotate Left Immediate and Mask Insert
PPC_vupkd3d,    // Vector Unpack D3Dtype

// Paired Singles (750CL/Gekko/Broadway)
PPC_ps_cmpu0,   // Paired Single Compare Unordered High
PPC_psq_lx,     // Paired Single Quantized Load Indexed
PPC_psq_stx,    // Paired Single Quantized Store Indexed
PPC_ps_sum0,    // Paired Single Vector Sum High
PPC_ps_sum1,    // Paired Single Vector Sum Low
PPC_ps_muls0,   // Paired Single Multiply Scalar High
PPC_ps_muls1,   // Paired Single Multiply Scalar Low
PPC_ps_madds0,  // Paired Single Multiply-Add Scalar High
PPC_ps_madds1,  // Paired Single Multiply-Add Scalar Low
PPC_ps_div,     // Paired Single Divide
PPC_ps_sub,     // Paired Single Subtract
PPC_ps_add,     // Paired Single Add
PPC_ps_sel,     // Paired Single Select
PPC_ps_res,     // Paired Single Reciprocal Estimate
PPC_ps_mul,     // Paired Single Multiply
PPC_ps_rsqrte,  // Paired Single Reciprocal Square Root Estimate
PPC_ps_msub,    // Paired Single Multiply-Subtract
PPC_ps_madd,    // Paired Single Multiply-Add
PPC_ps_nmsub,   // Paired Single Negative Multiply-Subtract
PPC_ps_nmadd,   // Paired Single Negative Multiply-Add
PPC_ps_cmpo0,   // Paired Single Compare Ordered High
PPC_psq_lux,    // Paired Single Quantized Load with update Indexed
PPC_psq_stux,   // Paired Single Quantized Store with update Indexed
PPC_ps_neg,     // Paired Single Negate
PPC_ps_cmpu1,   // Paired Single Compare Unordered Low
PPC_ps_mr,      // Paired Single Move Register
PPC_ps_cmpo1,   // Paired Single Compare Ordered Low
PPC_ps_nabs,    // Paired Single Negative Absolute Value
PPC_ps_abs,     // Paired Single Absolute Value
PPC_ps_merge00, // Paired Single Merge High
PPC_ps_merge01, // Paired Single Merge Direct
PPC_ps_merge10, // Paired Single Merge Swapped
PPC_ps_merge11, // Paired Single Merge Low
PPC_dcbz_l,     // Data Cache Block Set to Zero Locked
PPC_psq_l,      // Paired Single Quantized Load
PPC_psq_lu,     // Paired Single Quantized Load update
PPC_psq_st,     // Paired Single Quantized Store
PPC_psq_stu,    // Paired Single Quantized Store update

// additional SPE/EFPU2 instructions
PPC_evfsmadd,   // Vector Floating-Point Single-Precision Multiply-Add
PPC_evfsmsub,   // Vector Floating-Point Single-Precision Multiply-Subtract
PPC_evfssqrt,   // Vector Floating-Point Single-Precision Square Root
PPC_evfsnmadd,  // Vector Floating-Point Single-Precision Negative Multiply-Add
PPC_evfsnmsub,  // Vector Floating-Point Single-Precision Negative Multiply-Subtract
PPC_evfsmax,    // Vector Floating-Point Single-Precision Maximum
PPC_evfsmin,    // Vector Floating-Point Single-Precision Minimum
PPC_evfsaddsub, // Vector Floating-Point Single-Precision Add / Subtract
PPC_evfssubadd, // Vector Floating-Point Single-Precision Subtract / Add
PPC_evfssum,    // Vector Floating-Point Single-Precision Sums
PPC_evfsdiff,   // Vector Floating-Point Single-Precision Differences
PPC_evfssumdiff,// Vector Floating-Point Single-Precision Sum / Difference
PPC_evfsdiffsum,// Vector Floating-Point Single-Precision Difference / Sum
PPC_evfsaddx,   // Vector Floating-Point Single-Precision Add Exchanged
PPC_evfssubx,   // Vector Floating-Point Single-Precision Subtract Exchanged
PPC_evfsaddsubx,// Vector Floating-Point Single-Precision Add / Subtract Exchanged
PPC_evfssubaddx,// Vector Floating-Point Single-Precision Subtract / Add Exchanged
PPC_evfsmulx,   // Vector Floating-Point Single-Precision Multiply Exchanged
PPC_evfsmule,   // Vector Floating-Point Single-Precision Multiply By Even Element
PPC_evfsmulo,   // Vector Floating-Point Single-Precision Multiply By Odd Element
PPC_evfscfh,    // Vector Convert Floating-Point Single-Precision from Half-Precision
PPC_evfscth,    // Vector Convert Floating-Point Single-Precision to Half-Precision
PPC_efsmax,     // Floating-Point Single-Precision Maximum
PPC_efsmin,     // Floating-Point Single-Precision Minimum
PPC_efsmadd,    // Floating-Point Single-Precision Multiply-Add
PPC_efsmsub,    // Floating-Point Single-Precision Multiply-Subtract
PPC_efssqrt,    // Floating-Point Single-Precision Square Root
PPC_efsnmadd,   // Floating-Point Single-Precision Negative Multiply-Add
PPC_efsnmsub,   // Floating-Point Single-Precision Negative Multiply-Subtract
PPC_efscfh,     // Convert Floating-Point Single-Precision from Half-Precision
PPC_efscth,     // Convert Floating-Point Single-Precision to Half-Precision

// Volatile Context Save/Restore APU
PPC_lmvgprw,    // Load Multiple Volatile GPR Word
PPC_stmvgprw,   // Store Multiple Volatile GPR Word
PPC_lmvsprw,    // Load Multiple Volatile SPR Word
PPC_stmvsprw,   // Store Multiple Volatile SPR Word
PPC_lmvsrrw,    // Load Multiple Volatile SRR Word
PPC_stmvsrrw,   // Store Multiple Volatile SRR Word
PPC_lmvcsrrw,   // Load Multiple Volatile CSRR Word
PPC_stmvcsrrw,  // Store Multiple Volatile CSRR Word
PPC_lmvdsrrw,   // Load Multiple Volatile DSRR Word
PPC_stmvdsrrw,  // Store Multiple Volatile DSRR Word
PPC_lmvmcsrrw,  // Load Multiple Volatile MCSRR Word
PPC_stmvmcsrrw, // Store Multiple Volatile MCSRR Word

// SPE2 (Signal Processing Engine 2) instructions
PPC_evdotpwcssi,        // Vector Dot Product of Words, Complex, Signed, Saturate, Integer (to Accumulator)
PPC_evdotpwcsmi,        // Vector Dot Product of Words, Complex, Signed, Modulo, Integer (to Accumulator)
PPC_evdotpwcssfr,       // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotpwcssf,        // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotpwgasmf,       // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwxgasmf,      // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwgasmfr,      // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwxgasmfr,     // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwgssmf,       // Vector Dot Product of Words Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwxgssmf,      // Vector Dot Product of Words Exchanged, Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwgssmfr,      // Vector Dot Product of Words Guarded, Subtract, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwxgssmfr,     // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwcssiaaw3,    // Vector Dot Product of Words, Complex, Signed, Saturate, Integer and Accumulate into Words 3 operand
PPC_evdotpwcsmiaaw3,    // Vector Dot Product of Words, Complex, Signed, Modulo, Integer and Accumulate into Words 3 operand
PPC_evdotpwcssfraaw3,   // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional, Round and Accumulate into Words 3 operand
PPC_evdotpwcssfaaw3,    // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional and Accumulate into Words 3 operand
PPC_evdotpwgasmfaa3,    // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotpwxgasmfaa3,   // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotpwgasmfraa3,   // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional, Round and Accumulate 3 operand
PPC_evdotpwxgasmfraa3,  // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional, Round and Accumulate 3 operand
PPC_evdotpwgssmfaa3,    // Vector Multiply Word Complex, Real, Guarded, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotpwxgssmfaa3,   // Vector Dot Product of Words Exchanged, Guarded, Subtract, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotpwgssmfraa3,   // Vector Dot Product of Words Guarded, Subtract, Signed, Modulo, Fractional, Round and Accumulate 3 operand
PPC_evdotpwxgssmfraa3,  // Vector Dot Product of Words Exchanged, Guarded, Subtract, Signed, Modulo, Fractional, Round and Accumulate 3 operand
PPC_evdotpwcssia,       // Vector Dot Product of Words, Complex, Signed, Saturate, Integer (to Accumulator)
PPC_evdotpwcsmia,       // Vector Dot Product of Words, Complex, Signed, Modulo, Integer (to Accumulator)
PPC_evdotpwcssfra,      // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotpwcssfa,       // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotpwgasmfa,      // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwxgasmfa,     // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwgasmfra,     // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwxgasmfra,    // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwgssmfa,      // Vector Dot Product of Words Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwxgssmfa,     // Vector Dot Product of Words Exchanged, Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpwgssmfra,     // Vector Dot Product of Words Guarded, Subtract, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwxgssmfra,    // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evdotpwcssiaaw,     // Vector Dot Product of Words, Complex, Signed, Saturate, Integer and Accumulate into Words
PPC_evdotpwcsmiaaw,     // Vector Dot Product of Words, Complex, Signed, Modulo, Integer and Accumulate into Words
PPC_evdotpwcssfraaw,    // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional, Round and Accumulate into Words
PPC_evdotpwcssfaaw,     // Vector Dot Product of Words, Complex, Signed, Saturate, Fractional and Accumulate into Words
PPC_evdotpwgasmfaa,     // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional and Accumulate
PPC_evdotpwxgasmfaa,    // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional and Accumulate
PPC_evdotpwgasmfraa,    // Vector Dot Product of Words Guarded, Add, Signed, Modulo, Fractional, Round and Accumulate
PPC_evdotpwxgasmfraa,   // Vector Dot Product of Words Exchanged, Guarded, Add, Signed, Modulo, Fractional, Round and Accumulate
PPC_evdotpwgssmfaa,     // Vector Multiply Word Complex, Real, Guarded, Signed, Modulo, Fractional and Accumulate
PPC_evdotpwxgssmfaa,    // Vector Dot Product of Words Exchanged, Guarded, Subtract, Signed, Modulo, Fractional and Accumulate
PPC_evdotpwgssmfraa,    // Vector Dot Product of Words Guarded, Subtract, Signed, Modulo, Fractional, Round and Accumulate
PPC_evdotpwxgssmfraa,   // Vector Dot Product of Words Exchanged, Guarded, Subtract, Signed, Modulo, Fractional, Round and Accumulate
PPC_evdotphihcssi,      // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Integer (to Accumulator)
PPC_evdotplohcssi,      // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Integer (to Accumulator)
PPC_evdotphihcssf,      // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotplohcssf,      // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotphihcsmi,      // Vector Dot Product of High Halfwords, Complex, Signed, Modulo, Integer (to Accumulator)
PPC_evdotplohcsmi,      // Vector Dot Product of Low Halfwords, Complex, Signed, Modulo, Integer (to Accumulator)
PPC_evdotphihcssfr,     // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotplohcssfr,     // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotphihcssiaaw3,  // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Integer and Accumulate into Words, 3 operand
PPC_evdotplohcssiaaw3,  // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Integer and Accumulate into Words, 3 operand
PPC_evdotphihcssfaaw3,  // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional and Accumulate into Words, 3 operand
PPC_evdotplohcssfaaw3,  // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional and Accumulate into Words, 3 operand
PPC_evdotphihcsmiaaw3,  // Vector Dot Product of High Halfwords, Complex, Signed, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotplohcsmiaaw3,  // Vector Dot Product of Low Halfwords, Complex, Signed, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotphihcssfraaw3, // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional, Round and Accumulate into Words, 3 operand
PPC_evdotplohcssfraaw3, // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional, Round and Accumulate into Words, 3 operand
PPC_evdotphihcssia,     // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Integer (to Accumulator)
PPC_evdotplohcssia,     // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Integer (to Accumulator)
PPC_evdotphihcssfa,     // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotplohcssfa,     // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotphihcsmia,     // Vector Dot Product of High Halfwords, Complex, Signed, Modulo, Integer (to Accumulator)
PPC_evdotplohcsmia,     // Vector Dot Product of Low Halfwords, Complex, Signed, Modulo, Integer (to Accumulator)
PPC_evdotphihcssfra,    // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotplohcssfra,    // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotphihcssiaaw,   // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Integer and Accumulate into Words
PPC_evdotplohcssiaaw,   // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Integer and Accumulate into Words
PPC_evdotphihcssfaaw,   // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional and Accumulate into Words
PPC_evdotplohcssfaaw,   // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional and Accumulate into Words
PPC_evdotphihcsmiaaw,   // Vector Dot Product of High Halfwords, Complex, Signed, Modulo, Integer and Accumulate into Words
PPC_evdotplohcsmiaaw,   // Vector Dot Product of Low Halfwords, Complex, Signed, Modulo, Integer and Accumulate into Words
PPC_evdotphihcssfraaw,  // Vector Dot Product of High Halfwords, Complex, Signed, Saturate, Fractional, Round and Accumulate into Words
PPC_evdotplohcssfraaw,  // Vector Dot Product of Low Halfwords, Complex, Signed, Saturate, Fractional, Round and Accumulate into Words
PPC_evdotphausi,        // Vector Dot Product of Halfwords, Add, Unsigned, Saturate, Integer (to Accumulator)
PPC_evdotphassi,        // Vector Dot Product of Halfwords, Add, Signed, Saturate, Integer (to Accumulator)
PPC_evdotphasusi,       // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Saturate, Integer (to Accumulator)
PPC_evdotphassf,        // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotphsssf,        // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotphaumi,        // Vector Dot Product of Halfwords, Add, Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotphasmi,        // Vector Dot Product of Halfwords, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotphasumi,       // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotphassfr,       // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotphssmi,        // Vector Dot Product of Halfwords, Subtract, Signed, Modulo, Integer (to Accumulator)
PPC_evdotphsssfr,       // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotphausiaaw3,    // Vector Dot Product of Halfwords, Add, Unsigned, Saturate, Integer and Accumulate into Words, 3 operand
PPC_evdotphassiaaw3,    // Vector Dot Product of Halfwords, Add, Signed, Saturate, Integer and Accumulate into Words, 3 operand
PPC_evdotphasusiaaw3,   // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Saturate, Integer and Accumulate into Words, 3 operand
PPC_evdotphassfaaw3,    // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional and Accumulate into Words, 3 operand
PPC_evdotphsssiaaw3,    // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Integer and Accumulate into Words, 3 operand
PPC_evdotphsssfaaw3,    // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional and Accumulate into Words, 3 operand
PPC_evdotphaumiaaw3,    // Vector Dot Product of Halfwords, Add, Unsigned, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotphasmiaaw3,    // Vector Dot Product of Halfwords, Add, Signed, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotphasumiaaw3,   // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotphassfraaw3,   // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional, Round and Accumulate into Words, 3 operand
PPC_evdotphssmiaaw3,    // Vector Dot Product of Halfwords, Subtract, Signed, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotphsssfraaw3,   // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional, Round and Accumulate into Words, 3 operand
PPC_evdotphausia,       // Vector Dot Product of Halfwords, Add, Unsigned, Saturate, Integer (to Accumulator)
PPC_evdotphassia,       // Vector Dot Product of Halfwords, Add, Signed, Saturate, Integer (to Accumulator)
PPC_evdotphasusia,      // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Saturate, Integer (to Accumulator)
PPC_evdotphassfa,       // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotphsssfa,       // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional (to Accumulator)
PPC_evdotphaumia,       // Vector Dot Product of Halfwords, Add, Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotphasmia,       // Vector Dot Product of Halfwords, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotphasumia,      // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotphassfra,      // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotphssmia,       // Vector Dot Product of Halfwords, Subtract, Signed, Modulo, Integer (to Accumulator)
PPC_evdotphsssfra,      // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional, Round (to Accumulator)
PPC_evdotphausiaaw,     // Vector Dot Product of Halfwords, Add, Unsigned, Saturate, Integer and Accumulate into Words
PPC_evdotphassiaaw,     // Vector Dot Product of Halfwords, Add, Signed, Saturate, Integer and Accumulate into Words
PPC_evdotphasusiaaw,    // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Saturate, Integer and Accumulate into Words
PPC_evdotphassfaaw,     // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional and Accumulate into Words
PPC_evdotphsssiaaw,     // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Integer and Accumulate into Words
PPC_evdotphsssfaaw,     // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional and Accumulate into Words
PPC_evdotphaumiaaw,     // Vector Dot Product of Halfwords, Add, Unsigned, Modulo, Integer and Accumulate into Words
PPC_evdotphasmiaaw,     // Vector Dot Product of Halfwords, Add, Signed, Modulo, Integer and Accumulate into Words
PPC_evdotphasumiaaw,    // Vector Dot Product of Halfwords, Add, Signed by Unsigned, Modulo, Integer and Accumulate into Words
PPC_evdotphassfraaw,    // Vector Dot Product of Halfwords, Add, Signed, Saturate, Fractional, Round and Accumulate into Words
PPC_evdotphssmiaaw,     // Vector Dot Product of Halfwords, Subtract, Signed, Modulo, Integer and Accumulate into Words
PPC_evdotphsssfraaw,    // Vector Dot Product of Halfwords, Subtract, Signed, Saturate, Fractional, Round and Accumulate into Words
PPC_evdotp4hgaumi,      // Vector Dot Product of Four Halfwords, Guarded, Add, Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotp4hgasmi,      // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hgasumi,     // Vector Dot Product of Four Halfwords, Guarded, Add, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotp4hgasmf,      // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotp4hgssmi,      // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hgssmf,      // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotp4hxgasmi,     // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hxgasmf,     // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpbaumi,        // Vector Dot Product of Bytes, Add, Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotpbasmi,        // Vector Dot Product of Bytes, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotpbasumi,       // Vector Dot Product of Bytes, Add, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotp4hxgssmi,     // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hxgssmf,     // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotp4hgaumiaa3,   // Vector Dot Product of Four Halfwords, Guarded, Add, Unsigned, Modulo, Integer and Accumulate, 3 operand
PPC_evdotp4hgasmiaa3,   // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Integer and Accumulate, 3 operand
PPC_evdotp4hgasumiaa3,  // Vector Dot Product of Four Halfwords, Guarded, Add, Signed by Unsigned, Modulo, Integer and Accumulate, 3 operand
PPC_evdotp4hgasmfaa3,   // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Fractional and Accumulate, 3 operand
PPC_evdotp4hgssmiaa3,   // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Integer and Accumulate 3 operand
PPC_evdotp4hgssmfaa3,   // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotp4hxgasmiaa3,  // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Integer and Accumulate 3 operand
PPC_evdotp4hxgasmfaa3,  // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotpbaumiaaw3,    // Vector Dot Product of Bytes, Add, Unsigned, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotpbasmiaaw3,    // Vector Dot Product of Bytes, Add, Signed, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotpbasumiaaw3,   // Vector Dot Product of Bytes, Add, Signed by Unsigned, Modulo, Integer and Accumulate into Words, 3 operand
PPC_evdotp4hxgssmiaa3,  // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Integer and Accumulate 3 operand
PPC_evdotp4hxgssmfaa3,  // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Fractional and Accumulate 3 operand
PPC_evdotp4hgaumia,     // Vector Dot Product of Four Halfwords, Guarded, Add, Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotp4hgasmia,     // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hgasumia,    // Vector Dot Product of Four Halfwords, Guarded, Add, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotp4hgasmfa,     // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotp4hgssmia,     // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hgssmfa,     // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotp4hxgasmia,    // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hxgasmfa,    // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotpbaumia,       // Vector Dot Product of Bytes, Add, Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotpbasmia,       // Vector Dot Product of Bytes, Add, Signed, Modulo, Integer (to Accumulator)
PPC_evdotpbasumia,      // Vector Dot Product of Bytes, Add, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evdotp4hxgssmia,    // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Integer (to Accumulator)
PPC_evdotp4hxgssmfa,    // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Fractional (to Accumulator)
PPC_evdotp4hgaumiaa,    // Vector Dot Product of Four Halfwords, Guarded, Add, Unsigned, Modulo, Integer and Accumulate
PPC_evdotp4hgasmiaa,    // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Integer and Accumulate
PPC_evdotp4hgasumiaa,   // Vector Dot Product of Four Halfwords, Guarded, Add, Signed by Unsigned, Modulo, Integer and Accumulate
PPC_evdotp4hgasmfaa,    // Vector Dot Product of Four Halfwords, Guarded, Add, Signed, Modulo, Fractional and Accumulate
PPC_evdotp4hgssmiaa,    // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Integer and Accumulate
PPC_evdotp4hgssmfaa,    // Vector Dot Product of Four Halfwords, Guarded, Subtract, Signed, Modulo, Fractional and Accumulate
PPC_evdotp4hxgasmiaa,   // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Integer and Accumulate
PPC_evdotp4hxgasmfaa,   // Vector Dot Product of Four Halfwords Exchanged, Guarded, Add, Signed, Modulo, Fractional and Accumulate
PPC_evdotpbaumiaaw,     // Vector Dot Product of Bytes, Add, Unsigned, Modulo, Integer and Accumulate into Words
PPC_evdotpbasmiaaw,     // Vector Dot Product of Bytes, Add, Signed, Modulo, Integer and Accumulate into Words
PPC_evdotpbasumiaaw,    // Vector Dot Product of Bytes, Add, Signed by Unsigned, Modulo, Integer and Accumulate into Words
PPC_evdotp4hxgssmiaa,   // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Integer and Accumulate
PPC_evdotp4hxgssmfaa,   // Vector Dot Product of Four Halfwords Exchanged, Guarded, Subtract, Signed, Modulo, Fractional and Accumulate
PPC_evdotpwausi,        // Vector Dot Product of Words, Add, Unsigned, Saturate, integer (to Accumulator)
PPC_evdotpwassi,        // Vector Dot Product of Words, Add, Signed, Saturate, integer (to Accumulator)
PPC_evdotpwasusi,       // Vector Dot Product of Words, Add, Signed by Unsigned, Saturate, integer (to Accumulator)
PPC_evdotpwaumi,        // Vector Dot Product of Words, Add, Unsigned, Modulo, integer (to Accumulator)
PPC_evdotpwasmi,        // Vector Dot Product of Words, Add, Signed, Modulo, integer (to Accumulator)
PPC_evdotpwasumi,       // Vector Dot Product of Words, Add, Signed by Unsigned, Modulo, integer (to Accumulator)
PPC_evdotpwssmi,        // Vector Dot Product of Words, Subtract, Signed, Modulo, integer (to Accumulator)
PPC_evdotpwausiaa3,     // Vector Dot Product of Words, Add, Unsigned, Saturate, Integer and Accumulate, 3 operand
PPC_evdotpwassiaa3,     // Vector Dot Product of Words, Add, Signed, Saturate, Integer and Accumulate, 3 operand
PPC_evdotpwasusiaa3,    // Vector Dot Product of Words, Add, Signed by Unsigned, Saturate, Integer and Accumulate, 3 operand
PPC_evdotpwsssiaa3,     // Vector Dot Product of Words, Subtract, Signed, Saturate, Integer and Accumulate, 3 operand
PPC_evdotpwaumiaa3,     // Vector Dot Product of Words, Add, Unsigned, Modulo, Integer and Accumulate, 3 operand
PPC_evdotpwasmiaa3,     // Vector Dot Product of Words, Add, Signed, Modulo, Integer and Accumulate, 3 operand
PPC_evdotpwasumiaa3,    // Vector Dot Product of Words, Add, Signed by Unsigned, Modulo, Integer and Accumulate, 3 operand
PPC_evdotpwssmiaa3,     // Vector Dot Product of Words, Subtract, Signed, Modulo, Integer and Accumulate, 3 operand
PPC_evdotpwausia,       // Vector Dot Product of Words, Add, Unsigned, Saturate, integer (to Accumulator)
PPC_evdotpwassia,       // Vector Dot Product of Words, Add, Signed, Saturate, integer (to Accumulator)
PPC_evdotpwasusia,      // Vector Dot Product of Words, Add, Signed by Unsigned, Saturate, integer (to Accumulator)
PPC_evdotpwaumia,       // Vector Dot Product of Words, Add, Unsigned, Modulo, integer (to Accumulator)
PPC_evdotpwasmia,       // Vector Dot Product of Words, Add, Signed, Modulo, integer (to Accumulator)
PPC_evdotpwasumia,      // Vector Dot Product of Words, Add, Signed by Unsigned, Modulo, integer (to Accumulator)
PPC_evdotpwssmia,       // Vector Dot Product of Words, Subtract, Signed, Modulo, integer (to Accumulator)
PPC_evdotpwausiaa,      // Vector Dot Product of Words, Add, Unsigned, Saturate, Integer and Accumulate
PPC_evdotpwassiaa,      // Vector Dot Product of Words, Add, Signed, Saturate, Integer and Accumulate
PPC_evdotpwasusiaa,     // Vector Dot Product of Words, Add, Signed by Unsigned, Saturate, Integer and Accumulate
PPC_evdotpwsssiaa,      // Vector Dot Product of Words, Subtract, Signed, Saturate, Integer and Accumulate
PPC_evdotpwaumiaa,      // Vector Dot Product of Words, Add, Unsigned, Modulo, Integer and Accumulate
PPC_evdotpwasmiaa,      // Vector Dot Product of Words, Add, Signed, Modulo, Integer and Accumulate
PPC_evdotpwasumiaa,     // Vector Dot Product of Words, Add, Signed by Unsigned, Modulo, Integer and Accumulate
PPC_evdotpwssmiaa,      // Vector Dot Product of Words, Subtract, Signed, Modulo, Integer and Accumulate
PPC_evaddih,            // Vector Add Immediate Halfword
PPC_evaddib,            // Vector Add Immediate Byte
PPC_evsubifh,           // Vector Subtract Immediate from Halfword
PPC_evsubifb,           // Vector Subtract Immediate from Byte
PPC_evabsb,             // Vector Absolute Value Byte
PPC_evabsh,             // Vector Absolute Value Halfword
PPC_evabsd,             // Vector Absolute Value Doubleword
PPC_evabss,             // Vector Absolute Value (Word) and Saturate
PPC_evabsbs,            // Vector Absolute Value Byte and Saturate
PPC_evabshs,            // Vector Absolute Value Halfword and Saturate
PPC_evabsds,            // Vector Absolute Value Doubleword and Saturate
PPC_evnegwo,            // Vector Negate Word Odd
PPC_evnegb,             // Vector Negate Byte
PPC_evnegbo,            // Vector Negate Byte Odd
PPC_evnegh,             // Vector Negate Halfword
PPC_evnegho,            // Vector Negate Halfwords Odd
PPC_evnegd,             // Vector Negate Doubleword
PPC_evnegs,             // Vector Negate (Word) and Saturate
PPC_evnegwos,           // Vector Negate Word Odd and Saturate
PPC_evnegbs,            // Vector Negate Byte and Saturate
PPC_evnegbos,           // Vector Negate Byte Odd and Saturate
PPC_evneghs,            // Vector Negate Halfword and Saturate
PPC_evneghos,           // Vector Negate Halfwords Odd and Saturate
PPC_evnegds,            // Vector Negate Doubleword and Saturate
PPC_evextzb,            // Vector Extend Zero Byte
PPC_evextsbh,           // Vector Extend Sign of Bytes (Odd) to Halfwords
PPC_evextsw,            // Vector Extend Sign Word
PPC_evrndhb,            // Vector Round Halfword to Byte
PPC_evrnddw,            // Vector Round Doubleword to Word
PPC_evrndwhus,          // Vector Round Word to Halfword Unsigned and Saturate
PPC_evrndwhss,          // Vector Round Word to Halfword Signed and Saturate
PPC_evrndhbus,          // Vector Round Halfword to Byte Unsigned and Saturate
PPC_evrndhbss,          // Vector Round Halfword to Byte Signed and Saturate
PPC_evrnddwus,          // Vector Round Doubleword to Word Unsigned and Saturate
PPC_evrnddwss,          // Vector Round Doubleword to Word Signed and Saturate
PPC_evrndwnh,           // Vector Round Word to Nearest Even Halfword
PPC_evrndhnb,           // Vector Round Halfword to Nearest Even Byte
PPC_evrnddnw,           // Vector Round Doubleword to Nearest Even Word
PPC_evrndwnhus,         // Vector Round Word to Nearest Even Halfword Unsigned and Saturate
PPC_evrndwnhss,         // Vector Round Word to Nearest Even Halfword Signed and Saturate
PPC_evrndhnbus,         // Vector Round Halfword to Nearest Byte Unsigned and Saturate
PPC_evrndhnbss,         // Vector Round Halfword to Nearest Even Byte Signed and Saturate
PPC_evrnddnwus,         // Vector Round Doubleword to Nearest Even Word Unsigned and Saturate
PPC_evrnddnwss,         // Vector Round Doubleword to Nearest Even Word Signed and Saturate
PPC_evcntlzh,           // Vector Count Leading Zeros Halfword
PPC_evcntlsh,           // Vector Count Leading Signed Bits Halfword
PPC_evpopcntb,          // Vector Population Count Byte
PPC_circinc,            // Circular Increment
PPC_evunpkhibui,        // Vector Unpack High Bytes as Unsigned Integers
PPC_evunpkhibsi,        // Vector Unpack High Bytes as Signed Integers
PPC_evunpkhihui,        // Vector Extract High Halfwords as Unsigned Integers
PPC_evunpkhihsi,        // Vector Extract High Halfwords as Signed Integers
PPC_evunpklobui,        // Vector Unpack Low Bytes Unsigned Integers
PPC_evunpklobsi,        // Vector Unpack Low Bytes as Signed Integers
PPC_evunpklohui,        // Vector Unpack Low Halfwords as Unsigned Integers
PPC_evunpklohsi,        // Vector Unpack Low Halfwords as Signed Integers
PPC_evunpklohf,         // Vector Unpack Low Halfwords as Fractional
PPC_evunpkhihf,         // Vector Unpack High Halfwords as Fractional
PPC_evunpklowgsf,       // Vector Unpack Low Word to Guarded Signed Fraction
PPC_evunpkhiwgsf,       // Vector Unpack High Word to Guarded Signed Fraction
PPC_evsatsduw,          // Vector Saturate Signed Doubleword to Unsigned Word Range
PPC_evsatsdsw,          // Vector Saturate Signed Doubleword to Signed Word Range
PPC_evsatshub,          // Vector Saturate Signed Halfwords to Unsigned Byte Range
PPC_evsatshsb,          // Vector Saturate Signed Halfwords to Signed Byte Range
PPC_evsatuwuh,          // Vector Saturate Unsigned Words to Unsigned Halfword Range
PPC_evsatswsh,          // Vector Saturate Signed Words to Signed Halfword Range
PPC_evsatswuh,          // Vector Pack Signed Words to Unsigned Halfword Range
PPC_evsatuhub,          // Vector Saturate Unsigned Halfwords to Unsigned Byte Range
PPC_evsatuduw,          // Vector Saturate Unsigned Doubleword to Unsigned Word Range
PPC_evsatuwsw,          // Vector Saturate Unsigned Words to Signed Word Range
PPC_evsatshuh,          // Vector Saturate Signed Halfwords to Unsigned Halfword Range
PPC_evsatuhsh,          // Vector Saturate Unsigned Halfwords to Signed Halfword Range
PPC_evsatswuw,          // Vector Pack Signed Words to Unsigned Word Range
PPC_evsatswgsdf,        // Vector Saturate Signed Word Guarded to Signed Doubleword Fractional
PPC_evsatsbub,          // Vector Saturate Signed Bytes to Unsigned Byte Range
PPC_evsatubsb,          // Vector Saturate Unsigned Bytes to Signed Byte Range
PPC_evmaxhpuw,          // Vector Maximum of Halfword Pairs Unsigned to Word
PPC_evmaxhpsw,          // Vector Maximum of Halfword Pairs Signed to Word
PPC_evmaxbpuh,          // Vector Maximum of Byte Pairs Unsigned to Halfword
PPC_evmaxbpsh,          // Vector Maximum of Byte Pairs Signed to Halfword
PPC_evmaxwpud,          // Vector Maximum of Word Pair Unsigned to Doubleword
PPC_evmaxwpsd,          // Vector Maximum of Word Pair Signed to Doubleword
PPC_evminhpuw,          // Vector Minimum of Halfword Pairs Unsigned to Word
PPC_evminhpsw,          // Vector Minimum of Halfword Pairs Signed to Word
PPC_evminbpuh,          // Vector Minimum of Byte Pairs Unsigned to Halfword
PPC_evminbpsh,          // Vector Minimum of Byte Pairs Signed to Halfword
PPC_evminwpud,          // Vector Minimum of Word Pair Unsigned to Doubleword
PPC_evminwpsd,          // Vector Minimum of Word Pair Signed to Doubleword
PPC_evmaxmagws,         // Vector Maximum Magnitude Word Signed
PPC_evsl,               // Vector Shift Left
PPC_evsli,              // Vector Shift Left Immediate
PPC_evsplatie,          // Vector Splat Immediate Even (to Accumulator)
PPC_evsplatib,          // Vector Splat Immediate Byte (to Accumulator)
PPC_evsplatibe,         // Vector Splat Immediate Byte Even (to Accumulator)
PPC_evsplatih,          // Vector Splat Immediate Halfword (to Accumulator)
PPC_evsplatihe,         // Vector Splat Immediate Halfword Even (to Accumulator)
PPC_evsplatid,          // Vector Splat Immediate Doubleword (to Accumulator)
PPC_evsplatia,          // Vector Splat Immediate (to Accumulator)
PPC_evsplatiea,         // Vector Splat Immediate Even (to Accumulator)
PPC_evsplatiba,         // Vector Splat Immediate Byte (to Accumulator)
PPC_evsplatibea,        // Vector Splat Immediate Byte Even (to Accumulator)
PPC_evsplatiha,         // Vector Splat Immediate Halfword (to Accumulator)
PPC_evsplatihea,        // Vector Splat Immediate Halfword Even (to Accumulator)
PPC_evsplatida,         // Vector Splat Immediate Doubleword (to Accumulator)
PPC_evsplatfio,         // Vector Splat Fractional Immediate Odd (to Accumulator)
PPC_evsplatfib,         // Vector Splat Fractional Immediate Byte (to Accumulator)
PPC_evsplatfibo,        // Vector Splat Fractional Immediate Byte Odd (to Accumulator)
PPC_evsplatfih,         // Vector Splat Fractional Immediate Halfword (to Accumulator)
PPC_evsplatfiho,        // Vector Splat Fractional Immediate Halfword Odd (to Accumulator)
PPC_evsplatfid,         // Vector Splat Fractional Immediate Doubleword (to Accumulator)
PPC_evsplatfia,         // Vector Splat Fractional Immediate (to Accumulator)
PPC_evsplatfioa,        // Vector Splat Fractional Immediate Odd (to Accumulator)
PPC_evsplatfiba,        // Vector Splat Fractional Immediate Byte (to Accumulator)
PPC_evsplatfiboa,       // Vector Splat Fractional Immediate Byte Odd (to Accumulator)
PPC_evsplatfiha,        // Vector Splat Fractional Immediate Halfword (to Accumulator)
PPC_evsplatfihoa,       // Vector Splat Fractional Immediate Halfword Odd (to Accumulator)
PPC_evsplatfida,        // Vector Splat Fractional Immediate Doubleword (to Accumulator)
PPC_evcmpgtdu,          // Vector Compare Greater Than Doubleword Unsigned
PPC_evcmpgtds,          // Vector Compare Greater Than Doubleword Signed
PPC_evcmpltdu,          // Vector Compare Less Than Doubleword Unsigned
PPC_evcmpltds,          // Vector Compare Less Than Doubleword Signed
PPC_evcmpeqd,           // Vector Compare Equal Doubleword
PPC_evswapbhilo,        // Vector Swap Bytes High/Low
PPC_evswapblohi,        // Vector Swap Bytes Low/High
PPC_evswaphhilo,        // Vector Swap Halfwords High/Low
PPC_evswaphlohi,        // Vector Swap Halfwords Low/High
PPC_evswaphe,           // Vector Swap Halfwords Even
PPC_evswaphhi,          // Vector Swap Halfwords High
PPC_evswaphlo,          // Vector Swap Halfwords Low
PPC_evswapho,           // Vector Swap Halfwords Odd
PPC_evinsb,             // Vector Insert Byte
PPC_evxtrb,             // Vector Extract Byte
PPC_evsplath,           // Vector Splat Halfword
PPC_evsplatb,           // Vector Splat Byte
PPC_evinsh,             // Vector Insert Halfword
PPC_evclrbe,            // Vector Clear Bytes Even
PPC_evclrbo,            // Vector Clear Bytes Odd
PPC_evxtrh,             // Vector Extract Halfword
PPC_evclrh,             // Vector Clear Halfwords
PPC_evselbitm0,         // Vector Select Bit if Mask is 0
PPC_evselbitm1,         // Vector Select Bit if Mask is 1
PPC_evselbit,           // Vector Select Bit
PPC_evperm,             // Vector Permute
PPC_evperm2,            // Vector Permute (form 2)
PPC_evperm3,            // Vector Permute (form 3)
PPC_evxtrd,             // Vector Extract Doubleword
PPC_evsrbu,             // Vector Shift Right Byte Unsigned
PPC_evsrbs,             // Vector Shift Right Byte Signed
PPC_evsrbiu,            // Vector Shift Right Byte Immediate Unsigned
PPC_evsrbis,            // Vector Shift Right Byte Immediate Signed
PPC_evslb,              // Vector Shift Left Byte
PPC_evrlb,              // Vector Rotate Left Byte
PPC_evslbi,             // Vector Shift Left Byte Immediate
PPC_evrlbi,             // Vector Rotate Left Byte Immediate
PPC_evsrhu,             // Vector Shift Right Halfword Unsigned
PPC_evsrhs,             // Vector Shift Right Halfword Signed
PPC_evsrhiu,            // Vector Shift Right Halfword Immediate Unsigned
PPC_evsrhis,            // Vector Shift Right Halfword Immediate Signed
PPC_evslh,              // Vector Shift Left Halfword
PPC_evrlh,              // Vector Rotate Left Halfword
PPC_evslhi,             // Vector Shift Left Halfword Immediate
PPC_evrlhi,             // Vector Rotate Left Halfword Immediate
PPC_evsru,              // Vector Shift Right Unsigned
PPC_evsrs,              // Vector Shift Right Signed
PPC_evsriu,             // Vector Shift Right Immediate Unsigned
PPC_evsris,             // Vector Shift Right Immediate Signed
PPC_evlvsl,             // Load Vector for Shift Left
PPC_evlvsr,             // Load Vector for Shift Right
PPC_evsroiu,            // Vector Shift Right by Octet Immediate Unsigned
PPC_evsloi,             // Vector Shift Left by Octet Immediate
PPC_evsrois,            // Vector Shift Right by Octet Immediate Signed
PPC_evldbx,             // Vector Load Double into Eight Bytes [with Modify] Indexed
PPC_evldb,              // Vector Load Double into Eight Bytes [with Update]
PPC_evlhhsplathx,       // Vector Load Halfword into Halfword and Splat Halfwords [with Modify] Indexed
PPC_evlhhsplath,        // Vector Load Halfword into Halfword and Splat Halfwords [with Update]
PPC_evlwbsplatwx,       // Vector Load Word as Bytes and Splat Words [with Modify] Indexed
PPC_evlwbsplatw,        // Vector Load Word as Bytes and Splat Words [with Update]
PPC_evlwhsplatwx,       // Vector Load Word as Halfwords and Splat Words [with Modify] Indexed
PPC_evlwhsplatw,        // Vector Load Word as Halfwords and Splat Words [with Update]
PPC_evlbbsplatbx,       // Vector Load Byte into Byte and Splat Bytes [with Modify] Indexed
PPC_evlbbsplatb,        // Vector Load Byte into Byte and Splat Bytes [with Update]
PPC_evstdbx,            // Vector Store Double of Eight Bytes [with Modify] Indexed
PPC_evstdb,             // Vector Store Double of Eight Bytes [with Update]
PPC_evlwbex,            // Vector Load Word into Four Bytes Even [with Modify] Indexed
PPC_evlwbe,             // Vector Load Word into Four Bytes Even [with Update]
PPC_evlwboux,           // Vector Load Word into Four Bytes Odd Unsigned [with Modify] Indexed
PPC_evlwbou,            // Vector Load Word into Four Bytes Odd Unsigned [with Update]
PPC_evlwbosx,           // Vector Load Word into Two Halfwords Odd Signed [with Modify] Indexed
PPC_evlwbos,            // Vector Load Word into Four Bytes Odd Signed [with Update]
PPC_evstwbex,           // Vector Store Word of Four Bytes from Even [with Modify] Indexed
PPC_evstwbe,            // Vector Store Word of Four Bytes from Even [with Update]
PPC_evstwbox,           // Vector Store Word of Four Bytes from Odd [with Modify] Indexed
PPC_evstwbo,            // Vector Store Word of Four Bytes from Odd [with Update]
PPC_evstwbx,            // Vector Store Word of Four Bytes [with Modify] Indexed
PPC_evstwb,             // Vector Store Word of Four Bytes [with Update]
PPC_evsthbx,            // Vector Store Halfword of Two Bytes [with Modify] Indexed
PPC_evsthb,             // Vector Store Halfword of Two Bytes [with Update]
PPC_evlddmx,            // Vector Load Double Word into Double Word [with Modify] Indexed
PPC_evlddu,             // Vector Load Double Word into Double Word [with Update]
PPC_evldwmx,            // Vector Load Double into Two Words [with Modify] Indexed
PPC_evldwu,             // Vector Load Double into Two Words [with Update]
PPC_evldhmx,            // Vector Load Double into Four Halfwords [with Modify] Indexed
PPC_evldhu,             // Vector Load Double into Four Halfwords [with Update]
PPC_evldbmx,            // Vector Load Double into Eight Bytes [with Modify] Indexed
PPC_evldbu,             // Vector Load Double into Eight Bytes [with Update]
PPC_evlhhesplatmx,      // Vector Load Halfword into Halfwords Even and Splat [with Modify] Indexed
PPC_evlhhesplatu,       // Vector Load Halfword into Halfwords Even and Splat [with Update]
PPC_evlhhsplathmx,      // Vector Load Halfword into Halfword and Splat Halfwords [with Modify] Indexed
PPC_evlhhsplathu,       // Vector Load Halfword into Halfword and Splat Halfwords [with Update]
PPC_evlhhousplatmx,     // Vector Load Halfword into Halfword Odd Unsigned and Splat [with Modify] Indexed
PPC_evlhhousplatu,      // Vector Load Halfword into Halfword Odd Unsigned and Splat [with Update]
PPC_evlhhossplatmx,     // Vector Load Halfword into Halfword Odd Signed and Splat [with Modify] Indexed
PPC_evlhhossplatu,      // Vector Load Halfword into Halfword Odd Signed and Splat [with Update]
PPC_evlwhemx,           // Vector Load Word into Two Halfwords Even [with Modify] Indexed
PPC_evlwheu,            // Vector Load Word into Two Halfwords Even [with Update]
PPC_evlwbsplatwmx,      // Vector Load Word as Bytes and Splat Words [with Modify] Indexed
PPC_evlwbsplatwu,       // Vector Load Word as Bytes and Splat Words [with Update]
PPC_evlwhoumx,          // Vector Load Word into Two Halfwords Odd Unsigned [with Modify] Indexed (zero-extended)
PPC_evlwhouu,           // Vector Load Word into Two Halfwords Odd Unsigned (zero-extended) [with Update]
PPC_evlwhosmx,          // Vector Load Word into Two Halfwords Odd Signed [with Modify] Indexed (with sign extension)
PPC_evlwhosu,           // Vector Load Word into Two Halfwords Odd Signed (with sign extension) [with Update]
PPC_evlwwsplatmx,       // Vector Load Word into Word and Splat [with Modify] Indexed
PPC_evlwwsplatu,        // Vector Load Word into Word and Splat [with Update]
PPC_evlwhsplatwmx,      // Vector Load Word as Halfwords and Splat Words [with Modify] Indexed
PPC_evlwhsplatwu,       // Vector Load Word as Halfwords and Splat Words [with Update]
PPC_evlwhsplatmx,       // Vector Load Word into Two Halfwords and Splat [with Modify] Indexed
PPC_evlwhsplatu,        // Vector Load Word into Two Halfwords and Splat [with Update]
PPC_evlbbsplatbmx,      // Vector Load Byte into Byte and Splat Bytes [with Modify] Indexed
PPC_evlbbsplatbu,       // Vector Load Byte into Byte and Splat Bytes [with Update]
PPC_evstddmx,           // Vector Store Double of Double [with Modify] Indexed
PPC_evstddu,            // Vector Store Double of Double [with Update]
PPC_evstdwmx,           // Vector Store Double of Two Words [with Modify] Indexed
PPC_evstdwu,            // Vector Store Double of Two Words [with Update]
PPC_evstdhmx,           // Vector Store Double of Four Halfwords [with Modify] Indexed
PPC_evstdhu,            // Vector Store Double of Four Halfwords [with Update]
PPC_evstdbmx,           // Vector Store Double of Eight Bytes [with Modify] Indexed
PPC_evstdbu,            // Vector Store Double of Eight Bytes [with Update]
PPC_evlwbemx,           // Vector Load Word into Four Bytes Even [with Modify] Indexed
PPC_evlwbeu,            // Vector Load Word into Four Bytes Even [with Update]
PPC_evlwboumx,          // Vector Load Word into Four Bytes Odd Unsigned [with Modify] Indexed
PPC_evlwbouu,           // Vector Load Word into Four Bytes Odd Unsigned [with Update]
PPC_evlwbosmx,          // Vector Load Word into Two Halfwords Odd Signed [with Modify] Indexed
PPC_evlwbosu,           // Vector Load Word into Four Bytes Odd Signed [with Update]
PPC_evstwhemx,          // Vector Store Word of Two Halfwords from Even [with Modify] Indexed
PPC_evstwheu,           // Vector Store Word of Two Halfwords from Even [with Update]
PPC_evstwbemx,          // Vector Store Word of Four Bytes from Even [with Modify] Indexed
PPC_evstwbeu,           // Vector Store Word of Four Bytes from Even [with Update]
PPC_evstwhomx,          // Vector Store Word of Two Halfwords from Odd [with Modify] Indexed
PPC_evstwhou,           // Vector Store Word of Two Halfwords from Odd [with Update]
PPC_evstwbomx,          // Vector Store Word of Four Bytes from Odd [with Modify] Indexed
PPC_evstwbou,           // Vector Store Word of Four Bytes from Odd [with Update]
PPC_evstwwemx,          // Vector Store Word of Word from Even [with Modify] Indexed
PPC_evstwweu,           // Vector Store Word of Word from Even [with Update]
PPC_evstwbmx,           // Vector Store Word of Four Bytes [with Modify] Indexed
PPC_evstwbu,            // Vector Store Word of Four Bytes [with Update]
PPC_evstwwomx,          // Vector Store Word of Word from Odd [with Modify] Indexed
PPC_evstwwou,           // Vector Store Word of Word from Odd [with Update]
PPC_evsthbmx,           // Vector Store Halfword of Two Bytes [with Modify] Indexed
PPC_evsthbu,            // Vector Store Halfword of Two Bytes [with Update]
PPC_evmhusi,            // Vector Multiply Halfword Unsigned, Saturate, Integer
PPC_evmhssi,            // Vector Multiply Halfword Signed, Saturate, Integer
PPC_evmhsusi,           // Vector Multiply Halfword Signed by Unsigned, Saturate, Integer
PPC_evmhssf,            // Vector Multiply Halfword Signed, Saturate, Fractional
PPC_evmhumi,            // Vector Multiply Halfword Unsigned, Modulo, Integer
PPC_evmhssfr,           // Vector Multiply Halfword Signed, Saturate, Fractional and Round
PPC_evmhesumi,          // Vector Multiply Halfwords, Even, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmhosumi,          // Vector Multiply Halfwords, Odd, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmbeumi,           // Vector Multiply Bytes Even, Unsigned, Modulo, Integer (to Accumulator)
PPC_evmbesmi,           // Vector Multiply Bytes Even, Signed, Modulo, Integer (to Accumulator)
PPC_evmbesumi,          // Vector Multiply Bytes Even, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmboumi,           // Vector Multiply Bytes Odd, Unsigned, Modulo, Integer (to Accumulator)
PPC_evmbosmi,           // Vector Multiply Bytes Odd, Signed, Modulo, Integer (to Accumulator)
PPC_evmbosumi,          // Vector Multiply Bytes Odd, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmhesumia,         // Vector Multiply Halfwords, Even, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmhosumia,         // Vector Multiply Halfwords, Odd, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmbeumia,          // Vector Multiply Bytes Even, Unsigned, Modulo, Integer (to Accumulator)
PPC_evmbesmia,          // Vector Multiply Bytes Even, Signed, Modulo, Integer (to Accumulator)
PPC_evmbesumia,         // Vector Multiply Bytes Even, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmboumia,          // Vector Multiply Bytes Odd, Unsigned, Modulo, Integer (to Accumulator)
PPC_evmbosmia,          // Vector Multiply Bytes Odd, Signed, Modulo, Integer (to Accumulator)
PPC_evmbosumia,         // Vector Multiply Bytes Odd, Signed by Unsigned, Modulo, Integer (to Accumulator)
PPC_evmwusiw,           // Vector Multiply Word Unsigned, Saturate, Integer Word
PPC_evmwssiw,           // Vector Multiply Word Signed, Saturate, Integer Word
PPC_evmwhssfr,          // Vector Multiply Word High Signed, Saturate, Fractional and Round (to Accumulator)
PPC_evmwehgsmfr,        // Vector Multiply Word Even High Guarded Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evmwehgsmf,         // Vector Multiply Word Even High Guarded Signed, Modulo, Fractional (to Accumulator)
PPC_evmwohgsmfr,        // Vector Multiply Word Odd, High, Guarded, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evmwohgsmf,         // Vector Multiply Word Odd, High, Guarded, Signed, Modulo, Fractional (to Accumulator)
PPC_evmwhssfra,         // Vector Multiply Word High Signed, Saturate, Fractional and Round (to Accumulator)
PPC_evmwehgsmfra,       // Vector Multiply Word Even High Guarded Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evmwehgsmfa,        // Vector Multiply Word Even High Guarded Signed, Modulo, Fractional (to Accumulator)
PPC_evmwohgsmfra,       // Vector Multiply Word Odd, High, Guarded, Signed, Modulo, Fractional, Round (to Accumulator)
PPC_evmwohgsmfa,        // Vector Multiply Word Odd, High, Guarded, Signed, Modulo, Fractional (to Accumulator)
PPC_evaddusiaa,         // Vector Add Unsigned, Saturate, Integer to Accumulator
PPC_evaddssiaa,         // Vector Add Signed, Saturate, Integer to Accumulator
PPC_evsubfusiaa,        // Vector Subtract from (Accumulator) Unsigned, Saturate, Integer to Accumulator
PPC_evsubfssiaa,        // Vector Subtract from Signed, Saturate, Integer to Accumulator
PPC_evaddsmiaa,         // Vector Add to Accumulator
PPC_evsubfsmiaa,        // Vector Subtract from Accumulator to Accumulator
PPC_evaddh,             // Vector Add Halfwords
PPC_evaddhss,           // Vector Add Halfwords Signed and Saturate
PPC_evsubfh,            // Vector Subtract from Halfword
PPC_evsubfhss,          // Vector Subtract from Halfwords Signed and Saturate
PPC_evaddhx,            // Vector Add Halfwords Exchanged
PPC_evaddhxss,          // Vector Add Halfwords Exchanged, Signed and Saturate
PPC_evsubfhx,           // Vector Subtract from Halfwords Exchanged
PPC_evsubfhxss,         // Vector Subtract from Halfwords Exchanged, Signed and Saturate
PPC_evaddd,             // Vector Add Doubleword
PPC_evadddss,           // Vector Add Doubleword Signed and Saturate
PPC_evsubfd,            // Vector Subtract from Doubleword
PPC_evsubfdss,          // Vector Subtract from Doubleword Signed and Saturate
PPC_evaddb,             // Vector Add Bytes
PPC_evaddbss,           // Vector Add Bytes Signed and Saturate
PPC_evsubfb,            // Vector Subtract from Byte
PPC_evsubfbss,          // Vector Subtract from Byte Signed and Saturate
PPC_evaddsubfh,         // Vector Add / Subtract from Halfword
PPC_evaddsubfhss,       // Vector Add / Subtract from Halfword Signed and Saturate
PPC_evsubfaddh,         // Vector Subtract from / Add Halfwords
PPC_evsubfaddhss,       // Vector Subtract from / Add Halfwords Signed and Saturate
PPC_evaddsubfhx,        // Vector Add / Subtract from Halfword Exchanged
PPC_evaddsubfhxss,      // Vector Add / Subtract from Halfword Exchanged, Signed and Saturate
PPC_evsubfaddhx,        // Vector Subtract from / Add Halfwords Exchanged
PPC_evsubfaddhxss,      // Vector Subtract from / Add Halfwords Exchanged, Signed and Saturate
PPC_evadddus,           // Vector Add Doubleword Unsigned and Saturate
PPC_evaddbus,           // Vector Add Bytes Unsigned and Saturate
PPC_evsubfdus,          // Vector Subtract from Doubleword Unsigned and Saturate
PPC_evsubfbus,          // Vector Subtract from Byte Unsigned and Saturate
PPC_evaddwus,           // Vector Add Word Unsigned and Saturate
PPC_evaddwxus,          // Vector Add Word Exchanged Unsigned and Saturate
PPC_evsubfwus,          // Vector Subtract from Word Unsigned and Saturate
PPC_evsubfwxus,         // Vector Subtract from Word Exchanged Unsigned and Saturate
PPC_evadd2subf2h,       // Vector Add Upper 2 and Subtract from Lower 2 Halfwords
PPC_evadd2subf2hss,     // Vector Add Upper 2 and Subtract from Lower 2 Halfwords Signed and Saturate
PPC_evsubf2add2h,       // Vector Subtract from Upper 2 / Add Lower 2 Halfwords
PPC_evsubf2add2hss,     // Vector Subtract from Upper 2 / Add Lower 2 Halfwords Signed and Saturate
PPC_evaddhus,           // Vector Add Halfwords Unsigned and Saturate
PPC_evaddhxus,          // Vector Add Halfwords Exchanged, Unsigned and Saturate
PPC_evsubfhus,          // Vector Subtract from Halfwords Unsigned and Saturate
PPC_evsubfhxus,         // Vector Subtract from Halfwords Exchanged, Unsigned and Saturate
PPC_evaddwss,           // Vector Add Word Signed and Saturate
PPC_evsubfwss,          // Vector Subtract from Word Signed and Saturate
PPC_evaddwx,            // Vector Add Word Exchanged
PPC_evaddwxss,          // Vector Add Word Exchanged Signed and Saturate
PPC_evsubfwx,           // Vector Subtract from Word Exchanged
PPC_evsubfwxss,         // Vector Subtract from Word Exchanged Signed and Saturate
PPC_evaddsubfw,         // Vector Add / Subtract from Word
PPC_evaddsubfwss,       // Vector Add / Subtract from Word Signed and Saturate
PPC_evsubfaddw,         // Vector Subtract from / Add Word
PPC_evsubfaddwss,       // Vector Subtract from / Add Word Signed and Saturate
PPC_evaddsubfwx,        // Vector Add / Subtract from Word Exchanged
PPC_evaddsubfwxss,      // Vector Add / Subtract from Word Exchanged Signed and Saturate
PPC_evsubfaddwx,        // Vector Subtract from / Add Word Exchanged
PPC_evsubfaddwxss,      // Vector Subtract from / Add Word Exchanged Signed and Saturate
PPC_evmar,              // Store Accumulator
PPC_evsumwu,            // Vector Sum of Words Unsigned (to Accumulator)
PPC_evsumws,            // Vector Sum of Words Signed (to Accumulator)
PPC_evsum4bu,           // Vector Sum of 4 Bytes UnsIgned (to Accumulator)
PPC_evsum4bs,           // Vector Sum of 4 Bytes SIgned (to Accumulator)
PPC_evsum2hu,           // Vector Sum of 2 Halfwords Unsigned (to Accumulator)
PPC_evsum2hs,           // Vector Sum of 2 Halfwords Signed (to Accumulator)
PPC_evdiff2his,         // Vector Difference of 2 Halfwords Interleaved Signed (to Accumulator)
PPC_evsum2his,          // Vector Sum of 2 Halfwords Interleaved Signed (to Accumulator)
PPC_evsumwua,           // Vector Sum of Words Unsigned (to Accumulator)
PPC_evsumwsa,           // Vector Sum of Words Signed (to Accumulator)
PPC_evsum4bua,          // Vector Sum of 4 Bytes UnsIgned (to Accumulator)
PPC_evsum4bsa,          // Vector Sum of 4 Bytes SIgned (to Accumulator)
PPC_evsum2hua,          // Vector Sum of 2 Halfwords Unsigned (to Accumulator)
PPC_evsum2hsa,          // Vector Sum of 2 Halfwords Signed (to Accumulator)
PPC_evdiff2hisa,        // Vector Difference of 2 Halfwords Interleaved Signed (to Accumulator)
PPC_evsum2hisa,         // Vector Sum of 2 Halfwords Interleaved Signed (to Accumulator)
PPC_evsumwuaa,          // Vector Sum of Words Unsigned and Accumulate
PPC_evsumwsaa,          // Vector Sum of Words Signed and Accumulate
PPC_evsum4buaaw,        // Vector Sum of 4 Bytes Unsigned and Accumulate into Words
PPC_evsum4bsaaw,        // Vector Sum of 4 Bytes Signed and Accumulate into Words
PPC_evsum2huaaw,        // Vector Sum of 2 Halfwords Unsigned and Accumulate into Words
PPC_evsum2hsaaw,        // Vector Sum of 2 Halfwords Signed and Accumulate into Words
PPC_evdiff2hisaaw,      // Vector Difference of 2 Halfwords Interleaved Signed and Accumulate into Words
PPC_evsum2hisaaw,       // Vector Sum of 2 Halfwords Interleaved Signed and Accumulate into Words
PPC_evdivwsf,           // Vector Divide Word Signed Fractional
PPC_evdivwuf,           // Vector Divide Word Unsigned Fractional
PPC_evdivs,             // Vector Divide Signed
PPC_evdivu,             // Vector Divide Unsigned
PPC_evaddwegsi,         // Vector Add Word Even Guarded Signed Integer
PPC_evaddwegsf,         // Vector Add Word Even Guarded Signed Fraction
PPC_evsubfwegsi,        // Vector Subtract from Word Even Guarded Signed Integer
PPC_evsubfwegsf,        // Vector Subtract from Word Even Guarded Signed Fraction
PPC_evaddwogsi,         // Vector Add Word Odd Guarded Signed Integer
PPC_evaddwogsf,         // Vector Add Word Odd Guarded Signed Fraction
PPC_evsubfwogsi,        // Vector Subtract from Word Odd Guarded Signed Integer
PPC_evsubfwogsf,        // Vector Subtract from Word Odd Guarded Signed Fraction
PPC_evaddhhiuw,         // Vector Add Halfwords High Unsigned to Words
PPC_evaddhhisw,         // Vector Add Halfwords High Signed to Words
PPC_evsubfhhiuw,        // Vector Subtract from Halfwords High Unsigned to Words
PPC_evsubfhhisw,        // Vector Subtract from Halfwords High Signed to Words
PPC_evaddhlouw,         // Vector Add Halfwords Low Unsigned to Words
PPC_evaddhlosw,         // Vector Add Halfwords Low Signed to Words
PPC_evsubfhlouw,        // Vector Subtract from Halfwords Low Unsigned to Words
PPC_evsubfhlosw,        // Vector Subtract from Halfwords Low Signed to Words
PPC_evmhesusiaaw,       // Vector Multiply Halfwords, Even, Signed by Unsigned, Saturate, Integer and Accumulate into Words
PPC_evmhosusiaaw,       // Vector Multiply Halfwords, Odd, Signed by Unsigned, Saturate, Integer and Accumulate into Words
PPC_evmhesumiaaw,       // Vector Multiply Halfwords, Even, Signed by Unsigned, Modulo, Integer and Accumulate into Words
PPC_evmhosumiaaw,       // Vector Multiply Halfwords, Odd, Signed by Unsigned, Modulo, Integer and Accumulate into Words
PPC_evmbeusiaah,        // Vector Multiply Byte Even, Unsigned, Saturate, Integer and Accumulate Halfwords
PPC_evmbessiaah,        // Vector Multiply Byte Even, Signed, Saturate, Integer and Accumulate Halfwords
PPC_evmbesusiaah,       // Vector Multiply Byte Even, Signed by Unsigned, Saturate, Integer and Accumulate Halfwords
PPC_evmbousiaah,        // Vector Multiply Byte Odd, Unsigned, Saturate, Integer and Accumulate Halfwords
PPC_evmbossiaah,        // Vector Multiply Byte Odd, Signed, Saturate, Integer and Accumulate Halfwords
PPC_evmbosusiaah,       // Vector Multiply Byte Odd, Signed by Unsigned, Saturate, Integer and Accumulate Halfwords
PPC_evmbeumiaah,        // Vector Multiply Byte Even, Unsigned, Modulo, Integer and Accumulate Halfwords
PPC_evmbesmiaah,        // Vector Multiply Byte Even, Signed, Modulo, Integer and Accumulate Halfwords
PPC_evmbesumiaah,       // Vector Multiply Byte Even, Signed by Unsigned, Modulo, Integer and Accumulate Halfwords
PPC_evmboumiaah,        // Vector Multiply Byte Odd, Unsigned, Modulo, Integer and Accumulate Halfwords
PPC_evmbosmiaah,        // Vector Multiply Byte Odd, Signed, Modulo, Integer and Accumulate Halfwords
PPC_evmbosumiaah,       // Vector Multiply Byte Odd, Signed by Unsigned, Modulo, Integer and Accumulate Halfwords
PPC_evmwlusiaaw3,       // Vector Multiply Word Low Unsigned, Saturate, Integer and Accumulate in Words 3 operand
PPC_evmwlssiaaw3,       // Vector Multiply Word Low Signed, Saturate, Integer and Accumulate in Words
PPC_evmwhssfraaw3,      // Vector Multiply Word High Signed, Saturate, Fractional, Round and Accumulate into Words 3 operand
PPC_evmwhssfaaw3,       // Vector Multiply Word High Signed, Saturate, Fractional and Accumulate into Words 3 operand
PPC_evmwhssfraaw,       // Vector Multiply Word High Signed, Saturate, Fractional, Round and Accumulate into Words
PPC_evmwhssfaaw,        // Vector Multiply Word High Signed, Saturate, Fractional and Accumulate into Words
PPC_evmwlumiaaw3,       // Vector Multiply Word Low Unsigned, Modulo, Integer and Accumulate in Words 3 operand
PPC_evmwlsmiaaw3,       // Vector Multiply Word Low Signed, Modulo, Integer and Accumulate in Words 3 operand
PPC_evmwusiaa,          // Vector Multiply Word Unsigned, Saturate, Integer and Accumulate
PPC_evmwssiaa,          // Vector Multiply Word Signed, Saturate, Integer and Accumulate
PPC_evmwehgsmfraa,      // Vector Multiply Word Even High Guarded Signed, Modulo, Fractional, Round and Accumulate
PPC_evmwehgsmfaa,       // Vector Multiply Word Even High Guarded Signed, Modulo, Fractional and Accumulate
PPC_evmwohgsmfraa,      // Vector Multiply Word Odd High Guarded Signed, Modulo, Fractional, Round and Accumulate
PPC_evmwohgsmfaa,       // Vector Multiply Word Odd High Guarded Signed, Modulo, Fractional and Accumulate
PPC_evmhesusianw,       // Vector Multiply Halfwords, Even, Signed by Unsigned, Saturate, Integer and Accumulate Negative into Words
PPC_evmhosusianw,       // Vector Multiply Halfwords, Odd, Signed by Unsigned, Saturate, Integer and Accumulate Negative into Words
PPC_evmhesumianw,       // Vector Multiply Halfwords, Even, Signed by Unsigned, Modulo, Integer and Accumulate Negative into Words
PPC_evmhosumianw,       // Vector Multiply Halfwords, Odd, Signed by Unsigned, Modulo, Integer and Accumulate Negative into Words
PPC_evmbeusianh,        // Vector Multiply Byte Even, Unsigned, Saturate, Integer and Accumulate Negative Halfwords
PPC_evmbessianh,        // Vector Multiply Byte Even, Signed, Saturate, Integer and Accumulate Negative Halfwords
PPC_evmbesusianh,       // Vector Multiply Byte Even, Signed by Unsigned, Saturate, Integer and Accumulate Negative Halfwords
PPC_evmbousianh,        // Vector Multiply Byte Odd, Unsigned, Saturate, Integer and Accumulate Negative Halfwords
PPC_evmbossianh,        // Vector Multiply Byte Odd, Signed, Saturate, Integer and Accumulate Negative Halfwords
PPC_evmbosusianh,       // Vector Multiply Byte Odd, Signed by Unsigned, Saturate, Integer and Accumulate Negative Halfwords
PPC_evmbeumianh,        // Vector Multiply Byte Even, Unsigned, Modulo, Integer and Accumulate Negative Halfwords
PPC_evmbesmianh,        // Vector Multiply Byte Even, Signed, Modulo, Integer and Accumulate Negative Halfwords
PPC_evmbesumianh,       // Vector Multiply Byte Even, Signed by Unsigned, Modulo, Integer and Accumulate Negative Halfwords
PPC_evmboumianh,        // Vector Multiply Byte Odd, Unsigned, Modulo, Integer and Accumulate Negative Halfwords
PPC_evmbosmianh,        // Vector Multiply Byte Odd, Signed, Modulo, Integer and Accumulate Negative Halfwords
PPC_evmbosumianh,       // Vector Multiply Byte Odd, Signed by Unsigned, Modulo, Integer and Accumulate Negative Halfwords
PPC_evmwlusianw3,       // Vector Multiply Word Low Unsigned, Saturate, Integer and Accumulate Negative in Words 3 operand
PPC_evmwlssianw3,       // Vector Multiply Word Low Signed, Saturate, Integer and Accumulate Negative in Words 3 operand
PPC_evmwhssfranw3,      // Vector Multiply Word High Signed, Saturate, Fractional, Round, and Accumulate Negative into Words 3 operand
PPC_evmwhssfanw3,       // Vector Multiply Word High Signed, Saturate, Fractional and Accumulate Negative into Words 3 operand
PPC_evmwhssfranw,       // Vector Multiply Word High Signed, Saturate, Fractional, Round, and Accumulate Negative into Words
PPC_evmwhssfanw,        // Vector Multiply Word High Signed, Saturate, Fractional and Accumulate Negative into Words
PPC_evmwlumianw3,       // Vector Multiply Word Low Unsigned, Modulo, Integer and Accumulate Negative in Words 3 operand
PPC_evmwlsmianw3,       // Vector Multiply Word Low Signed, Modulo, Integer and Accumulate Negative in Words 3 operand
PPC_evmwusian,          // Vector Multiply Word Unsigned, Saturate, Integer and Accumulate Negative
PPC_evmwssian,          // Vector Multiply Word Signed, Saturate, Integer and Accumulate Negative
PPC_evmwehgsmfran,      // Vector Multiply Word Even High, Guarded, Signed, Modulo, Fractional, Round and Accumulate Negative
PPC_evmwehgsmfan,       // Vector Multiply Word Even High, Guarded, Signed, Modulo, Fractional and Accumulate Negative
PPC_evmwohgsmfran,      // Vector Multiply Word Odd, High, Guarded, Signed, Modulo, Fractional, Round and Accumulate Negative
PPC_evmwohgsmfan,       // Vector Multiply Word Odd, High, Guarded, Signed, Modulo, Fractional and Accumulate Negative
PPC_evseteqb,           // Vector Set if Equal Byte [and Record]
PPC_evseteqh,           // Vector Set if Equal Halfword [and Record]
PPC_evseteqw,           // Vector Set if Equal Word [and Record]
PPC_evsetgthu,          // Vector Set if Greater Than Halfword UnsIgned [and Record]
PPC_evsetgths,          // Vector Set if Greater Than Halfword SIgned [and Record]
PPC_evsetgtwu,          // Vector Set if Greater Than Word Unsigned [and Record]
PPC_evsetgtws,          // Vector Set if Greater Than Word Signed [and Record]
PPC_evsetgtbu,          // Vector Set if Greater Than Byte Unsigned [and Record]
PPC_evsetgtbs,          // Vector Set if Greater Than Byte SIgned [and Record]
PPC_evsetltbu,          // Vector Set if Less Than Byte Unsigned [and Record]
PPC_evsetltbs,          // Vector Set if Less Than Byte SIgned [and Record]
PPC_evsetlthu,          // Vector Set if Less Than Halfword UnsIgned [and Record]
PPC_evsetlths,          // Vector Set if Less Than Halfword SIgned [and Record]
PPC_evsetltwu,          // Vector Set if Less Than Word Unsigned [and Record]
PPC_evsetltws,          // Vector Set if Less Than Word Signed [and Record]
PPC_evsaduw,            // Vector Sum of Absolute Differences of Unsigned Words (to Accumulator)
PPC_evsadsw,            // Vector Sum of Absolute Differences of Signed Words (to Accumulator)
PPC_evsad4ub,           // Vector Sum of Absolute Differences of 4 Unsigned Bytes (to Accumulator)
PPC_evsad4sb,           // Vector Sum of Absolute Differences of 4 Signed Bytes (to Accumulator)
PPC_evsad2uh,           // Vector Sum of Absolute Differences of 2 Unsigned Halfwords (to Accumulator)
PPC_evsad2sh,           // Vector Sum of Absolute Differences of 2 Signed Halfwords (to Accumulator)
PPC_evsaduwa,           // Vector Sum of Absolute Differences of Unsigned Words (to Accumulator)
PPC_evsadswa,           // Vector Sum of Absolute Differences of Signed Words (to Accumulator)
PPC_evsad4uba,          // Vector Sum of Absolute Differences of 4 Unsigned Bytes (to Accumulator)
PPC_evsad4sba,          // Vector Sum of Absolute Differences of 4 Signed Bytes (to Accumulator)
PPC_evsad2uha,          // Vector Sum of Absolute Differences of 2 Unsigned Halfwords (to Accumulator)
PPC_evsad2sha,          // Vector Sum of Absolute Differences of 2 Signed Halfwords (to Accumulator)
PPC_evabsdifuw,         // Vector Absolute Difference of Unsigned Words
PPC_evabsdifsw,         // Vector Absolute Difference of Signed Words
PPC_evabsdifub,         // Vector Absolute Difference of Unsigned Bytes
PPC_evabsdifsb,         // Vector Absolute Difference of Signed Bytes
PPC_evabsdifuh,         // Vector Absolute Difference of Unsigned Halfwords
PPC_evabsdifsh,         // Vector Absolute Difference of Signed Halfwords
PPC_evsaduwaa,          // Vector Sum of Absolute Differences of Unsigned Words and Accumulate
PPC_evsadswaa,          // Vector Sum of Absolute Differences of Signed Words and Accumulate
PPC_evsad4ubaaw,        // Vector Sum of Absolute Differences of 4 Unsigned Bytes and Accumulate into Words
PPC_evsad4sbaaw,        // Vector Sum of Absolute Differences of 4 Signed Bytes and Accumulate into Words
PPC_evsad2uhaaw,        // Vector Sum of Absolute Differences of 2 Unsigned Halfwords and Accumulate into Words
PPC_evsad2shaaw,        // Vector Sum of Absolute Differences of 2 Signed Halfwords and Accumulate into Words
PPC_evpkshubs,          // Vector Pack Signed Halfwords to Unsigned Bytes and Saturate
PPC_evpkshsbs,          // Vector Pack Signed Halfwords to Signed Bytes and Saturate
PPC_evpkswuhs,          // Vector Pack Signed Words to Unsigned Halfwords and Saturate
PPC_evpkswshs,          // Vector Pack Signed Words to Signed Halfwords and Saturate
PPC_evpkuhubs,          // Vector Pack Unsigned Halfwords to Unsigned Bytes and Saturate
PPC_evpkuwuhs,          // Vector Pack Unsigned Words to Unsigned Halfwords and Saturate
PPC_evpkswshilvs,       // Vector Pack Signed Words to Signed Halfwords Interleaved and Saturate
PPC_evpkswgshefrs,      // Vector Pack Signed Words Guarded to Signed Halfwords Even Fractional Round and Saturate
PPC_evpkswshfrs,        // Vector Pack Signed Words to Signed Halfwords Fractional, Round and Saturate
PPC_evpkswshilvfrs,     // Vector Pack Signed Words to Signed Halfwords Interleaved, Fractional, Round and Saturate
PPC_evpksdswfrs,        // Vector Pack Signed Doublewords to Signed Words Fractional, Round and Saturate
PPC_evpksdshefrs,       // Vector Pack Signed Doublewords to Signed Halfwords Even, Fractional, Round, and Saturate
PPC_evpkuduws,          // Vector Pack Unsigned Doublewords to Unsigned Words and Saturate
PPC_evpksdsws,          // Vector Pack Signed Doublewords to Signed Words and Saturate
PPC_evpkswgswfrs,       // Vector Pack Signed Words Guarded to Signed Words Fractional Round and Saturate
PPC_evilveh,            // Vector Interleave Even Halfwords
PPC_evilveoh,           // Vector Interleave Even/Odd Halfwords
PPC_evilvhih,           // Vector Interleave High Halfwords
PPC_evilvhiloh,         // Vector Interleave High/Low Halfwords
PPC_evilvloh,           // Vector Interleave Low Halfwords
PPC_evilvlohih,         // Vector Interleave Low/High Halfwords
PPC_evilvoeh,           // Vector Interleave Odd/Even Halfwords
PPC_evilvoh,            // Vector Interleave Odd Halfwords
PPC_evdlveb,            // Vector De-interleave Even Bytes
PPC_evdlveh,            // Vector De-interleave Even Halfwords
PPC_evdlveob,           // Vector De-interleave Even/Odd Bytes
PPC_evdlveoh,           // Vector De-interleave Even/Odd Halfwords
PPC_evdlvob,            // Vector De-interleave Odd Bytes
PPC_evdlvoh,            // Vector De-interleave Odd Halfwords
PPC_evdlvoeb,           // Vector De-interleave Odd/Even Bytes
PPC_evdlvoeh,           // Vector De-interleave Odd/Even Halfwords
PPC_evmaxbu,            // Vector Maximum Byte Unsigned
PPC_evmaxbs,            // Vector Maximum Byte Signed
PPC_evmaxhu,            // Vector Maximum Halfword Unsigned
PPC_evmaxhs,            // Vector Maximum Halfword Signed
PPC_evmaxwu,            // Vector Maximum Word Unsigned
PPC_evmaxws,            // Vector Maximum Word Signed
PPC_evmaxdu,            // Vector Maximum Doubleword Unsigned
PPC_evmaxds,            // Vector Maximum Doubleword Signed
PPC_evminbu,            // Vector Minimum Byte Unsigned
PPC_evminbs,            // Vector Minimum Byte Signed
PPC_evminhu,            // Vector Minimum Halfword Unsigned
PPC_evminhs,            // Vector Minimum Halfword Signed
PPC_evminwu,            // Vector Minimum Word Unsigned
PPC_evminws,            // Vector Minimum Word Signed
PPC_evmindu,            // Vector Minimum Doubleword Unsigned
PPC_evminds,            // Vector Minimum Doubleword Signed
PPC_evavgwu,            // Vector Average Word Unsigned
PPC_evavgws,            // Vector Average Word Signed
PPC_evavgbu,            // Vector Average Byte Unsigned
PPC_evavgbs,            // Vector Average Byte Signed
PPC_evavghu,            // Vector Average Halfword Unsigned
PPC_evavghs,            // Vector Average Halfword Signed
PPC_evavgdu,            // Vector Average Doubleword Unsigned
PPC_evavgds,            // Vector Average Doubleword Signed
PPC_evavgwur,           // Vector Average Word Unsigned with Round
PPC_evavgwsr,           // Vector Average Word Signed with Round
PPC_evavgbur,           // Vector Average Byte Unsigned with Round
PPC_evavgbsr,           // Vector Average Byte Signed with Round
PPC_evavghur,           // Vector Average Halfword Unsigned with Round
PPC_evavghsr,           // Vector Average Halfword Signed with Round
PPC_evavgdur,           // Vector Average Doubleword Unsigned with Round
PPC_evavgdsr,           // Vector Average Doubleword Signed with Round

// Some simplifications that were missed previously
PPC_tdui,       // Trap Doubleword Unconditionally with Immediate
PPC_tdu,        // Trap Doubleword Unconditionally
PPC_twui,       // Trap Word Unconditionally with Immediate
PPC_twu,        // Trap Word Unconditionally

// Power ISA 2.07
PPC_bctar,        // Branch Conditional to Branch Target Address Register
PPC_clrbhrb,      // Clear BHRB
PPC_mfbhrbe,      // Move From Branch History Rolling Buffer
PPC_mtsle,        // Move To Split Little Endian
PPC_mfvsrd,       // Move From VSR Doubleword
PPC_mfvsrwz,      // Move From VSR Word and Zero
PPC_mtvsrd,       // Move To VSR Doubleword
PPC_mtvsrwa,      // Move To VSR Word Algebraic
PPC_mtvsrwz,      // Move To VSR Word and Zero
PPC_fmrgew,       // Floating Merge Even Word
PPC_fmrgow,       // Floating Merge Odd Word
PPC_vpksdss,      // Vector Pack Signed Doubleword Signed Saturate
PPC_vpksdus,      // Vector Pack Signed Doubleword Unsigned Saturate
PPC_vpkudus,      // Vector Pack Unsigned Doubleword Unsigned Saturate
PPC_vpkudum,      // Vector Pack Unsigned Doubleword Unsigned Modulo
PPC_vupkhsw,      // Vector Unpack High Signed Word
PPC_vupklsw,      // Vector Unpack Low Signed Word
PPC_vmrgew,       // Vector Merge Even Word
PPC_vmrgow,       // Vector Merge Odd Word
PPC_vaddudm,      // Vector Add Unsigned Doubleword Modulo
PPC_vadduqm,      // Vector Add Unsigned Quadword Modulo
PPC_vaddeuqm,     // Vector Add Extended Unsigned Quadword Modulo
PPC_vaddcuq,      // Vector Add & write Carry Unsigned Quadword
PPC_vaddecuq,     // Vector Add Extended & write Carry Unsigned Quadword
PPC_vsubudm,      // Vector Subtract Unsigned Doubleword Modulo
PPC_vsubuqm,      // Vector Subtract Unsigned Quadword Modulo
PPC_vsubeuqm,     // Vector Subtract Extended Unsigned Quadword Modulo
PPC_vsubcuq,      // Vector Subtract & write Carry Unsigned Quadword
PPC_vsubecuq,     // Vector Subtract Extended & write Carry Unsigned Quadword
PPC_vmulesw,      // Vector Multiply Even Signed Word
PPC_vmuleuw,      // Vector Multiply Even Unsigned Word
PPC_vmulosw,      // Vector Multiply Odd Signed Word
PPC_vmulouw,      // Vector Multiply Odd Unsigned Word
PPC_vmuluwm,      // Vector Multiply Unsigned Word Modulo
PPC_vmaxsd,       // Vector Maximum Signed Doubleword
PPC_vmaxud,       // Vector Maximum Unsigned Doubleword
PPC_vminsd,       // Vector Minimum Signed Doubleword
PPC_vminud,       // Vector Minimum Unsigned Doubleword
PPC_vcmpequd,     // Vector Compare Equal To Unsigned Doubleword
PPC_vcmpgtsd,     // Vector Compare Greater Than Signed Doubleword
PPC_vcmpgtud,     // Vector Compare Greater Than Unsigned Doubleword
PPC_veqv,         // Vector Equivalence
PPC_vnand,        // Vector NAND
PPC_vorc,         // Vector OR with Complement
PPC_vrld,         // Vector Rotate Left Doubleword
PPC_vsld,         // Vector Shift Left Doubleword
PPC_vsrd,         // Vector Shift Right Doubleword
PPC_vsrad,        // Vector Shift Right Algebraic Doubleword
PPC_vcipher,      // Vector AES Cipher
PPC_vcipherlast,  // Vector AES Cipher Last
PPC_vncipher,     // Vector AES Inverse Cipher
PPC_vncipherlast, // Vector AES Inverse Cipher Last
PPC_vsbox,        // Vector AES S-Box
PPC_vshasigmad,   // Vector SHA-512 Sigma Doubleword
PPC_vshasigmaw,   // Vector SHA-256 Sigma Word
PPC_vpmsumb,      // Vector Polynomial Multiply-Sum Byte
PPC_vpmsumd,      // Vector Polynomial Multiply-Sum Doubleword
PPC_vpmsumh,      // Vector Polynomial Multiply-Sum Halfword
PPC_vpmsumw,      // Vector Polynomial Multiply-Sum Word
PPC_vpermxor,     // Vector Permute and Exclusive-OR
PPC_vgbbd,        // Vector Gather Bits by Byte by Doubleword
PPC_vclzb,        // Vector Count Leading Zeros Byte
PPC_vclzh,        // Vector Count Leading Zeros Halfword
PPC_vclzw,        // Vector Count Leading Zeros Word
PPC_vclzd,        // Vector Count Leading Zeros Doubleword
PPC_vpopcntb,     // Vector Population Count Byte
PPC_vpopcntd,     // Vector Population Count Doubleword
PPC_vpopcnth,     // Vector Population Count Halfword
PPC_vpopcntw,     // Vector Population Count Word
PPC_vbpermq,      // Vector Bit Permute Quadword
PPC_bcdadd,       // Decimal Add Modulo
PPC_bcdsub,       // Decimal Subtract Modulo
PPC_lxsiwax,      // Load VSX Scalar as Integer Word Algebraic Indexed
PPC_lxsspx,       // Load VSX Scalar Single-Precision Indexed
PPC_lxsiwzx,      // Load VSX Scalar as Integer Word and Zero Indexed
PPC_stxsiwx,      // Store VSX Scalar as Integer Word Indexed
PPC_stxsspx,      // Store VSR Scalar Word Indexed
PPC_xsaddsp,      // VSX Scalar Add Single-Precision
PPC_xscvdpspn,    // VSX Scalar Convert Double-Precision to Single-Precision format Non-signalling
PPC_xscvspdpn,    // Scalar Convert Single-Precision to Double-Precision format Non-signalling
PPC_xscvsxdsp,    // VSX Scalar Convert Signed Fixed-Point Doubleword to Single-Precision
PPC_xscvuxdsp,    // VSX Scalar Convert Unsigned Fixed-Point Doubleword to Single-Precision
PPC_xsdivsp,      // VSX Scalar Divide Single-Precision
PPC_xsmaddasp,    // VSX Scalar Multiply-Add Type-A Single-Precision
PPC_xsmaddmsp,    // VSX Scalar Multiply-Add Type-M Single-Precision
PPC_xsmsubasp,    // VSX Scalar Multiply-Subtract Type-A Single-Precision
PPC_xsmsubmsp,    // VSX Scalar Multiply-Subtract Type-M Single-Precision
PPC_xsmulsp,      // VSX Scalar Multiply Single-Precision
PPC_xsnmaddasp,   // VSX Scalar Negative Multiply-Add Type-A Single-Precision
PPC_xsnmaddmsp,   // VSX Scalar Negative Multiply-Add Type-M Single-Precision
PPC_xsnmsubasp,   // VSX Scalar Negative Multiply-Subtract Type-A Single-Precision
PPC_xsnmsubmsp,   // VSX Scalar Negative Multiply-Subtract Type-M Single-Precision
PPC_xsresp,       // VSX Scalar Reciprocal Estimate Single-Precision
PPC_xsrsp,        // VSX Scalar Round to Single-Precision
PPC_xsrsqrtesp,   // VSX Scalar Reciprocal Square Root Estimate Single-Precision
PPC_xssqrtsp,     // VSX Scalar Square Root Single-Precision
PPC_xssubsp,      // VSX Scalar Subtract Single-Precision
PPC_xxleqv,       // VSX Logical Equivalence
PPC_xxlnand,      // VSX Logical NAND
PPC_xxlorc,       // VSX Logical OR with Complement
PPC_lqarx,        // Load Quadword And Reserve Indexed
PPC_stqcx,        // Store Quadword Conditional Indexed and record CR0
PPC_tbegin,       // Transaction Begin
PPC_tend,         // Transaction End
PPC_tabort,       // Transaction Abort
PPC_tabortwc,     // Transaction Abort Word Conditional
PPC_tabortwci,    // Transaction Abort Word Conditional Immediate
PPC_tabortdc,     // Transaction Abort Doubleword Conditional
PPC_tabortdci,    // Transaction Abort Doubleword Conditional Immediate
PPC_tsr,          // Transaction Suspend or Resume
PPC_tcheck,       // Transaction Check
PPC_rfebb,        // Return from Event Based Branch
PPC_treclaim,     // Transaction Reclaim
PPC_trechkpt,     // Transaction Recheckpoint
PPC_msgsndp,      // Message Send Privileged
PPC_msgclrp,      // Message Clear Privileged
PPC_dcblq,        // Data Cache Block Lock Query
PPC_icblq,        // Instruction Cache Block Lock Query

// Simplifications for Power ISA 2.07
PPC_vmr,        // Vector Move Register
PPC_vnot,       // Vector Complement Register
PPC_tendall,    // Transaction End All
PPC_tsuspend,   // Transaction Suspend
PPC_tresume,    // Transaction Resume
PPC_mtppr,      // Move To Program Priority Register
PPC_mfppr,      // Move From Program Priority Register
PPC_mtppr32,    // Move To Program Priority Register 32-Bit
PPC_mfppr32,    // Move From Program Priority Register 32-Bit
PPC_mtic,       // Move To IC
PPC_mfic,       // Move From IC
PPC_mtvtb,      // Move to VTB
PPC_mfvtb,      // Move From VTB
PPC_miso,       // "or" Cache Control Hint
PPC_mdoio,      // "or" Shared Resource Hint
PPC_mdoom,      // "or" Shared Resource Hint
// these are mentioned in the Power ISA 2.07 document but not implemented by
// binutils; since they are just simplifications better to ignore them than to
// emit "weird" mnemonics
// PPC_mtuamr,     // Move To Authority Mask Register
// PPC_mfuamr,     // Move From Authority Mask Register
// PPC_dcbtct,     // Data Cache Block Touch
// PPC_dcbtds,     // Data Cache Block Touch
// PPC_dcbna,      // Data Cache Block Touch
// PPC_dcbtctep,   // Data Cache Block Touch by External PID
// PPC_dcbtdsep,   // Data Cache Block Touch by External PID
// PPC_dcbflep,    // Data Cache Block Flush by External PID
// PPC_dcbflpep,   // Data Cache Block Flush by External PID
// PPC_dcbtstctep, // Data Cache Block Touch for Store by External PID
//
// these are only needed by assemblers
// PPC_lxvx,       // Load VSX Vector Doubleword*2 Indexed
// PPC_stxvx,      // Store VSX Vector Doubleword*2 Indexed
// PPC_mffprd,     // Move From FPR Doubleword
// PPC_mfvrd,      // Move From VR Doubleword
// PPC_mffprwz,    // Move From FPR Word and Zero
// PPC_mfvrwz,     // Move From VR Word and Zero
// PPC_mtfprd,     // Move To FPR Doubleword
// PPC_mtvrd,      // Move To VR Doubleword
// PPC_mtfprwa,    // Move To FPR Word Algebraic
// PPC_mtvrwa,     // Move To VR Word Algebraic
// PPC_mtfprwz,    // Move To FPR Word and Zero
// PPC_mtvrwz,     // Move To VR Word and Zero

PPC_last,
    };

/*
*      Interactive disassembler (IDA).
*      Copyright (c) 1990-2009 Hex-Rays
*      ALL RIGHTS RESERVED.
*
*/



//----------------------------------------------------------------------

extern instruc_t Instructions[];

enum NEC850_Instructions
{
  NEC850_NULL = 0,
  NEC850_BREAKPOINT,
  NEC850_XORI,
  NEC850_XOR,
  NEC850_TST1,
  NEC850_TST,
  NEC850_TRAP,
  NEC850_SUBR,
  NEC850_SUB,
  NEC850_STSR,
  NEC850_ST_B,
  NEC850_ST_H,
  NEC850_ST_W,
  NEC850_SST_B,
  NEC850_SST_H,
  NEC850_SST_W,
  NEC850_SLD_B,
  NEC850_SLD_H,
  NEC850_SLD_W,
  NEC850_SHR,
  NEC850_SHL,
  NEC850_SET1,
  NEC850_SETFV,
  NEC850_SETFL,
  NEC850_SETFZ,
  NEC850_SETFNH,
  NEC850_SETFN,
  NEC850_SETFT,
  NEC850_SETFLT,
  NEC850_SETFLE,
  NEC850_SETFNV,
  NEC850_SETFNC,
  NEC850_SETFNZ,
  NEC850_SETFH,
  NEC850_SETFP,
  NEC850_SETFSA,
  NEC850_SETFGE,
  NEC850_SETFGT,
  NEC850_SATSUBR,
  NEC850_SATSUBI,
  NEC850_SATSUB,
  NEC850_SATADD,
  NEC850_SAR,
  NEC850_RETI,
  NEC850_ORI,
  NEC850_OR,
  NEC850_NOT1,
  NEC850_NOT,
  NEC850_NOP,
  NEC850_MULHI,
  NEC850_MULH,
  NEC850_MOVHI,
  NEC850_MOVEA,
  NEC850_MOV,
  NEC850_LDSR,
  NEC850_LD_B,
  NEC850_LD_H,
  NEC850_LD_W,
  NEC850_JR,
  NEC850_JMP,
  NEC850_JARL,
  NEC850_HALT,
  NEC850_EI,
  NEC850_DIVH,
  NEC850_DI,
  NEC850_CMP,
  NEC850_CLR1,
  NEC850_BV,
  NEC850_BL,
  NEC850_BZ,
  NEC850_BNH,
  NEC850_BN,
  NEC850_BR,
  NEC850_BLT,
  NEC850_BLE,
  NEC850_BNV,
  NEC850_BNC,
  NEC850_BNZ,
  NEC850_BH,
  NEC850_BP,
  NEC850_BSA,
  NEC850_BGE,
  NEC850_BGT,
  NEC850_ANDI,
  NEC850_AND,
  NEC850_ADDI,
  NEC850_ADD,

  //
  // V850E
  //
  NEC850_SWITCH,
  NEC850_ZXB,
  NEC850_SXB,
  NEC850_ZXH,
  NEC850_SXH,
  NEC850_DISPOSE_r0,
  NEC850_DISPOSE_r,
  NEC850_CALLT,
  NEC850_DBTRAP,
  NEC850_DBRET,
  NEC850_CTRET,

  NEC850_SASFV,
  NEC850_SASFL,
  NEC850_SASFZ,
  NEC850_SASFNH,
  NEC850_SASFN,
  NEC850_SASFT,
  NEC850_SASFLT,
  NEC850_SASFLE,
  NEC850_SASFNV,
  NEC850_SASFNC,
  NEC850_SASFNZ,
  NEC850_SASFH,
  NEC850_SASFP,
  NEC850_SASFSA,
  NEC850_SASFGE,
  NEC850_SASFGT,

  NEC850_PREPARE_sp,
  NEC850_PREPARE_i,

  NEC850_MUL,
  NEC850_MULU,

  NEC850_DIVH_r3,
  NEC850_DIVHU,
  NEC850_DIV,
  NEC850_DIVU,

  NEC850_BSW,
  NEC850_BSH,
  NEC850_HSW,

  NEC850_CMOVV,
  NEC850_CMOVL,
  NEC850_CMOVZ,
  NEC850_CMOVNH,
  NEC850_CMOVN,
  NEC850_CMOV,
  NEC850_CMOVLT,
  NEC850_CMOVLE,
  NEC850_CMOVNV,
  NEC850_CMOVNC,
  NEC850_CMOVNZ,
  NEC850_CMOVH,
  NEC850_CMOVP,
  NEC850_CMOVSA,
  NEC850_CMOVGE,
  NEC850_CMOVGT,

  NEC850_SLD_BU,
  NEC850_SLD_HU,
  NEC850_LD_BU,
  NEC850_LD_HU,

  NEC850_LAST_INSTRUCTION
};


/*
 *      Tricore Disassembly Module
 *      Version 1.0
 *      Copyright (c) 2000 by Juergen Krumm
 *      ALL RIGHTS RESERVED
 *
 *
 */


extern instruc_t Instructions[];

enum 
{
 TRICORE_null = 0,           // Unknown Operation
 TRICORE_abs,
 TRICORE_abs_b,
 TRICORE_abs_h,
 TRICORE_absdif,
 TRICORE_absdif_b,
 TRICORE_absdif_h,
 TRICORE_absdifs,
 TRICORE_absdifs_h,
 TRICORE_abss,
 TRICORE_abss_h,
 TRICORE_add_b,
 TRICORE_add_f,
 TRICORE_add_h,
 TRICORE_add16,
 TRICORE_add16_a,
 TRICORE_add32,
 TRICORE_add32_a,
 TRICORE_addc,
 TRICORE_addi,
 TRICORE_addih,
 TRICORE_addih_a,
 TRICORE_adds,
 TRICORE_adds_h,
 TRICORE_adds_hu,
 TRICORE_adds_u,
 TRICORE_adds16,
 TRICORE_addsc_at,
 TRICORE_addsc16_a,
 TRICORE_addsc32_a,
 TRICORE_addx,
 TRICORE_and_and_t,
 TRICORE_and_andn_t,
 TRICORE_and_eq,
 TRICORE_and_ge,
 TRICORE_and_ge_u,
 TRICORE_and_lt,
 TRICORE_and_lt_u,
 TRICORE_and_ne,
 TRICORE_and_nor_t,
 TRICORE_and_or_t,
 TRICORE_and_t,
 TRICORE_and16,
 TRICORE_and32,
 TRICORE_andn,
 TRICORE_andn_t,
 TRICORE_bisr16,
 TRICORE_bisr32,
 TRICORE_bmerge,
 TRICORE_bsplit,
 TRICORE_cachea_i,
 TRICORE_cachea_w,
 TRICORE_cachea_wi,
 TRICORE_cadd16,
 TRICORE_cadd32,
 TRICORE_caddn16,
 TRICORE_caddn32,
 TRICORE_call16,
 TRICORE_call32,
 TRICORE_calla,
 TRICORE_calli,
 TRICORE_clo,
 TRICORE_clo_h,
 TRICORE_cls,
 TRICORE_cls_h,
 TRICORE_clz,
 TRICORE_clz_h,
 TRICORE_cmov16,
 TRICORE_cmovn16,
 TRICORE_cmp_f,
 TRICORE_csub,
 TRICORE_csubn,
 TRICORE_debug16,
 TRICORE_debug32,
 TRICORE_dextr,
 TRICORE_disable,
 TRICORE_div_f,
 TRICORE_dsync,
 TRICORE_dvadj,
 TRICORE_dvinit,
 TRICORE_dvinit_b,
 TRICORE_dvinit_bu,
 TRICORE_dvinit_h,
 TRICORE_dvinit_hu,
 TRICORE_dvinit_u,
 TRICORE_dvstep,
 TRICORE_dvstep_u,
 TRICORE_enable,
 TRICORE_eq_a,
 TRICORE_eq_b,
 TRICORE_eq_h,
 TRICORE_eq_w,
 TRICORE_eq16,
 TRICORE_eq32,
 TRICORE_eqany_b,
 TRICORE_eqany_h,
 TRICORE_eqz_a,
 TRICORE_extr,
 TRICORE_extr_u,
 TRICORE_ftoi,
 TRICORE_ftoq31,
 TRICORE_ftou,
 TRICORE_ge,
 TRICORE_ge_a,
 TRICORE_ge_u,
 TRICORE_imask,
 TRICORE_ins_t,
 TRICORE_insert,
 TRICORE_insn_t,
 TRICORE_isync,
 TRICORE_itof,
 TRICORE_ixmax,
 TRICORE_ixmax_u,
 TRICORE_ixmin,
 TRICORE_ixmin_u,
 TRICORE_j16,
 TRICORE_j32,
 TRICORE_ja,
 TRICORE_jeq_a,
 TRICORE_jeq16,
 TRICORE_jeq32,
 TRICORE_jge,
 TRICORE_jge_u,
 TRICORE_jgez16,
 TRICORE_jgtz16,
 TRICORE_ji16,
 TRICORE_ji32,
 TRICORE_jl,
 TRICORE_jla,
 TRICORE_jlez16,
 TRICORE_jli,
 TRICORE_jlt,
 TRICORE_jlt_u,
 TRICORE_jltz16,
 TRICORE_jne_a,
 TRICORE_jne16,
 TRICORE_jne32,
 TRICORE_jned,
 TRICORE_jnei,
 TRICORE_jnz16,
 TRICORE_jnz16_a,
 TRICORE_jnz16_t,
 TRICORE_jnz32_a,
 TRICORE_jnz32_t,
 TRICORE_jz16,
 TRICORE_jz16_a,
 TRICORE_jz16_t,
 TRICORE_jz32_a,
 TRICORE_jz32_t,
 TRICORE_ld_b,
 TRICORE_ld_d,
 TRICORE_ld_da,
 TRICORE_ld_hu,
 TRICORE_ld_q,
 TRICORE_ld16_a,
 TRICORE_ld16_bu,
 TRICORE_ld16_h,
 TRICORE_ld16_w,
 TRICORE_ld32_a,
 TRICORE_ld32_bu,
 TRICORE_ld32_h,
 TRICORE_ld32_w,
 TRICORE_ldlcx,
 TRICORE_ldmst,
 TRICORE_lducx,
 TRICORE_lea,
 TRICORE_loop16,
 TRICORE_loop32,
 TRICORE_loopu,
 TRICORE_lt_a,
 TRICORE_lt_b,
 TRICORE_lt_bu,
 TRICORE_lt_h,
 TRICORE_lt_hu,
 TRICORE_lt_u,
 TRICORE_lt_w,
 TRICORE_lt_wu,
 TRICORE_lt16,
 TRICORE_lt32,
 TRICORE_madd,
 TRICORE_madd_f,
 TRICORE_madd_h,
 TRICORE_madd_q,
 TRICORE_madd_u,
 TRICORE_maddm_h,
 TRICORE_maddms_h,
 TRICORE_maddr_h,
 TRICORE_maddr_q,
 TRICORE_maddrs_h,
 TRICORE_maddrs_q,
 TRICORE_madds,
 TRICORE_madds_h,
 TRICORE_madds_q,
 TRICORE_madds_u,
 TRICORE_maddsu_h,
 TRICORE_maddsum_h,
 TRICORE_maddsums_h,
 TRICORE_maddsur_h,
 TRICORE_maddsurs_h,
 TRICORE_maddsus_h,
 TRICORE_max,
 TRICORE_max_b,
 TRICORE_max_bu,
 TRICORE_max_h,
 TRICORE_max_hu,
 TRICORE_max_u,
 TRICORE_mfcr,
 TRICORE_min,
 TRICORE_min_b,
 TRICORE_min_bu,
 TRICORE_min_h,
 TRICORE_min_hu,
 TRICORE_min_u,
 TRICORE_mov_u,
 TRICORE_mov16,
 TRICORE_mov16_a,
 TRICORE_mov16_aa,
 TRICORE_mov16_d,
 TRICORE_mov32,
 TRICORE_mov32_a,
 TRICORE_mov32_aa,
 TRICORE_mov32_d,
 TRICORE_movh,
 TRICORE_movh_a,
 TRICORE_msub,
 TRICORE_msub_f,
 TRICORE_msub_h,
 TRICORE_msub_q,
 TRICORE_msub_u,
 TRICORE_msubad_h,
 TRICORE_msubadm_h,
 TRICORE_msubadms_h,
 TRICORE_msubadr_h,
 TRICORE_msubadrs_h,
 TRICORE_msubads_h,
 TRICORE_msubm_h,
 TRICORE_msubms_h,
 TRICORE_msubr_h,
 TRICORE_msubr_q,
 TRICORE_msubrs_h,
 TRICORE_msubrs_q,
 TRICORE_msubs,
 TRICORE_msubs_h,
 TRICORE_msubs_q,
 TRICORE_msubs_u,
 TRICORE_mtcr,
 TRICORE_mul_f,
 TRICORE_mul_h,
 TRICORE_mul_q,
 TRICORE_mul_u,
 TRICORE_mul16,
 TRICORE_mul32,
 TRICORE_mulm_h,
 TRICORE_mulms_h,
 TRICORE_mulr_h,
 TRICORE_mulr_q,
 TRICORE_muls,
 TRICORE_muls_u,
 TRICORE_nand,
 TRICORE_nand_t,
 TRICORE_ne,
 TRICORE_ne_a,
 TRICORE_nez_a,
 TRICORE_nop16,
 TRICORE_nop32,
 TRICORE_nor_t,
 TRICORE_nor16,
 TRICORE_nor32,
 TRICORE_or_and_t,
 TRICORE_or_andn_t,
 TRICORE_or_eq,
 TRICORE_or_ge,
 TRICORE_or_ge_u,
 TRICORE_or_lt,
 TRICORE_or_lt_u,
 TRICORE_or_ne,
 TRICORE_or_nor_t,
 TRICORE_or_or_t,
 TRICORE_or_t,
 TRICORE_or16,
 TRICORE_or32,
 TRICORE_orn,
 TRICORE_orn_t,
 TRICORE_pack,
 TRICORE_parity,
 TRICORE_q31tof,
 TRICORE_qseed_f,
 TRICORE_ret16,
 TRICORE_ret32,
 TRICORE_rfe16,
 TRICORE_rfe32,
 TRICORE_rfm,
 TRICORE_rslcx,
 TRICORE_rstv,
 TRICORE_rsub16,
 TRICORE_rsub32,
 TRICORE_rsubs,
 TRICORE_rsubs_u,
 TRICORE_sat16_b,
 TRICORE_sat16_bu,
 TRICORE_sat16_h,
 TRICORE_sat16_hu,
 TRICORE_sat32_b,
 TRICORE_sat32_bu,
 TRICORE_sat32_h,
 TRICORE_sat32_hu,
 TRICORE_sel,
 TRICORE_seln,
 TRICORE_sh_and_t,
 TRICORE_sh_andn_t,
 TRICORE_sh_eq,
 TRICORE_sh_ge,
 TRICORE_sh_ge_u,
 TRICORE_sh_h,
 TRICORE_sh_lt,
 TRICORE_sh_lt_u,
 TRICORE_sh_nand_t,
 TRICORE_sh_ne,
 TRICORE_sh_nor_t,
 TRICORE_sh_or_t,
 TRICORE_sh_orn_t,
 TRICORE_sh_xnor_t,
 TRICORE_sh_xor_t,
 TRICORE_sh16,
 TRICORE_sh32,
 TRICORE_sha_h,
 TRICORE_sha16,
 TRICORE_sha32,
 TRICORE_shas,
 TRICORE_st_d,
 TRICORE_st_da,
 TRICORE_st_q,
 TRICORE_st_t,
 TRICORE_st16_a,
 TRICORE_st16_b,
 TRICORE_st16_h,
 TRICORE_st16_w,
 TRICORE_st32_a,
 TRICORE_st32_b,
 TRICORE_st32_h,
 TRICORE_st32_w,
 TRICORE_stlcx,
 TRICORE_stucx,
 TRICORE_sub_b,
 TRICORE_sub_f,
 TRICORE_sub_h,
 TRICORE_sub16,
 TRICORE_sub16_a,
 TRICORE_sub32,
 TRICORE_sub32_a,
 TRICORE_subc,
 TRICORE_subs_h,
 TRICORE_subs_hu,
 TRICORE_subs_u,
 TRICORE_subs16,
 TRICORE_subs32,
 TRICORE_subx,
 TRICORE_svlcx,
 TRICORE_swap_w,
 TRICORE_syscall,
 TRICORE_tlbdemap,
 TRICORE_tlbflush_a,
 TRICORE_tlbflush_b,
 TRICORE_tlbmap,
 TRICORE_tlbprobe_a,
 TRICORE_tlbprobe_i,
 TRICORE_trapsv,
 TRICORE_trapv,
 TRICORE_unpack,
 TRICORE_updfl,
 TRICORE_utof,
 TRICORE_xnor,
 TRICORE_xnor_t,
 TRICORE_xor_eq,
 TRICORE_xor_ge,
 TRICORE_xor_ge_u,
 TRICORE_xor_lt,
 TRICORE_xor_lt_u,
 TRICORE_xor_ne,
 TRICORE_xor_t,
 TRICORE_xor16,
 TRICORE_xor32,

 // new v1.6 instructions
 TRICORE_cachei_i,
 TRICORE_cachei_w,
 TRICORE_cachei_wi,
 TRICORE_div,
 TRICORE_div_u,
 TRICORE_fcall,
 TRICORE_fcalla,
 TRICORE_fcalli,
 TRICORE_fret16,
 TRICORE_fret32,
 TRICORE_ftoiz,
 TRICORE_ftoq31z,
 TRICORE_ftouz,
 TRICORE_restore,
 TRICORE_last
};


/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2008 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

  ARC_null = 0,                 // Unknown Operation

  ARC_ld,                       // Load
  ARC_lr,                       // Load from auxiliary register
  ARC_st,                       // Store
  ARC_sr,                       // Store to auxiliary register
  ARC_store_instructions = ARC_sr,
  ARC_flag,                     // Set flags
  ARC_asr,                      // Arithmetic shift right
  ARC_lsr,                      // Logical shift right
  ARC_sexb,                     // Sign extend byte
  ARC_sexw,                     // Sign extend word
  ARC_extb,                     // Zero extend byte
  ARC_extw,                     // Zero extend word
  ARC_ror,                      // Rotate right
  ARC_rrc,                      // Rotate right through carry
  ARC_b,                        // Branch
  ARC_bl,                       // Branch and link
  ARC_lp,                       // Zero-overhead loop setup
  ARC_j,                        // Jump
  ARC_jl,                       // Jump and link
  ARC_add,                      // Add
  ARC_adc,                      // Add with carry
  ARC_sub,                      // Subtract
  ARC_sbc,                      // Subtract with carry
  ARC_and,                      // Logical bitwise AND
  ARC_or,                       // Logical bitwise OR
  ARC_bic,                      // Logical bitwise AND with invert
  ARC_xor,                      // Logical bitwise exclusive-OR

  // pseudo instructions
  ARC_mov,                      // Move
  ARC_nop,                      // No operation
  ARC_lsl,                      // Logical shift left
  ARC_rlc,                      // Rotate left through carry

  // arc7
  ARC_brk,                      // Breakpoint
  ARC_sleep,                    // Sleep until interrupt or restart

  // arc8
  ARC_swi,                      // Software interrupt

  // extra optional instrutions
  ARC_asl,                      // Arithmetic shift left
  ARC_mul64,                    // Signed 32x32 multiply
  ARC_mulu64,                   // Unsigned 32x32 multiply
  ARC_max,                      // Maximum of two signed integers
  ARC_min,                      // Minimum of two signed integers
  ARC_swap,                     // Exchange upper and lower 16 bits
  ARC_norm,                     // Normalize (find-first-bit)

  // ARCompact instructions
  ARC_bbit0,                    // Branch if bit cleared to 0
  ARC_bbit1,                    // Branch if bit set to 1
  ARC_br,                       // Branch on compare
  ARC_pop,                      // Restore register value from stack
  ARC_push,                     // Store register value on stack

  ARC_abs,                      // Absolute value
  ARC_add1,                     // Add with left shift by 1 bit
  ARC_add2,                     // Add with left shift by 2 bits
  ARC_add3,                     // Add with left shift by 3 bits
  ARC_bclr,                     // Clear specified bit (to 0)
  ARC_bmsk,                     // Bit Mask
  ARC_bset,                     // Set specified bit (to 1)
  ARC_btst,                     // Test value of specified bit
  ARC_bxor,                     // Bit XOR
  ARC_cmp,                      // Compare
  ARC_ex,                       // Atomic Exchange
  ARC_mpy,                      // Signed 32x32 multiply (low)
  ARC_mpyh,                     // Signed 32x32 multiply (high)
  ARC_mpyhu,                    // Unsigned 32x32 multiply (high)
  ARC_mpyu,                     // Unsigned 32x32 multiply (low)
  ARC_neg,                      // Negate
  ARC_not,                      // Logical bit inversion
  ARC_rcmp,                     // Reverse Compare
  ARC_rsub,                     // Reverse Subtraction
  ARC_rtie,                     // Return from Interrupt/Exception
  ARC_sub1,                     // Subtract with left shift by 1 bit
  ARC_sub2,                     // Subtract with left shift by 2 bits
  ARC_sub3,                     // Subtract with left shift by 3 bits
  ARC_sync,                     // Synchronize
  ARC_trap,                     // Raise an exception
  ARC_tst,                      // Test
  ARC_unimp,                    // Unimplemented instruction

  ARC_abss,                     // Absolute and saturate
  ARC_abssw,                    // Absolute and saturate of word
  ARC_adds,                     // Add and saturate
  ARC_addsdw,                   // Add and saturate dual word
  ARC_asls,                     // Arithmetic shift left and saturate
  ARC_asrs,                     // Arithmetic shift right and saturate
  ARC_divaw,                    // Division assist
  ARC_negs,                     // Negate and saturate
  ARC_negsw,                    // Negate and saturate of word
  ARC_normw,                    // Normalize to 16 bits
  ARC_rnd16,                    // Round to word
  ARC_sat16,                    // Saturate to word
  ARC_subs,                     // Subtract and saturate
  ARC_subsdw,                   // Subtract and saturate dual word

  // mac d16
  ARC_muldw, 
  ARC_muludw,
  ARC_mulrdw,
  ARC_macdw, 
  ARC_macudw,
  ARC_macrdw,
  ARC_msubdw,

  // 32x16 MUL/MAC
  ARC_mululw,
  ARC_mullw,
  ARC_mulflw,
  ARC_maclw,
  ARC_macflw,
  ARC_machulw,
  ARC_machlw,
  ARC_machflw,
  ARC_mulhlw,
  ARC_mulhflw,

  ARC_last,

};

/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2013 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


extern instruc_t Instructions[];

enum 
{

TMS28_null = 0,      // Unknown Operation

TMS28_aborti,
TMS28_abs,
TMS28_abstc,
TMS28_add,
TMS28_addb,
TMS28_addcl,
TMS28_addcu,
TMS28_addl,
TMS28_addu,
TMS28_addul,
TMS28_adrk,
TMS28_and,
TMS28_andb,
TMS28_asp,
TMS28_asr,
TMS28_asr64,
TMS28_asrl,
TMS28_b,
TMS28_banz,
TMS28_bar,
TMS28_bf,
TMS28_c27map,
TMS28_c27obj,
TMS28_c28addr,
TMS28_c28map,
TMS28_c28obj,
TMS28_clrc,
TMS28_cmp,
TMS28_cmp64,
TMS28_cmpb,
TMS28_cmpl,
TMS28_cmpr,
TMS28_csb,
TMS28_dec,
TMS28_dint,
TMS28_dmac,
TMS28_dmov,
TMS28_eallow,
TMS28_edis,
TMS28_eint,
TMS28_estop0,
TMS28_estop1,
TMS28_ffc,
TMS28_flip,
TMS28_iack,
TMS28_idle,
TMS28_imacl,
TMS28_impyal,
TMS28_impyl,
TMS28_impysl,
TMS28_impyxul,
TMS28_in,
TMS28_inc,
TMS28_intr,
TMS28_iret,
TMS28_lb,
TMS28_lc,
TMS28_lcr,
TMS28_loopnz,
TMS28_loopz,
TMS28_lpaddr,
TMS28_lret,
TMS28_lrete,
TMS28_lretr,
TMS28_lsl,
TMS28_lsl64,
TMS28_lsll,
TMS28_lsr,
TMS28_lsr64,
TMS28_lsrl,
TMS28_mac,
TMS28_max,
TMS28_maxcul,
TMS28_maxl,
TMS28_min,
TMS28_mincul,
TMS28_minl,
TMS28_mov,
TMS28_mova,
TMS28_movad,
TMS28_movb,
TMS28_movdl,
TMS28_movh,
TMS28_movl,
TMS28_movp,
TMS28_movs,
TMS28_movu,
TMS28_movw,
TMS28_movx,
TMS28_movz,
TMS28_mpy,
TMS28_mpya,
TMS28_mpyb,
TMS28_mpys,
TMS28_mpyu,
TMS28_mpyxu,
TMS28_nasp,
TMS28_neg,
TMS28_neg64,
TMS28_negtc,
TMS28_nop,
TMS28_norm,
TMS28_not,
TMS28_or,
TMS28_orb,
TMS28_out,
TMS28_pop,
TMS28_pread,
TMS28_push,
TMS28_pwrite,
TMS28_qmacl,
TMS28_qmpyal,
TMS28_qmpyl,
TMS28_qmpysl,
TMS28_qmpyul,
TMS28_qmpyxul,
TMS28_rol,
TMS28_ror,
TMS28_rpt,
TMS28_sat,
TMS28_sat64,
TMS28_sb,
TMS28_sbbu,
TMS28_sbf,
TMS28_sbrk,
TMS28_setc,
TMS28_sfr,
TMS28_spm,
TMS28_sqra,
TMS28_sqrs,
TMS28_sub,
TMS28_subb,
TMS28_subbl,
TMS28_subcu,
TMS28_subcul,
TMS28_subl,
TMS28_subr,
TMS28_subrl,
TMS28_subu,
TMS28_subul,
TMS28_sxtb,
TMS28_tbit,
TMS28_tclr,
TMS28_test,
TMS28_trap,
TMS28_tset,
TMS28_uout,
TMS28_xb,
TMS28_xbanz,
TMS28_xcall,
TMS28_xmac,
TMS28_xmacd,
TMS28_xor,
TMS28_xorb,
TMS28_xpread,
TMS28_xpwrite,
TMS28_xret,
TMS28_xretc,
TMS28_zalr,
TMS28_zap,
TMS28_zapa,

TMS28_last,

};


extern instruc_t Instructions[];

enum 
{
UNSP_null = 0,   // Unknown Operation

//ALU
UNSP_add,        // Add
UNSP_adc,        // Add with Carry
UNSP_sub,        // Subtract
UNSP_sbc,        // Subtract with Carry
UNSP_cmp,        // Compare
UNSP_cmpc,       // Compare with Carry?
UNSP_neg,        // Negate
UNSP_negc,       // Negate with Carry?
UNSP_xor,        // Exclusive-OR
UNSP_load,       // Load
UNSP_or,         // OR
UNSP_and,        // AND
UNSP_test,       // Test
UNSP_store,      // Store

UNSP_add_s,      // Add (Simple)
UNSP_adc_s,      // Add with Carry (Simple)
UNSP_sub_s,      // Subtract (Simple)
UNSP_sbc_s,      // Subtract with Carry (Simple)
UNSP_cmp_s,      // Compare (Simple)
UNSP_cmpc_s,     // Compare with Carry? (Simple)
UNSP_neg_s,      // Negate (Simple)
UNSP_negc_s,     // Negate with Carry? (Simple)
UNSP_xor_s,      // Exclusive-OR (Simple)
UNSP_load_s,     // Load (Simple)
UNSP_or_s,       // OR (Simple)
UNSP_and_s,      // AND (Simple)
UNSP_test_s,     // Test (Simple)
UNSP_store_s,    // Store (Simple)


//Misc
UNSP_retf,       // Return Function
UNSP_reti,       // Return Interrupt
UNSP_pop,        // Pop from Stack
UNSP_push,       // Push to Stack
UNSP_call,       // Call
UNSP_goto,       // Goto (far jump)
UNSP_nop,        // NOP
UNSP_exp,        // Effective Exponent

//Jump
UNSP_jb,         // Jump if Below (unsigned) [jcc, jnae]
UNSP_jae,        // Jump if Above or Equal (unsigned) [jcs, jnb]
UNSP_jge,        // Jump if Greater or Equal (signed) [jsc, jnl]
UNSP_jl,         // Jump if Less (signed) [jss, jnge]
UNSP_jne,        // Jump if Not Equal [jnz]
UNSP_je,         // Jump if Equal [jz]
UNSP_jpl,        // Jump if Positive
UNSP_jmi,        // Jump if Negative
UNSP_jbe,        // Jump if Below or Equal (unsigned) [jna]
UNSP_ja,         // Jump if Above (unsigned) [jnbe]
UNSP_jle,        // Jump if Less or Equal (signed) [jng]
UNSP_jg,         // Jump if Greater (signed) [jnle]
UNSP_jvc,        // Jump if Not Overflow
UNSP_jvs,        // Jump if Overflow
UNSP_jmp,        // Jump

//Multiplication/Division
UNSP_mulss,      // Multiply Signed * Signed
UNSP_mulus,      // Multiply Unsigned * Signed
UNSP_muluu,      // Multiply Unsigned * Unsigned
UNSP_divs,       // Divide (Sign)
UNSP_divq,       // Divide (Quotient)

//Interrupt
UNSP_int1,       // Interrupt flags
UNSP_int2,       // Interrupt flags
UNSP_fir_mov,    // FIR_MOV setting
UNSP_fraction,   // FRACTION setting
UNSP_irq,        // IRQ setting
UNSP_secbank,    // SECBANK setting
UNSP_fiq,        // FIQ setting
UNSP_irqnest,    // IRQNEST setting
UNSP_break,      // BREAK

//Shift
UNSP_asr,        // Arithmetic shift right
UNSP_asror,      // Arithmetic shift right OR
UNSP_lsl,        // Logical shift left
UNSP_lslor,      // Logical shift left OR
UNSP_lsr,        // Logical shift right
UNSP_lsror,      // Logical shift right OR
UNSP_rol,        // Rotate left
UNSP_ror,        // Rotate right

//Bitops
UNSP_tstb,       // Test bit
UNSP_setb,       // Set bit
UNSP_clrb,       // Clear bit
UNSP_invb,       // Invert bit

UNSP_last,
    };

/*
 * Dalvik opcode enumeration.
 */

extern instruc_t Instructions[];

/*
 * Dalvik opcode list.
 */

enum 
{
  DALVIK_NOP                          = 0x00,

  DALVIK_MOVE                         = 0x01,
  DALVIK_MOVE_FROM16                  = 0x02,
  DALVIK_MOVE_16                      = 0x03,
  DALVIK_MOVE_WIDE                    = 0x04,
  DALVIK_MOVE_WIDE_FROM16             = 0x05,
  DALVIK_MOVE_WIDE_16                 = 0x06,
  DALVIK_MOVE_OBJECT                  = 0x07,
  DALVIK_MOVE_OBJECT_FROM16           = 0x08,
  DALVIK_MOVE_OBJECT_16               = 0x09,

  DALVIK_MOVE_RESULT                  = 0x0a,
  DALVIK_MOVE_RESULT_WIDE             = 0x0b,
  DALVIK_MOVE_RESULT_OBJECT           = 0x0c,
  DALVIK_MOVE_EXCEPTION               = 0x0d,

  DALVIK_RETURN_VOID                  = 0x0e,
  DALVIK_RETURN                       = 0x0f,
  DALVIK_RETURN_WIDE                  = 0x10,
  DALVIK_RETURN_OBJECT                = 0x11,

  DALVIK_CONST_4                      = 0x12,
  DALVIK_CONST_16                     = 0x13,
  DALVIK_CONST                        = 0x14,
  DALVIK_CONST_HIGH16                 = 0x15,
  DALVIK_CONST_WIDE_16                = 0x16,
  DALVIK_CONST_WIDE_32                = 0x17,
  DALVIK_CONST_WIDE                   = 0x18,
  DALVIK_CONST_WIDE_HIGH16            = 0x19,
  DALVIK_CONST_STRING                 = 0x1a,
  DALVIK_CONST_STRING_JUMBO           = 0x1b,
  DALVIK_CONST_CLASS                  = 0x1c,

  DALVIK_MONITOR_ENTER                = 0x1d,
  DALVIK_MONITOR_EXIT                 = 0x1e,

  DALVIK_CHECK_CAST                   = 0x1f,
  DALVIK_INSTANCE_OF                  = 0x20,

  DALVIK_ARRAY_LENGTH                 = 0x21,

  DALVIK_NEW_INSTANCE                 = 0x22,
  DALVIK_NEW_ARRAY                    = 0x23,

  DALVIK_FILLED_NEW_ARRAY             = 0x24,
  DALVIK_FILLED_NEW_ARRAY_RANGE       = 0x25,
  DALVIK_FILL_ARRAY_DATA              = 0x26,

  DALVIK_THROW                        = 0x27,
  DALVIK_GOTO                         = 0x28,
  DALVIK_GOTO_16                      = 0x29,
  DALVIK_GOTO_32                      = 0x2a,
  DALVIK_PACKED_SWITCH                = 0x2b,
  DALVIK_SPARSE_SWITCH                = 0x2c,

  DALVIK_CMPL_FLOAT                   = 0x2d,
  DALVIK_CMPG_FLOAT                   = 0x2e,
  DALVIK_CMPL_DOUBLE                  = 0x2f,
  DALVIK_CMPG_DOUBLE                  = 0x30,
  DALVIK_CMP_LONG                     = 0x31,

  DALVIK_IF_EQ                        = 0x32,
  DALVIK_IF_NE                        = 0x33,
  DALVIK_IF_LT                        = 0x34,
  DALVIK_IF_GE                        = 0x35,
  DALVIK_IF_GT                        = 0x36,
  DALVIK_IF_LE                        = 0x37,
  DALVIK_IF_EQZ                       = 0x38,
  DALVIK_IF_NEZ                       = 0x39,
  DALVIK_IF_LTZ                       = 0x3a,
  DALVIK_IF_GEZ                       = 0x3b,
  DALVIK_IF_GTZ                       = 0x3c,
  DALVIK_IF_LEZ                       = 0x3d,

  DALVIK_UNUSED_3E                    = 0x3e,
  DALVIK_UNUSED_3F                    = 0x3f,
  DALVIK_UNUSED_40                    = 0x40,
  DALVIK_UNUSED_41                    = 0x41,
  DALVIK_UNUSED_42                    = 0x42,
  DALVIK_UNUSED_43                    = 0x43,

  DALVIK_AGET                         = 0x44,
  DALVIK_AGET_WIDE                    = 0x45,
  DALVIK_AGET_OBJECT                  = 0x46,
  DALVIK_AGET_BOOLEAN                 = 0x47,
  DALVIK_AGET_BYTE                    = 0x48,
  DALVIK_AGET_CHAR                    = 0x49,
  DALVIK_AGET_SHORT                   = 0x4a,
  DALVIK_APUT                         = 0x4b,
  DALVIK_APUT_WIDE                    = 0x4c,
  DALVIK_APUT_OBJECT                  = 0x4d,
  DALVIK_APUT_BOOLEAN                 = 0x4e,
  DALVIK_APUT_BYTE                    = 0x4f,
  DALVIK_APUT_CHAR                    = 0x50,
  DALVIK_APUT_SHORT                   = 0x51,

  DALVIK_IGET                         = 0x52,
  DALVIK_IGET_WIDE                    = 0x53,
  DALVIK_IGET_OBJECT                  = 0x54,
  DALVIK_IGET_BOOLEAN                 = 0x55,
  DALVIK_IGET_BYTE                    = 0x56,
  DALVIK_IGET_CHAR                    = 0x57,
  DALVIK_IGET_SHORT                   = 0x58,
  DALVIK_IPUT                         = 0x59,
  DALVIK_IPUT_WIDE                    = 0x5a,
  DALVIK_IPUT_OBJECT                  = 0x5b,
  DALVIK_IPUT_BOOLEAN                 = 0x5c,
  DALVIK_IPUT_BYTE                    = 0x5d,
  DALVIK_IPUT_CHAR                    = 0x5e,
  DALVIK_IPUT_SHORT                   = 0x5f,

  DALVIK_SGET                         = 0x60,
  DALVIK_SGET_WIDE                    = 0x61,
  DALVIK_SGET_OBJECT                  = 0x62,
  DALVIK_SGET_BOOLEAN                 = 0x63,
  DALVIK_SGET_BYTE                    = 0x64,
  DALVIK_SGET_CHAR                    = 0x65,
  DALVIK_SGET_SHORT                   = 0x66,
  DALVIK_SPUT                         = 0x67,
  DALVIK_SPUT_WIDE                    = 0x68,
  DALVIK_SPUT_OBJECT                  = 0x69,
  DALVIK_SPUT_BOOLEAN                 = 0x6a,
  DALVIK_SPUT_BYTE                    = 0x6b,
  DALVIK_SPUT_CHAR                    = 0x6c,
  DALVIK_SPUT_SHORT                   = 0x6d,

  DALVIK_INVOKE_VIRTUAL               = 0x6e,
  DALVIK_INVOKE_SUPER                 = 0x6f,
  DALVIK_INVOKE_DIRECT                = 0x70,
  DALVIK_INVOKE_STATIC                = 0x71,
  DALVIK_INVOKE_INTERFACE             = 0x72,

  DALVIK_UNUSED_73                    = 0x73,

  DALVIK_INVOKE_VIRTUAL_RANGE         = 0x74,
  DALVIK_INVOKE_SUPER_RANGE           = 0x75,
  DALVIK_INVOKE_DIRECT_RANGE          = 0x76,
  DALVIK_INVOKE_STATIC_RANGE          = 0x77,
  DALVIK_INVOKE_INTERFACE_RANGE       = 0x78,

  DALVIK_UNUSED_79                    = 0x79,
  DALVIK_UNUSED_7A                    = 0x7a,

  DALVIK_NEG_INT                      = 0x7b,
  DALVIK_NOT_INT                      = 0x7c,
  DALVIK_NEG_LONG                     = 0x7d,
  DALVIK_NOT_LONG                     = 0x7e,
  DALVIK_NEG_FLOAT                    = 0x7f,
  DALVIK_NEG_DOUBLE                   = 0x80,
  DALVIK_INT_TO_LONG                  = 0x81,
  DALVIK_INT_TO_FLOAT                 = 0x82,
  DALVIK_INT_TO_DOUBLE                = 0x83,
  DALVIK_LONG_TO_INT                  = 0x84,
  DALVIK_LONG_TO_FLOAT                = 0x85,
  DALVIK_LONG_TO_DOUBLE               = 0x86,
  DALVIK_FLOAT_TO_INT                 = 0x87,
  DALVIK_FLOAT_TO_LONG                = 0x88,
  DALVIK_FLOAT_TO_DOUBLE              = 0x89,
  DALVIK_DOUBLE_TO_INT                = 0x8a,
  DALVIK_DOUBLE_TO_LONG               = 0x8b,
  DALVIK_DOUBLE_TO_FLOAT              = 0x8c,
  DALVIK_INT_TO_BYTE                  = 0x8d,
  DALVIK_INT_TO_CHAR                  = 0x8e,
  DALVIK_INT_TO_SHORT                 = 0x8f,

  DALVIK_ADD_INT                      = 0x90,
  DALVIK_SUB_INT                      = 0x91,
  DALVIK_MUL_INT                      = 0x92,
  DALVIK_DIV_INT                      = 0x93,
  DALVIK_REM_INT                      = 0x94,
  DALVIK_AND_INT                      = 0x95,
  DALVIK_OR_INT                       = 0x96,
  DALVIK_XOR_INT                      = 0x97,
  DALVIK_SHL_INT                      = 0x98,
  DALVIK_SHR_INT                      = 0x99,
  DALVIK_USHR_INT                     = 0x9a,

  DALVIK_ADD_LONG                     = 0x9b,
  DALVIK_SUB_LONG                     = 0x9c,
  DALVIK_MUL_LONG                     = 0x9d,
  DALVIK_DIV_LONG                     = 0x9e,
  DALVIK_REM_LONG                     = 0x9f,
  DALVIK_AND_LONG                     = 0xa0,
  DALVIK_OR_LONG                      = 0xa1,
  DALVIK_XOR_LONG                     = 0xa2,
  DALVIK_SHL_LONG                     = 0xa3,
  DALVIK_SHR_LONG                     = 0xa4,
  DALVIK_USHR_LONG                    = 0xa5,

  DALVIK_ADD_FLOAT                    = 0xa6,
  DALVIK_SUB_FLOAT                    = 0xa7,
  DALVIK_MUL_FLOAT                    = 0xa8,
  DALVIK_DIV_FLOAT                    = 0xa9,
  DALVIK_REM_FLOAT                    = 0xaa,
  DALVIK_ADD_DOUBLE                   = 0xab,
  DALVIK_SUB_DOUBLE                   = 0xac,
  DALVIK_MUL_DOUBLE                   = 0xad,
  DALVIK_DIV_DOUBLE                   = 0xae,
  DALVIK_REM_DOUBLE                   = 0xaf,

  DALVIK_ADD_INT_2ADDR                = 0xb0,
  DALVIK_SUB_INT_2ADDR                = 0xb1,
  DALVIK_MUL_INT_2ADDR                = 0xb2,
  DALVIK_DIV_INT_2ADDR                = 0xb3,
  DALVIK_REM_INT_2ADDR                = 0xb4,
  DALVIK_AND_INT_2ADDR                = 0xb5,
  DALVIK_OR_INT_2ADDR                 = 0xb6,
  DALVIK_XOR_INT_2ADDR                = 0xb7,
  DALVIK_SHL_INT_2ADDR                = 0xb8,
  DALVIK_SHR_INT_2ADDR                = 0xb9,
  DALVIK_USHR_INT_2ADDR               = 0xba,

  DALVIK_ADD_LONG_2ADDR               = 0xbb,
  DALVIK_SUB_LONG_2ADDR               = 0xbc,
  DALVIK_MUL_LONG_2ADDR               = 0xbd,
  DALVIK_DIV_LONG_2ADDR               = 0xbe,
  DALVIK_REM_LONG_2ADDR               = 0xbf,
  DALVIK_AND_LONG_2ADDR               = 0xc0,
  DALVIK_OR_LONG_2ADDR                = 0xc1,
  DALVIK_XOR_LONG_2ADDR               = 0xc2,
  DALVIK_SHL_LONG_2ADDR               = 0xc3,
  DALVIK_SHR_LONG_2ADDR               = 0xc4,
  DALVIK_USHR_LONG_2ADDR              = 0xc5,

  DALVIK_ADD_FLOAT_2ADDR              = 0xc6,
  DALVIK_SUB_FLOAT_2ADDR              = 0xc7,
  DALVIK_MUL_FLOAT_2ADDR              = 0xc8,
  DALVIK_DIV_FLOAT_2ADDR              = 0xc9,
  DALVIK_REM_FLOAT_2ADDR              = 0xca,
  DALVIK_ADD_DOUBLE_2ADDR             = 0xcb,
  DALVIK_SUB_DOUBLE_2ADDR             = 0xcc,
  DALVIK_MUL_DOUBLE_2ADDR             = 0xcd,
  DALVIK_DIV_DOUBLE_2ADDR             = 0xce,
  DALVIK_REM_DOUBLE_2ADDR             = 0xcf,

  DALVIK_ADD_INT_LIT16                = 0xd0,
  DALVIK_RSUB_INT                     = 0xd1, /* no _LIT16 suffix for this */
  DALVIK_MUL_INT_LIT16                = 0xd2,
  DALVIK_DIV_INT_LIT16                = 0xd3,
  DALVIK_REM_INT_LIT16                = 0xd4,
  DALVIK_AND_INT_LIT16                = 0xd5,
  DALVIK_OR_INT_LIT16                 = 0xd6,
  DALVIK_XOR_INT_LIT16                = 0xd7,

  DALVIK_ADD_INT_LIT8                 = 0xd8,
  DALVIK_RSUB_INT_LIT8                = 0xd9,
  DALVIK_MUL_INT_LIT8                 = 0xda,
  DALVIK_DIV_INT_LIT8                 = 0xdb,
  DALVIK_REM_INT_LIT8                 = 0xdc,
  DALVIK_AND_INT_LIT8                 = 0xdd,
  DALVIK_OR_INT_LIT8                  = 0xde,
  DALVIK_XOR_INT_LIT8                 = 0xdf,
  DALVIK_SHL_INT_LIT8                 = 0xe0,
  DALVIK_SHR_INT_LIT8                 = 0xe1,
  DALVIK_USHR_INT_LIT8                = 0xe2,

  /* verifier/optimizer output -- nothing below here is generated by "dx" */

  DALVIK_IGET_VOLATILE                = 0xe3,
  DALVIK_IPUT_VOLATILE                = 0xe4,
  DALVIK_SGET_VOLATILE                = 0xe5,
  DALVIK_SPUT_VOLATILE                = 0xe6,
  DALVIK_IGET_OBJECT_VOLATILE         = 0xe7,

  DALVIK_IGET_WIDE_VOLATILE           = 0xe8,
  DALVIK_IPUT_WIDE_VOLATILE           = 0xe9,
  DALVIK_SGET_WIDE_VOLATILE           = 0xea,
  DALVIK_SPUT_WIDE_VOLATILE           = 0xeb,

  /*
   * The "breakpoint" instruction is special, in that it should never
   * be seen by anything but the debug interpreter.  During debugging
   * it takes the place of an arbitrary opcode, which means operations
   * like "tell me the opcode width so I can find the next instruction"
   * aren't possible.  (This is correctable, but probably not useful.)
   */
  DALVIK_BREAKPOINT                   = 0xec,

  DALVIK_THROW_VERIFICATION_ERROR     = 0xed,

  DALVIK_EXECUTE_INLINE               = 0xee,
  DALVIK_EXECUTE_INLINE_RANGE         = 0xef,

  DALVIK_INVOKE_DIRECT_EMPTY          = 0xf0,
  DALVIK_RETURN_VOID_BARRIER          = 0xf1,
  DALVIK_IGET_QUICK                   = 0xf2,
  DALVIK_IGET_WIDE_QUICK              = 0xf3,
  DALVIK_IGET_OBJECT_QUICK            = 0xf4,
  DALVIK_IPUT_QUICK                   = 0xf5,
  DALVIK_IPUT_WIDE_QUICK              = 0xf6,
  DALVIK_IPUT_OBJECT_QUICK            = 0xf7,

  DALVIK_INVOKE_VIRTUAL_QUICK         = 0xf8,
  DALVIK_INVOKE_VIRTUAL_QUICK_RANGE   = 0xf9,
  DALVIK_INVOKE_SUPER_QUICK           = 0xfa,
  DALVIK_INVOKE_SUPER_QUICK_RANGE     = 0xfb,
  DALVIK_IPUT_OBJECT_VOLATILE         = 0xfc,
  DALVIK_SGET_OBJECT_VOLATILE         = 0xfd,
  DALVIK_SPUT_OBJECT_VOLATILE         = 0xfe,
  DALVIK_UNUSED_FF                    = 0xff, /* reserved for code expansion */
  DALVIK_LAST,
};


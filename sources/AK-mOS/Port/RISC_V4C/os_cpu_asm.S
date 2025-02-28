

.global os_cpu_start_first_task
.extern tcb_curr_ptr
.extern tcb_high_rdy_ptr
.extern irq_stack_ptr

.text
.align 2
.type os_cpu_start_first_task, %function
os_cpu_start_first_task:

      lw t0, tcb_curr_ptr        # Load current task control block pointer
      lw sp, 0x00(t0)            # Load stack pointer from TCB

      j       restore_context


.align 2
.type restore_context, %function
restore_context:

      lw t0, 0x00(sp)            # t0 = sp
      csrw mepc, t0              # pc = *sp
      lw t0, 0x04(sp)            # t0 = sp
      csrw mstatus, t0              # mstatus = *sp

      # Load registers from stack
      lw x1, 0x08(sp)            # Load x1 from offset 0x08
      lw x3, 0x0C(sp)            # Load x3 from offset 0x0C
      lw x4, 0x10(sp)            # Load x4 from offset 0x10
      lw x5, 0x14(sp)            # Load x5 from offset 0x14
      lw x6, 0x18(sp)            # Load x6 from offset 0x18
      lw x7, 0x1C(sp)            # Load x7 from offset 0x1C
      lw x8, 0x20(sp)            # Load x8 from offset 0x20
      lw x9, 0x24(sp)            # Load x9 from offset 0x24
      lw x10, 0x28(sp)           # Load x10 from offset 0x28
      lw x11, 0x2C(sp)           # Load x11 from offset 0x2C
      lw x12, 0x30(sp)           # Load x12 from offset 0x30
      lw x13, 0x34(sp)           # Load x13 from offset 0x34
      lw x14, 0x38(sp)           # Load x14 from offset 0x38
      lw x15, 0x3C(sp)           # Load x15 from offset 0x3C
      lw x16, 0x40(sp)           # Load x16 from offset 0x40
      lw x17, 0x44(sp)           # Load x17 from offset 0x44
      lw x18, 0x48(sp)           # Load x18 from offset 0x48
      lw x19, 0x4C(sp)           # Load x19 from offset 0x4C
      lw x20, 0x50(sp)           # Load x20 from offset 0x50
      lw x21, 0x54(sp)           # Load x21 from offset 0x54
      lw x22, 0x58(sp)           # Load x22 from offset 0x58
      lw x23, 0x5C(sp)           # Load x23 from offset 0x5C
      lw x24, 0x60(sp)           # Load x24 from offset 0x60
      lw x25, 0x64(sp)           # Load x25 from offset 0x64
      lw x26, 0x68(sp)           # Load x26 from offset 0x68
      lw x27, 0x6C(sp)           # Load x27 from offset 0x6C
      lw x28, 0x70(sp)           # Load x28 from offset 0x70
      lw x29, 0x74(sp)           # Load x29 from offset 0x74
      lw x30, 0x78(sp)           # Load x30 from offset 0x78
      lw x31, 0x7C(sp)           # Load x31 from offset 0x7C

      add sp, sp, 128            # Adjust stack pointer

      mret                       # Return from machine mode


/* just switch at Software interrupt */
.align 2
.global SW_Handler
SW_Handler:
      # li t0, 0x1800              # Load immediate value into t1
      # csrw mstatus, t0           # Disable interrupts

    addi sp, sp, -128            # Allocate space on the stack (128 bytes)

    csrr t0, mstatus              # Read mstatus into t0
    sw   t0, 0x04(sp)             # Save mstatus to the stack at offset for mstatus (0x04)

    csrr t0, mepc                 # Read mepc into t0
    sw   t0, 0x00(sp)             # Save mepc to the stack at offset for mepc (0x00)

    # Load registers from stack
    sw   x1, 0x08(sp)             # Save x1 to the stack at offset for x1 (0x08)
    sw   x3, 0x0C(sp)             # Save x3 to the stack at offset for x3 (0x0C)
    sw   x4, 0x10(sp)             # Save x4 to the stack at offset for x4 (0x10)
    sw   x5, 0x14(sp)             # Save x5 to the stack at offset for x5 (0x14)
    sw   x6, 0x18(sp)             # Save x6 to the stack at offset for x6 (0x18)
    sw   x7, 0x1C(sp)             # Save x7 to the stack at offset for x7 (0x1C)
    sw   x8, 0x20(sp)             # Save x8 to the stack at offset for x8 (0x20)
    sw   x9, 0x24(sp)             # Save x9 to the stack at offset for x9 (0x24)
    sw   x10, 0x28(sp)            # Save x10 to the stack at offset for x10 (0x28)
    sw   x11, 0x2C(sp)            # Save x11 to the stack at offset for x11 (0x2C)
    sw   x12, 0x30(sp)            # Save x12 to the stack at offset for x12 (0x30)
    sw   x13, 0x34(sp)            # Save x13 to the stack at offset for x13 (0x34)
    sw   x14, 0x38(sp)            # Save x14 to the stack at offset for x14 (0x38)
    sw   x15, 0x3C(sp)            # Save x15 to the stack at offset for x15 (0x3C)
    sw   x16, 0x40(sp)            # Save x16 to the stack at offset for x16 (0x40)
    sw   x17, 0x44(sp)            # Save x17 to the stack at offset for x17 (0x44)
    sw   x18, 0x48(sp)            # Save x18 to the stack at offset for x18 (0x48)
    sw   x19, 0x4C(sp)            # Save x19 to the stack at offset for x19 (0x4C)
    sw   x20, 0x50(sp)            # Save x20 to the stack at offset for x20 (0x50)
    sw   x21, 0x54(sp)            # Save x21 to the stack at offset for x21 (0x54)
    sw   x22, 0x58(sp)            # Save x22 to the stack at offset for x22 (0x58)
    sw   x23, 0x5C(sp)            # Save x23 to the stack at offset for x23 (0x5C)
    sw   x24, 0x60(sp)            # Save x24 to the stack at offset for x24 (0x60)
    sw   x25, 0x64(sp)            # Save x25 to the stack at offset for x25 (0x64)
    sw   x26, 0x68(sp)            # Save x26 to the stack at offset for x26 (0x68)
    sw   x27, 0x6C(sp)            # Save x27 to the stack at offset for x27 (0x6C)
    sw   x28, 0x70(sp)            # Save x28 to the stack at offset for x28 (0x70)
    sw   x29, 0x74(sp)            # Save x29 to the stack at offset for x29 (0x74)
    sw   x30, 0x78(sp)            # Save x30 to the stack at offset for x30 (0x78)
    sw   x31, 0x7C(sp)            # Save x31 to the stack at offset for x31 (0x7C)

#     # Switch to IRQ stack
#     mv t0, sp                     # Move current stack pointer to t0
#     csrrw sp, mscratch, sp          # Load IRQ stack top into sp
#     sw t0, 0(sp)                  # Save old stack pointer at the top of the new stack

#     # Clear software interrupt
#     call os_cpu_clear_sw_irq             # Call function to clear pending interrupts

#     # Resume old stack pointer
#     lw sp, 0(sp)                  # Restore old stack pointer

    csrrw sp,mscratch,sp // pop isr stack pointer
    jal os_cpu_clear_sw_irq
    csrrw sp,mscratch,sp // push isr stack pointer

    la t0, tcb_curr_ptr             # Load address of current task into t0
    la t1, tcb_high_rdy_ptr         # Load address of next task into t1

    # Save sp to tcb_curr_ptr.sp
    lw t2, (t0)                   # Load current task pointer into t2
    sw sp, (t2)                   # Save current stack pointer to current task

    # Switch task
    lw t1, (t1)                   # Load next task pointer into t1
    sw t1, (t0)                   # Set current task to next task

    # Load new task stack pointer
    lw sp, (t1)                   # Load new task stack pointer from next task

      j restore_context
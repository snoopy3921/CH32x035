Install the toolchain (GCC compiler, openocd):

SDK for WCH family:
+ http://mounriver.com/download

Setup wch toolchain, openocd:
+ https://nc-pin.com/index.php/2022/04/25/openocd-for-ch32v-series/
+ https://github.com/kprasadvnsi/riscv-openocd-wch

Notes when handling interrupt for WCH:
+ https://www.reddit.com/r/RISCV/comments/126262j/notes_on_wch_fast_interrupts/

WCH toolchain has special attribute to handle the interrupt, so make sure you have wch-toolchain installed and the funtion handler looks like this (otherwise it will not work).

According  to WCH words: 

Interrupt functions using the ``hardware stack`` need to be compiled using MRS or its provided toolchain
and the interrupt functions need to be declared with__attribute__((interrupt("WCH-Interrupt-fast"))). 

``NOTE: Hardware stack in WCH is not located in our normal ram, it is specific region to handle fast interrupt feature``

```cpp
#ifdef __cplusplus
extern "C"
{
#endif	
    __attribute__((interrupt("WCH-Interrupt-fast")))
    void SysTick_Handler(void)
	{
        GPIOB->OUTDR ^= (1 << 12);
        SysTick->SR = 0;
	}
#ifdef __cplusplus
}
#endif
```

Enter exception with machine mode:

https://forums.sifive.com/t/user-mode-stack-memory-after-machine-mode-call/4318
```
The switch between User mode and Machine mode does not touch memory at all.

What you do after that is completely up to you. RISC-V does not mandate anything, it’s up to whatever convention you or the OS you’re using wants to use.

There is a CSR “mscratch” which is available for Machine mode to store any data it wishes.

One typical thing to do is keep the Machine mode stack pointer there, and on entry to a trap handler use a CSR swap instruction to swap SP with mscratch.

It’s definitely best to do something like that if you’re running user code or less trustworthy code in user mode. But many embedded systems that trust the quality of the code being interrupted do just continue on the same stack.
```

Handle interrupt for this mcu:
https://github.com/dragonlock2/zephyrboards/blob/main/soc/wch/ch32/common/irq.c

References:

https://caxapa.ru/1300017.html?todo=full&hilite=attribute
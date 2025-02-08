Install the toolchain (GCC compiler, openocd):

Toolchain for gcc and openocd:
+ http://mounriver.com/download

Setup wch openocd:
+ https://nc-pin.com/index.php/2022/04/25/openocd-for-ch32v-series/
+ https://github.com/kprasadvnsi/riscv-openocd-wch

Notes when handling interrupt for WCH:
+ https://www.reddit.com/r/RISCV/comments/126262j/notes_on_wch_fast_interrupts/

WCH toolchain has special attribute to handle the interrupt, so make sure you have wch-toolchain installed and the funtion handler looks like this (otherwise it will not work):

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
	};
#ifdef __cplusplus
}
#endif
```
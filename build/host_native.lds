OUTPUT_ARCH( "riscv" )

MEMORY
{
  MYMEMORY (rwx) : ORIGIN = 0x21000, LENGTH = 128K
}

SECTIONS
{
    .text : 
    {
        PROVIDE (_start = .);
        *(.text) /* テキストセクション（コード） */
        *(.rodata*) /* 読み取り専用データ */
        __global_pointer$ = . + 0x800; /* グローバルポインタの設定 */
    } > MYMEMORY

    .data : 
    {
        *(.data) /* データセクション */
    } > MYMEMORY

    .bss : 
    {
        *(.bss) /* BSSセクション */
    } > MYMEMORY

    /* 初期化および終了処理用のセクション */
    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } > MYMEMORY

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP (*(.init_array ))
        KEEP (*(*entry))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > MYMEMORY

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
        KEEP (*(.fini_array ))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > MYMEMORY

    /*.myRegisterFunctionSegment 0x110000 : {KEEP(*(.myRegisterFunctionSection))}*/

    /*. = 0x120000;*/
    /*PROVIDE(main = .);*/
}

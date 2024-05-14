OUTPUT_ARCH( "riscv" )

SECTIONS
{
  .myWrapperSegment 0x00010000 : {KEEP(*(.myWrapperSection))}

  . = 0x000011000;
  PROVIDE(main = .);

  /*.myEntrySegment 0x00011000 : {KEEP(*(.myEntrySection))}*/
}

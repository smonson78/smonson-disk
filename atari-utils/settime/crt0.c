#include <stdint.h>
#include <libc.h>
#include <tos.h>

extern void *stack;
int main(int argc, char **argv);
void __do_global_ctors_aux();
void __do_global_dtors_aux();
void __cxa_finalize(void *f);

int __libc_start_main()
{
    char *args[] = { "" };
    __do_global_ctors_aux();
    int result = main(1, args);
    __do_global_dtors_aux();

    return result;
}

__attribute__ ((noreturn))
__attribute__ ((section(".entry")))
void _start()
{
    __asm__ __volatile__
    (
            "moveal    %%sp@(4),%%a0\n\t"      //; Pointer to BASEPAGE
            "lea       stack,%%sp\n\t"    //; Set stack pointer
            "move.l    #0x100,%%d0\n\t"      //; Length of basepage
            "add.l     0xc(%%a0),%%d0\n\t"  //   ; Length of the TEXT segment
            "add.l     0x14(%%a0),%%d0\n\t" //  ; Length of the DATA segment
            "add.l     0x1c(%%a0),%%d0\n\t" //   ; Length of the BSS segment
            "move.l    %%d0,-(%%sp)\n\t"    //  ; Return to the stack
            "move.l    %%a0,-(%%sp)\n\t"  //    ; Basepage address to stack
            "clr.w     -(%%sp)\n\t"       //  ; Fill parameter
            "move.w    #0x4a,-(%%sp)\n\t" // ; Mshrink
            "trap      #1\n\t"            //; Call GEMDOS
            "lea       0xc(%%sp),%%sp\n\t"// Correct stack
            "jsr       __libc_start_main\n\t"          // Call main program
            "move.w    %%d0,-(%%sp)\n\t"  //    ; Return value of the program
            "move.w    #0x4c,-(%%sp)\n\t" //  ; Pterm
            "trap      #1\n\t"            //; Call GEMDOS
            :
            :
            : "d0", "a0"
            );
    __builtin_unreachable();
}

__attribute__ ((noreturn))
void _exit(uint16_t retval)
{
    __asm__ __volatile__
    (
            "move.w    %0,-(%%sp)\n\t"  //    ; Return value of the program
            "move.w    #0x4c,-(%%sp)\n\t" //  ; Pterm
            "trap      #1\n\t"            //; Call GEMDOS
            :
            : "r"(retval)
            :
            );
    __builtin_unreachable();
}

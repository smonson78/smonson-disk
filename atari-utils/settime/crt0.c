#include <stdint.h>
#include <libc.h>
#include <tos.h>

extern void *stack;
int main(int argc, char **argv);
void __do_global_ctors_aux();
void __do_global_dtors_aux();
void __cxa_finalize(void *f);

typedef struct {
   void *p_lowtpa;          /* Start address of the TPA            */
   void *p_hitpa;           /* First byte after the end of the TPA */
   void *p_tbase;           /* Start address of the program code   */
   int32_t p_tlen;          /* Length of the program code          */
   void *p_dbase;           /* Start address of the DATA segment   */
   int32_t p_dlen;          /* Length of the DATA section          */
   void *p_bbase;           /* Start address of the BSS segment    */
   int32_t p_blen;          /* Length of the BSS section           */
   void *p_dta;             /* Pointer to the default DTA          */
                            /* Warning: Points first to the command line */
   struct pd *p_parent;     /* Pointer to the basepage of the calling processes */
   int32_t p_resrvd0;       /* Reserved                            */
   int8_t *p_env;           /* Address of the environment string   */
   int8_t p_resrvd1[80];    /* Reserved                            */
   int8_t p_cmdlin[128];    /* Command line                        */
} basepage_t;

basepage_t *_basepage;

int __libc_start_main(basepage_t *basepage)
{
    // Empty POSIX-style call name
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
            "moveal    %%sp@(4),%%a0\n\t"      // Pointer to BASEPAGE --> a0
            "move.l    %%a0,%0\n\t"            // Store location of basepage in global variable
            "lea       stack,%%sp\n\t"         // Set stack pointer
            "move.l    #0x100,%%d0\n\t"        // Length of basepage
            "add.l     0xc(%%a0),%%d0\n\t"     // Length of the TEXT segment
            "add.l     0x14(%%a0),%%d0\n\t"    // Length of the DATA segment
            "add.l     0x1c(%%a0),%%d0\n\t"    // Length of the BSS segment
            "move.l    %%d0,-(%%sp)\n\t"       // Return to the stack
            "move.l    %%a0,-(%%sp)\n\t"       // Basepage address to stack
            "clr.w     -(%%sp)\n\t"            // Fill parameter
            "move.w    #0x4a,-(%%sp)\n\t"      // Mshrink
            "trap      #1\n\t"                 // Call GEMDOS
            "lea       0xc(%%sp),%%sp\n\t"     // Correct stack
            "jsr       __libc_start_main\n\t"  // Call C entry point
            "move.w    %%d0,-(%%sp)\n\t"       // Return value of the program
            "move.w    #0x4c,-(%%sp)\n\t"      // Pterm
            "trap      #1\n\t"                 // Call GEMDOS
            : "=m"(_basepage) // Outputs
            : // Inputs
            : "d0", "a0"
            );
    __builtin_unreachable();
}

__attribute__ ((noreturn))
void _exit(uint16_t retval)
{
    __asm__ __volatile__
    (
            "move.w    %0,-(%%sp)\n\t"         // Return value of the program
            "move.w    #0x4c,-(%%sp)\n\t"      // Pterm
            "trap      #1\n\t"                 // Call GEMDOS
            :
            : "r"(retval)
            :
            );
    __builtin_unreachable();
}

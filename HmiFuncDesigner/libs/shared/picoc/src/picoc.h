#ifndef PICOC_H
#define PICOC_H

/* picoc version number */
#define PICOC_VERSION "v2.1"

/* handy definitions */
#ifndef TRUE
    #define TRUE 1
    #define FALSE 0
#endif


#ifdef UNIX_HOST
    #include <setjmp.h>

    /* mark where to end the program for platforms which require this */
    extern jmp_buf PicocExitBuf;

    /* this has to be a macro, otherwise errors will occur due to the stack being corrupt */
    #define PicocPlatformSetExitPoint() setjmp(PicocExitBuf)
#endif

#ifdef WIN_HOST

    #define PicocPlatformSetExitPoint()

#endif

#ifdef SURVEYOR_HOST
    /* mark where to end the program for platforms which require this */
    extern int PicocExitBuf[];

    #define PicocPlatformSetExitPoint() setjmp(PicocExitBuf)
#endif

/* parse.c */
void PicocParse(const char *FileName, const char *Source, int SourceLen, int RunIt, int CleanupNow, int CleanupSource);
void PicocParseInteractive();

/* platform.c */
void PicocCallMain(int argc, char **argv);
void PicocInitialise(int StackSize);
void PicocCleanup();
void PicocPlatformScanFile(const char *FileName);

extern int PicocExitValue;

/* include.c */
void PicocIncludeAllSystemHeaders();




int picoc_main(int argc, char **argv);
int picoc(char *SourceStr);

#endif /* PICOC_H */

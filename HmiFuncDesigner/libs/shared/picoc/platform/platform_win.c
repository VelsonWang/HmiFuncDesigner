#include "../src/picoc.h"
#include "../src/interpreter.h"

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

void PlatformCleanup()
{
}

/* get a line of interactive input */
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{
#ifdef USE_READLINE
    if (Prompt != NULL)
    {
        /* use GNU readline to read the line */
        char *InLine = readline(Prompt);
        if (InLine == NULL)
            return NULL;
    
        Buf[MaxLen] = '\0';
        strncpy(Buf, InLine, MaxLen-1);
        strncat(Buf, "\n", MaxLen-1);
        
        if (InLine[0] != '\0')
            add_history(InLine);
            
        free(InLine);
        return Buf;
    }
#endif

    if (Prompt != NULL)
        printf("%s", Prompt);
        
//    fflush(stdout);
//    return fgets(Buf, MaxLen, _fileno(stdin));
    return gets(Buf);
}

/* get a character of interactive input */
int PlatformGetCharacter()
{
//    fflush(stdout);
    return getchar();
}

/* write a character to the console */
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    putchar(OutCh);
}

/* read a file into memory */
char *PlatformReadFile(const char *FileName)
{
    char *ReadText;
    FILE *InFile;
    int FileLength;
    int BytesRead;

    InFile = fopen((const char*)FileName, "r");
    if (InFile == NULL)
        ProgramFail(NULL, "can't read file %s\n", FileName);
    
    fseek (InFile, 0, SEEK_END);
    FileLength = ftell (InFile);

    rewind(InFile);

    ReadText = malloc(FileLength + 1);
    if (ReadText == NULL)
        ProgramFail(NULL, "out of memory\n");

    BytesRead = fread(ReadText, 1, FileLength, InFile);
    if (BytesRead == 0)
        ProgramFail(NULL, "can't read file %s\n", FileName);

    ReadText[BytesRead] = '\0';
    fclose(InFile);
    
    return ReadText;    
}

/* read and scan a file for definitions */
void PicocPlatformScanFile(const char *FileName)
{
    char *SourceStr = PlatformReadFile(FileName);

    PicocParse(FileName, SourceStr, strlen(SourceStr), TRUE, FALSE, TRUE);
}

/* exit the program */
void PlatformExit(int RetVal)
{
    PicocExitValue = RetVal;
//    longjmp(PicocExitBuf, 1);
}


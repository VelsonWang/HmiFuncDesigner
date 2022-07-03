#include "../src/interpreter.h"
#ifdef WIN_HOST
    #include <windows.h>
#endif
#ifdef UNIX_HOST
    #include <sys/time.h>
    #include <time.h>
#endif


extern void rtTagAddValue(char *id, char *val);
extern void rtTagSubValue(char *id, char *val);
extern void rtTagCopyValue(char *srcId, char *desId);
extern void rtTagSetValue(char *id, char *val);
extern void rtTagStateChange(char *id);
extern void rtSwitchGraphPage(char *pageId);
extern void rtReturnGraphPage();
extern void rtShowControlElement(char *eleId);
extern void rtHideControlElement(char *eleId);
extern void rtEnableControlElement(char *eleId);
extern void rtDisableControlElement(char *eleId);
extern void rtMoveControlElement(char *eleId, int x, int y);
extern void rtBlinkControlElement(char *eleId);
extern void rtStopBlinkControlElement(char *eleId);


/**
 * @brief HMI_AddTagValue 变量值自增
 *        示例: HMI_AddTagValue("var0", "1")  // 变量var0值自增1
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_AddTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtTagAddValue(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

/**
 * @brief HMI_SubTagValue 变量值自减
 *        示例: HMI_SubTagValue("var0", "1")  // 变量var0值自减1
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_SubTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtTagSubValue(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

/**
 * @brief HMI_SwitchGraphPage 切换画面
 *        示例: HMI_SwitchGraphPage("page1")  // 切换至page1画面
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_SwitchGraphPage(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtSwitchGraphPage(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_ReturnGraphPage 返回画面
 *        示例: HMI_ReturnGraphPage()  // 返回至上一画面
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_ReturnGraphPage(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    (void)Param;
    rtReturnGraphPage();
}

/**
 * @brief HMI_SetDateTime 设置系统时间
 *        示例:HMI_SetDateTime(2022,10,1,1,2,3)  // 设置系统时间为2022-10-01 01:02:03
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_SetDateTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    int year = Param[0]->Val->Integer;
    int month = Param[1]->Val->Integer;
    int day = Param[2]->Val->Integer;
    int hour = Param[3]->Val->Integer;
    int minute = Param[4]->Val->Integer;
    int second = Param[5]->Val->Integer;
#ifdef WIN_HOST
    (void)year;
    (void)month;
    (void)day;
    (void)hour;
    (void)minute;
    (void)second;
#endif
#ifdef UNIX_HOST
    char buffer[256] = {0};
    memset(buffer, 256, '\0');
    snprintf(buffer, 256, "date -s '%04d-%02d-%02d %02d:%02d:%02d'", year, month, day, hour, minute, second);
    system(buffer);
    system("hwclock -w");
#endif
}

/**
 * @brief HMI_WaitForMillisec 延时[1,65536]毫秒
 *        示例:HMI_WaitForMillisec(1)  // 延时1毫秒
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_WaitForMillisec(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    int msecs = Param[0]->Val->Integer;
#ifdef WIN_HOST
    Sleep(msecs);
#endif
#ifdef UNIX_HOST
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct timespec ti;

    ti.tv_nsec = (tv.tv_usec + (msecs % 1000) * 1000) * 1000;
    ti.tv_sec = tv.tv_sec + (msecs / 1000) + (ti.tv_nsec / 1000000000);
    ti.tv_nsec %= 1000000000;
    thread_sleep(&ti);
#endif
}

/**
 * @brief HMI_Sleep 控制延时，系统休眠[1,65536]秒
 *        示例: HMI_Sleep(1) // 延时1秒
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_Sleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    int secs = Param[0]->Val->Integer;
#ifdef WIN_HOST
    Sleep(secs * 1000);
#endif
#ifdef UNIX_HOST
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct timespec ti;
    ti.tv_sec = tv.tv_sec + secs;
    ti.tv_nsec = (tv.tv_usec * 1000);
    thread_sleep(&ti);
#endif
}

/**
 * @brief HMI_GetTagValue 获取变量的值
 *        示例: HMI_GetTagValue("var0", "var1")  // 获取变量var0的值保存至变量var1"
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_GetTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtTagCopyValue(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

/**
 * @brief HMI_SetTagValue 设置变量值
 *        示例: HMI_SetTagValue("var0", "123.12")  // 设置变量var0的值为123.12
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_SetTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtTagSetValue(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

/**
 * @brief HMI_StateChange 变量0-1切换
 *        示例: HMI_StateChange("var0")  // 切换变量var0的状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_StateChange(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtTagStateChange(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_ShowControlElement 显示控件
 *        示例: HMI_ShowControlElement("main.label")  // 将文本控件"main.label"变为可见状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_ShowControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtShowControlElement(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_HideControlElement 隐藏控件
 *        示例: HMI_HideControlElement("main.label")  // 将文本控件"main.label"变为不可见状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_HideControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtHideControlElement(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_EnableControlElement 生效控件
 *        示例: HMI_EnableControlElement("main.label")  // 将文本控件"main.label"变为可用状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_EnableControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtEnableControlElement(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_DisableControlElement 失效控件
 *        示例: HMI_DisableControlElement("main.label")  // 将文本控件"main.label"变为不可用状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_DisableControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtDisableControlElement(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_MoveControlElement 移动控件
 *        示例: HMI_MoveControlElement("main.label", -5, -10)  // 将文本控件main.label坐标左移5，上移10
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_MoveControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtMoveControlElement(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

/**
 * @brief HMI_BlinkControlElement 闪烁控件
 *        示例: HMI_BlinkControlElement("main.label")  // 将文本控件"main.label"闪烁显示
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_BlinkControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtBlinkControlElement(Param[0]->Val->Pointer);
}

/**
 * @brief HMI_StopBlinkControlElement 停止闪烁控件
 *        示例: HMI_StopBlinkControlElement("main.label")  // 将文本控件"main.label"变为正常显示，即停止闪烁显示
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HMI_StopBlinkControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    (void)Parser;
    (void)ReturnValue;
    (void)NumArgs;
    rtStopBlinkControlElement(Param[0]->Val->Pointer);
}


struct LibraryFunction HmiFunctions[] =
{
    { HMI_AddTagValue, "void AddTagValue(char *,char *);" },
    { HMI_SubTagValue, "void SubTagValue(char *,char *);" },
    { HMI_SwitchGraphPage, "void SwitchGraphPage(char *);" },
    { HMI_ReturnGraphPage, "void ReturnGraphPage();" },
    { HMI_SetDateTime, "void SetDateTime(int,int,int,int,int,int);" },
    { HMI_WaitForMillisec, "void WaitForMillisec(int);" },
    { HMI_Sleep, "void Sleep(int);" },
    { HMI_GetTagValue, "void GetTagValue(char *,char *);" },
    { HMI_SetTagValue, "void SetTagValue(char *,char *);" },
    { HMI_StateChange, "void StateChange(char *);" },
    { HMI_ShowControlElement, "void ShowControlElement(char *);" },
    { HMI_HideControlElement, "void HideControlElement(char *);" },
    { HMI_EnableControlElement, "void EnableControlElement(char *);" },
    { HMI_DisableControlElement, "void DisableControlElement(char *);" },
    { HMI_MoveControlElement, "void MoveControlElement(char *,int,int);" },
    { HMI_BlinkControlElement, "void BlinkControlElement(char *);" },
    { HMI_StopBlinkControlElement, "void StopBlinkControlElement(char *);" },
    { NULL, NULL }
};

void PlatformLibraryInit()
{
    LibraryAdd(&GlobalTable, "hmi library", &HmiFunctions[0]);
}

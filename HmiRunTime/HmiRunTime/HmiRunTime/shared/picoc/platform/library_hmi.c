#include "../src/interpreter.h"

/**
 * @brief AddTagValue 变量值自增
 *        示例: AddTagValue("var0", "1")  // 变量var0值自增1
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void AddTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief SubTagValue 变量值自减
 *        示例: SubTagValue("var0", "1")  // 变量var0值自减1
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void SubTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief SwitchGraphPage 切换画面
 *        示例: SwitchGraphPage("page1")  // 切换至page1画面
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void SwitchGraphPage(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief ReturnGraphPage 返回画面
 *        示例: ReturnGraphPage()  // 返回至上一画面
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void ReturnGraphPage(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief SetDateTime 设置系统时间
 *        示例:SetDateTime(2022,10,1,1,2,3)  // 设置系统时间为2022-10-01 01:02:03
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void SetDateTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief WaitForMillisec 延时[1,65536]毫秒
 *        示例:延时(1)  // 延时1毫秒
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void WaitForMillisec(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief Sleep 控制延时，系统休眠[1,65536]秒
 *        示例: Sleep(1) // 延时1秒
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void Sleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief GetTagValue 获取变量的值
 *        示例: GetTagValue("var0", "var1")  // 获取变量var0的值保存至变量var1"
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void GetTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief SetTagValue 设置变量值
 *        示例: SetTagValue("var0", "123.12")  // 设置变量var0的值为123.12
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void SetTagValue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief StateChange 变量0-1切换
 *        示例: StateChange("var0")  // 切换变量var0的状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void StateChange(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief ShowControlElement 显示控件
 *        示例: ShowControlElement("main.label")  // 将文本控件"main.label"变为可见状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void ShowControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief HideControlElement 隐藏控件
 *        示例: HideControlElement("main.label")  // 将文本控件"main.label"变为不可见状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void HideControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief EnableControlElement 生效控件
 *        示例: EnableControlElement("main.label")  // 将文本控件"main.label"变为可用状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void EnableControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief DisableControlElement 失效控件
 *        示例: DisableControlElement("main.label")  // 将文本控件"main.label"变为不可用状态
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void DisableControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief MoveControlElement 移动控件
 *        示例: MoveControlElement("main.label", -5, -10)  // 将文本控件main.label坐标左移5，上移10
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void MoveControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief BlinkControlElement 闪烁控件
 *        示例: BlinkControlElement("main.label")  // 将文本控件"main.label"闪烁显示
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void BlinkControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;
}

/**
 * @brief StopBlinkControlElement 停止闪烁控件
 *        示例: StopBlinkControlElement("main.label")  // 将文本控件"main.label"变为正常显示，即停止闪烁显示
 * @param Parser
 * @param ReturnValue
 * @param Param
 * @param NumArgs
 */
void StopBlinkControlElement(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    //printf("test(%d)\n", Param[0]->Val->Integer);
    //Param[0]->Val->Integer = 1234;

    //ReturnValue->Val->Integer = Parser->Line;
}


struct LibraryFunction HmiFunctions[] =
{
    { AddTagValue, "void AddTagValue(char *,char *);" },
    { SubTagValue, "void SubTagValue(char *,char *);" },
    { SwitchGraphPage, "void SwitchGraphPage(char *);" },
    { ReturnGraphPage, "void ReturnGraphPage();" },
    { SetDateTime, "void SetDateTime(int,int,int,int,int,int);" },
    { WaitForMillisec, "void WaitForMillisec(int);" },
    { Sleep, "void Sleep(int);" },
    { GetTagValue, "void GetTagValue(char *,char *);" },
    { SetTagValue, "void SetTagValue(char *,char *);" },
    { StateChange, "void StateChange(char *);" },
    { ShowControlElement, "void ShowControlElement(char *);" },
    { HideControlElement, "void HideControlElement(char *);" },
    { EnableControlElement, "void EnableControlElement(char *);" },
    { DisableControlElement, "void DisableControlElement(char *);" },
    { MoveControlElement, "void MoveControlElement(char *,int,int);" },
    { BlinkControlElement, "void BlinkControlElement(char *);" },
    { StopBlinkControlElement, "void StopBlinkControlElement(char *);" },
    { NULL, NULL }
};

void PlatformLibraryInit()
{
    LibraryAdd(&GlobalTable, "hmi library", &HmiFunctions[0]);
}

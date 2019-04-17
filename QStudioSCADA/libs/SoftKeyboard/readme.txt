
PS:引入三个文件，main函数中加入两行代码即可，支持所有输入框。

//以下方法打开中文输入法
//InputMethodAlphabet::instance()->init("control", "black", 10, 10);
InputMethodAlphabet::instance()->init("control", "", 10, 10);
//以下方法打开数字键盘
//InputMethodNumber::instance()->init("lightgray", 10);
InputMethodNumber::instance()->init("", 10);


字库文件：py.db



## 简介 Brief introduction

​    **HmiFuncDesigner是一款集HMI，数据采集于一体的软件，开源旨在技术分享、相互学习、提升技术。目前软件处于开发中，功能不完善，很多代码也需要重构，但是我相信在不久的将来本软件功能会更加完善！如果这份代码有幸被你看到了，而且对此也有兴趣，那么期待你的加入！！**



**本源码参考了SoftBox设计思路，在此特别感谢SoftBox的作者！**



### 1. 代码下载 How to download HmiFuncDesigner code

1. **打开Git Bash命令行工具， 执行 git clone https://gitee.com/VelsonWang/HmiFuncDesigner.git 克隆代码至本地目录。**
2. **同步远端更新代码至本地，执行 git pull origin。**

### 2. 软件环境 Software development environment

​    1.**Qt5.10.1 + MinGW530_32** 

​    2.**软件安装时必须包含Qt Script 模块**

​    3.**如果需要在Visual Studio下编译并不产生乱码，请参考文档 (doc/Visual Studio utf8-NOBOM.docx)**

### 3. HmiFuncDesigner软件编译 How the HmiFuncDesigner compiles

1. **工程路径不要太长，最好控制在256字符以内，以免编译出错！（注：Windows系统）**

2. **打开"Qt Creator 4.5.1 (Community)"软件，打开HmiFuncDesigner/HmiFuncDesigner.pro工程。**

3. **选择Release模式。**

4. **清除以前编译工程产生的数据文件。**

5. **重新编译工程。**

6. **编译完成功后生成的软件位于HmiFuncDesignerBin/bin目录下。**

7. **拷贝所需要的运行库，打开命令窗口“Qt 5.10.1 for Desktop (MinGW 5.3.0 32 bit)”，HmiFuncDesignerBin/bin目录，执行windeployqt 软件名称.exe。**
   
   **具体操作如下图所示：**
   
   ![buildHmiFuncDesigner](md/buildHmiFuncDesigner.png)

​    

### 4. HmiRunTime软件编译 How the HmiRunTime compiles

1. **必须先编译HmiFuncDesigner然后再编译HmiRunTime工程。否则会出现找不到连接库。**

2. **打开"Qt Creator 4.5.1 (Community)"软件，打开HmiRunTime/HmiRunTime.pro工程。**

3. **选择Release模式。**

4. **清除以前编译工程产生的数据文件。**

5. **重新编译工程。**

6. **编译完成功后生成的软件位于RuntimeBin目录下。**

7. **拷贝所需要的运行库，打开命令窗口“Qt 5.10.1 for Desktop (MinGW 5.3.0 32 bit)”，RuntimeBin目录，执行windeployqt 软件名称.exe。**

### 5. HmiFuncDesigner工程管理器 ProjectManager HmiFuncDesigner

![ProjectManager](md/_projectman_1546500878_8823.png)

**工程管理器具有如下功能：**

1. **系统参数，设置与运行有关的参数。**

2. **通讯设备，建立通信链路和协议。**

3. **数据库配置，建立系统IO变量表，并操作存盘、报警、转换等处理。**

4. **数据库管理，运行中的实时内存数据和SQL数据管理监视。**

5. **画面，建立系统画面。**

6. **逻辑编程，JavaScript编程。**

### 6.系统变量管理器 System variable manager

![SystemVariableManager](md/SystemVariableManager.png)

### 7.实时数据库显示 Real-time database data display

![RTDBView](md/RTDBView.png)

### 8.画面编辑 Graphic control editor

![GraphPageEdit](md/GraphPageEdit.png)

![GraphPageEdit](md/GraphPageEdit2.png)

### 9. 画面解析运行 Run the designed UI

![RuntimeViewShow](md/RuntimeViewShow.png)

![RuntimeViewShow](md/RuntimeViewShow2.png)

### 10.支持控件 Support controls

​    **文本、椭圆、直线、矩形、箭头、切换按钮，变量文本列表、图片、弹出按钮、指示灯、数值棒图、时钟、移动文本、罐形容器、输入编辑框**

### 11. 支持的通信协议  Supported communication protocol

1.**Modbus RTU**

2.**Modbus ASCII**

3.**Modbus TCPIP**

4.**Mitsubishi Fx**

### 12. 联系方式  Contact

**Email：VelsonWang@qq.com**

**QQ:706409617**

**QQ交流群:568268522**

#### 如果觉得代码写的还可以的话，请赏一个Star吧！

#### **您的支持是我继续前行的动力！！！**

![JasonWang_qrcode](md/JasonWang_qrcode.png)
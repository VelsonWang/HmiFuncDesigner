## 简介

​	**QStudioSCADA是一款集HMI，数据采集于一体的软件，开源旨在技术分享、相互学习、提升技术。目前软件处于开发中，功能不完善，很多代码也需要重构，但是我相信在不久的将来本软件功能会更加完善！如果这份代码有幸被你看到了，而且对此也有兴趣，那么期待你的加入！！**


### 0. 代码下载
 1. **打开Git Bash命令行工具， 执行 git clone https://github.com/VelsonWang/QStudioSCADA.git 克隆代码至本地目录。**
 
 
 
### 1. 软件环境

​	**Qt5.10.1 + MinGW530_32**



### 2. QStudioSCADA软件编译	

 1. **打开"Qt Creator 4.5.1 (Community)"软件，打开QStudioSCADA/QStudioSCADA.pro工程。**

 2. **选择Release模式。**

 3. **清除以前编译工程产生的数据文件。**

 4. **重新编译工程。**

 5. **编译完成功后生成的软件位于QStudioSCADABin/bin目录下。**

 6. **拷贝所需要的运行库，打开命令窗口“Qt 5.10.1 for Desktop (MinGW 5.3.0 32 bit)”，QStudioSCADABin/bin目录，执行windeployqt 软件名称.exe。**

    **具体操作如下图所示：**

    ![buildQStudioSCADA](md/buildQStudioSCADA.png)

​	

### 3. QSCADARunTime软件编译

1. **打开"Qt Creator 4.5.1 (Community)"软件，打开QSCADARunTime/QSCADARunTime.pro工程。**

2. **选择Release模式。**

3. **清除以前编译工程产生的数据文件。**

4. **重新编译工程。**

5. **编译完成功后生成的软件位于RuntimeBin目录下。**

6. **拷贝所需要的运行库，打开命令窗口“Qt 5.10.1 for Desktop (MinGW 5.3.0 32 bit)”，RuntimeBin目录，执行windeployqt 软件名称.exe。**

   

### 4.工程管理器QStudioSCADA

![ProjectManager](md/_projectman_1546500878_8823.png)

**工程管理器具有如下功能：**

1. **系统参数，设置与运行有关的参数。**

2. **通讯设备，建立通信链路和协议。**

3. **数据库配置，建立系统IO变量表，并操作存盘、报警、转换等处理。**

4. **数据库管理，运行中的实时内存数据和SQL数据管理监视。**

5. **画面，建立系统画面。**

6. **逻辑编程，JavaScript编程。**

    

### 5.系统变量管理器

![SystemVariableManager](md/SystemVariableManager.png)



### 6.实时数据库显示

![RTDBView](md/RTDBView.png)



### 7.画面编辑

![GraphPageEdit](md/GraphPageEdit.png)



### 8.联系方式

**Email：VelsonWang@qq.com**

**QQ:706409617**

**QQ交流群:568268522**



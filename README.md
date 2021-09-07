# Lotos

loto，意为一种莲花，也有谐音photo的意思。

QT课设项目，是一款windows平台下对（第一阶段，smms）图床进行桌面端操作的管理软件。

启发项目[PicGo](https://github.com/PicGo/PicGo-Core)。



## 功能逻辑

界面模块：包含一个主GUI，还有相应的菜单

上传选项卡：负责上传图片的相关操作，如进行图片的选择，修改，如果成功可以进行连接复制与格式化

图片集选项卡：显示所有上传图片的缩略，点击可以查看大图与具体信息，可以分组，可以筛选

可以区分登录用户上传与非登录上传，可以主动添加不是由本软件上传的图片，可以删除图片，复制链接，可以批量选择

图床设置选项卡：设置图床和连接的相关部分

设置：设置软件信息



功能模块

软件管理：设置的读写，软件信息的确认，ui提示与帮助，软件信息的处理，拖放处理，剪贴板处理，事件处理

用户信息管理：用户上传图片的信息的管理

图片处理：进行图片的简单操作，例如裁剪压缩与缩放

图床交互：调用图床接口，处理返回信息



通用处理模块

json处理：所有的数据都是用json处理

网路处理：实现get与post方法，实现sync与async方法，对连接进行处理

URL字符串处理：对URL字符串进行格式化

图片处理：调用外部库，实现信息读取

文件处理：文件路径与文本或二进制读取



| 目录        | 描述     |
| :---------- | -------- |
| src         | 源代码   |
| third-party | 第三方库 |



## 开发流程

在这个github的[project](https://github.com/users/median-dxz/projects/)板块进行check。
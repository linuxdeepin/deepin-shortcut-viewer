
## deepin-shortcut-viewer 使用说明文档

#### 1. 快捷键文档目录位置:usr/share/deepin-shortcut-viewer
>  * 快捷键目录结构：
 >![快捷键目录结构图](./file-tree.png)              
>  * 每个应用程序可以在这个目录创建自己的快捷键文档，拿文件管理器为例：文件管理器的应用明是 dde-file-manager，拿文件管理器可以在这里创建目录：usr/share/deepin-shortcut-viewer/dde-file-manager。然后把快捷键文件拷进这个目录。
>
>  * 由于快捷键文档要支持多语言，所以需要把翻译好的shortcut文档拷进对应语言目录中，比如中文：~/deepin-shortcut-viewer/dde-file-manager/zh_CN/shortcut.txt；英文：~/deepin-shortcut-viewer/dde-file-manager/en_US/shortcut.txt，等等。


#### 2. 快捷键文档格式：shortcut.txt
>     [分类1]                                
>     标题1=快捷键符                       
>     标题2=快捷键符                
>     标题3=快捷键符                
>     .
>     .
>     .
>     [分类2]
>     标题1=快捷键符
>     标题2=快捷键符
>     .
>     .
>     .
##### 当快捷键文件都拷贝对应程序明的目录后，就可以通过命令行调用打开快捷键预览界面了，deepin-shortcut-viewer 是一个单进程应用，当第一次启动后，如果再调用一次，它就会关闭。于此类推来控制显示快捷键预览或者关闭快捷键预览。下面介绍如何通过命令行调用快捷键。
![Alt text](./cmd.png)
打开命令：deepin-shortcut-viewer -h看帮助
> * 参数 -t,--target是指向应用程序名称，deepin-shortcut-viewer会根据应用程序名称找到对应目录的快捷键文件，比如dde-file-manager，deepin-shortcut-viewer会读取/usr/share/deepin-shortcut-viewer/dde-file-manager/zh_CN/shortcut.txt这个文件，如果当前系统语言是其他语言，deepin-shortcut-viewer会读取对应语言目录的shortcut.txt文件，比如../en_US/shortcut.txt。
>
> * 参数-r,--rect是用来暴露你当前程序窗口的位置信息，当加上这个参数暴露当前要居中的窗口坐标信息的时候，deepin-shortcut-viewer会居中到这个窗口，如果不加这个参数，deepin-shortcut-viewer会居中到屏幕。
>
> * 参数-j,--json-data是把快捷键内容转换成下面json格式字符串传给deepin-shortcut-viewer读取的一种方式。比起使用-t参数的方式，这个可以在程序中把快捷键内容传给deepin-shortcut-viewer显示，从而不用额外配置快捷键文件。

#### 命令行打开命令事例（使用-t参数的方式）：
>        deepin-shortcut-viewer -t=dde-file-manager -r=200,200,700,450
>
>  应用截图：
>  ![应用截图](./preview.png)
#### 3,直接通过传送json数据
json 数据格式如下：
~~~json
{
    "shortcut": [
        {
            "groupItems": [
                {
                    "name": "name0",
                    "value": "value0"
                },
                {
                    "name": "name1",
                    "value": "value1"
                },
                {
                    "name": "name2",
                    "value": "value2"
                }
            ],
            "groupName": "group0"
        },
        {
            "groupItems": [
                {
                    "name": "name0",
                    "value": "value0"
                },
                {
                    "name": "name1",
                    "value": "value1"
                },
                {
                    "name": "name2",
                    "value": "value2"
                }
            ],
            "groupName": "group1"
        }
    ]
}
~~~

数据的root为shortcut每个object中,groupName是快捷键分组标签；groupItems是该分组的快捷键；groupItems中,name为快捷键标签，value为快捷键符号。

### 程序实现实例（使用-j参数的方式）：

~~~c++
#include "widget.h"

#include <QApplication>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QJsonObject root;
    QJsonArray shortcuts;

    for(int a = 0; a<2;a++){

        QJsonObject group;
        QJsonArray items;
        group.insert("groupName","group" + QString::number(a));

        for(int b = 0; b<3; b++){
            QJsonObject item;
            item.insert("name","name" + QString::number(b));
            item.insert("value","value" + QString::number(b));
            items.append(item);
        }
        group.insert("groupItems",items);
        shortcuts.append(group);
    }
    root.insert("shortcut",shortcuts);

    QJsonDocument doc(root);
    QStringList args;
    QString param = "-j=";
    param+=doc.toJson().data();
    args<<param<<"-r=200,200,20,20";
    QProcess::startDetached("deepin-shortcut-viewer",args);

    return a.exec();
}

~~~

程序截图：

![应用截图](./preview1.png)

#### 说明：

命令行调用的时候若是-r 和 -j的参数都使用了，deepin-shortcut-viewer会默认启用-r参数格式显示快捷键内容。

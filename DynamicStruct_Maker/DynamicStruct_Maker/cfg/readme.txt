主要相关的文件

StructDef.h(由程序根据配置文件自动生成)
ps: 结构定义，

DynamicStructMgr.h
DynamicStructMgr.cpp(由程序根据配置文件自动生成)
ps: 结构信息的管理

DynamicStruct.h
DynamicStruct.cpp
ps: 结构信息的定义和实现

生成结构和结构信息的配置文件：structs.xml
主要的配置类型：
StructDefBegin ： 生成 StructDef.h 开头信息
StructDefEnd    ： 生成 StructDef.h 结尾信息

StructInitBegin： 生成DynamicStructMgr.cpp 开头信息，
StructInitEnd：    生成DynamicStructMgr.cpp 结尾信息

StructNewBegin： 生成到 DynamicStructMgr.cpp 文件里面的 每个数据结构信息的设置的开头信息
StructNewEnd   ： 生成到 DynamicStructMgr.cpp 文件里面的 每个数据结构信息的设置的结尾信息

<structs>标签结构

结构配置选项：
1.可以指定是 class 或者 struct,默认是struct
例子：<Image type="class">

2.可以指定 继承的 结构 inherit="Item"
例子：<Image inherit="Item">

完整例子：
<structs>
     <Item type="struct">
          <HAlign type="bool">true</HAlign>
          <VAlign type="bool">true</VAlign>
     </Item>
     <Image inherit="Item" type="struct">
          <MultiLine type="bool">true</MultiLine>
          <RowSpacing  type="int">0</RowSpacing>
          <FontSpacing  type="int">0</FontSpacing>
     </Image>
</structs>

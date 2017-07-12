---
layout:        projects
title:         File Classifier - 文件分类器
describe:      根据文件名的第一个字将文件分类至不同目录，建议用来整理音乐。
currVersion:   1
buildTime:     20170706.2143
state:         Updating
isOpenSource:  true
dlID:          9
downloadUrl:   
---

这是一个可以将某一目录下（不含子目录）的所有文件，依据正则表达式，整理到特定目录的Python小脚本。

如果不加改动，脚本将按首字符为数字、英文字母、特殊符号、汉字、韩语符号（朝鲜文音节）、日语假名整理到不同文件夹。具体参见fc.py的第275行（随版本迭代，行号可能有变动）的二维数组dictionary。另外，请阅读注释。
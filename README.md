NEEDNOTREADME
==============================
程序的大概步骤是：

1. 根据html中System Testing这个注释找到包含数据的table，用vector保存table中的每一行。(readLines以及getDataTable)
2. 弃置那些没有valign属性的tr标签，以及他们包含的行。
3. 剩下的每个有valign="top"属性的tr标签，都包含一组数据。 把每个标签所包含的行放在同一个vector里面，用另一个vector，也就是main中的dtrs，来保存这些vector。(以上两步，都在mergeRows中完成)
4. 对于dtrs中的每个vector中的每个string，去掉'&lt;'以及'&gt;'以及它们之中的文本。 这时我们发现，每一个vector都变成了：

    * 一些空行
    * 输入
    * 一些空行
    * 输出
    * 一些空行
    * Passed

    这样的形式。
5. 于是我们对每一个vector，按照上面的形式找到输入输出。再写到相应的文件。（writeToFile)

# JS性能优化
性能优化的方法有：

1、减少HTTP请求：合并CSS/JS，使用CSS sprite等

2、压缩CSS/JS/图片

3、样式表放头部，JS放body底部：JS放在head中，将会等到js全部下载解析和执行后才会显示页面内容。

4、减少DOM操作，DOM操作很消耗性能，另外注意HTMLCollection和NodeList。这两个对象是动态的，每次访问都会进行一次查询。在迭代中避免重复访问。

历史上的DOM集合接口。主要不同在于HTMLCollection是元素集合而NodeList是节点集合（即可以包含元素，也可以包含文本节点）。所以 node.childNodes 返回 NodeList，而 node.children 和 node.getElementsByXXX 返回 HTMLCollection 。（来自知乎贺师俊）
需要时可以将NodeList对象转换为数组。
```Javascript
function convertToArray(nodes){
    var array=null;
    try{
        array=Array.prototype.slice.call(nodes,0);//针对非IE
    }catch(ex){
        array=new Array();
        for(var i=0,len=nodes.length;i<len;i++){
            array.push(nodes[i];
        }
    }

    return array;
}
```
querySelectorAll()方法返回的是一个NodeList实例，类似于一组元素的快照，非动态。

5、关于重绘（repaint）和重排（reflow）。

重绘并不一定导致重排，比如修改某个元素的颜色，只会导致重绘；而重排之后，浏览器需要重新绘制受重排影响的部分。导致重排的原因有：

添加或删除DOM元素
元素位置、大小、内容改变
浏览器窗口大小改变
滚动条出现
6、事件委托

利用事件冒泡，指定一个事件处理程序，管理某一个类型的所有事件。在DOM树中尽量搞的层次上添加一个事件处理程序，这样可以只取得一个DOM元素，添加一个事件处理程序

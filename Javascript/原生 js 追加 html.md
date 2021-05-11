element.insertAdjacentHTML(position, text);

position 是相对于 element 元素的位置，并且只能是以下的字符串之一：

beforebegin：在 element 元素的前面。

afterbegin：在 element 元素的第一个子节点前面。

beforeend：在 element 元素的最后一个子节点后面。

afterend：在 element 元素的后面。

text 是字符串，会被解析成 HTML 或 XML，并插入到 DOM 树中。

// <div id="one">one</div> 
var d1 = document.getElementById('one'); 
d1.insertAdjacentHTML('afterend', '<div id="two">two</div>');

// 此时，新结构变成：
// <div id="one">one</div><div id="two">two</div>

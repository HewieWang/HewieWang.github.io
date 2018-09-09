//&cls&dir /a-d/b *.txt|cscript -nologo -e:jscript "%~f0"&pause&exit

/*
用法：
一、转换文件
1、指定某个文件 //&cls&dir /a-d/b "指定文本.txt"|cscript -nologo -e:jscript "%~f0"&pause&exit
2、多个文件 //&cls&dir /a-d/b *.txt|cscript -nologo -e:jscript "%~f0"&pause&exit
二、转换字符串
//&cls&cscript -nologo -e:jscript "%~f0" "需转换的字符串"&pause&exit
*/

var pattern=1;    //设置转换模式


var crlf=0;     //是否转换回车换行符，1为是，0为否

if(!/^[1-4]$/.test(pattern) || !/^[01]$/.test(crlf)){WSH.echo('参数有误！');WSH.quit()}
var fso = new ActiveXObject('Scripting.Filesystemobject');

if(WSH.Arguments.length<1){
    while(!WSH.StdIn.AtEndOfStream){
        try{
            var file = WSH.StdIn.ReadLine();
            var f = fso.OpenTextFile(file, 1);
            var txt = f.ReadAll();
            f.Close();
            var f = file.replace(/(.+\\)?([^\\]+)/, '$1$New_$2');
            fso.CreateTextFile(f, 2).Write(Choice(txt));
        }catch(e){}
    }
    WSH.echo('Done');
}else WSH.Echo(Choice(WSH.Arguments(0)));

function Choice(txt){
    switch(pattern){
        case 1: return AsciiToUnicode(txt);break;
        case 2: return UnicodeToAscii(txt);break;
        case 3: return HanziToUnicode(txt);break;
        case 4: return UnicodeToHanzi(txt);break;
    }
}

function AsciiToUnicode(txt){
    var s='';
    for (var i=0; i<txt.length; i++){
        var num=txt.charCodeAt(i);
        if(!crlf && (num==10 ||num==13)){
            s += String.fromCharCode(num);
        }else s += '&#'+num+';';
    }
    return s;
}

function UnicodeToAscii(txt) {
    return txt.replace(/(&#)(\d+);/gi,function($0){return String.fromCharCode(parseInt(escape($0).replace(/(%26%23)(\d+)(%3B)/g,"$2")));});
}

function HanziToUnicode(txt){
    var s='';
    for (var i=0; i<txt.length; i++){
        var num=txt.charCodeAt(i);
        if(!crlf && (num==10 ||num==13)){
            s += String.fromCharCode(num);
        }else s += '\\u' + ('00'+parseInt(num).toString(16)).slice(-4);
    }
    return s;
}

function UnicodeToHanzi(txt){
    return txt.replace(/(\\u)(\w{1,4})/gi,function($0){return (String.fromCharCode(parseInt((escape($0).replace(/(%5Cu)(\w{1,4})/g,"$2")),16)));});
}
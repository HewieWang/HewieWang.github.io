//&cls&dir /a-d/b *.txt|cscript -nologo -e:jscript "%~f0"&pause&exit

/*
�÷���
һ��ת���ļ�
1��ָ��ĳ���ļ� //&cls&dir /a-d/b "ָ���ı�.txt"|cscript -nologo -e:jscript "%~f0"&pause&exit
2������ļ� //&cls&dir /a-d/b *.txt|cscript -nologo -e:jscript "%~f0"&pause&exit
����ת���ַ���
//&cls&cscript -nologo -e:jscript "%~f0" "��ת�����ַ���"&pause&exit
*/

var pattern=1;    //����ת��ģʽ


var crlf=0;     //�Ƿ�ת���س����з���1Ϊ�ǣ�0Ϊ��

if(!/^[1-4]$/.test(pattern) || !/^[01]$/.test(crlf)){WSH.echo('��������');WSH.quit()}
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
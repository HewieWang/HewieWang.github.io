## 遍历目录文件
```PHP
function glob2foreach($path, $include_dirs=false) {
    $path = rtrim($path, '/*');
    if (is_readable($path)) {
        $dh = opendir($path);
        while (($file = readdir($dh)) !== false) {
            if (substr($file, 0, 1) == '.')
                continue;
            $rfile = "{$path}/{$file}";
            if (is_dir($rfile)) {
                $sub = glob2foreach($rfile, $include_dirs);
                while ($sub->valid()) {
                    yield $sub->current();
                    $sub->next();
                }
                if ($include_dirs)
                    yield $rfile;
            } else {
                yield $rfile;
            }
        }
        closedir($dh);
    }
}
$glob = glob2foreach('/var/www');
while ($glob->valid()) {
    $filename = $glob->current();
    echo $filename;
    $glob->next();
}
```
## 读取文本文件(大文件)
```PHP
function read_file($path) {
    if ($handle = fopen($path, 'r')) {
        while (! feof($handle)) {
            yield trim(fgets($handle));
        }
        fclose($handle);
    }
}
// 使用
$glob = read_file('/var/www/hello.txt');
while ($glob->valid()) {
    // 当前行文本
    $line = $glob->current();
    // 逐行处理数据
    // echo $line；
    // 指向下一个，不能少
    $glob->next();
}
```
## SplFileObject 从指定行数开始读取
```PHP
function read_file2arr($path, $count, $offset=0) {
    $arr = array();
    if (! is_readable($path))
        return $arr;
    $fp = new SplFileObject($path, 'r');
    // 定位到指定的行数开始读
    if ($offset)
        $fp->seek($offset); 
    $i = 0;
    while (! $fp->eof()) {
        // 必须放在开头
        $i++;
        // 只读 $count 这么多行
        if ($i > $count)
            break;
        $line = $fp->current();
        $line = trim($line);
        $arr[] = $line;
        // 指向下一个，不能少
        $fp->next();
    }
    return $arr;
}
```
## 复制大文件(小文件用copy,大文件用数据流)
```PHP
function copy_file($path, $to_file) {
    if (! is_readable($path))
        return false;
    if(! is_dir(dirname($to_file)))
        @mkdir(dirname($to_file).'/', 0747, TRUE);
    if (
        ($handle1 = fopen($path, 'r')) 
        && ($handle2 = fopen($to_file, 'w'))
    ) {
        stream_copy_to_stream($handle1, $handle2);
        fclose($handle1);
        fclose($handle2);
    }
}
```
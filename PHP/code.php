<?php 
// 验证码类
    session_start();
    class Code {
        //验证码的宽度
        private $width;
        //验证码的高度
        private $height;
        //验证码字符的个数
        private $codeNum;
        //图像资源
        private $image;
        //干扰点的个数
        private $disturbColorNum;
        //验证码字符内容
        private $checkCode;
        
        //构造方法，默认宽为80，高为20，字符个数为4
        function __construct($width=80, $height=20, $codeNum=4){
            //赋值成员变量
            $this->width=$width;
            $this->height=$height;
            $this->codeNum=$codeNum;
            //获得验证码字符内容
            $this->checkCode=$this->createCheckCode();
            //设置干扰点的个数
            $number=floor($width*$height/15);
            if($number > 240-$codeNum){
                $this->disturbColorNum= 240-$codeNum;
            }else{
                $this->disturbColorNum=$number;
            }
        }
        
        //通过访问该方法向浏览器中输出图像
        function showImage($fontFace=""){
            //第一步：创建图像背景
            $this->createImage();
            //第二步：设置干扰元素
            $this->setDisturbColor();
            //第三步：向图像中随机画出文本
            $this->outputText($fontFace);
            //第四步：输出图像
            $this->outputImage();
        }
            
        //通过调用该方法获取随机创建的验证码字符串
        function getCheckCode(){
            return $this->checkCode;
        }
        
        //设置图像资源
        private function createImage(){
            //创建图像资源
            $this->image=imagecreatetruecolor($this->width, $this->height);
            //随机背景色
            $backColor=imagecolorallocate($this->image, rand(225, 255), rand(225,255), rand(225, 255));
            //为背景添充颜色
            imagefill($this->image, 0, 0, $backColor);
            //设置边框颜色
            $border=imagecolorallocate($this->image, 0, 0, 0);
            //画出矩形边框
            imagerectangle($this->image, 0, 0, $this->width-1, $this->height-1, $border);
        }
        
        //创建干扰元素
        private function  setDisturbColor(){
            //创建干扰的点
            for($i=0; $i<$this->disturbColorNum; $i++){
                $color=imagecolorallocate($this->image, rand(0, 255), rand(0, 255), rand(0, 255));
                imagesetpixel($this->image, rand(1, $this->width-2), rand(1, $this->height-2), $color); 
            }
            //创建干扰的线条
            for($i=0; $i<10; $i++){
                $color=imagecolorallocate($this->image, rand(200, 255), rand(200, 255), rand(200, 255));
                imagearc($this->image, rand(-10, $this->width), rand(-10, $this->height), rand(30, 300), rand(20, 200), 55, 44, $color);
            }
        }
        
        //创建随机验证码
        private function createCheckCode(){
            $code="23456789abcdefghijkmnpqrstuvwxyzABCDEFGHIJKMNPQRSTUVWXYZ";
            $string='';
            //从字符串中取出随机的字符
            for($i=0; $i < $this->codeNum; $i++){
                $char=$code{rand(0, strlen($code)-1)};
                $string.=$char;
            }
            //返回字符内容
            return $string;
        }
        
        //设置验证码的字符
        private function outputText($fontFace=""){
            for($i=0; $i<$this->codeNum; $i++){
                $fontcolor=imagecolorallocate($this->image, rand(0, 128), rand(0, 128), rand(0, 128));
                if($fontFace==""){
                    $fontsize=rand(3, 5);
                    $x=floor($this->width/$this->codeNum)*$i+3;
                    $y=rand(0, $this->height-15);
                    imagechar($this->image,$fontsize, $x, $y, $this->checkCode{$i},$fontcolor);
                }else{
                    $fontsize=rand(12, 16);
                    $x=floor(($this->width-8)/$this->codeNum)*$i+8;
                    $y=rand($fontSize+5, $this->height);
                    imagettftext($this->image,$fontsize,rand(-30, 30),$x,$y ,$fontcolor, $fontFace, $this->checkCode{$i});
                }
            }
        }
        
        //输出验证码图像资源
        private function outputImage() {
            if(imagetypes() & IMG_GIF){
                header("Content-Type:image/gif");
                imagepng($this->image);
            }else if(imagetypes() & IMG_JPG){
                header("Content-Type:image/jpeg");
                imagepng($this->image);
            }else if(imagetypes() & IMG_PNG){
                header("Content-Type:image/png");
                imagepng($this->image);
            }else if(imagetypes() & IMG_WBMP){
                header("Content-Type:image/vnd.wap.wbmp");
                imagepng($this->image);
            }else{
                die("PHP不支持图像创建");
            }
        }
        //析构方法
        function __destruct(){
            //销毁图像资源
            imagedestroy($this->image);
        }
    }
    $code=new Code(80, 20, 4);
    $code->showImage();
    $_SESSION["code"]=$code->getCheckCode();
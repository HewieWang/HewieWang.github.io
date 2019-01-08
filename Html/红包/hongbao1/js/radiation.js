
// 监听页面加载处理
window.addEventListener('load', onloadHandler, false);


// 页面加载处理

function onloadHandler()
{
   var canvas = document.getElementById('canvas');
   canvas.width = window.innerWidth;
   canvas.height = window.innerHeight;
   var k3dmain = new K3D.Controller(canvas, true);

   // 添加页面渲染循环回调函数
   var ctx = canvas.getContext('2d');
   var rotationOffset = 0;
   var len = (canvas.height > canvas.width ? canvas.height : canvas.width) * 0.7;
   k3dmain.clearBackground = false;
   k3dmain.callback = function()
   {
      // 清除画布
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      
      // 绘制3d效果
      ctx.save();
      ctx.translate(canvas.width/2, canvas.height/2);
      ctx.rotate(rotationOffset);
      
      // 绘制辐射条
      var RAYCOUNT = 24;
      ctx.fillStyle = "#fdd971";
      ctx.beginPath();
      for (var i=0; i<RAYCOUNT; i++)
      {
         ctx.rotate(TWOPI / RAYCOUNT);
         ctx.moveTo(0, 0);
         ctx.lineTo(-20, len);
         ctx.lineTo(20, len);
      }
      ctx.closePath();
      ctx.fill();
      ctx.fillStyle = "#fdd971";//设置辐射条颜色
      ctx.beginPath();
      for (var i=0; i<RAYCOUNT; i++)
      {
         // 辐射条坐标
         ctx.rotate(TWOPI / RAYCOUNT);
         ctx.moveTo(0, 0);
         ctx.lineTo(-15, len);
         ctx.lineTo(15, len);
      }
      ctx.closePath();
      ctx.fill();
      ctx.restore();
      rotationOffset += 0.005;
      
      // 转动辐射条
      for (var i=0, objs=k3dmain.objects; i<objs.length; i++)
{
   objs[i].ophi += targetRotationX;
}

   if (targetRotationX > -0.5) targetRotationX -= 0.05;
   else if (targetRotationX < -0.55) targetRotationX += 0.05;
   if (targetRotationX > -0.55 && targetRotationX < -0.5) targetRotationX = -0.5;
};

   // 循环转动
   k3dmain.paused = true;
   setInterval(function(){k3dmain.tick()}, 1000/60);
}
var targetRotationX = 0;

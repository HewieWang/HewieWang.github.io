
// ����ҳ����ش���
window.addEventListener('load', onloadHandler, false);


// ҳ����ش���

function onloadHandler()
{
   var canvas = document.getElementById('canvas');
   canvas.width = window.innerWidth;
   canvas.height = window.innerHeight;
   var k3dmain = new K3D.Controller(canvas, true);

   // ���ҳ����Ⱦѭ���ص�����
   var ctx = canvas.getContext('2d');
   var rotationOffset = 0;
   var len = (canvas.height > canvas.width ? canvas.height : canvas.width) * 0.7;
   k3dmain.clearBackground = false;
   k3dmain.callback = function()
   {
      // �������
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      
      // ����3dЧ��
      ctx.save();
      ctx.translate(canvas.width/2, canvas.height/2);
      ctx.rotate(rotationOffset);
      
      // ���Ʒ�����
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
      ctx.fillStyle = "#fdd971";//���÷�������ɫ
      ctx.beginPath();
      for (var i=0; i<RAYCOUNT; i++)
      {
         // ����������
         ctx.rotate(TWOPI / RAYCOUNT);
         ctx.moveTo(0, 0);
         ctx.lineTo(-15, len);
         ctx.lineTo(15, len);
      }
      ctx.closePath();
      ctx.fill();
      ctx.restore();
      rotationOffset += 0.005;
      
      // ת��������
      for (var i=0, objs=k3dmain.objects; i<objs.length; i++)
{
   objs[i].ophi += targetRotationX;
}

   if (targetRotationX > -0.5) targetRotationX -= 0.05;
   else if (targetRotationX < -0.55) targetRotationX += 0.05;
   if (targetRotationX > -0.55 && targetRotationX < -0.5) targetRotationX = -0.5;
};

   // ѭ��ת��
   k3dmain.paused = true;
   setInterval(function(){k3dmain.tick()}, 1000/60);
}
var targetRotationX = 0;

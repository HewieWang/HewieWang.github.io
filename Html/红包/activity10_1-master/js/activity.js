$(document).ready(function(){
//闪光
	$("canvas.flare").let_it_snow({
		windPower: 0,
		speed: 0,
		color: "#392F52",
		size:40,
		opacity: 0.7,
		count: 10,
		interaction: false
	});
//小碎片
	$("canvas.snow").let_it_snow({
		windPower: 3,
		speed: 1,
		count: 250,
		size: 0
	});
	$(".open-btn").click(function(){
		$('.activity-box').fadeOut(200,function(){
			//雪花
			$(".js-point-box").fadeIn();
			$("canvas.flake").let_it_snow({
				windPower: -3,
				speed: 2,
				count: 10,
				size: 40,
				image: "images/white-snowflake.png"
			});
		});
	})
});

; /* Start:"a:4:{s:4:"full";s:92:"/bitrix/templates/aspro_next/components/bitrix/news.detail/projects/script.js?15438444901326";s:6:"source";s:77:"/bitrix/templates/aspro_next/components/bitrix/news.detail/projects/script.js";s:3:"min";s:0:"";s:3:"map";s:0:"";}"*/
$(document).ready(function(){
	if($('.detail .galery-block .flexslider .item').length)
	{
		$('.detail .galery-block .flexslider .item').sliceHeight({lineheight: -3});
		if($('.detail .galery #carousel').length)
		{
			$('.detail .galery #carousel').flexslider({
				animation: 'slide',
				controlNav: false,
				animationLoop: true,
				slideshow: false,
				itemWidth: 77,
				itemMargin: 7.5,
				minItems: 2,
				maxItems: 4,
				asNavFor: '.detail .galery #slider'
			});
		}
	}
	if($('.docs-block .blocks').length)
		$('.docs-block .blocks .inner-wrapper').sliceHeight({'row': '.blocks', 'item': '.inner-wrapper'});
	if($('.projects.item-views').length)
		$('.projects.item-views .item').sliceHeight();
	$('.items-services .item').sliceHeight();
	SetFixedAskBlock();
});
BX.addCustomEvent('onSlideInit', function(eventdata){
	try{
		ignoreResize.push(true);
		if(eventdata)
		{
			var slider = eventdata.slider;
			if(slider.hasClass('top_slider'))
			{
				slider.find('.item').css('opacity',1);
			}
			if(slider.hasClass('small_slider'))
			{
				$('.detail .small-gallery-block .item').sliceHeight({lineheight: -3});
			}
			if(slider.hasClass('big_slider'))
			{
				$('.detail .big_slider .item').sliceHeight({lineheight: -3});
				$(window).resize();
			}
		}
	}
	catch(e){}
	finally{
		ignoreResize.pop();
	}
})
/* End */
;
; /* Start:"a:4:{s:4:"full";s:100:"/bitrix/templates/aspro_next/components/bitrix/catalog.top/products_slider_1/script.js?1589479308449";s:6:"source";s:86:"/bitrix/templates/aspro_next/components/bitrix/catalog.top/products_slider_1/script.js";s:3:"min";s:0:"";s:3:"map";s:0:"";}"*/
BX.addCustomEvent('onSlideInit', function(eventdata){
	try{
		ignoreResize.push(true);
		if(eventdata)
		{
			var slider = eventdata.slider;
			$('.wrapper_block .content_inner .slides').equalize({children: '.item-title'}); 
			$('.wrapper_block .content_inner .slides').equalize({children: '.item_info'}); 
			$('.wrapper_block .content_inner .slides').equalize({children: '.catalog_item'});
		}
	}
	catch(e){}
	finally{
		ignoreResize.pop();
	}
})
/* End */
;
; /* Start:"a:4:{s:4:"full";s:98:"/bitrix/templates/aspro_next/components/bitrix/catalog.top/products_slider/script.js?1543844490449";s:6:"source";s:84:"/bitrix/templates/aspro_next/components/bitrix/catalog.top/products_slider/script.js";s:3:"min";s:0:"";s:3:"map";s:0:"";}"*/
BX.addCustomEvent('onSlideInit', function(eventdata){
	try{
		ignoreResize.push(true);
		if(eventdata)
		{
			var slider = eventdata.slider;
			$('.wrapper_block .content_inner .slides').equalize({children: '.item-title'}); 
			$('.wrapper_block .content_inner .slides').equalize({children: '.item_info'}); 
			$('.wrapper_block .content_inner .slides').equalize({children: '.catalog_item'});
		}
	}
	catch(e){}
	finally{
		ignoreResize.pop();
	}
})
/* End */
;; /* /bitrix/templates/aspro_next/components/bitrix/news.detail/projects/script.js?15438444901326*/
; /* /bitrix/templates/aspro_next/components/bitrix/catalog.top/products_slider_1/script.js?1589479308449*/
; /* /bitrix/templates/aspro_next/components/bitrix/catalog.top/products_slider/script.js?1543844490449*/

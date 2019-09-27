var NavigationBarShowDefault = 2
var NavigationBarHide = 'показать'
var NavigationBarShow = 'скрыть'
 
var hasClass = (
	function (){
	var reCache = {}
	return function (element, className){
		return (reCache[className] ? reCache[className] : (reCache[className] = new RegExp("(?:\\s|^)" + className + "(?:\\s|$)"))).test(element.className)
	}
})()
 

function collapseDiv(idx) {
	 var div = document.getElementById('NavFrame' + idx)
	 var btn = document.getElementById('NavToggle' + idx)
	 if (!div || !btn) return false
	 var isShown = (btn.firstChild.data == NavigationBarHide)
	 btn.firstChild.data = isShown ? NavigationBarShow : NavigationBarHide 
	 var disp = isShown ? 'block' : 'none'
	 for (var child = div.firstChild;  child != null;  child = child.nextSibling)
	   if (hasClass(child, 'NavPic') || hasClass(child, 'NavContent')) 
		  child.style.display = disp
}
 

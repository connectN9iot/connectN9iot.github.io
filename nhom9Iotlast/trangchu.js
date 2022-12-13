const menuBar = document.querySelector(".menu-bar")
menuBar.addEventListener("click",function(){
	menuBar.classList.toggle("active")
	document.querySelector(".menu-items").classList.toggle("active")
})
const toP = document.querySelector(".top")
window.addEventListener("scroll",function(){
	const x = this.pageYOffset;
	if(x>80){toP.classList.add("active")}
	else{toP.classList.remove("active")}
})
//-------------------------
const adressbtn = document.querySelector("#adress-form")
const adressclose = document.querySelector("#adress-close")
adressbtn.addEventListener("click",function(){
	document.querySelector(".adress-form").style.display = "flex"
})
adressclose.addEventListener("click",function(){
	document.querySelector(".adress-form").style.display = "none"
})
//-------------------------
const loginbtn = document.querySelector("#login-form")
const loginclose = document.querySelector("#login-close")
loginbtn.addEventListener("click",function(){
	document.querySelector(".login-form").style.display = "flex"
})
loginclose.addEventListener("click",function(){
	document.querySelector(".login-form").style.display = "none"
})
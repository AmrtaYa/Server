const LoginBtn = document.getElementById('LoginBtn');
const userInput = document.getElementById("userInput");
const pwdInput = document.getElementById("pwdInput");
LoginBtn.onclick = function(){
    var user = userInput.value;
    var pwd = pwdInput.value;
    var url = window.location.href +"?user="+userInput.value+"&pwd="+pwdInput.value+".confirm";
    var req = new XMLHttpRequest();
    console.log(url);
    req.open("POST",url);
    req.send(null);
};


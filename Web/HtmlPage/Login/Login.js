const LoginBtn = document.getElementById('LoginBtn');
const userInput = document.getElementById("userInput");
const pwdInput = document.getElementById("pwdInput");
LoginBtn.onclick = function () {
    var user = userInput.value;
    var pwd = pwdInput.value;
    if (user == "") {
        alert("账号不能为空");
        return;
    }
    if (pwd == "") {
        alert("密码不能为空");
        return;
    }
    var url = window.location.href + "?user=" + userInput.value + "&pwd=" + pwdInput.value + ".confirm";
    var req = new XMLHttpRequest();
    req.open("POST", url);
    
    req.onload = function () {
        var token = parseInt(req.response);
        console.log(req.response);
        if (token == 1) {
            location.href = "http://127.0.0.1:1146/Home/Amrta.html"
        }
        else {
            alert("密码错误");
            pwdInput.value = "";
        }

    };
    req.send(null);
};


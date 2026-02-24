/**
 * 在使用该方法前，必须在html中包含id为message的唯一DOM元素，必须已经包含要指定的CSS样式信息，包含message，message.success，message.error或要指定的message样式
 * @function showMessage
 * @param {string} text 要显示的文本
 * @param {string} type 样式类型，指定显示的样式，包括 success和error
 * @param {boolean} fade 指定是否在显示一段时间后隐藏该消息
 * @param {number} fadeTime 指定从开始显示到隐藏时的时间
 */
export function showMessage(text, type, fade = true, fadeTime = 3000) {
    const msg = document.getElementById('message');
    msg.textContent = text;
    msg.className = `message ${type}`;
    msg.style.display = 'block';
    if (fade) setTimeout(() => {
        msg.style.display = 'none';
    }, fadeTime);
}

//删除登录记录并退出
export function logout() {
    localStorage.removeItem('user');
    window.location.href = 'login.html';
}
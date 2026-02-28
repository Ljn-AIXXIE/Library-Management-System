/**
 * 检查存储的用户状态，如果没有登录，那么返回登录界面
 * @function checkAndBackUser
 * @return {any|null}
 */
export function checkAndBackUser() {
    const user = JSON.parse(localStorage.getItem('user') || '{}');
    if (!user.userId) {
        alert('无登录');
        window.location.href = 'login.html';
        return null;
    }
    return user;
}
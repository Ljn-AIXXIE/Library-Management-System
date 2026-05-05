/**
 * 检查存储的用户状态，如果没有登录，那么返回登录界面
 * @function checkAndBackUser
 * @return {any|null}
 */
export function checkAndBackUser() {
    const user = JSON.parse(localStorage.getItem('user') || '{}');
    if (!user.userId || (user.role !== 'admin' && user.role !== 'super_admin')) {
        alert('需要管理员权限');
        window.location.href = 'login.html';
        return null;
    }
    return user;
}

/**
 * 仅超级管理员可访问的页面
 */
export function checkAndBackSuperAdmin() {
    const user = JSON.parse(localStorage.getItem('user') || '{}');
    if (!user.userId || user.role !== 'super_admin') {
        alert('需要超级管理员权限');
        window.location.href = 'login.html';
        return null;
    }
    return user;
}
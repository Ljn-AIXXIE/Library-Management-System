export function checkAndBackUser() {
    const user = JSON.parse(localStorage.getItem('user') || '{}');
    if (!user.userId || user.role !== 'admin') {
        alert('需要管理员权限');
        window.location.href = 'login.html';
        return null;
    }
    return user;
}
export function checkAndBackUser() {
    const user = JSON.parse(localStorage.getItem('user') || '{}');
    if (!user.userId) {
        alert('无登录');
        window.location.href = 'login.html';
        return null;
    }
    return user;
}
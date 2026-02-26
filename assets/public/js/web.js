export const API_BASE_URL = 'http://localhost:8080';

/**
 * 登录请求
 * @function login
 * @param {string} userId 用户名
 * @param {string} password 密码
 * @return {Promise<any>}
 */
export async function login(userId, password) {
    const response = await fetch(`${API_BASE_URL}/api/auth/login`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ userId, password })
    });
    return await response.json();
}

/**
 * 注册请求
 * @function register
 * @param {string} userId 用户名
 * @param {string} name 名称
 * @param {string} password 密码
 * @return {Promise<any>}
 */
export async function register(userId, name, password) {
    // 发送明文密码到后端，后端会自动加密
    const response = await fetch(`${API_BASE_URL}/api/auth/register`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ userId, name, password })
    });
    return await response.json();
}

/**
 * 获取管理员综合信息请求
 * @function adminStats
 * @return {Promise<any>}
 */
export async function adminStats() {
    const response = await fetch(`${API_BASE_URL}/api/admin/stats`);
    return await response.json();
}

/**
 * 备份数据库请求
 * @function backupDataBase
 * @return {Promise<any>}
 */
export async function adminBackupDataBase() {
    const response = await fetch(`${API_BASE_URL}/api/admin/backupDataBase`);
    return await response.json();
}

/**
 * 处理借书请求
 * @function adminBorrowBook
 * @param {string} userId 用户名
 * @param {string} copyId 图书副本ID
 * @param {string} adminId 管理员ID
 * @return {Promise<any>}
 */
export async function adminBorrowBook(userId, copyId, adminId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/borrow/manual`, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({
            userId: userId,
            borrowCopyId: copyId,
            adminId: adminId
        })
    });
    return await response.json();
}

/**
 * 处理还书请求
 * @function adminReturnBook
 * @param {string} userId 用户名
 * @param {string} copyId 图书副本ID
 * @param {string} adminId 管理员ID
 * @return {Promise<any>}
 */
export async function adminReturnBook(userId, copyId, adminId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/return/manual`, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({
            userId: userId,
            copyBookId: copyId,
            adminId: adminId
        })
    });
    return await response.json();
}

/**
 * 所有图书请求
 * @function adminLoadBooks
 * @return {Promise<any>}
 */
export async function adminLoadBooks() {
    const response = await fetch(`${API_BASE_URL}/api/admin/books`);
    return await response.json();
}

/**
 * 搜索图书请求
 * @function adminSearchBooks
 * @return {Promise<any>}
 */
export async function adminSearchBooks(keyword) {
    const response = await fetch(`${API_BASE_URL}/api/admin/books/search?keyword=${encodeURIComponent(keyword)}`);
    return await response.json();
}

/**
 * 更新图书请求
 * @function adminUpdateBook
 * @param {string} isbn 图书ISBN
 * @param {string} title 书名
 * @param {string} author 作者
 * @param {string} category 类别
 * @param {string} publisher 出版社
 * @param {string} publishDate 出版时间
 * @param {string} price 价格
 * @param {string} pages 页数
 * @param {string} description 简介
 * @return {Promise<any>}
 */
export async function adminUpdateBook(isbn, title, author, category, publisher,
                                      publishDate, price, pages, description) {
    const response = await fetch(`${API_BASE_URL}/api/admin/books/update`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            isbn: isbn,
            title: title,
            author: author,
            category: category,
            publisher: publisher,
            publishDate: publishDate,
            price: price,
            pages: pages,
            description: description
        })
    });
    return await response.json();
}

/**
 * 添加图书请求
 * @function adminAddBook
 * @param {string} isbn 图书ISBN
 * @param {string} title 书名
 * @param {string} author 作者
 * @param {string} category 类别
 * @param {string} publisher 出版社
 * @param {string} publishDate 出版时间
 * @param {string} price 价格
 * @param {string} pages 页数
 * @param {string} description 简介
 * @return {Promise<any>}
 */
export async function adminAddBook(isbn, title, author, category, publisher,
                                   publishDate, price, pages, description) {
    const response = await fetch(`${API_BASE_URL}/api/admin/books/add`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            isbn: isbn,
            title: title,
            author: author,
            category: category,
            publisher: publisher,
            publishDate: publishDate,
            price: price,
            pages: pages,
            description: description
        })
    });
    return await response.json();
}

/**
 * 删除图书请求
 * @function adminDeleteBook
 * @param {string} isbn 图书ISBN
 * @return {Promise<any>}
 */
export async function adminDeleteBook(isbn) {
    const response = await fetch(`${API_BASE_URL}/api/admin/books/delete`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ isbn })
    });
    return await response.json();
}

/**
 * 所有图书副本请求
 * @function adminLoadCopies
 * @param {string} isbn 图书ISBN
 * @return {Promise<any>}
 */
export async function adminLoadCopies(isbn) {
    const response = await fetch(`${API_BASE_URL}/api/admin/copies?isbn=${isbn}`);
    return await response.json();
}

/**
 * 添加图书副本请求
 * @function adminAddCopy
 * @param {string} isbn 图书ISBN
 * @return {Promise<any>}
 */
export async function adminAddCopy(isbn) {
    const response = await fetch(`${API_BASE_URL}/api/admin/copies/add`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ isbn: isbn })
    });
    return await response.json();
}

/**
 * 删除图书副本请求
 * @function adminDeleteCopy
 * @param {string} copyId 图书副本ID
 * @return {Promise<any>}
 */
export async function adminDeleteCopy(copyId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/copies/delete`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ copyId })
    });
    return await response.json();
}

/**
 * 异常详细请求
 * @function adminLoadExceptionDetail
 * @param {string} copyId 图书副本ID
 * @param {string} adminId 管理员ID
 * @param {string} submitTime 提交时间
 * @return {Promise<any>}
 */
export async function adminLoadExceptionDetail(copyId, adminId, submitTime) {
    const response = await fetch(
        `${API_BASE_URL}/api/exception/detail/view?copyId=${copyId}&adminId=${adminId}&submitTime=${submitTime}`
    );
    return await response.json();
}

/**
 * 异常处理请求
 * @function adminLoadExceptionDetail
 * @param {string} copyId 图书副本ID
 * @param {string} adminId 管理员ID
 * @param {string} submitTime 提交时间
 * @return {Promise<any>}
 */
export async function adminHandleExceptionDetail(copyId, adminId, submitTime) {
    const response = await fetch(`${API_BASE_URL}/api/exception/reduce`, {
        method: 'POST',
        headers: {'content-type': 'application/json'},
        body: JSON.stringify({copyId, adminId, submitTime})
    });
    return await response.json();
}

/**
 * 异常处理请求
 * @function adminLoadException
 * @param {string} copyId 图书副本ID
 * @param {string} exceptionType 异常类型
 * @param {string} note 备注
 * @param {string} adminId 管理员ID
 * @return {Promise<any>}
 */
export async function adminHandleException(copyId, exceptionType, note, adminId) {
    const response = await fetch(`${API_BASE_URL}/api/exception/handle`, {
        method: 'POST',
        headers: {'content-type': 'application/json'},
        body: JSON.stringify({
            copyId: copyId,
            exceptionType: exceptionType,
            note: note,
            adminId: adminId
        })
    });
    return await response.json();
}

/**
 * 加载异常请求
 * @function adminLoadExceptionDetails
 * @return {Promise<any>}
 */
export async function adminLoadExceptionDetails() {
    const response = await fetch(`${API_BASE_URL}/api/exception/reduce`);
    return await response.json();
}

/**
 * 搜索异常请求
 * @function adminSearchExceptionDetails
 * @param {string} searchType 搜索类型
 * @param {string} searchUnReduced 搜索未完成的
 * @param {string} searchInput 搜索内容
 * @return {Promise<any>}
 */
export async function adminSearchExceptionDetails(searchType, searchUnReduced, searchInput) {
    const searchUrl = searchInput ?
        `${API_BASE_URL}/api/exception/search?type=${searchType}&unreduced=${searchUnReduced}&keyword=${encodeURIComponent(searchInput)}` :
        `${API_BASE_URL}/api/exception/search?type=${searchType}&unreduced=${searchUnReduced}`;
    const response = await fetch(searchUrl);
    return await response.json();
}

/**
 * 加载所有读者请求
 * @function adminLoadReaders
 * @return {Promise<any>}
 */
export async function adminLoadReaders() {
    const response = await fetch(`${API_BASE_URL}/api/admin/readers`);
    return await response.json();
}

/**
 * 搜索读者请求
 * @function adminSearchReaders
 * @param {string} userId 用户名
 * @return {Promise<any>}
 */
export async function adminSearchReaders(userId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/readers/search?userId=${encodeURIComponent(userId)}`);
    return await response.json();
}

/**
 * 查看读者请求
 * @function adminViewReader
 * @param {string} userId 用户名
 * @return {Promise<any>}
 */
export async function adminViewReader(userId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/readers/detail?userId=${userId}`);
    return await response.json();
}

/**
 * 冻结读者请求
 * @function adminFreezeReader
 * @param {string} userId 用户名
 * @return {Promise<any>}
 */
export async function adminFreezeReader(userId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/readers/freeze`, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({userId})
    });
    return await response.json();
}

/**
 * 解冻读者请求
 * @function adminUnfreezeReader
 * @param {string} userId 用户名
 * @return {Promise<any>}
 */
export async function adminUnfreezeReader(userId) {
    const response = await fetch(`${API_BASE_URL}/api/admin/readers/unfreeze`, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({userId})
    });
    return await response.json();
}
# 📚 Library Management System

一个图书馆管理系统（C++ + SQLite3(后续可能迁移至MySQL)）

---

## 使用说明

本项目依赖以下第三方库。请在编译前，将对应文件下载并放置到项目的 external/include 目录（需要自己创建）下：

- httplib.h[[https://github.com/yhirose/cpp-httplib/blob/master/httplib.h]]
- json.hpp
- sqlite3.h [[https://www.sqlite.org/2026/sqlite-amalgamation-3510200.zip]]

出现以下报错可以通过在main.cpp中添加以下代码到文件开头来解决

```
#error "cpp-httplib doesn't support Windows 8 or lower. Please use Windows 10 or later."
```

```
#define _WIN32_WINNT 0x0A00
#define WINVER 0x0A00
```

user表中role为admin的用户为管理员，role为student的用户为普通用户，需要先创建普通用户，然后通过手动修改数据库中的role字段来设置管理员用户

---

### 环境变量

#### `LIBRARY_LOG_DIR`（可选）

- **作用**：指定日志根目录；建议使用**绝对路径**。设置后，访问 / 业务 / 错误日志分别写入该目录下的 `access`、`business`、
  `error` 子目录，文件按日期命名（如 `2026-04-22.log`）。
- **未设置时**：默认写入 **CMake 配置时的项目源码根目录**下的 `logs` 目录（同样包含上述子目录）。
- **部署注意**：若将可执行文件拷贝到其他机器、或构建机路径在目标环境无效，请设置 `LIBRARY_LOG_DIR` 指向目标机上可写目录（例如
  Linux 下的 `/var/log/<应用名>`）。

**Windows（PowerShell）示例**

```powershell
$env:LIBRARY_LOG_DIR = "D:\logs\library-system"
.\Library_Management_System.exe
```

**Linux / macOS 示例**

```bash
export LIBRARY_LOG_DIR=/var/log/library-system
./Library_Management_System
```

---

### 已经实现的功能

##### 普通学生端

- [x] 查找图书(支持作者，书名，类别模糊查询，isbn精确查询)
- [x] 借阅图书
- [x] 查看借阅记录和状态
- [x] 查看个人信息
- [x] 修改密码
- [x] 借阅统计(在个人信息界面和主页面显示当前借阅数量，历史借阅数量，超期图书数量，即将到期图书数量)
- [x] 归还图书
- [x] 用户注册

##### 管理员端

- [x] 图书管理（添加，编辑（字段可以选择,bookId不可修改），删除图书(删除所有副本)；添加副本，删除副本）
- [x] 冻结/解冻账户
- [x] 查看读者详细信息
- [x] 搜索用户
- [x] 人工借书
- [x] 人工还书
- [x] 提交书籍异常处理报告
- [x] 统计图书总数，读者总数，当前借阅，超期图书
- [x] 文件上传（支持txt文件批量添加,批量添加过程中如果有错误，会生成并自动下载错误报告）
- [x] 处理书籍异常处理报告

##### 其他

- [x] 密码加密
- [x] 黑名单
- [x] 日志（文件分流、按日切分；路径见上文「环境变量」）

---

### 前端待解决的问题

- [ ] 用户登录和修改密码时boby中的密码未加密就传到了后端，应该在前端加密后传到后端

### 后端待解决的问题

--- 

### 🚀 功能扩展规划TODO（后端）

#### 管理员端

- [ ] 导出书籍异常处理报告为excel
- [ ] 对超期图书进行操作
- [ ] 后台应有对距离当天多久为新书的设置
- [ ] 支持批量导入读者信息

#### 1.1 用户体验优化

- [ ] 密码找回功能

#### 1.2 数据备份

- [ ] **数据库备份功能**

#### 1.3 借阅规则完善

- [ ] **续借功能**，读者向管理员发送续借请求，管理员审核通过后，延长借阅时间，读者续借按钮在我的借阅界面中，管理员端审核界面未设计
- [ ] **逾期提醒功能**，弹窗提醒，并显示超期天数和书本基本信息

#### 1.4 推荐系统

推荐系统的仓库地址[[https://github.com/huyunke/Book_Recommendation_System]]

- [ ] **基于自身借阅内容的推荐**
- [ ] **基于相似读者的推荐**
- [ ] **基于热门度 / 趋势的推荐**
- [ ] **基于类别平衡 / 多样性的推荐**
- [ ] **基于新书 / 时间因素的推荐**
- [ ] **混合推荐**

#### 1.5 其他

- [ ] 多线程
- [ ] 从SQLite迁移到MySQL

---
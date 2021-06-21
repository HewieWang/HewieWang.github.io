## 获取当前登录用户
```PHP
use Illuminate\Support\Facades\Auth;
...
Auth::guard('admin')->user();
//Auth::guard('admin')->user()->toArray();
```

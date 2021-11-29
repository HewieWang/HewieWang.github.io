## 获取当前登录用户
```PHP
use Illuminate\Support\Facades\Auth;
...
Auth::guard('admin')->user();
//Auth::guard('admin')->user()->toArray();
```
## Dcat Select
```PHP
 $form->select('模板')->options(function () {
      $mbs = DB::select("select id,title as text from mban where status = '正常'");
      $arr=array();
      foreach ($mbs as $k => $v) {
          $arr[$v->id]= $v->text;
      }
      return $arr;
});

```
## request接收数据
```PHP
use Illuminate\Http\Request;
public function xxx(Request $request)
所有数据:
 $domain=$request->all();
```
## DcatAdmin单页
```PHP
use Dcat\Admin\Layout\Content;
public function index(Content $content){
    return $content
    ->title('title')
    ->body("55555");
}
```

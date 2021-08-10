# C# 发送HttpWebRequest获得网络图片的大小和尺寸

```C#
/// 获取图片的大小和尺寸
        /// </summary>
        /// <param name="aPhotoUrl">图片url</param>
        /// <param name="aSize">图片大小</param>
        /// <param name="aLength">图片尺寸</param>
        private void GetPhotoInfo(string aPhotoUrl, ref string aSize, ref string aLength)
        {
            try
            {
                Uri mUri = new Uri(aPhotoUrl);
                HttpWebRequest mRequest = (HttpWebRequest)WebRequest.Create(mUri);                
                mRequest.Method = "GET";
                mRequest.Timeout = 200;
                mRequest.ContentType = "text/html;charset=utf-8";                 
                HttpWebResponse mResponse = (HttpWebResponse)mRequest.GetResponse();
                Stream mStream = mResponse.GetResponseStream();
                aSize = (mResponse.ContentLength / 1024).ToString() + "KB";
                Image mImage = Image.FromStream(mStream);
                aLength = mImage.Width.ToString() + "x" + mImage.Height.ToString();
                mStream.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(aPhotoUrl + "获取失败");
            }
        }
```

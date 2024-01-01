# 在ESP8266開發板(NodeMCU)上，透過Make.com的webhook trigger服務來發送Line Notify訊息
因為IFTTT突然宣布webhook服務不再免費試用，造成正在修我課程的學生可能無法再利用IFTTT於他們的期末專題作品中，因此在倉促之中找到了IFTTT的替代品：[Make.com](https://make.com)，它也有提供免費試用的webhook trigger服務，因此很容易將原來的IFTTT實驗：「當溫溼度超標時就發送Line Notify訊息」修改成使用make.com提供的服務來完成。
步驟如下：
1. 到make.com網站註冊並建立一個連動Line Notify的webhook trigger服務：(感謝旗標工程師的分享)請參考此hackmd頁面[「使用 Make 取代 IFTTT」](https://hackmd.io/@flagmaker/HkvK0aMDp?utm_source=preview-mode&utm_medium=rec)有詳細步驟(包含測試)。
2. 接下來，先讀一下這個網頁[「ESP8266 NodeMCU HTTPS Requests (Arduino IDE)」](https://randomnerdtutorials.com/esp8266-nodemcu-https-requests/)的內容，了解一下甚麼是HTTPS，以及如何利用Server所提供的憑證來確認網站是可信任的。我的例子是使用Server的Root Certificate來做確認，這裡也有講到如何下載Root Certificate內容並存成單一憑證檔。
3. 下載webhook服務所對應的網址的Root Certificate: 在步驟1最後使用瀏覽器做測試後，利用步驟二的參考網頁內的方法，下載此Server的Root Certificate的單一憑證檔。
4. 準備ESP8266板子(本例採用NodeMCU)，連接上DHTxx感測器(本例是用DHT11，信號腳接在D3)，然後打開本專案程式，依DHT型態與連接腳位修改程式，並將cert.h檔裡的remote host改成你的webhook網址內的主機網域名稱，並用文字編輯器將你的Root Certificate檔案打開，複製所有內容貼到

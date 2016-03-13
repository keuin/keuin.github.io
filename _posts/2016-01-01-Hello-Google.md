---
layout: posts
title: 使用Openshift+Nginx代理Google实现方便跨栏
changeTime: 2016-01-01 08:10:10
---

由于众所周知的原因，在大陆访问Google需要跨栏。但是每次搜索都要启动跨栏软件有点不方便。下面介绍一种使用Openshift来跨栏的方法。首先是效果图：
  
![Chrome + Gogole](http://images2015.cnblogs.com/blog/585442/201601/585442-20160101085838026-457015255.png)
  
点击搜索结果会跳出代理：
  
![Chrome + Google + WOW64](http://images2015.cnblogs.com/blog/585442/201601/585442-20160101085908698-1993570021.png)
  
步骤：
  
一、准备空间：在WEB CONSOLE里创建一个DIY项目，使用SSH登录到后台。
  
二、编译Nginx：从nginx.org上wget下来，tar zxvf解包，同时准备ngx_cache_purge和pcre，和nginx一起放在$OPENSHIFT_DATA_DIR里，解包并编译。
  
三、配置代理：编辑conf/nginx.conf，我的配置文档如下：
  
{% highlight YAML %}
#user nobody;
worker_processes 1;
#error_log logs/error.log;
#error_log logs/error.log notice;
#error_log logs/error.log info;
 
#pid logs/nginx.pid;
 
events {
 worker_connections 1024;
}
 
http {
 include mime.types;
 default_type application/octet-stream;
 
 #log_format main '$remote_addr - $remote_user [$time_local]"$request"'
 # '$status $body_bytes_sent"$http_referer"'
 # '"$http_user_agent""$http_x_forwarded_for"';
 #access_log logs/access.log main;
 port_in_redirect off;
 sendfile on;
 #tcp_nopush on;
 #keepalive_timeout 0;
 keepalive_timeout 65;
#A
 #gzip on;
 upstream google {
 server 74.125.239.112:80 max_fails=3;
 server 74.125.239.113:80 max_fails=3;
 server 74.125.239.114:80 max_fails=3;
 server 74.125.239.115:80 max_fails=3;
 server 74.125.239.116:80 max_fails=3;
}
 server {
 listen 127.10.227.129:8080;
 server_name localhost;
 #server_name google-i51.rhcloud.com;
 #rewrite ^(.*) https://XXX.rhcloud.com$1 permanent;
 #charset koi8-r;
 #access_log logs/host.access.log main;
 #location / {
 # root html;
 # index index.html index.htm;
#}
 
 #error_page 404 /404.html;
 
 # redirect server error pages to the static page /50x.html
#
 #error_page 500 502 503 504 /50x.html;
 location / {
 #proxy_cache one;
 #proxy_cache_valid 200 302 1h;
 #proxy_cache_valid 404 1m;
 proxy_redirect https://www.google.com/ /;
 proxy_cookie_domain google.com XXX.rhcloud.com;
 proxy_pass http://www.google.com;
 proxy_set_header Host "www.google.com";
 proxy_set_header Accept-Encoding "UTF-8";
 proxy_set_header User-Agent $http_user_agent;
 #proxy_set_header Accept-Language "zh-CN";
 #proxy_set_header Cookie "PREF=ID=047808f19f6de346:U=0f62f33dd8549d11:FF=2:LD=zh-CN:NW=1:TM=1325338577:LM=1332142444:GM=1:SG=2:S=rE0SyJh2w1IQ-Maw";
 sub_filter "www.google.com" "XXX.rhcloud.com";
 sub_filter_once off;
}
}
}
{% endhighlight %}
  
其中，`XXX.rhcloud.com`请自行替换为你的地址。
  
四、大功告成：执行sbin/nginx来测试，如果不能运行，请根据日志调试。如果正常运行，可将其加入自启动中，这里不再赘述。
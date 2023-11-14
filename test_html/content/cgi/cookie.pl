#!/usr/bin/perl

# 1998.03.01 リニューアル初版
# 1998.11.08 IE4.0で正常に設定できない問題に対処
# 2004.06.11 曜日を修正。タグを小文字に修正。

#
# Cookieの値を得る
#
&getCookie();
$time = $COOKIE{'KAISUU'};
if ($time eq "") {
    $time = 1;
}
$date = $COOKIE{'HIZUKE'};
if ($date eq "") {
    $date = "???";
}

#
# 書き込むCookieの値を得る
#
$time_new = $time + 1;
($sec, $min, $hour, $mday, $mon, $year) = localtime();
$date_new = sprintf("%04d/%02d/%02d %02d:%02d:%02d",
	$year + 1900, $mon + 1, $mday, $hour, $min, $sec);

#
# ページを表示する。
#
$setcook1 = &setCookie("KAISUU", $time_new);
$setcook2 = &setCookie("HIZUKE", $date_new);
print <<END_OF_DATA;
Content-type: text/html
$setcook1
$setcook2

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Cookie Test</title>
<style>
* { font-size: 10pt; font-family: Consolas; }
</style>
</head>
<body>
<h1>Cookieのテスト</h1>
<hr>
<h2>読み込んだCookieの値(整形前)</h2>
<pre>
HTTP_COOKIE: $ENV{'HTTP_COOKIE'}
</pre>
<hr>
<h2>読み込んだCookieの値(整形後)</h2>
<div>これまでの訪問回数：$time</div>
<div>前回の訪問日：$date</div>
<hr>
<h2>書き込んだCookieの値</h2>
<div>$setcook1</div>
<div>$setcook2</div>
<hr>
<p>再表\示してみてください。一度ブラウザを終了させても、値が保存されています。</p>
</body>
</html>
END_OF_DATA

#
# Cookieの値を読み出す
#
sub getCookie {
    local($xx, $name, $value);
    foreach $xx (split(/; */, $ENV{'HTTP_COOKIE'})) {
        ($name, $value) = split(/=/, $xx);
        $value =~ s/%([0-9A-Fa-f][0-9A-Fa-f])/pack("C", hex($1))/eg;
        $COOKIE{$name} = $value;
    }
}

#
# Cookieに値を書き込むためのSet-Cookie:ヘッダを生成する
#
sub setCookie {
    local($tmp, $val);
    $val = $_[1];
    $val =~ s/(\W)/sprintf("%%%02X", unpack("C", $1))/eg;
    $tmp = "Set-Cookie: ";
    $tmp .= "$_[0]=$val; ";
    $tmp .= "expires=Tue, 1-Jan-2030 00:00:00 GMT;";
    return($tmp);
}

#
# Cookieを削除するためのSet-Cookie:ヘッダを生成する
#
sub clearCookie {
    $tmp = "Set-Cookie: ";
    $tmp .= "$_[0]=xx; ";
    $tmp .= " expires=Tue, 1-Jan-1980 00:00:00 GMT;";
    return($tmp);
}
# 概要

ckw-mod (https://github.com/ckw-mod/ckw-mod) のテストです。

# 準備

テストには [googletest](http://code.google.com/p/googletest/) を利用しています。
バージョン 1.6.0 を `gtest` ディレクトリに配置してください。

ckw のソースに patches 以下を適用して、テストが private メンバにアクセス
できるようにしてください。

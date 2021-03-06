# Peryaudo Kayou Typing Patcher

Copyright(C) 2010 Peryaudo. All rights reserved.

## はじめに

Peryaudo Kayou Typing Patcherは、不完全ながらハンゲームの歌謡タイピング劇場を、
VMWare等の仮想マシン上で起動できるようにするツールです。

## 対応環境

いかなる環境においても完全には動作しません。(以下の注意事項を参照)

VMWare上のWindows XP SP3にて動作確認しています。

おそらく98系のOSでは動作しないと思います。
(もっとも今の時代VMWareで98系のOSを使ってゲームをしようという人はあまり居ないと思いますが…)

NT系のOSでは基本的に動作はすると思いますが、Windows Vistaや7では、
右クリックから管理者権限で実行しないと動作しない可能性があります。

## 使い方

ダブルクリックで起動するとタスクバーに常駐します。緑色のPrアイコンです。

通知領域のPrアイコンを左右どちらかのクリック、で終了です。

あとは常駐させた状態で、通常の手順でInternet Explorerから歌謡タイピング劇場を起動してください。

## 注意事項・既知の問題

起動中のプロセスのメモリを動的に書き換える為に、一部のアンチウイルスソフトウエア等が
インストールされている環境においては動作しない可能性があります。

また、基本的に歌謡タイピング劇場の現在のバージョン以外には対応していない為、
将来的に動作しなくなる可能性があります。

また根本的な問題として、公式に述べられている訳ではないため、
簡単に調べた上での個人の感想ではあるのですが、
歌謡タイピング劇場が仮想マシン上での起動を禁止している理由は
アンチチート等ではなく、単に互換性上の問題に依る物です。

その証拠として、VMWare上での歌謡タイピング劇場の動作は非常に不安定で、謎の理由で落ちる事が頻繁にあります。
特にホストとゲストの切り替え等を頻繁に行なった場合に落ちる事が多いようですが、
詳しい検証等はしていないためそうと断言する事はできません。

## ソースコード、ライセンスなど

ソースコード一式はsrc.zipの中にあります。コンパイル環境はMinGW + MSYSです。
といってもリソースファイル、アイコンファイル、Cソースコードしか無いためVCなどでも容易にコンパイルできると思います。
ライセンスはMITライセンスとします。

## 更新履歴

2010/05/16 初版

2010/07/08 歌謡タイピング劇場自体のアップデートにつき動作しなくなったのを修正

## 連絡先・ブログ

peryaudoのプログラミングとか
http://d.hatena.ne.jp/peryaudo/

メール: peryaudo@gmail.com

何か連絡等がある際はメールないしはブログのコメント欄等にお願いします。

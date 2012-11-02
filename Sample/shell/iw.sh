#!/bin/bash
 
my_input_path="/mnt/disk1/comix/automatic/"
my_script_path=""
 
# fileが移動してくるのを待つ
while inotifywait -e moved_to ${my_input_path}; do
   while true; do
       # 別のファイルを処理中の場合は待つ
       PID=`pgrep -f "{実行するコマンド}"`
       if [ -z $PID ]; then
          # zipファイルのリストを取得
          LIST=`ls ${my_input_path}*.{txt,html} 2>/dev/null | head -n 1`
          if [ -z "$LIST" ]; then
            # 無かったら出る
            break
          fi
 
          cd ${my_script_path}
          {実行するコマンド} "$LIST"
          //不要になったファイルを削除
          rm -f "$LIST"
       fi
       sleep 20
   done
done

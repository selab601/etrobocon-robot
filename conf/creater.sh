#!/bin/bash

#
# パス指定
#
base_dir_path=/vagrant/src/ET2017
conf_dir_path=$base_dir_path/conf
parts_dir_path=$conf_dir_path/parts
tmp_file_path=$conf_dir_path/tmp.txt

ignore_dir_names=( ".git" "conf" )

if [ -e $tmp_file_path ]; then
  rm $tmp_file_path
fi

#
# オブジェクトファイル名取得
#
file_names=()
for file_path in `\find $base_dir_path -type f`; do
  file_names+=($(basename ${file_path}))
done
# 重複削除
file_names=(`echo ${file_names[*]} | sed 's/\s/\n/g' | sort | uniq`)

obj_names=()
for file_name in ${file_names[@]}; do
  if [[ $file_name =~ .*\.cpp$ ]]; then
    obj_names+=(${file_name%.cpp}.o)
  fi
done

# ディレクトリ名取得
dir_names=()
for dir_path in `\find $base_dir_path -type d`; do
  dir_names+=($dir_path)
done

#
# app.cfg の生成
#
for name in ${obj_names[@]}; do
   echo "ATT_MOD(\"$name\");" >> $tmp_file_path
done

cat $parts_dir_path/app.cfg.header $tmp_file_path > $conf_dir_path/app.cfg

rm $tmp_file_path

#
# Makefile.inc の生成
#
echo "APPL_CXXOBJS += \\" > $tmp_file_path
for name in ${obj_names[@]}; do
  if [ $name = "app.o" ]; then continue; fi
  echo "    $name \\" >> $tmp_file_path
done

cat $tmp_file_path $parts_dir_path/Makefile.inc.footer > $conf_dir_path/Makefile.inc

rm $tmp_file_path

#
# Makefile.app の生成
#
echo "SRCDIRS  += \\" > $tmp_file_path
for dir_name in ${dir_names[@]}; do
  continue_flg=0
  for ignore_dir_name in ${ignore_dir_names[@]}; do
    # ルートディレクトリのみ比較
    if [[ $ignore_dir_name = $(echo ${dir_name#$base_dir_path} | cut -d "/" -f2) ]]; then
      continue_flg=1
      break
    fi
  done
  if [ $continue_flg = 1 ]; then continue; fi
  echo "            @(APPLDIR)${dir_name#$base_dir_path} \\" >> $tmp_file_path
done

cat $parts_dir_path/Makefile.app.header $tmp_file_path $parts_dir_path/Makefile.app.footer > $conf_dir_path/Makefile.app

rm $tmp_file_path

echo "Configurations updated!"

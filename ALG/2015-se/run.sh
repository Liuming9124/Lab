#!/bin/bash

# 取得當前目錄
currentDir=$(pwd)

# 讀取檔案路徑與執行次數
read -p "add file name: " filePath
read -p "add execution count: " executionCount
executable="$(realpath ./a.out)"
filePath="$(realpath "$filePath")"

# 逐行讀取檔案中的命令
while IFS= read -r command; do
    (
        # 設置當前目錄
        cd "$currentDir"

        # 建立完整命令
        fullCommand="$executable $executionCount $command"
        echo "running: $fullCommand"

        # 執行命令
        eval "$fullCommand"
    ) &
done < "$filePath"

# 等待所有後台進程完成
wait

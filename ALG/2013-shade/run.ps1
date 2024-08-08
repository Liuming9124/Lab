# 1. 詢問執行檔名稱
$executable = Read-Host "add executable name"
# 2. 詢問執行次數
$executionCount = Read-Host "add execution count"
# 3. 讀取 run.txt 檔案
$filePath = "test.txt"
$commands = Get-Content -Path $filePath

foreach ($command in $commands) {
    $fullCommand = "$executable $executionCount $command"
    Write-Host "running: $fullCommand"
    Invoke-Expression $fullCommand
}

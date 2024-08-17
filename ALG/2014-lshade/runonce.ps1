# Script uses file type for UTF8-BOM
# get the current directory
$currentDir = Get-Location

$filePath = Read-Host "add file name"
$executionCount = Read-Host "add execution count"
$executable = (Resolve-Path ".\a.exe").Path
$filePath = (Resolve-Path $filePath).Path

$commands = Get-Content -Path $filePath

foreach ($command in $commands) {
    # set the current directory
    Set-Location $currentDir
    
    # create the full command
    $fullCommand = "& `"$executable`" $executionCount $command"
    Write-Host "running: $fullCommand"
    
    # run the command
    Invoke-Expression $fullCommand
}

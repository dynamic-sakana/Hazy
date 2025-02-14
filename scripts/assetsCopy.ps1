# 获取当前脚本所在目录
$curDir = $PSScriptRoot

# 获取输出目录
$debugOutDir = Join-Path -Path $curDir -ChildPath "../output/bin/debug"
$releaseOutDir = Join-Path -Path $curDir -ChildPath "../output/bin/release"

# 获取assets的目录
$assets = Join-Path -Path $curDir -ChildPath "../assets"

Copy-Item -Path $assets -Destination $debugOutDir -Recurse
Copy-Item -Path $assets -Destination $releaseOutDir -Recurse


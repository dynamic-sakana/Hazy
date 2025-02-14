# 获取当前脚本所在目录
$curDir = $PSScriptRoot
$vendorDir = Join-Path -Path $curDir -ChildPath "../vendor/"

# 获取输出目录（即构建生成的文件所在的目录）
$debugOutDir = Join-Path -Path $curDir -ChildPath "../output/bin/debug"
$releaseOutDir = Join-Path -Path $curDir -ChildPath "../output/bin/release"

# 递归查找此文件夹下的所有DLL文件
$dllFiles = Get-ChildItem -Path $vendorDir -Filter *.dll -Recurse -File | Where-Object {
    $_.FullName -like "*x64*" -and $_.Name -notlike "lib*.dll"
    # 只复制x64平台的DLL文件，并且不复制lib*.dll
}

# 复制DLL文件到bin目录
foreach ($dllFile in $dllFiles) {
    try {
        Copy-Item -Path $dllFile.FullName -Destination $debugOutDir -Force
        Copy-Item -Path $dllFile.FullName -Destination $releaseOutDir -Force
    } catch {
        Write-Error "复制DLL文件失败：$($_.Exception.Message)"
    }
}
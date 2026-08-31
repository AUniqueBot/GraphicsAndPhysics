@echo off
setlocal

set "TARGET=%~1"

if "%TARGET%"=="" (
    echo Usage: GenerateMeta.bat "path\to\folder"
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "$rng = [System.Security.Cryptography.RandomNumberGenerator]::Create();" ^
    "$bytes = New-Object byte[] 8;" ^
    "Get-ChildItem -LiteralPath '%TARGET%' -File | ForEach-Object {" ^
        "$meta = $_.FullName + '.meta';" ^
        "if (-not (Test-Path -LiteralPath $meta)) {" ^
            "$rng.GetBytes($bytes);" ^
            "$guid = [BitConverter]::ToUInt64($bytes, 0);" ^
            "$json = @{" ^
                "version = 1;" ^
                "guid = $guid" ^
            "} | ConvertTo-Json;" ^
            "[System.IO.File]::WriteAllText($meta, $json);" ^
        "}" ^
    "};" ^
    "$rng.Dispose()"

endlocal
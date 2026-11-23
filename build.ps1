# Uso (desde la raiz del repo, en PowerShell):
#   .\build.ps1            -> compila Release + corre tests
#   .\build.ps1 -Debug     -> compila Debug
#   .\build.ps1 -NoTests   -> compila sin correr tests
param(
    [switch]$Debug,
    [switch]$NoTests
)

$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$config = if ($Debug) { "Debug" } else { "Release" }
$preset = if ($Debug) { "vs2022-debug" } else { "vs2022-release" }

# Configurar solo si la carpeta build aun no existe (la primera vez baja JUCE y Catch2).
if (-not (Test-Path "build\CMakeCache.txt")) {
    cmake --preset vs2022
}
else {
    # Reconfigurar es rapido y detecta archivos .cpp nuevos agregados al CMakeLists.
    cmake --preset vs2022 | Out-Null
}

cmake --build --preset $preset

if (-not $NoTests) {
    ctest --test-dir build -C $config --output-on-failure
}

$vst3 = Get-ChildItem -Path "build" -Recurse -Filter "MasterZaster.vst3" -Directory -ErrorAction SilentlyContinue |
    Select-Object -First 1
if ($vst3) { Write-Host "VST3 listo en: $($vst3.FullName)" }

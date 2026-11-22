# Compilar MasterZaster en Windows

Requisitos: Windows 10/11 de 64 bits, Visual Studio 2022 con la carga de trabajo
"Desarrollo para el escritorio con C++", CMake 3.22 o superior y Git.
La primera configuración descarga JUCE y Catch2 (necesita Internet y tarda unos minutos).

## Opción A: línea de comandos (PowerShell)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

El VST3 queda en:
`build\MasterZaster_artefacts\Release\VST3\MasterZaster.vst3`

## Opción B: Visual Studio

*Archivo → Abrir → Carpeta...* y elige la carpeta del repo. Visual Studio detecta
`CMakeLists.txt` y `CMakePresets.json`. Elige la configuración `vs2022` y compila en Release.

## Instalar para Ableton Live

Copia la carpeta `MasterZaster.vst3` a `C:\Program Files\Common Files\VST3\` (requiere
administrador). En Ableton: *Preferencias → Plug-ins* y activa VST3 y "Rescan" si hace falta.

Alternativa sin administrador: define una carpeta de VST3 personalizada en las
preferencias de Ableton y copia ahí el plugin.

## Opciones de CMake

| Opción | Por defecto | Efecto |
|---|---|---|
| `MZ_BUILD_TESTS` | ON | Compila los tests del núcleo |
| `MZ_BUILD_STANDALONE` | OFF | Añade una versión Standalone para probar sin DAW |
| `MZ_COPY_PLUGIN_AFTER_BUILD` | OFF | Copia el VST3 a la carpeta del sistema al compilar (requiere administrador) |

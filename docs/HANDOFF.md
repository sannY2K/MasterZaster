# MasterZaster — Prompt de continuación

Vas a continuar el desarrollo de **MasterZaster**, un plugin VST3 profesional (C++, JUCE, CMake, Visual Studio 2022) para Windows 10/11 de 64 bits y Ableton Live: un "ingeniero de mezcla virtual" que compara mi mezcla con canciones de referencia, diagnostica problemas, sugiere EQ y funciona sin conexión (IA local con ONNX Runtime en V3).

Otra sesión de Claude ya trabajó en esto y se agotó la cuota. Retómalo sin repetir lo hecho.

## Paso 1: leer antes de escribir código
1. En mi GitHub (repo `MasterZaster`) lee `docs/MASTERZASTER_SPEC.md` (fuente de verdad: 21 secciones, versiones V1 a V4), esta sección "ESTADO ACTUAL", `README.md`, `CMakeLists.txt` y `Source/`.
2. Revisa los últimos commits e issues abiertos.
3. Resúmeme en pocas líneas: versión actual, módulos terminados, a medias y próximo paso. Si algo no coincide con lo que recuerdo, espera mi confirmación.

## Reglas innegociables
- Nada simulado ni interfaces vacías.
- No declares terminado un módulo que no esté implementado y probado. No afirmes haber compilado, probado o publicado lo que no hayas hecho. Si no puedes compilar, dilo y dame instrucciones para compilar yo y pasarte los errores.
- Versiones en orden (V1 Core, V2 Intelligence, V3 AI, V4 Professional); no avances sin verificar la estabilidad de la anterior.
- Audio en tiempo real separado del análisis, carga de archivos e inferencia; sin asignaciones ni bloqueos en el hilo de audio.
- Procesamiento no destructivo (PREVIEW, APPLY, BYPASS, RESET, UNDO, A/B, HISTORY); sugerencias siempre opcionales.
- Las mediciones DSP funcionan aunque la IA esté desactivada.
- La referencia se reproduce desde un sistema independiente, sin filtrarse a la salida.
- Los perfiles se guardan independientes del archivo original; si falta, se pide relocalizarlo solo al necesitar reproducirlo.

## Cómo trabajar
- Entorno: Windows, PowerShell, Visual Studio 2022. Dame comandos en PowerShell.
- Pasos pequeños y compilables, un commit por cambio coherente.
- Al terminar la sesión (o si ves que la cuota se agota): actualiza "ESTADO ACTUAL", haz commit y push, y dime qué pegar en la siguiente sesión.
- Si algo de la especificación es inviable o ambiguo, avísame y propón alternativa antes de construir.
- La conexión de GitHub de Claude es de solo lectura (403 al escribir): los archivos se entregan para subirlos con GitHub Desktop.

## Primera acción
Haz el Paso 1, dame el resumen y continúa con el siguiente pendiente de la versión actual.

## ESTADO ACTUAL (actualizar en cada sesión)
- Versión en curso: V1 (Core)
- Hito M0 (esqueleto): el VST3 compila y Ableton Live lo reconoce (confirmado por el usuario). Pendiente confirmar: workflow de GitHub Actions en verde y resultado de `pluginval`.
- UI: estilo futurista aplicado (`Source/Plugin/UI/MZLookAndFeel.h` + editor). Solo hay un knob de ganancia de salida; aún no hay medidores ni espectro.
- Hito M1 (núcleo de medición), en curso:
  - Implementado y probado con g++ 13 (-Wall -Wextra -Wpedantic -Wconversion -Wshadow, sin advertencias; 17 pruebas OK con un shim local de Catch2): `LevelAccumulator` (pico, RMS, factor de cresta) y `StereoAccumulator` (correlación, anchura). Tests en `Tests/MeteringTests.cpp`.
  - Pendiente de verificar: compilación con MSVC y ejecución con Catch2 real (CI o Visual Studio); `ctest` en verde.
  - Pendiente de implementar: LUFS (momentáneo, corto plazo, integrado), LRA y True Peak (BS.1770-4 / EBU R128; el plan propone libebur128, ver decisión abierta), y conectar los medidores al `PluginProcessor` sin asignaciones en el hilo de audio.
- Decisión abierta: LUFS/True Peak con libebur128 (MIT, dependencia vía FetchContent, como dice el plan) o con implementación propia en MZCore (sin red, verificable en Linux). Pendiente de que el usuario elija.
- Probado en Ableton Live: solo reconocimiento del plugin. Sin prueba de audio, CPU ni guardado de sesión.
- Problemas conocidos: el 22/11/2026 se borró por error el contenido del repo con un commit ("Nosé"); se restauró desde el historial. Vigilar que no falte ningún archivo.

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

## Primera acción
Haz el Paso 1, dame el resumen y continúa con el siguiente pendiente de la versión actual.

## ESTADO ACTUAL (actualizar en cada sesión)
- Versión en curso: V1 (Core)
- Fase: especificación y planificación; todavía no hay código implementado ni probado.
- Pendiente inmediato: 1) análisis de viabilidad, 2) arquitectura, dependencias y plan de V1, 3) estructura del repo y CMakeLists.txt (JUCE, VST3, VS2022), 4) motor de análisis e interfaz inicial.
- Compilado: no
- Probado en Ableton Live: no
- Problemas conocidos: ninguno registrado

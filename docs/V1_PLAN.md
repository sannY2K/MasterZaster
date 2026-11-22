# MasterZaster V1 — Viabilidad, arquitectura y plan de implementación

Estado: **planificación**. No hay código, no se ha compilado nada y no se ha probado nada todavía.

## 1. Viabilidad técnica

**Conclusión: V1 es viable** con C++, JUCE y CMake. Todo lo que pide V1 (FFT, RMS/Peak/LUFS/True Peak, importación WAV/AIFF, perfiles, biblioteca, A/B, health check básico) es DSP y gestión de archivos estándar. No requiere IA.

### Limitaciones reales del formato VST3 en Ableton Live (afectan al diseño)

| Tema | Realidad | Decisión de diseño |
|---|---|---|
| Un plugin solo ve su propia entrada | Una instancia en una pista solo mide esa pista. La mezcla completa solo se ve en el Master (o en un grupo, para ese grupo). | El "Mix Health Check" y el spectrum matching tienen sentido sobre todo en Master/grupos. En pistas sueltas se limitan a gain staging de esa pista. |
| Analizar "todas las pistas" desde una instancia | No es posible sin comunicación entre instancias. | Fuera de V1. En V2+ se puede evaluar comunicación entre instancias (memoria compartida) o un modo de sidechain. |
| Reproducir la referencia dentro del plugin | Un plugin de efecto puede generar audio y enviarlo a su salida. | El reproductor de referencia vive dentro del plugin. En modo A/B la salida es **mezcla o referencia, nunca suma** (conmutación con crossfade corto). Así no hay duplicación ni fuga de la referencia a la salida. |
| Transporte de Ableton | El reproductor interno es independiente del transporte de Live. | La referencia tiene su propio play/stop/posición. Sincronizar con el transporte queda como mejora posterior. |
| Latencia | V1 no aplica procesamiento con latencia (solo ganancia y A/B). | Latencia reportada: 0. |
| Frecuencias de muestreo / buffers | Varían por sesión. | La referencia se remuestrea al sample rate del host en un hilo de fondo; los análisis dependen de la frecuencia de muestreo real. |

### Riesgos principales

1. **Licencia de JUCE.** JUCE se distribuye bajo AGPLv3 o licencia comercial. Para vender MasterZaster hay que revisar y contratar la licencia comercial que corresponda (consultar términos vigentes en juce.com). Para desarrollo y pruebas personales no hay problema.
2. **Precisión de LUFS/True Peak.** Hay que implementar ITU-R BS.1770-4 / EBU R128 correctamente (filtro K-weighting, gating, oversampling 4x para True Peak). Se usa la librería libebur128 (MIT) como base y se valida con señales de prueba conocidas.
3. **Comparación espectral engañosa.** Dos canciones distintas no deberían tener espectros idénticos. La comparación se hará con loudness igualado y siempre presentada como diferencia orientativa, no como error.
4. **Validación sin Ableton en este entorno.** Yo no puedo abrir Ableton ni compilar con Visual Studio aquí. Cada hito se marcará como "implementado" o "probado" por separado, y las pruebas en Ableton las ejecutas tú.

## 2. Qué puedo verificar y qué no

| Verificación | Quién / dónde |
|---|---|
| Tests unitarios del núcleo DSP (FFT, LUFS, bandas, perfiles) | Se pueden ejecutar en Linux (CI y entorno de trabajo) si el núcleo no depende de JUCE |
| Compilación VST3 en Windows | GitHub Actions (runner Windows) y tú con Visual Studio 2022 |
| Validación del VST3 | `pluginval` (herramienta de Tracktion) en CI |
| Reconocimiento y uso en Ableton Live, A/B, guardado de sesión, CPU en sesión real | Solo tú, con checklist en `docs/TEST_CHECKLIST.md` |

## 3. Arquitectura

Principio clave: **el núcleo (`MZCore`) no depende de JUCE**. Contiene todo el DSP y análisis en C++ estándar, por lo que se puede probar en cualquier plataforma. El plugin JUCE es una capa fina que lo conecta al audio, la UI y el guardado de estado.

```
MasterZaster/
├─ CMakeLists.txt
├─ cmake/                      # utilidades CMake (FetchContent de dependencias)
├─ docs/                       # SPEC, HANDOFF, este plan, checklist de pruebas
├─ Source/
│  ├─ Core/                    # MZCore: sin JUCE, testeable
│  │  ├─ Dsp/                  # FFT wrapper, ventanas, bandas 1/3 y 1/6 de octava
│  │  ├─ Metering/             # RMS, Peak, True Peak, LUFS (M/S/I), LRA, crest factor, correlación, anchura
│  │  ├─ Analysis/             # ReferenceAnalyzer: espectro promedio, secciones, perfil
│  │  ├─ Profile/              # ReferenceProfile (struct versionada) + serialización
│  │  ├─ Matching/             # SpectrumMatchingEngine (diferencias por banda con loudness igualado)
│  │  ├─ Health/               # MixHealthCheck (reglas basadas en mediciones, 3 estados)
│  │  └─ GainStaging/          # GainStagingAnalyzer
│  ├─ Plugin/                  # capa JUCE
│  │  ├─ PluginProcessor.*     # AudioProcessor: hilo de audio, parámetros, estado
│  │  ├─ PluginEditor.*        # UI
│  │  ├─ ReferencePlayer.*     # reproducción de la referencia (buffer en memoria)
│  │  ├─ ReferenceLibraryManager.*  # biblioteca en disco, búsqueda, etiquetas
│  │  ├─ AnalysisThread.*      # análisis pesado y carga de archivos fuera del hilo de audio
│  │  └─ UI/                   # componentes: SpectrumView, Meters, LibraryPanel, HealthPanel
│  └─ ...
├─ Tests/                      # Catch2: Core (sin JUCE) + algunos tests del plugin
└─ .github/workflows/          # CI: build Windows + tests + pluginval + artefacto VST3
```

### Hilos y comunicación

- **Hilo de audio:** mide (peak, RMS, acumuladores de LUFS, buffer circular para FFT), aplica ganancia y conmutación A/B. Sin asignaciones de memoria, sin locks, sin E/S de archivos.
- **Hilo de análisis (background):** importa WAV/AIFF, decodifica, remuestrea, calcula el perfil de referencia y la FFT de la mezcla en vivo, actualiza el health check.
- **Hilo de UI:** dibuja; lee resultados mediante atomics y copias de instantáneas inmutables.
- **Comunicación:** `juce::AbstractFifo`/colas lock-free audio → análisis; `std::atomic` para medidores; punteros intercambiables atómicamente para publicar resultados de análisis y la referencia cargada (RCU simple), de modo que el hilo de audio nunca espera.

### Estado y persistencia

- **Parámetros automatizables** (ganancia de salida, A/B ref/mix, loudness match on/off, promedio espectral, etc.): `AudioProcessorValueTreeState`.
- **Estado no paramétrico** (referencia seleccionada, vista activa): `ValueTree` guardado en `getStateInformation` para que Ableton lo conserve con el proyecto.
- **Biblioteca de referencias:** carpeta en `%APPDATA%\MasterZaster\Library\`, un archivo de perfil por referencia (formato binario o JSON comprimido, con número de versión y checksum) más un índice. El perfil guarda el análisis **independiente del archivo de audio**; guarda la ruta original y un identificador del archivo. Si el archivo falta, el perfil sigue usable para comparar; solo se pide relocalizarlo cuando se quiera **reproducir**.

### Reproductor de referencia (A/B)

- La referencia se decodifica y remuestrea al sample rate del host en el hilo de fondo y queda en memoria (float, estéreo).
- A/B: `MIX` deja pasar la entrada; `REF` emite la referencia y silencia la entrada; el cambio usa un crossfade breve para evitar clics.
- Igualación de loudness: ganancia calculada comparando LUFS integrado de la referencia con el LUFS de la mezcla en ventana; con límites y opción de desactivarla.
- La entrada y la referencia **nunca se suman**.

## 4. Dependencias

| Dependencia | Uso | Licencia | Cómo se integra |
|---|---|---|---|
| JUCE (8.x) | Framework de plugin, UI, I/O de audio, FFT | AGPLv3 / comercial | CMake `FetchContent` o submódulo |
| VST3 SDK | Formato VST3 | MIT (SDK reciente) | Incluido por JUCE |
| libebur128 | LUFS, LRA, True Peak | MIT | `FetchContent` |
| Catch2 | Tests unitarios | BSL-1.0 | `FetchContent` |
| pluginval | Validación del VST3 en CI | GPLv3 (herramienta externa) | Descarga en el workflow |
| ONNX Runtime | IA local | MIT | **No en V1** (V3) |

Herramientas: Visual Studio 2022 (MSVC), CMake ≥ 3.22, Git. Verificar las versiones exactas al crear el `CMakeLists.txt`.

## 5. Alcance de V1 (según la especificación)

Incluye: VST3 para Windows/Ableton, UI inicial, importación WAV/AIFF, Reference Analyzer, FFT, Spectrum Matching, RMS/Peak/LUFS, Gain Staging, A/B, Mix Health Check básico, guardado de perfiles, Reference Library inicial, guardado de parámetros.

**Mix Health Check básico (V1):** reglas simples sobre mediciones reales (por ejemplo headroom, True Peak por encima de un umbral, correlación estéreo baja, energía por banda respecto a la referencia con loudness igualado). Cada aviso muestra el valor medido que lo origina y uno de los tres estados: Correcto, Necesita atención, Problema potencial. Los umbrales serán configurables y se documentarán.

**Fuera de V1:** Smart EQ, Match Score, comparación por secciones en la UI, historial, IA, módulos de procesamiento (EQ, compresor, etc.). El perfil ya almacenará los datos por secciones para que V2 no obligue a re-analizar.

## 6. Hitos de V1

Cada hito se cierra solo cuando está **implementado y probado** y el proyecto sigue compilando.

| Hito | Contenido | Criterio de cierre |
|---|---|---|
| M0 | Esqueleto CMake + plugin VST3 vacío que pasa audio | Compila en Windows (CI + local), `pluginval` OK, Ableton lo reconoce |
| M1 | Núcleo de medición: Peak, RMS, True Peak, LUFS, crest, correlación, anchura | Tests con señales de referencia (senoides, ruido, señales con LUFS conocido) |
| M2 | Analizador FFT en vivo + vista de espectro | Test de FFT con tonos conocidos; revisión visual en Ableton |
| M3 | Importación WAV/AIFF y Reference Analyzer + perfil | Tests con archivos sintéticos; perfil se guarda y se recarga idéntico |
| M4 | Reference Library (importar, nombrar, género, etiquetas, comentarios, buscar, duplicar, eliminar, relocalizar) | Tests del gestor; persistencia tras reiniciar |
| M5 | Reproductor de referencia + A/B + loudness match | Prueba en Ableton en pista, grupo y Master: sin fuga de referencia a la salida |
| M6 | Spectrum Matching (curva ref vs mezcla, diferencias por banda) | Tests de la lógica de diferencias; revisión visual |
| M7 | Gain Staging + Mix Health Check básico | Tests de reglas con casos sintéticos; avisos con valores medidos |
| M8 | Persistencia de parámetros y estado en proyectos de Ableton | Guardar/cerrar/abrir proyecto conserva todo |
| M9 | CI completo, README, docs de instalación y compilación, checklist de pruebas | Workflow verde y artefacto VST3 descargable |

**Salida de V1:** checklist de `docs/TEST_CHECKLIST.md` completada en una sesión real, con CPU y memoria medidas, y limitaciones documentadas. No se pasa a V2 sin eso.

## 7. Decisiones pendientes del usuario

1. **Imagen de referencia estética:** la especificación menciona una imagen adjunta, pero no la he recibido. Hace falta para el diseño de la interfaz.
2. **Licencia de JUCE:** decidir si el objetivo comercial exige licencia comercial desde ya (afecta solo a la distribución, no al desarrollo).
3. **Escritura en GitHub:** mi conexión solo lee el repo. Hasta darle permiso de escritura, los archivos del repo se suben con GitHub Desktop.
4. **Entorno de compilación local:** confirmar que tienes Visual Studio 2022 con la carga de trabajo "Desarrollo para el escritorio con C++" y CMake.

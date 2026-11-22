# MASTERZASTER V3 — AI MIXING ENGINEER & REFERENCE ANALYZER
## Desarrollo de un plugin VST3 profesional para Windows y Ableton Live

Quiero desarrollar un plugin profesional VST3 llamado **MasterZaster**, diseñado para Windows 10 y Windows 11 de 64 bits, con compatibilidad optimizada para Ableton Live.

No quiero un prototipo, una demostración visual ni funciones simuladas. Quiero desarrollar un producto real, funcional, estable, modular y con posibilidades de comercialización.

Debes asumir los roles de:
- Ingeniero de software especializado en C++.
- Desarrollador de plugins VST3 y JUCE.
- Ingeniero profesional de mezcla y mastering.
- Especialista en DSP y análisis espectral.
- Especialista en inteligencia artificial aplicada al audio.
- Diseñador UI/UX de plugins profesionales.
- Ingeniero de pruebas y control de calidad.

# 1. IDENTIDAD Y TECNOLOGÍA

- Nombre: MasterZaster.
- Formato: VST3.
- Sistema operativo: Windows 10 y 11 de 64 bits.
- DAW principal: Ableton Live.
- Lenguaje: C++.
- Framework: JUCE.
- Compilación: CMake y Visual Studio 2022.
- IA: arquitectura modular, ONNX Runtime y modelos neuronales locales cuando sean viables.
- Repositorio: mi cuenta personal de GitHub.

Prioriza estabilidad, eficiencia, baja latencia y compatibilidad con sesiones profesionales.

# 2. CONCEPTO PRINCIPAL

MasterZaster debe comportarse como un ingeniero de mezcla virtual inteligente.

Quiero cargar una canción profesional ya masterizada y utilizarla como referencia para trabajar en mi propia producción.

El plugin debe analizar ambas señales y detectar diferencias en:

- Balance tonal.
- Distribución de frecuencias.
- Dinámica.
- Loudness.
- Transitorios.
- Imagen estéreo.
- Energía espectral.
- Densidad sonora.
- Graves, medios y agudos.

Su objetivo será ayudarme a conseguir una mezcla profesional antes del mastering.

# 3. DOS MODOS PRINCIPALES

## MODO 1: MIX ENGINEER

Pensado para utilizarse durante mis sesiones de mezcla.

Debe analizar:
- Gain staging.
- Balance tonal.
- Frecuencias problemáticas.
- Graves y subgraves.
- Medios bajos, medios y medios altos.
- Presencia y agudos.
- Dinámica y transitorios.
- Imagen estéreo.
- Clipping.
- Loudness.

Debe comportarse como un ingeniero que escucha mi mezcla y explica qué aspectos debería revisar.

## MODO 2: REFERENCE ANALYZER

Pensado para estudiar canciones profesionales.

Debe permitir importar archivos WAV y AIFF y generar perfiles sonoros completos.

Cada perfil debe almacenar:
- Espectro promedio.
- Distribución de frecuencias.
- LUFS.
- True Peak.
- RMS.
- Crest Factor.
- LRA.
- Anchura estéreo.
- Correlación estéreo.
- Características dinámicas.
- Análisis por secciones.

# 4. REFERENCE LIBRARY — BIBLIOTECA DE REFERENCIAS

Esta función es obligatoria.

Quiero que MasterZaster tenga una biblioteca interna donde pueda almacenar diferentes canciones profesionales y sus respectivos análisis.

Debe permitirme:

- Importar múltiples referencias.
- Guardar perfiles sonoros.
- Asignar nombres personalizados.
- Clasificar por género musical.
- Añadir etiquetas.
- Añadir comentarios personales.
- Buscar referencias.
- Eliminar perfiles.
- Duplicar perfiles.
- Comparar diferentes referencias.
- Seleccionar una referencia como objetivo de mezcla.
- Cambiar de referencia sin tener que volver a analizarla.

Cada referencia debe tener una ficha con sus características sonoras.

Ejemplo:

REFERENCE LIBRARY

- Reference 01 — Pop Moderno.
- Reference 02 — Hip-Hop.
- Reference 03 — R&B.
- Reference 04 — Electronic.
- Reference 05 — Rock.

Cada canción tendrá su propio perfil espectral, dinámico y estéreo.

**IMPORTANTE:** Guarda los análisis de manera independiente del archivo original. Si el archivo se mueve o elimina, informa que la referencia necesita localizarse nuevamente cuando sea necesario reproducirla.

La biblioteca debe permitir almacenar perfiles y reutilizar análisis sin consumir recursos innecesariamente.

# 5. REFERENCE SPECTRUM MATCHING

Desarrolla una gráfica profesional que permita comparar la curva espectral de mi mezcla con la referencia.

Debe incorporar:

- Curva de referencia.
- Curva de mi mezcla.
- Visualización simultánea.
- Diferencias entre señales.
- Análisis en tiempo real.
- Promedio espectral configurable.
- Análisis por bandas.
- Identificación de excesos y deficiencias de energía.

Debe detectar diferencias en:
- Subgraves.
- Graves.
- Medios bajos.
- Medios.
- Presencia.
- Agudos.

Incluye una vista de diferencias.

La comparación debe considerar loudness, alineación temporal y diferencias de contenido musical. No debe interpretar cualquier diferencia espectral como un problema.

# 6. MIX HEALTH CHECK

Desarrolla un diagnóstico técnico general de mi mezcla.

**LOW END**
- Subgraves.
- Graves.
- Acumulaciones.
- Exceso de energía.
- Posibles problemas entre bombo y bajo.

**MID RANGE**
- Medios bajos.
- Medios centrales.
- Medios altos.
- Acumulaciones.
- Inteligibilidad.

**HIGH END**
- Presencia.
- Brillo.
- Agudos.
- Energía excesiva.
- Falta de definición.

**DYNAMICS**
- RMS.
- LUFS.
- True Peak.
- Crest Factor.
- Transitorios.
- Compresión.
- Rango dinámico.

**STEREO**
- Anchura.
- Correlación.
- Compatibilidad mono.
- Balance entre canales.
- Distribución de frecuencias.

**GAIN STAGING**
- Nivel de entrada.
- Picos.
- Headroom.
- Clipping.
- Niveles de señal.

Presenta resultados con tres estados:

- Correcto.
- Necesita atención.
- Problema potencial.

Cada advertencia debe estar respaldada por mediciones.

# 7. SMART EQ SUGGESTIONS

Desarrolla un asistente inteligente de ecualización.

Debe identificar diferencias entre la referencia y mi mezcla y proponer ajustes concretos.

Ejemplo:

«Se detecta una acumulación de energía entre 180 y 300 Hz respecto a la referencia».

Sugerencia:
- Frecuencia central: 240 Hz.
- Ganancia: -1.5 dB.
- Q: 1.2.
- Tipo: Bell.

Debe poder:
- Proponer filtros.
- Proponer cortes y realces.
- Identificar resonancias.
- Recomendar ajustes correctivos.
- Comparar antes y después.
- Permitir escuchar cambios.
- Aplicar ajustes bajo autorización.

Incorpora PREVIEW y RESET.

Los parámetros deben calcularse a partir del análisis real y no utilizar valores fijos universales.

# 8. NON-DESTRUCTIVE MIXING — PROCESAMIENTO NO DESTRUCTIVO

Esta característica es fundamental.

MasterZaster nunca debe modificar permanentemente mi mezcla original sin mi autorización.

Quiero tener control absoluto sobre el procesamiento.

Implementa:

- PREVIEW: escuchar los cambios antes de aplicarlos.
- APPLY: aplicar los ajustes seleccionados.
- BYPASS: desactivar el procesamiento.
- RESET: restaurar los parámetros originales.
- UNDO: deshacer cambios cuando sea posible.
- A/B: comparar el sonido original con el procesado.
- HISTORY: consultar los ajustes realizados durante la sesión.

Cada recomendación debe ser opcional.

El plugin debe permitir probar varias alternativas sin perder la configuración anterior.

Los cambios deben ser reversibles mediante los parámetros del plugin y respetar el guardado de sesiones de Ableton.

No debe modificar directamente los archivos de audio originales ni realizar procesamiento destructivo fuera del DAW.

# 9. REFERENCE MATCH SCORE

Crea un sistema de similitud técnica de 0 a 100.

Debe evaluar:

- Tonal Balance.
- Dynamics.
- Stereo Image.
- Loudness.
- Spectral Similarity.

Ejemplo:

REFERENCE MATCH: 82/100

TONAL BALANCE: 85  
DYNAMICS: 78  
STEREO IMAGE: 90  
LOUDNESS: 88  
SPECTRAL SIMILARITY: 81

Debe explicar qué factores afectan la puntuación.

No debe presentar el resultado como una medida objetiva de calidad musical.

Debe considerar género, arreglos, instrumentación, secciones y diferencias de contenido.

# 10. GAIN STAGING

Implementa un módulo dedicado a la gestión de niveles.

Debe:
- Medir RMS.
- Medir Peak.
- Medir True Peak.
- Analizar headroom.
- Detectar clipping.
- Identificar señales elevadas o débiles.
- Recomendar ajustes de ganancia.
- Analizar pistas individuales.
- Analizar grupos y buses.
- Analizar el bus de mezcla.

Las recomendaciones deben ser contextuales.

# 11. SISTEMA A/B INTELIGENTE

Permite comparar:

REFERENCE: canción profesional.

MIX: producción actual.

Incorpora:
- Cambio instantáneo.
- Igualación de loudness.
- Comparación espectral.
- Comparación dinámica.
- Comparación estéreo.
- Comparación de loudness.
- Comparación por secciones.

La referencia debe reproducirse desde un sistema independiente del procesamiento principal del plugin.

Evita duplicaciones, problemas de ruteo y reproducción accidental de la referencia en la señal de salida.

# 12. OFFLINE AI — INTELIGENCIA ARTIFICIAL LOCAL

Quiero que MasterZaster funcione principalmente sin conexión a Internet.

Las funciones principales deben ejecutarse localmente.

No quiero depender obligatoriamente de:
- Servicios en la nube.
- APIs externas.
- Suscripciones.
- Servidores de terceros.

Investiga e implementa una arquitectura que permita:

- Análisis de características musicales.
- Clasificación sonora.
- Detección de patrones de mezcla.
- Identificación de posibles problemas.
- Recomendaciones contextuales.
- Comparación inteligente de referencias.
- Análisis espectral mediante modelos neuronales.

Investiga ONNX Runtime y modelos compatibles con ejecución local.

La IA debe complementar el motor DSP.

Las mediciones objetivas deben continuar funcionando incluso cuando los modelos neuronales estén desactivados.

Incluye:
- Gestión de modelos.
- Carga y descarga de modelos.
- Control del consumo de memoria.
- Procesamiento fuera del hilo de audio.
- Manejo de errores.
- Posibilidad de desactivar la IA.

Si un modelo no es viable para la primera versión, implementa una alternativa DSP funcional.

# 13. MÓDULOS DE PROCESAMIENTO

Incorpora cuando sean necesarios:

- Equalizer.
- Dynamic EQ.
- Compressor.
- Transient Control.
- Saturation.
- Stereo Imaging.
- High Pass Filter.
- Low Pass Filter.
- Gain Control.
- Limiter.

Cada módulo debe poder activarse y desactivarse independientemente.

No quiero procesamiento automático destructivo.

# 14. DISEÑO GRÁFICO

Utiliza la imagen adjunta como referencia estética.

Quiero una interfaz oscura, elegante, moderna y premium.

Debe incorporar:
- Analizador espectral.
- Gráficas superpuestas.
- Medidores profesionales.
- Indicadores de diagnóstico.
- Curvas de EQ.
- Panel de recomendaciones.
- Medidores de loudness.
- Indicadores estéreo.
- Reference Library.
- Reference Match Score.
- Botones intuitivos.
- Tipografía profesional.
- Animaciones sutiles.
- Escalado de interfaz.

Debe tener una identidad visual propia, sin copiar marcas ni elementos protegidos.

# 15. ARQUITECTURA TÉCNICA

Utiliza C++ y JUCE.

Organiza el proyecto en módulos independientes:

- Audio Processor.
- Audio Editor.
- DSP Engine.
- Reference Analyzer.
- Reference Library Manager.
- Spectrum Matching Engine.
- Mix Health Check.
- Smart EQ Engine.
- Reference Match Scoring.
- AI Engine.
- Gain Staging Analyzer.
- Audio Metering.
- Preset Manager.
- Session History Manager.

Separa el procesamiento de audio en tiempo real de las operaciones pesadas de análisis, carga de archivos e inferencia neuronal.

Implementa:
- Gestión segura de memoria.
- Comunicación entre hilos.
- Manejo de errores.
- Compatibilidad con distintas frecuencias de muestreo.
- Compatibilidad con distintos tamaños de buffer.
- Persistencia de parámetros.
- Guardado de perfiles.

# 16. COMPATIBILIDAD CON ABLETON LIVE

El plugin debe funcionar como VST3.

Debe poder insertarse en:
- Canales individuales.
- Grupos.
- Buses.
- Master Channel.

Debe conservar parámetros y configuraciones al guardar y abrir proyectos.

Comprueba el funcionamiento de la referencia cuando el plugin esté insertado en diferentes canales.

Evita duplicaciones y errores de ruteo.

# 17. GITHUB Y COMPILACIÓN

Quiero que todo el proyecto esté organizado en mi cuenta de GitHub.

Crea:
- Repositorio MasterZaster.
- Código fuente completo.
- Arquitectura de carpetas.
- CMakeLists.txt.
- README.md.
- Documentación.
- Instrucciones de instalación.
- Instrucciones de compilación.
- Configuración para Visual Studio 2022.
- GitHub Actions.
- Compilación automatizada para Windows.
- Generación de artefactos VST3.
- Pruebas automatizadas.

Organiza el desarrollo mediante commits y versiones.

Si tienes acceso autorizado a GitHub, publica el proyecto. Si no, prepara todo para conectarlo.

No afirmes haber publicado, compilado o probado algo que no hayas realizado.

# 18. DESARROLLO POR VERSIONES

No quiero que intentes construir todas las funciones al mismo tiempo.

Quiero que desarrolles MasterZaster mediante versiones progresivas.

## V1 — MASTERZASTER CORE

Prioridad máxima: conseguir un plugin real, funcional y compilable.

Debe incluir:
- Plugin VST3.
- Compatibilidad con Windows.
- Compatibilidad con Ableton.
- Interfaz gráfica inicial.
- Importación de referencias WAV y AIFF.
- Reference Analyzer.
- Análisis FFT.
- Spectrum Matching.
- Medición RMS, Peak y LUFS.
- Gain Staging.
- Comparación A/B.
- Mix Health Check básico.
- Guardado de perfiles.
- Reference Library inicial.
- Guardado de parámetros.

La V1 debe poder utilizarse realmente en una sesión de mezcla.

## V2 — MASTERZASTER INTELLIGENCE

Incorpora:
- Smart EQ Suggestions.
- Reference Match Score.
- Análisis dinámico avanzado.
- Análisis estéreo avanzado.
- Comparación por secciones.
- Preview de ajustes.
- Historial de cambios.
- Mejoras en Reference Library.
- Recomendaciones contextuales.

## V3 — MASTERZASTER AI

Incorpora:
- Modelos neuronales locales.
- ONNX Runtime.
- Clasificación sonora.
- Análisis musical avanzado.
- Detección inteligente de patrones.
- Recomendaciones mediante IA.
- Modelos especializados en mezcla.

## V4 — MASTERZASTER PROFESSIONAL

Incorpora:
- Mejoras de rendimiento.
- Interfaz comercial definitiva.
- Gestión avanzada de referencias.
- Mejoras de procesamiento.
- Optimización de CPU.
- Pruebas exhaustivas.
- Preparación para distribución comercial.

No avances a una versión nueva sin verificar la estabilidad de la anterior.

# 19. PRUEBAS Y CONTROL DE CALIDAD

Comprueba:

1. Compilación del proyecto.
2. Generación del VST3.
3. Reconocimiento en Ableton Live.
4. Importación WAV y AIFF.
5. Análisis espectral.
6. Comparación de referencias.
7. Gain Staging.
8. Mix Health Check.
9. Smart EQ Suggestions.
10. Reference Match Score.
11. Reference Library.
12. Procesamiento no destructivo.
13. Funcionamiento offline.
14. Guardado de parámetros.
15. Persistencia de perfiles.
16. Consumo de CPU y memoria.
17. Estabilidad del audio.

Documenta errores, limitaciones y funciones pendientes.

# 20. METODOLOGÍA DE TRABAJO

Desarrolla el proyecto profesionalmente.

Primero analiza los requisitos y la viabilidad técnica.

Después:
1. Diseña la arquitectura.
2. Define las dependencias.
3. Organiza el repositorio.
4. Diseña la interfaz.
5. Implementa el motor DSP.
6. Implementa Reference Analyzer.
7. Implementa Reference Library.
8. Implementa Gain Staging.
9. Implementa Spectrum Matching.
10. Implementa Mix Health Check.
11. Incorpora Smart EQ.
12. Incorpora Reference Match Score.
13. Implementa procesamiento no destructivo.
14. Integra IA local.
15. Realiza pruebas.
16. Compila.
17. Documenta.
18. Publica en GitHub cuando exista autorización.

Mantén el proyecto compilable en cada etapa importante.

# 21. RESULTADO FINAL

Quiero que MasterZaster sea un verdadero ingeniero de mezcla virtual.

Debe permitirme:

- Cargar canciones profesionales como referencia.
- Crear una biblioteca de referencias.
- Analizar sus características sonoras.
- Comparar mi producción en tiempo real.
- Identificar problemas de mezcla.
- Recibir recomendaciones concretas.
- Previsualizar correcciones.
- Controlar el procesamiento.
- Trabajar sin conexión.
- Utilizar inteligencia artificial local.
- Guardar mis perfiles y configuraciones.
- Utilizarlo directamente en Ableton Live.

Quiero desarrollar una herramienta con identidad propia, tecnología real y potencial comercial.

**INSTRUCCIÓN FINAL: COMIENZA POR LA V1. Primero analiza la viabilidad técnica y presenta la arquitectura, dependencias y plan de implementación. Después comienza a construir el software real. No simules funciones, no entregues únicamente interfaces visuales y no declares terminado ningún módulo que no hayas implementado y probado.**

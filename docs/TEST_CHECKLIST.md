# Checklist de pruebas manuales

Marca cada punto solo tras comprobarlo. Anota la fecha, el commit y la versión de Ableton.

## M0 — Esqueleto

- [ ] Compila en Release con Visual Studio 2022 sin errores
- [ ] Los tests del núcleo pasan (`ctest`)
- [ ] El workflow de GitHub Actions termina en verde y genera el artefacto VST3
- [ ] Ableton Live reconoce el plugin (aparece en Plug-ins → VST3)
- [ ] Se puede insertar en una pista, en un grupo y en el Master
- [ ] El audio pasa sin cambios con la ganancia en 0 dB (comparar con el plugin desactivado)
- [ ] El control "Output Gain" cambia el nivel sin clics
- [ ] El valor de "Output Gain" se conserva al guardar el set, cerrarlo y volver a abrirlo
- [ ] Cambiar el tamaño de buffer y la frecuencia de muestreo en Ableton no provoca errores
- [ ] Abrir y cerrar la ventana del plugin varias veces no provoca cierres

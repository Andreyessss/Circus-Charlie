Instrucciones de ejecución (Windows)
---------------------------------

1) Extraer el ZIP completo
- Descarga `release_CircusCharlie.zip` y extrae todo en una carpeta (por ejemplo `C:\Users\TuUsuario\Descargas\CircusCharlie_release`).
- No intentes ejecutar el EXE desde dentro del ZIP.

2) Estructura esperada
- La carpeta extraída debe contener `bin\\` y `assets\\` en el mismo nivel. Ejemplo:
  - CircusCharlie_release\\
    - bin\\CircusCharlie.exe
    - bin\\libsfml-*.dll
    - assets\\textures\\...
    - assets\\sounds\\...

3) Desbloquear el ejecutable (si Windows lo marca)
- Botón derecho sobre `bin\\CircusCharlie.exe` 14 Propiedades 14 si aparece "Desbloquear", marcarlo y aplicar.
- O desde PowerShell (ejecutar como Administrador si es necesario):
  Unblock-File .\\bin\\CircusCharlie.exe

4) Ejecutar y ver errores desde PowerShell
- Abre PowerShell, navega a la carpeta raíz del release y ejecuta:
  .\\bin\\CircusCharlie.exe
- Si aparece error sobre DLLs faltantes, asegarate de que las `libsfml-*.dll` estén junto al `.exe` (están incluidas en el ZIP).

5) Recomendaciones si SmartScreen o Windows bloquea
- En el aviso de SmartScreen: Más información 14 Ejecutar de todos modos.
- O ejecutar con botón derecho 14 "Ejecutar como administrador".

6) Verificar integridad
- Comprueba el hash del ZIP descargado y compáralo con el hash proporcionado por quien lo subió:
  Get-FileHash .\\release_CircusCharlie.zip -Algorithm SHA256

7) Soporte
- Si al ejecutar obtienes mensajes de error, copia la salida de PowerShell y pégala en el ticket o envíamela para diagnóstico.

Gracias — disfruta del juego.
